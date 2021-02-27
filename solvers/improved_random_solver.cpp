//
// Created by rinat on 9/15/2020.
//

#include "improved_random_solver.h"
#include <limits>

vector<int> best_solution;
double best_score = 999999999;
bool exit_ = false;

vector<int> StochasticWalkSolver::solve() {
    vector<int> schedule(data.interventions.size());
    vector<int> &best = best_solution;
    Checker checker(data);
    int counter = 0;
    cout.precision(10);
    while (true) {
        counter++;
        cout << counter << endl;
        for (int i = 0; i < 1000; i++) {
            for (int j = 0; j < schedule.size(); j++) {
                int cur = rand() % data.interventions[j].tmax;
                schedule[j] = cur;
            }
            double score = checker.checkAll(schedule);
            if (score < best_score) {

                solution_lock.lock();
                best = schedule;
                solution_lock.unlock();

                best_score = score;
                cout << "pre iteration " << i << " new best found " << best_score << endl;
            }
        }
        cout << "Pre iterations done, starting to improve best solution" << endl;
        int i = 0;

        vector<int> bad_res;
        while (not exit_) {
            vector<int> schedule = best;
            i ++ ;
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
            if (score < best_score | (score > 0 && score == best_score) | (score < 0 && round(score) == round(best_score) && rand() % 100 < 100*(1 - data.Alpha))) {
                if (!checker.wrong_resource_intervention.empty())
                    bad_res = checker.wrong_resource_intervention;

                solution_lock.lock();
                best = schedule;
                solution_lock.unlock();

                best_score = score;

                //cout << "iteration " << i << " best found new" << best_score << endl;
            }
            if (i % 10000 == 0)
                cout << "iteration " << i << " best found " << best_score << endl;
            if (i % 1000 == 0){
                best_score = checker.checkAll(schedule);
            }
            i ++ ;
        }
        if (exit_)
            break;
    }
    cout << "Iterations done " << endl;
    return best;
}
