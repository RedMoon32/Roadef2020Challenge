//
// Created by rinat on 9/15/2020.
//

#include "improved_solver.h"
#include "checker.h"

ImprovedRandomSolver::ImprovedRandomSolver(const DataInstance &data, int computationTime) :
        data(data),
        computationTime(computationTime) {
}

vector<int> ImprovedRandomSolver::solve() {
    vector<int> schedule(data.interventions.size());
    vector<int> best;
    int best_score = 999999999;
    Checker checker({}, data);

    for (int i = 0; i < 100000; i++) {
        for (int j = 0; j < schedule.size(); j++) {
            int cur = rand() % data.interventions[j].tmax;
            schedule[j] = cur;
        }
        checker.schedule = schedule;
        int score = checker.checkAll();
        if (score < best_score) {
            best = schedule;
            best_score = score;
            cout << "pre iteration " << i << " new best found " << best_score << endl;
        }
        //cout << "pre - iteration " << i << ", result " << best_score << endl;
    }
    cout << "Pre iterations done, starting to improve best solution";
    for (int i = 0; i < 20000000; i++) {
        vector<int> schedule = best;
        for (int j = 0; j < schedule.size() * 0.3; j++) {
            int cur = rand() % data.interventions[j].tmax;
            int randindex = rand() % schedule.size();
            schedule[randindex] = cur;
        }

//        checker.schedule = schedule;
//        for (int k = 0; k < schedule.size(); k++){
//            cout << schedule[k] << " ";
//        }
//        cout << endl;

        int score = checker.checkAll();
        if (score < best_score) {
            best = schedule;
            best_score = score;
            cout << "iteration " << i << " new best found " << best_score << endl;
        }

        // cout << "iteration " << i << ", result " << best_score << endl;
    }
    return schedule;
}
