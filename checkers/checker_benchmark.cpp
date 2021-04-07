#include <iostream>
#include <cstdlib>
#include <time.h>
#include <unistd.h>
#include <csignal>
#include "random_solver.h"
#include "stochastic_walk_solver.h"
#include "parser.h"
#include "hill_climber_solver.h"
#include "genetic_solver.h"

using namespace std;
using namespace nlohmann;
using namespace std::chrono;

DataInstance d;
int time_limit = 15;
string instance_path = "../A_set/A_";
string instance_num;
string solution_path;
bool name;
int seed;
int timeid = 70;
string out_file;
int instance_number;
double param1 = 1;
double param2 = 0.1;
double param3 = 0.2;

clock_t c_start;

void catchAlarm(int) {
    cout << "Stop Signal Arrised, writing best found solution" << endl;
    solution_lock.lock();
    write_result(solution_path, best_solution, d.interventions);

    ofstream outfile;
    outfile.open(out_file, ios_base::app);
    auto best_score = Checker(d).checkAll(best_solution);
    outfile << instance_num << "," << d.interventions.size() << "," << d.resources.size() << "," << param1 << ","
            << param2 << "," << param3 << "," << time_limit << "," << (best_score < 0 ? best_score + 500000 : best_score)
            << (best_score < 0)
            << endl;
    outfile.close();
    exit(timeid);
}

//  executable -t time_limit -p instance_name -o new_solution_filename -name -s seed
void parse_args(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
        string arg = argv[i];
        string next_arg = ( i < argc - 1 ? argv[i + 1]: "");

        if (arg == "-t")
            time_limit = stoi(next_arg);
        if (arg == "-p"){
            instance_path += (next_arg.size() == 1 ? "0" : "") + next_arg + ".json";
            instance_num = next_arg;
        }
        if (arg == "-o")
            solution_path = next_arg;
        if (arg == "-name")
            name = true;
        if (arg == "-s")
            seed = stoi(next_arg);
        if (arg == "-p1")
            param1 = stod(next_arg);
        if (arg == "-p2")
            param2 = stod(next_arg);
        if (arg == "-p3")
            param3 = stod(next_arg);
        if (arg == "-out")
            out_file = next_arg;
    }
}


int main(int argc, char *argv[]) {
    c_start = clock();
    srand(0);
    cout << "==== Reading Data ===" << endl;
    parse_args(argc, argv);
    signal(SIGALRM, catchAlarm);
    alarm(time_limit*60);
    Parser p(instance_path);
    d = p.parseJsonToSchedule();
    cout << "==== Parsed Successfully ====" << endl;

    GeneticSolver solver1(d, param1, param2, param3);

    thread thread1, thread2;

    thread1 = thread([&](AbstractSolver *solver) { solver->solve(); }, &solver1);

    thread1.join();

}
