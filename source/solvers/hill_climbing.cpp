//
// Created by rinat on 9/15/2020.
//

#include "hill_climbing.h"

double best_score = 999999999;

void HillClimbingSASolver::pregenerateBest() {
    vector<int> &best = best_solution;
    Checker checker(data);
    cout.precision(10);
    for (int i = 0; i < 1000; i++) {
        auto schedule = generateRandomSchedule();
        double score = checker.checkAll(schedule);
        if (score < best_score) {
            update_solution(schedule, checker);
            best_score = score;
        }
    }
}

void HillClimbingSASolver::improvePregenerated() {
    Checker checker(data);

    int neighbors_count = max(1, (int) (neighbors_percent * data.interventions.size()));
    int rand_count = max(1, (int) (change_percent * data.interventions.size()));
    cur_best = best_solution;

    for (int i = 0; !exit_; i++) {

        auto schedule = cur_best;
        prev_schedule = schedule;

        for (int n = 0; n < neighbors_count; n++) {

            schedule = prev_schedule;

            for (int j = 0; j < rand_count; j++) {

                int randindex;
                if (rand() % 2 == 0 && !bad_res.empty())
                    randindex = bad_res[rand() % bad_res.size()];
                else
                    randindex = rand() % schedule.size();

                int cur = rand() % data.interventions[randindex].tmax;
                schedule[randindex] = cur;
            }

            double score = checker.checkAll(schedule);
            checkForUpdate(score, schedule, i, checker);

        }
    }
}


vector<int> HillClimbingSASolver::solve() {
    vector<int> schedule(data.interventions.size());
    vector<int> &best = best_solution;
    cout.precision(10);
    while (true) {
        pregenerateBest();
        improvePregenerated();
        if (exit_)
            break;
    }
    return best;
}

void HillClimbingSASolver::checkForUpdate(double score, vector<int> &solution, int iter, Checker &checker) {
    float t = initial_tempreature / (float) (iter + 1);
    float p = exp(-(float)(score - best_score)/t);

    bool simul_ = (score > best_score) && (simulated_annealing) && (rand() % 100 <= p * 100);

    if (score <= cur_best_score || simul_) {
        if (!checker.wrong_resource_intervention.empty())
            bad_res = checker.wrong_resource_intervention;

        if (simul_) {
            prev_schedule = solution;
            cur_best = solution;
            cur_best_score = score;
        }

        if (score <= cur_best_score){
            cur_best = solution;
            cur_best_score = score;
        }

        if (score <= best_score) {
            update_solution(solution, checker);
        }
    }
}

HillClimbingSASolver::HillClimbingSASolver(const DataInstance &data, float change_percent, float neighbors_percent,
                                           float initial_tempreature,
                                           bool simulated_annealing) : RandomSolver(data),
                                                                       change_percent(change_percent),
                                                                       neighbors_percent(neighbors_percent),
                                                                       initial_tempreature(initial_tempreature),
                                                                       simulated_annealing(simulated_annealing) {

}
