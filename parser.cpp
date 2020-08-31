//
// Created by rinat on 22.08.2020.
//

#include "parser.h"
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

vector<Resource> Parser::parseResources() {
    vector<Resource> resources;
    for (auto &el : this->data[RESOURCES].items()) {
        auto val = el.value();
        resources.push_back({el.key(), 0, parseArray(val["max"]), parseArray(val["min"])});
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

vector<Intervention> Parser::parseInterventions(vector<Resource> resources) {
    vector<Intervention> interventions;
    for (auto &intr : this->data[INTERVENTIONS].items()) {
        auto cur = intr.value();
        int tmax = cur["tmax"];
        vector<int> delta = parseArray(cur["Delta"]);
        auto workload = parseWorkload(resources, cur);
        interventions.push_back({intr.key(), 0, tmax, delta, workload});
    }
    return vector<Intervention>();
}

vector<Exclusion> Parser::parseExclusions() {
    vector<Exclusion> exclusions;
    for (auto &exc: this->data[EXCLUSIONS].items()) {
        string first = exc.value()[0].get<std::string>();
        string second = exc.value()[1].get<std::string>();
        string name = exc.value()[2].get<std::string>();
    }
    return vector<Exclusion>();
}

vector<int> Parser::parseScenarious() {
    return parseArray(this->data[SCENARIO_NUMBER]);
}

vector<int> Parser::parseArray(const json &j) {
    vector<int> nums;
    for (auto &it: j)
        nums.push_back(it);
    return nums;
}

vector<pair<Resource &, vector<vector<int>>>>
Parser::parseWorkload(vector<Resource> resources, const json &intervention) {
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
        workloads.push_back(
                pair<Resource &, vector<vector<int>>>(resources[stoi(workload.key())], vector<vector<int>>()));
    }
    return workloads;
}

DataInstance Parser::parseJsonToSchedule() {
    vector<Resource> resources = parseResources();
    vector<Season> seasons = parseSeasons();
    vector<Intervention> interventions = parseInterventions(resources);
    vector<Exclusion> exclusions = parseExclusions();
    vector<int> scenarious_number = parseScenarious();
    int T = data[T];
    double Quantile = data[QUANTILE];
    double Alpha = data[ALPHA];
    return DataInstance{};
}

Parser::Parser(const string &path) {
    std::ifstream file(path);
    file >> data;
}


