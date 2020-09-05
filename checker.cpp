//
// Created by rinat on 9/2/2020.
//

#include <iostream>
#include <algorithm>
#include "checker.h"


using namespace std;

int Checker::checkAll() {
    if (data.interventions.size() != schedule.size())
        return false;
    if (!checkHorizon() || !checkResourceConstraint() || !checkExclusion())
        return false;
    return true;
}

int Checker::checkResourceConstraint() {
    vector<vector<int>> resource_consumption(data.resources.size());
    for (int inter = 0; inter < schedule.size(); inter++) {
        int time = schedule[inter];
        for (const auto &res: data.interventions[inter].workload) {
            for (int tsht = 0; tsht < res.second[time].size(); tsht++) {
                auto &target = resource_consumption[res.first.id][tsht];
                target += res.second[time][tsht];
                if (target > data.resources[res.first.id].max[time])
                    return false;
            }
        }
    }
    for (int res = 0; res < resource_consumption.size(); res++) {
        for (int time = 0; time < data.T; time++)
            if (resource_consumption[res][time] < data.resources[res].min[time])
                return false;
    }
    return true;
}

int Checker::checkHorizon() {
    for (int inter = 0; inter < schedule.size(); inter++) {
        int time = schedule[inter];
        if (time > data.interventions[inter].tmax)
            return false;
    }
    return true;
}

int Checker::checkExclusion() {
    for (const auto& exc: data.exclusions){
        int time1 = schedule[exc.int1.id];
        int time2 = schedule[exc.int2.id];
        auto &v = exc.season.times;
        if (find(v.begin(), v.end(), time1) != v.end() && find(v.begin(), v.end(), time2) != v.end()){
            return false;
        }
    }
    return true;
}

Checker::Checker(const vector<int> schedule, const DataInstance &data) :
        data(data), schedule(schedule) {

}
