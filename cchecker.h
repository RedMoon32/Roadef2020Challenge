//
// Created by rinat on 22.08.2020.
//


#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
#include "json.h"


using namespace std;


#define RESOURCES_STR "Resources"
#define SEASONS_STR "Seasons"
#define INTERVENTIONS_STR = "Interventions"
#define EXCLUSIONS_STR "Exclusions"
#define T_STR = "T"
#define SCENARIO_NUMBER = "Scenarios_number"
#define RESOURCE_CHARGE_STR  "workload"
#define TMAX_STR  "tmax"
#define DELTA_STR  "Delta"
#define MAX_STR  "max"
#define MIN_STR  "min"
#define RISK_STR  "risk"
#define START_STR "start"
#define QUANTILE_STR  "Quantile"
#define ALPHA_STR  "Alpha"

nlohmann::json read_file(const string& path);
