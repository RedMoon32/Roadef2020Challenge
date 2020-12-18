//
// Created by rinat on 9/2/2020.
//

#include <iostream>
#include <algorithm>
#include "checker.h"


using namespace std;

int Checker::checkAll(vector<int> schedule) {
    this->schedule = schedule;
    if (data.interventions.size() != schedule.size())
        return -1;

    int res1 = checkHorizon();
    if (res1 != 0) {
        return 100000 * checkHorizon();
    }

    res1 = checkExclusions();
    if (res1 != 0) {
        return 1000 * res1;
    }

    res1 = checkResourceConstraint();
    if (res1 != 0) {
        return res1;
    }

    return computeMetric() - 500000;
}

int Checker::checkResourceConstraint() {
    vector<vector<int>> all_resource(data.T);
    wrong_resource_intervention = vector<int>();
    int wrong_res = 0;

    for (int inter = 0; inter < schedule.size(); inter++) {
        int start_time = schedule[inter];
        auto cur_job = data.interventions[inter];
        for (auto &res: cur_job.workload) {
            for (auto &req: res.second[start_time]) {
                int new_time = req.first;
                float &target = resource_consumption[res.first.id][new_time];
                target += req.second;
                all_resource[new_time].push_back(inter);
            }
        }
    }

    for (int res = 0; res < resource_consumption.size(); res++) {
        for (int time = 0; time < data.T; time++) {
            if (resource_consumption[res][time] < data.resources[res].min[time] |
                resource_consumption[res][time] > data.resources[res].max[time]) {
                wrong_res += 1;
                wrong_resource_intervention.insert(wrong_resource_intervention.end(), all_resource[time].begin(),
                                                   all_resource[time].end());
            }
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
    wrong_exclusion.clear();
    for (const auto &exc: data.exclusions) {
        auto &job1 = exc.int1;
        auto &job2 = exc.int2;
        int time1 = schedule[job1.id] + 1;
        int time2 = schedule[job2.id] + 1;
        auto &v = exc.season.times;
        for (auto time: v) {
            if (time1 <= time && time <= time1 + job1.delta[time1 - 1] &&
                time2 <= time && time <= time2 + job2.delta[time2 - 1]) {
                excounter += 1;
                wrong_exclusion.push_back(job1.id);
                wrong_exclusion.push_back(job2.id);
            }
        }
    }
    return excounter;
}

Checker::Checker(const DataInstance &data) :
        data(data) {
    resource_consumption = vector<vector<float>>(data.resources.size(), vector<float>(data.T));
}

Checker::Checker(const vector<int> &temp_schedule, const DataInstance &data) {
    resource_consumption = vector<vector<float>>(data.resources.size(), vector<float>(data.T));
    schedule = temp_schedule;
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


// ============================== this is faster version of original checker ================================

vector<int> FastChecker::computeChange() {
    vector<int> changed;
    for (int i = 0; i < schedule.size(); i++) {
        if (prevSchedule[i] != schedule[i])
            changed.push_back(i);
    }
    return changed;
}

void FastChecker::setConsumption(int resource_id, int time, double value) {
    auto consumption = wrongResource[resource_id][time];

    //check for max
    if (!consumption.first && value > data.resources[resource_id].max[time]) {
        consumption.first = true;
        prevWrong++;
    } else if (consumption.first && value <= data.resources[resource_id].max[time]) {
        consumption.first = false;
        prevWrong--;
    }
    // ===== check for min
    if (!consumption.second && value < data.resources[resource_id].min[time]) {
        consumption.second = true;
        prevWrong++;
    } else if (consumption.second && value >= data.resources[resource_id].min[time]) {
        consumption.second = false;
        prevWrong--;
    }
}


int FastChecker::checkResourceConstraint() {

    if (prevSchedule.empty()) {
        wrongResource = vector<vector<pair<bool, bool>>>(data.resources.size(), vector<pair<bool, bool>>(data.T));
        prevWrong = Checker::checkResourceConstraint();
        prevSchedule = schedule;
        for (int res = 0; res < resource_consumption.size(); res++) {
            for (int time = 0; time < data.T; time++) {
                if (resource_consumption[res][time] > data.resources[res].max[time])
                    wrongResource[res][time].first = true;
                if (resource_consumption[res][time] < data.resources[res].min[time])
                    wrongResource[res][time].second = true;
            }
        }
        return prevWrong;
    }
    auto changed = computeChange();

    for (int inter: changed) {
        int start_time = schedule[inter];
        int prev_start = prevSchedule[inter];
        auto cur_job = data.interventions[inter];

        // increment for new timeslot
        for (auto &res: cur_job.workload) {
            for (int tsht = 0; tsht < res.second[start_time].size(); tsht++) {
                int new_time = tsht + start_time;
                auto resource = res.first.id;
                float &target = resource_consumption[resource][new_time];
                //target += res.second[start_time][tsht];
                setConsumption(resource, new_time, target);
            }
            for (int tsht = 0; tsht < res.second[prev_start].size(); tsht++) {
                int new_time = tsht + prev_start;
                auto resource = res.first.id;
                float &target = resource_consumption[resource][new_time];
                //target -= res.second[prev_start][tsht];
                setConsumption(resource, new_time, target);
            }
        }
    }
    cout << prevWrong;
    prevSchedule = schedule;
    return prevWrong;
}
