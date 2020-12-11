//
// Created by rinat on 22.08.2020.
//

#ifndef PARSER_H
#define PARSER_H

#include "json.hpp"
#include "structs.h"

using namespace std;

using json = nlohmann::ordered_json;

class Parser {
public:
    Parser() = default;
    Parser(const string &path);

    DataInstance parseJsonToSchedule();

    vector<Resource> parseResources();

    vector<Season> parseSeasons();

    vector<Intervention> parseInterventions(vector<Resource> resources);

    vector<Exclusion> parseExclusions(vector<Intervention> interventions, vector<Season> seasons);

    vector<int> parseScenarious();

    workloadVec parseWorkload(vector<Resource> resources, const json &intervention, int tmax);

    riskVector parseRisk(const json &intervention);

    vector<float> parseArray(const json &j);

    vector<int> parseIntArray(const json &j);

    json data;

};

#endif