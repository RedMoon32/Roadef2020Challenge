//
// Created by rinat on 9/15/2020.
//

#include "improved_random_solver.h"
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

    for (int i = 0; i < 1000000; i++) {
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

    }
    cout << "Pre iterations done, starting to improve best solution" << endl;
    for (int i = 0; i < 300000000; i++) {
        if (best_score == 0)
            break;
        vector<int> schedule = best;
        double rand_count = ((double) rand() / (RAND_MAX)) / 3;
        for (int j = 0; j < min((double) 1, rand_count * schedule.size()); j++) {
            int randindex = rand() % schedule.size();
            int cur = rand() % data.interventions[randindex].tmax;
            schedule[randindex] = cur;
        }

        checker.schedule = schedule;
        int score = checker.checkAll();
        if (score < best_score) {
            best = schedule;
            best_score = score;
            cout << "iteration " << i << " new best found " << best_score << endl;
        }

        if (i%100000 == 0)
            cout << "iteration " << i << ", result " << best_score << endl;
    }
    cout << "Iterations done " << endl;
    return best;
}
