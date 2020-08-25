//
// Created by rinat on 22.08.2020.
//


#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
#include "json.h"


using namespace std;


using json = nlohmann::json;

json readFile(const string& path);
void parseJson(const json& j);