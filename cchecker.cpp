//
// Created by rinat on 22.08.2020.
//

#include "cchecker.h"

using namespace nlohmann;


struct Resource {
    string name;
    int id;
    vector<int> max;
    vector<int> min;
};

struct Season {
    string name;
    int id;
    vector<int> times;
};

struct Intervention {
    string name;
    int id;
    int tmax;
    vector<int> delta;
    vector<pair<Resource &, vector<pair<Season &, vector<int>>>>> workload;
    vector<pair<Season&, vector<vector<int>>>> risk;
};

struct Exclusion{
    string name;
    int id;
    Intervention& int1;
    Intervention& int2;
    Season& season;
};

struct DataInstance {
    int T;
    double Quantile;
    double Alpha;

    vector<Intervention> interventions;
    vector<Resource> resources;
    vector<Exclusion> exclusions;
    vector<int> scenarious_number;
};

json read_file(const string &path) {
    std::ifstream file(path);
    json j;
    file >> j;
    return j;
}

/**
def read_solution_from_txt(Instance: dict, solution_filename: str):
    """Read a txt formated Solution file, and store the solution informations in Instance"""

    print('Loading solution from ' + solution_filename + '...')
    # Load interventions
    Interventions = Instance[INTERVENTIONS_STR]
    # Read file line by line, and store starting time value (no checks yet, except format and duplicate)
    solution_file = open(solution_filename, 'r')
    for line in solution_file:
        # Split line to retrive infos: Intervention name and decided starting date
        tmp = line.split(' ')
        intervention_name = tmp[0]
        start_time_str = tmp[1].split('\n')[0]
        # Assert Intervention exists
        if not intervention_name in Interventions:
            print('ERROR: Unexpected Intervention ' + intervention_name + ' in solution file ' + solution_filename + '.')
            continue
        # Assert starting date is an integer
        start_time: int
        try:
            start_time = int(start_time_str)
        except ValueError:
            print('ERROR: Unexpected starting time ' + start_time_str + ' for Intervention ' + intervention_name + '. Expect integer value.')
            continue
        # Assert no duplicate
        if START_STR in Interventions[intervention_name]:
            print('ERROR: Duplicate entry for Intervention ' + intervention_name + '. Only first read value is being considered.')
            continue
        # Store starting time
        Interventions[intervention_name][START_STR] = start_time
    solution_file.close()
    print('Done')
*/