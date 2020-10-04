//
// Created by rinat on 9/29/2020.
//

#include "partial_checker.h"

PartialChecker::PartialChecker(vector<int> schedule, const DataInstance &data): Checker(schedule, data){

}

int PartialChecker::checkResourceConstraint() {
    int wrong_res = 0;


    for (const auto &_pair: changed) {

        int prev_slot = _pair.first;
        int new_slot = _pair.second;

        for (const auto &res: data.interventions[prev_slot].workload) {
            if (res.second.size() >= prev_slot)
                for (int tsht = 0; tsht < res.second[prev_slot].size(); tsht++) {
                    bool wrong = false;
                    auto &target = resource_consumption[res.first.id][tsht];

                    if (target > data.resources[res.first.id].max[prev_slot])
                        wrong_res += 1;

                    if (tsht < res.second[prev_slot].size())
                        target -= res.second[new_slot][tsht];

                }
        }

        for (const auto &res: data.interventions[new_slot].workload) {
            if (res.second.size() >= new_slot)
                for (int tsht = 0; tsht < res.second[new_slot].size(); tsht++) {
                    auto &target = resource_consumption[res.first.id][tsht];
                    if (tsht < res.second[new_slot].size())
                        target += res.second[new_slot][tsht];
                    if (target > data.resources[res.first.id].max[new_slot])
                        wrong_res += 1;
                }
        }
    }

//    for (const auto &num: changed) {
//        for (int time = 0; time < data.T; time++) {
//            if (resource_consumption[num][time] < data.resources[num].min[time])
//                wrong_res += 1;
//        }
//    }
    return wrong_res;
}

int PartialChecker::checkHorizon() {
    return Checker::checkHorizon();
}
