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
        // cout << endl << "wrong horizon" << endl;
        return checkHorizon();
    }

    res1 = checkExclusions();
    if (res1 != 0) {
        // cout << endl << "wrong exclusions " << endl;
        return 10000 * res1;
    }

    res1 = checkResourceConstraint();
    if (res1 != 0) {
        // cout << endl << "wrong resources" << endl;
        return res1;
    }

    return 0;
}

int Checker::checkResourceConstraint() {

    int wrong_res = 0;

    for (int inter = 0; inter < schedule.size(); inter++) {
        int time = schedule[inter];
        for (const auto &res: data.interventions[inter].workload) {
            if (res.second.size() >= time)
                for (int tsht = 0; tsht < res.second[time].size(); tsht++) {
                    auto &target = resource_consumption[res.first.id][tsht];
                    if (time < res.second.size() && tsht < res.second[time].size())
                        target += res.second[time][tsht];
                    if (target > data.resources[res.first.id].max[time])
                        wrong_res += 1;
                }
        }
    }

    for (int res = 0; res < resource_consumption.size(); res++) {
        for (int time = 0; time < data.T; time++) {
            if (resource_consumption[res][time] < data.resources[res].min[time])
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
        if (time >= data.interventions[inter].tmax)
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
