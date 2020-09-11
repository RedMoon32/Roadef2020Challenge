#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "parser.h"
#include "json.h"
#include "checker.h"
#include <cstdlib>

using namespace std;
using namespace nlohmann;


int main() {
    Parser p("../A_set/A_05.json");
    DataInstance d = p.parseJsonToSchedule();
    cout << "==== Parsed Successfully ====" << endl;
    auto checker = Checker({}, d);
    vector<int> schedule(d.interventions.size());

    for (int i = 0; i < 100000; i++) {

        for (int j = 0; j < schedule.size(); j++) {
            int cur = rand() % d.T;
            schedule[j] = cur;
        }
        checker.schedule = schedule;
        int res = checker.checkAll();
        cout << "Processed " << i << " , result " << res << endl;
    }

    return 0;
}
