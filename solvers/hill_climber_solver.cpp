//
// Created by rinat on 2/27/2021.
//

#include "hill_climber_solver.h"
#include <limits>

void HillClimbing::improvePregenerated() {
    vector<int> &best = best_solution;
    Checker checker(data);
    int i = 0;
    while (not exit_) {
        vector<int> schedule = best;

        int rand_count = rand() % 4 == 0 ? 2 : 1;
        for (int c = 0; c < rand_count; c++) {
            int randindex;

            if (rand() % 4 > 0 && !bad_res.empty())
                randindex = bad_res[rand() % bad_res.size()];
            else
                randindex = rand() % schedule.size();

            double cur_best = checker.checkAll(schedule);
            int best_time = schedule[randindex];

            for (int j = 0; j < min(30, data.interventions[randindex].tmax); j++) {
                schedule[randindex] = rand() % data.interventions[randindex].tmax;
                double score = checker.checkAll(schedule);
                if (score <= cur_best) {
                    best_time = schedule[randindex];
                    cur_best = score;
                }
            }

            schedule[randindex] = best_time;
        }

        auto score = checker.checkAll(schedule);
        checkForUpdate(score, schedule, checker);

        if (i % 100 == 0)
            cout << "#" << i << " Current best " << best_score << endl;
        i++;
    }
}
