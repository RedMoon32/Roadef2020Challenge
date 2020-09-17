//
// Created by rinat on 22.08.2020.
//

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
#include "json.h"


using namespace std;

using json = nlohmann::json;


struct Resource {
    string name;
    int id;
    vector<float> max;
    vector<float> min;
};

struct Season {
    string name;
    int id;
    vector<int> times;
};

typedef vector<pair<Resource , vector<vector<float>>>> workloadVec;

struct Intervention {
    string name;
    int id;
    int tmax;
    vector<int> delta;
    workloadVec workload;
    vector<pair<Season &, vector<vector<float>>>> risk;
};

struct Exclusion {
    string name;
    int id;
    Intervention int1;
    Intervention int2;
    Season season;
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

    vector<pair<Resource , vector<vector<float>>>> parseWorkload(vector<Resource> resources, const json &intervention);

    vector<float> parseArray(const json &j);

    vector<int> parseIntArray(const json &j);

    json data;
};

#endif