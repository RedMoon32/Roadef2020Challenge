#include <iostream>
#include <cstdlib>
#include <time.h>
#include <unistd.h>
#include <csignal>
#include "random_solver.h"
#include "stochastic_walk_solver.h"
#include "parser.h"
#include "hill_climber_solver.h"

using namespace std;
using namespace nlohmann;
using namespace std::chrono;

DataInstance d;
int time_limit = 15;
string instance_path;
string solution_path;
bool name;
int seed;
int timeid = 70;
clock_t c_start;

void catchAlarm(int) {
    cout << "Stop Signal Arrised, writing best found solution" << endl;
    solution_lock.lock();
    write_result(solution_path, best_solution, d.interventions);
    exit(timeid);
}

//  executable -t time_limit -p instance_name -o new_solution_filename -name -s seed
void parse_args(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
        auto arg = string(argv[i]);
        if (arg == "-t")
            time_limit = stoi(argv[i + 1]);
        if (arg == "-p")
            instance_path = argv[i + 1];
        if (arg == "-o")
            solution_path = argv[i + 1];
        if (arg == "-name")
            name = true;
        if (arg == "-s")
            seed = stoi(argv[i + 1]);
    }
}


int main(int argc, char *argv[]) {
    c_start = clock();
    srand(0);
    cout << "==== Reading Data ===" << endl;
    parse_args(argc, argv);

    signal(SIGALRM, catchAlarm);
    signal(SIGKILL, catchAlarm);
    //signal(SIGSTOP, catchAlarm);

    alarm((time_limit - 1) * 60);

    Parser p(instance_path);
    d = p.parseJsonToSchedule();
    cout << "==== Parsed Successfully ====" << endl;

    HillClimbing solver1(d);
    HillClimbing solver2(d);

    thread thread1, thread2;

    thread1 = thread([&](AbstractSolver *solver) { solver->solve(); }, &solver1);
    thread2 = thread([&](AbstractSolver *solver) { solver->solve(); }, &solver2);

    thread1.join();
    thread2.join();

}
