//
// Created by rinat on 9/2/2020.
//

#include <iostream>
#include <algorithm>
#include "checker.h"


using namespace std;

int Checker::checkAll() {
    if (data.interventions.size() != schedule.size())
        return -1;

    int res1 = checkHorizon();
    if (res1 != 0) {
        return -checkHorizon();
    }

    res1 = checkExclusions();
    if (res1 != 0) {
        return -10000 * res1;
    }

    res1 = checkResourceConstraint();
    if (res1 != 0) {
        return res1;
    }

    return 0;
}

int Checker::checkResourceConstraint() {

    int wrong_res = 0;

    for (int inter = 0; inter < schedule.size(); inter++) {
        int start_time = schedule[inter];
        auto cur_job = data.interventions[inter];
        for (auto &res: cur_job.workload) {
            for (int tsht = 0; tsht < res.second[start_time].size(); tsht++) {
                float &target = resource_consumption[res.first.id][tsht + start_time];
                target += res.second[start_time][tsht];
            }
        }
    }

    for (int res = 0; res < resource_consumption.size(); res++) {
        for (int time = 0; time < data.T; time++) {
            if (resource_consumption[res][time] < data.resources[res].min[time] |
                resource_consumption[res][time] > data.resources[res].max[time])
                wrong_res += 1;
            resource_consumption[res][time] = 0;
        }
    }
    return wrong_res;
}

int Checker::checkHorizon() {
    int wrong = 0;
    for (int inter = 0; inter < schedule.size(); inter++) {
        int time = schedule[inter];
        if (time > data.interventions[inter].tmax)
            wrong++;
    }
    return wrong;
}

int Checker::checkExclusions() {
    int excounter = 0;
    for (const auto &exc: data.exclusions) {
        int time1 = schedule[exc.int1.id] + 1;
        int time2 = schedule[exc.int2.id] + 1;
        auto &v = exc.season.times;
        if (binary_search(v.begin(), v.end(), time1) && binary_search(v.begin(), v.end(), time2)) {
            excounter += 1;
        }
    }
    return excounter;
}

Checker::Checker(vector<int> schedule, const DataInstance &data) :
        data(data), schedule(schedule) {
    resource_consumption = vector<vector<float>>(data.resources.size(), vector<float>(data.T));
}

vector<vector<double>> Checker::computeRiskDistribution() {
    vector<vector<double>> risk;
    for (int t = 0; t < data.T; t++) {
        risk.push_back(vector<double>(data.scenarious_number[t]));
    }
    for (int i = 0; i < schedule.size(); i++) {
        auto intervention = data.interventions[i];
        int start_time = schedule[i];
        int delta = intervention.delta[start_time];
        for (int time_idx = 0; time_idx < delta; time_idx++) {
            auto time = time_idx + start_time;
            int idex = (intervention.risk[time].second.size() - 1) - (intervention.risk[time].first - start_time);
            vector<double> cur_risk = intervention.risk[time].second[idex];
            for (int risk_index = 0; risk_index < cur_risk.size(); risk_index++) {
                risk[time][risk_index] += cur_risk[risk_index];
            }
        }
    }
    return risk;
}

vector<double> Checker::computeMeanRisk(const vector<vector<double>> &risk) {
    vector<double> mean_risk(data.T);
    for (int t = 0; t < data.T; t++) {
        double sum_of_elements = 0;
        for (auto &n : risk[t])
            sum_of_elements += n;
        mean_risk[t] = sum_of_elements / data.scenarious_number[t];
    }
    return mean_risk;
}

vector<double> Checker::computeQuantile(vector<vector<double>> &risk) {
    vector<double> q(data.T);
    for (int t = 0; t < data.T; t++) {
        sort(risk[t].begin(), risk[t].end());
        q[t] = risk[t][int(ceil(data.scenarious_number[t] * data.Quantile)) - 1];
    }
    return q;
}

tuple<vector<double>, vector<double>> Checker::getRiskDistribution() {
    auto risk = computeRiskDistribution();
    auto mean_risk = computeMeanRisk(risk);
    auto q = computeQuantile(risk);
    return make_tuple(mean_risk, q);
}

double Checker::computeObjective1(vector<double> mean_risk) {
    double obj_1 = accumulate(mean_risk.begin(), mean_risk.end(), 0.0) / mean_risk.size();
    return obj_1;
}

double Checker::computeObjective2(vector<double> q, vector<double> mean_risk) {
    double sum = 0;
    for (int i = 0; i < q.size(); i++) {
        sum += max(q[i] - mean_risk[i], 0.);
    }
    double obj_2 = sum / (double) q.size();
    return obj_2;
}

double Checker::computeMetric() {
    vector<double> mean_risk, q;
    tie(mean_risk, q) = getRiskDistribution();
    double obj_1 = computeObjective1(mean_risk);
    double obj_2 = computeObjective2(q, mean_risk);
    double obj_total = data.Alpha * obj_1 + (1 - data.Alpha) * obj_2;
    return obj_total;
}