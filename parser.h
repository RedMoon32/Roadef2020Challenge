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

    vector<pair<Resource &, vector<vector<int>>>> parseWorkload(vector<Resource> resources, const json &intervention);

    vector<int> parseArray(const json &j);

    json data;
};

#endif