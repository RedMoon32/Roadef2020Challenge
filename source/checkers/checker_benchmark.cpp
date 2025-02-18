#include <bits/stdc++.h>
#include <time.h>
#include <unistd.h>

#include "random_solver.h"
#include "hill_climbing.h"
#include "parser.h"
#include "genetic_solver.h"

using namespace std;
using namespace nlohmann;
using namespace std::chrono;

DataInstance d;
int time_limit = 15;
string base_path = "../B_set_rounded/B_";
string instance_path;
string instance_num;
string solution_path;
bool name;
int seed;
int timeid = 70;
string out_file;
int instance_number;
double param1 = 1;
double param2 = -1;
double param3 = -1;
string solver;

clock_t c_start;

void write_cur_result(int minute) {
    cout << "Writing best found solution" << endl;
    solution_lock.lock();
    write_result(solution_path + "_m" + to_string(minute), best_solution, d.interventions);

    ofstream outfile;
    outfile.open(out_file, ios_base::app);
    auto best_score = Checker(d).checkAll(best_solution);

    auto score_normalized = best_score < 0 ? best_score + 500000 : best_score;

    outfile << minute << "," << base_path << "," << instance_num << "," << d.interventions.size() << "," << d.resources.size() << ","
            << d.exclusions.size() << "," << d.scenarious_number.size() << ","
            << param1 << ","
            << param2 << "," << param3 << "," << time_limit << ","
            << score_normalized << ","
            << (best_score < 0)
            << endl;

    cout << "# Minute: " << minute << ", Fitness: " << score_normalized << endl << endl;

    outfile.close();
    solution_lock.unlock();
}

//  executable -t time_limit -p instance_name -o new_solution_filename -name -s seed
void parse_args(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
        string arg = argv[i];
        string next_arg = (i < argc - 1 ? argv[i + 1] : "");

        if (arg == "-b")
            base_path = next_arg;

        if (arg == "-t")
            time_limit = stoi(next_arg);

        if (arg == "-p") {
            instance_path = base_path + (next_arg.size() == 1 ? "0" : "") + next_arg + ".json";
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

        if (arg == "-solver")
            solver = next_arg;
    }
}


int main(int argc, char *argv[]) {
    c_start = clock();
    //srand(0);
    parse_args(argc, argv);
    Parser p(instance_path);
    d = p.parseJsonToSchedule();

    //GeneticSolver solver1(d, param1, param2, param3);
    HillClimbingSASolver SA(d, param1, param2, param3, true);
    GeneticSolver GA(d, param1, param2, param3);
    HillClimbingSASolver HC(d, param1, param2, param3, false);

    thread thread1, thread2;

    cout << "Computing solution......." << endl;

    if (solver == "sa")
        thread1 = thread([&](AbstractSolver *solver) { solver->solve(); }, &SA);
    else if (solver == "ga")
        thread1 = thread([&](AbstractSolver *solver) { solver->solve(); }, &GA);
    else if (solver == "hc")
        thread1 = thread([&](AbstractSolver *solver) { solver->solve(); }, &HC);
    else{
        cout << "WRONG SOLVER" << endl;
        exit(-1);
    }

    for (int i = 0; i < time_limit; i++) {
        sleep(60);
        write_cur_result(i + 1);
    }
    cout << "exiting...." << endl;
    exit_ = true;
    sleep(10);
    thread1.detach();
}
