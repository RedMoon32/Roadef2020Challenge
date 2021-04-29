//
// Created by rinat on 9/15/2020.
//

#include "stochastic_walk_solver.h"

double best_score = 999999999;

void StochasticWalkSolver::pregenerateBest() {
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

void StochasticWalkSolver::improvePregenerated() {
    Checker checker(data);

    int neighbors_count = max(1, (int) (neighbors_percent * data.interventions.size()));
    int rand_count = max(1, (int) (change_percent * data.interventions.size()));

    for (int i = 0; !exit_; i++) {

        auto schedule = best_solution;
        auto prev_schedule = schedule;

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
            checkForUpdate(score, schedule, checker);

            if (!hill_climbing)
                break;
        }
    }
}


vector<int> StochasticWalkSolver::solve() {
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

void StochasticWalkSolver::checkForUpdate(double score, vector<int> &solution, Checker &checker) {
    if (score <= best_score) {
        if (!checker.wrong_resource_intervention.empty())
            bad_res = checker.wrong_resource_intervention;
        update_solution(solution, checker);
    }
}

StochasticWalkSolver::StochasticWalkSolver(const DataInstance &data, float change_percent, float neighbors_percent,
                                           bool hill_climbing) : RandomSolver(data), change_percent(change_percent),
                                                                 neighbors_percent(neighbors_percent),
                                                                 hill_climbing(hill_climbing) {

}
