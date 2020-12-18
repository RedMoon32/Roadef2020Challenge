#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include <unistd.h>
#include <csignal>
#include <sys/types.h>

#include "json.hpp"
#include "random_solver.h"
#include "improved_random_solver.h"
#include "smart_solver.h"
#include "checker.h"
#include "parser.h"

using namespace std;
using namespace nlohmann;
using namespace std::chrono;

#define TIMES 100
#define EXECUITON_TIME_MIN 30

DataInstance d;

void catchAlarm(int) {
    cout << "Stop Signal Arrised, writing best found solution" << endl;
    write_result("../out.txt", best_solution, d.interventions);
    exit(0);
}

int main() {
    srand(0);
    cout << "==== Reading Data ===" << endl;

    signal(SIGALRM, catchAlarm);
    signal(SIGTERM, catchAlarm);

    alarm(EXECUITON_TIME_MIN*60);

    Parser p("../A_set/A_03.json");
    d = p.parseJsonToSchedule();
    cout << "==== Parsed Successfully ====" << endl;
    ImprovedRandomSolver solver(d);
    vector<int> result;
    float duration;

    clock_t tStart = clock();
    result = solver.solve();
    unique_ptr<AbstractChecker> checker(new Checker(d));
    float res = checker->checkAll(result);
    clock_t tStop = clock();
    duration = (double)(tStop - tStart) / CLOCKS_PER_SEC ;

    write_result("../out.txt", result, d.interventions);
    cout << "\n Resultant score: " << res << "\nExecution time " << duration <<
    ", average per check time " << (float) duration / (float) TIMES << endl;
    return 0;
}
