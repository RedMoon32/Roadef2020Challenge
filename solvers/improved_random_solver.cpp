//
// Created by rinat on 9/15/2020.
//

#include "improved_random_solver.h"


vector<int> best_solution;

vector<int> ImprovedRandomSolver::solve() {
    vector<int> schedule(data.interventions.size());
    vector<int> &best = best_solution;
    Checker checker(data);
    int counter = 0;
    long long best_score = 999999999;
    while (best_score > 0 && counter < 5)
    {
        long long best_score = 999999999;
        counter ++;
        cout << counter << endl;
        for (int i = 0; i < 100000; i++) {
            for (int j = 0; j < schedule.size(); j++) {
                int cur = rand() % data.interventions[j].tmax;
                schedule[j] = cur;
            }
            int score = checker.checkAll(schedule);
            if (score < best_score) {
                best = schedule;
                best_score = score;
                cout << "pre iteration " << i << " new best found " << best_score << endl;
            }
        }
        cout << "Pre iterations done, starting to improve best solution" << endl;
        for (int i = 0; i < 40000; i++) {
            if (best_score == 0)
                break;

            vector<int> schedule = best;
            int rand_count = rand() % 2 + 1;
            auto swap_ids = checker.wrong_resource_intervention;
            for (int j = 0; j < rand_count; j++) {
                int randindex;
                if (rand() % 3 == 0 && !swap_ids.empty())
                    randindex = swap_ids[rand() % swap_ids.size()]; //schedule.size();
                else
                    randindex = rand() % schedule.size();
                int cur = rand() % data.interventions[randindex].tmax;
                schedule[randindex] = cur;
            }
            int score = checker.checkAll(schedule);
            if (score <= best_score || (rand() % 100 < 3 && score - best_score <= 1)) {
                best = schedule;
                best_score = score;
                cout << "iteration " << i << " new best found " << best_score << endl;
            }
            if (i % 100000 == 0)
                cout << "iteration " << i << ", result " << best_score << endl;
        }
        break;
    }
    cout << "Iterations done " << endl;
    return best;
}
