#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "parser.h"
#include "json.hpp"

using namespace std;
using namespace nlohmann;

int main() {
    Parser p;
    auto data = "{\"Resources\": { \"c1\": { \"max\": [ 49, 23, 15 ], \"min\": [ 10, 0, 6 ] } }}"_json;
    p.data = data;
    vector<Resource> res = p.parseResources();
    return 0;
}
