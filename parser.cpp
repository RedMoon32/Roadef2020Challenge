//
// Created by rinat on 22.08.2020.
//

#include "parser.h"
#include <utility>
#include <map>

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

map<string, int> resource_name_mapper;
map<string, int> intervention_name_mapper;

vector<Resource> Parser::parseResources() {
    vector<Resource> resources;
    int id = 0;
    for (auto &el : this->data[RESOURCES].items()) {
        auto val = el.value();
        resource_name_mapper[el.key()] = id;
        resources.push_back({el.key(), id, parseArray(val["max"]), parseArray(val["min"])});
        id++;
    }
    return resources;
}

vector<Season> Parser::parseSeasons() {
    vector<Season> seasons;
    for (auto &el : this->data[SEASONS].items()) {
        auto first = el.key();
        auto second = el.value();
        seasons.push_back({first, 0, parseArray(second)});
    }
    return seasons;
}

vector<Intervention> Parser::parseInterventions(vector<Resource> resources) {
    vector<Intervention> interventions;
    int id = 0;
    for (auto &intr : this->data[INTERVENTIONS].items()) {
        auto cur = intr.value();
        int tmax = stoi(cur["tmax"].get<std::string>());
        vector<int> delta = parseArray(cur["Delta"]);
        vector<pair<Resource, vector<vector<int>>>> workload = parseWorkload(resources, cur);
        intervention_name_mapper[intr.key()] = id;
        interventions.push_back({intr.key(), id, tmax, delta, workload});
        id += 1;
    }
    return interventions;
}


vector<Exclusion> Parser::parseExclusions(vector<Intervention> interventions, vector<Season> seasons) {
    vector<Exclusion> exclusions;
    for (auto &exc: this->data[EXCLUSIONS].items()) {
        int first = intervention_name_mapper[exc.value()[0].get<std::string>()];
        int second = intervention_name_mapper[exc.value()[1].get<std::string>()];
        string name = exc.value()[2].get<std::string>();
        // remake with references and hash map search
        for (const auto &season: seasons) {
            if (season.name == name) {
                exclusions.push_back({exc.key(), 0, interventions[first], interventions[second], season});
                break;
            }
        }
    }
    return exclusions;
}

vector<int> Parser::parseScenarious() {
    return parseArray(this->data[SCENARIO_NUMBER]);
}

vector<int> Parser::parseArray(const json &j) {
    vector<int> nums;
    for (auto &it: j) {
        auto type = it.type();
        if (type == nlohmann::detail::value_t::string) {
            string s = it.get<std::string>();
            nums.push_back(stoi(s));
        }
        else{
            nums.push_back(it);
        }
    }
    return nums;
}

vector<pair<Resource , vector<vector<int>>>>
Parser::parseWorkload(vector<Resource> resources, const json &intervention) {
    vector<pair<Resource , vector<vector<int>>>> workloads;
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
        string name = workload.key();
        int id = resource_name_mapper[name];
        workloads.push_back(
                pair<Resource , vector<vector<int>>>(resources[id], first));
    }
    return workloads;
}

DataInstance Parser::parseJsonToSchedule() {
    vector<Resource> resources = parseResources();
    vector<Season> seasons = parseSeasons();
    vector<Intervention> interventions = parseInterventions(resources);
    vector<Exclusion> exclusions = parseExclusions(interventions, seasons);
    vector<int> scenarious_number = parseScenarious();
    int T = data[T_STR].get<int>();
    double Quantile = data[QUANTILE];
    double Alpha = data[ALPHA];
    return DataInstance{T, Quantile, Alpha, interventions, resources, exclusions, scenarious_number};
}

Parser::Parser(const string &path) {
    std::ifstream file(path);
    file >> data;
}


