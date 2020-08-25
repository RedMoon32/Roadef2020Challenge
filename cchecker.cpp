//
// Created by rinat on 22.08.2020.
//

#include "cchecker.h"
#include <utility>

using namespace nlohmann;

#define RESOURCES "Resources"
#define SEASONS "Seasons"
#define INTERVENTIONS "Interventions"
#define EXCLUSIONS "Exclusions"
#define T_STR  "T"
#define SCENARIO_NUMBER  "Scenarios_number"
#define RESOURCE_CHARGE  "workload"
#define TMAX  "tmax"
#define DELTA  "Delta"
#define MAX  "max"
#define MIN  "min"
#define RISK  "risk"
#define START "start"
#define QUANTILE  "Quantile"
#define ALPHA  "Alpha"

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
    vector<pair<Resource &, vector<vector<int>>>> workload;
    vector<pair<Season &, vector<vector<int>>>> risk;
};

struct Exclusion {
    string name;
    int id;
    Intervention &int1;
    Intervention &int2;
    Season &season;
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

void parseJson(const json &j);

class Parser {
public:
    vector<Resource> parseResources();

    vector<Season> parseSeasons();

    vector<Intervention> parseInterventions();

    vector<Exclusion> parseExclusions();

    vector<int> parseScenarious();

    vector<pair<Resource &, vector<vector<int>>>> parseWorkload(vector<Resource> resources, const json& intervention);

    vector<int> parseArray(const json &j);

private:
    const json &data;
};

vector<Resource> Parser::parseResources() {
    vector<Resource> resources;
    for (auto &el : this->data[RESOURCES].items()) {
        for (auto &c: el.value().items()) {
            resources.push_back({el.key(), 0, parseArray(c.value()["max"]), parseArray(c.value()["min"])});
        }
    }
    return resources;
}

vector<Season> Parser::parseSeasons() {
    vector<Season> seasons;
    for (auto &el : this->data[SEASONS].items()) {
        seasons.push_back({el.key(), 0, parseArray(el.value())});
    }
    return seasons;
}

vector<Intervention> Parser::parseInterventions() {
    vector<Intervention> interventions;
    return vector<Intervention>();
}

vector<Exclusion> Parser::parseExclusions() {
    return vector<Exclusion>();
}

vector<int> Parser::parseScenarious() {
    return vector<int>();
}

vector<int> Parser::parseArray(const json &j) {
    vector<int> nums;
    for (auto &it: j)
        nums.push_back(it);
    return nums;
}

vector<pair<Resource &, vector<vector<int>>>> Parser::parseWorkload(vector<Resource> resources, const json& intervention) {
    vector<pair<Resource &, vector<vector<int>>>> workloads;
        // parse workload (iterate over el.value().items()
    for (auto &workload : intervention["workload"].items()) {
        vector<vector<int>> first;
        for (auto &t: workload.value().items()) {
            vector<int> second;
            for (auto &tsht: t.value().items()) {
                second.push_back(tsht.value());
            }
            first.push_back(second);
        }
        workloads.push_back(make_pair(resources[stoi(workload.key())],first));
    }
    return workloads;
}

json readFile(const string &path) {
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