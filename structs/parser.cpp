//
// Created by rinat on 22.08.2020.
//

#include "parser.h"
#include <utility>
#include <map>

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

typedef vector<double> dvect;

vector<Resource> Parser::parseResources() {
    vector<Resource> resources;
    int id = 0;
    for (auto &el : this->data[RESOURCES].items()) {
        auto val = el.value();
        resource_name_mapper[el.key()] = id;
        resources.push_back({el.key(), id, parseArray(val[MAX]), parseArray(val[MIN])});
        id++;
    }
    return resources;
}

vector<Season> Parser::parseSeasons() {
    vector<Season> seasons;
    int id = 0;
    for (auto &el : this->data[SEASONS].items()) {
        auto first = el.key();
        auto second = el.value();
        seasons.push_back({first, id++, parseIntArray(second)});
    }
    return seasons;
}

vector<Intervention> Parser::parseInterventions(vector<Resource> resources) {
    vector<Intervention> interventions;
    int id = 0;
    for (auto &intervention : this->data[INTERVENTIONS].items()) {
        auto cur = intervention.value();
        vector<int> delta = parseIntArray(cur[DELTA]);
        int tmax = stoi(cur[TMAX].get<std::string>());
        auto workload = parseWorkload(resources, cur, tmax);
        auto risk = parseRisk(cur);
        intervention_name_mapper[intervention.key()] = id;
        interventions.push_back({intervention.key(), id, tmax, delta, workload, risk});
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
    auto res = parseIntArray(this->data[SCENARIO_NUMBER]);
    return res;
}

vector<float> Parser::parseArray(const json &j) {
    vector<float> nums;
    for (auto &it: j) {
        auto type = it.type();
        if (type == nlohmann::detail::value_t::string) {
            string s = it.get<std::string>();
            nums.push_back(stod(s));
        }
        else{
            nums.push_back(it);
        }
    }
    return nums;
}

riskVector Parser::parseRisk(const json &intervention){
    riskVector risk_vec;
    // parse risk (iterate over el.value().items()
    for (auto &risk : intervention[RISK].items()) {
        vector<vector<double>> first;
        int last;
        for (auto &t: risk.value().items()) {
            vector<double> second;
            for (auto &tsht: t.value().items()) {
                for (auto& element : tsht.value().items()) {
                    second.push_back((double)element.value());
                }
            }
            last = stoi(t.key())-1;
            first.push_back(second);
        }
        risk_vec.push_back({last, first});
    }
    return risk_vec;
}

workloadVec Parser::parseWorkload(vector<Resource> resources, const json &intervention, int tmax) {
    workloadVec workloads;
    // parse workload (iterate over el.value().items()
    for (auto &workload : intervention[RESOURCE_CHARGE].items()) {
        resourceWorkload workload_by_start_time(tmax+1);
        for (auto &t: workload.value().items()) {
            for (auto &tsht: t.value().items()) {
                int time_when_required = stoi(t.key())-1;
                int time_start = stoi(tsht.key()) - 1;
                workload_by_start_time[time_start].push_back({time_when_required, tsht.value()});
            }
        }
        string name = workload.key();
        int id = resource_name_mapper[name];
        workloads.push_back({resources[id], workload_by_start_time});
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
    return DataInstance{T, Quantile, Alpha, interventions, resources, exclusions, seasons,  scenarious_number};
}

Parser::Parser(const string &path) {
    std::ifstream file(path);
    file >> data;

}

vector<int> Parser::parseIntArray(const json &j) {
    auto res = parseArray(j);
    return vector<int>(res.begin(), res.end());
}


