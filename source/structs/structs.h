//
// Created by rinat on 10/8/2020.
//

#ifndef ROADEF2020CHALLENGE_STRUCTS_H
#define ROADEF2020CHALLENGE_STRUCTS_H

#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>

using namespace std;

typedef vector<pair<int, vector<vector<double>>>> riskVector;

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

typedef vector<vector<pair<int, double>>> resourceWorkload;
typedef vector<pair<Resource, resourceWorkload>> workloadVec;

struct Intervention {
    string name;
    int id;
    int tmax;
    vector<int> delta;
    workloadVec workload;
    riskVector risk;
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
    vector<Season> seasons;
    vector<int> scenarious_number;

};

#endif //ROADEF2020CHALLENGE_STRUCTS_H
