#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <time.h>

#include "json.hpp"
#include "random_solver.h"
#include "improved_random_solver.h"
#include "checker.h"
#include "parser.h"

using namespace std;
using namespace nlohmann;
using namespace std::chrono;

#define TIMES 1000

int main() {
    srand(time(NULL));
    cout << "==== Reading Data ===" << endl;
    Parser p("../A_set/A_06.json");
    DataInstance d = p.parseJsonToSchedule();
    cout << "==== Parsed Successfully ====" << endl;
    ImprovedRandomSolver solver(d, -1);
    vector<int> result;
    float duration = 0;

    for (int i = 0; i < 1; i++) {

        result = solver.solve();

        clock_t tStart = clock();
        unique_ptr<AbstractChecker> checker(new Checker(result, d));
        float res = checker->checkAll();

        clock_t tStop = clock();

        duration += (double)(tStop - tStart) / CLOCKS_PER_SEC ;

        cout << "Processed " << i << " , result " << res << endl;
    }
    write_result("../out.txt", result, d.interventions);
    cout << "\nExecution time " << duration << ", average per check time " << (float) duration / (float) TIMES << endl;
    return 0;
}
