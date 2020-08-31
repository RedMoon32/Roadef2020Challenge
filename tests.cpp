//
// Created by rinat on 8/27/2020.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "parser.h"
#include "catch.hpp"

Parser p;

#define RESOURCES_STR

TEST_CASE("Resources are parsed correctly", "[parser]") {
    auto data = "{\"Resources\": { \"c1\": { \"max\": [ 49, 23, 15 ], \"min\": [ 10, 0, 6 ] } }}"_json;
    p.data = data;
    vector<Resource> res = p.parseResources();
    REQUIRE(res.size() == 1);
    REQUIRE(res[0].name == "c1");
    REQUIRE(res[0].max == vector<int>{49, 23, 15});
    REQUIRE(res[0].min == vector<int>{10, 0, 6});
}

TEST_CASE("Seasons are parsed correctly", "[parser]") {
    auto data = "{\"Seasons\": { \"full\": [ 1, 2, 3 ] , \"winter\": [ 1, 3 ]}}"_json;
    p.data = data;
    vector<Season> res = p.parseSeasons();
    REQUIRE(res.size() == 2);
    REQUIRE(res[0].name == "full");
    REQUIRE(res[0].times == vector<int>{1, 2, 3});
    REQUIRE(res[1].name == "winter");
    REQUIRE(res[1].times == vector<int>{1, 3});
}

TEST_CASE("Interventions are parsed correctly", "[parser]"){

}