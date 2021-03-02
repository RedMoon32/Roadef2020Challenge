//
// Created by rinat on 9/15/2020.
//

#include "improved_random_solver.h"

double best_score = 999999999;

void StochasticWalkSolver::pregenerateBest() {
    vector<int> &best = best_solution;
    Checker checker(data);
    cout.precision(10);
    for (int i = 0; i < 1000; i++) {
        auto schedule = generateRandomSchedule();
        double score = checker.checkAll(schedule);
        if (score < best_score) {
            update_solution(schedule);
            best_score = score;
            cout << "pre iteration " << i << " new best found " << best_score << endl;
        }
    }
}

void StochasticWalkSolver::improvePregenerated() {
    vector<int> bad_res;
    vector<int> &best = best_solution;
    Checker checker(data);
    int i = 0;
    while (not exit_) {
        vector<int> schedule = best;
        int rand_count = rand() % 4 == 0 ? 2 : 1;

        for (int j = 0; j < rand_count; j++) {
            int randindex;
            if (rand() % 4 > 0 && !bad_res.empty())
                randindex = bad_res[rand() % bad_res.size()];
            else
                randindex = rand() % schedule.size();
            int cur = rand() % data.interventions[randindex].tmax;
            schedule[randindex] = cur;
        }
        double score = checker.checkAll(schedule);
        if (score < best_score | (score > 0 && score == best_score) |
            (round(score) == round(best_score) && rand() % 100 <= 1)) {
            if (!checker.wrong_resource_intervention.empty())
                bad_res = checker.wrong_resource_intervention;

            update_solution(schedule);

            best_score = score;
        }
        if (i % 100 == 0) {
            cout << "iteration " << i << " best found " << best_score
                 << endl;
            best_score = checker.checkAll(best);// other thread syncronization
        }
        i++;
    }
}


vector<int> StochasticWalkSolver::solve() {
    vector<int> schedule(data.interventions.size());
    vector<int> &best = best_solution;
    cout.precision(10);
    while (true) {
        pregenerateBest();
        cout << "Pre iterations done, starting to improve best solution" << endl;
        improvePregenerated();
        if (exit_)
            break;
    }
    cout << "Iterations done " << endl;
    return best;
}
