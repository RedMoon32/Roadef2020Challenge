//
// Created by rinat on 8/27/2020.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "parser.h"
#include "catch.hpp"
#include <string>

Parser p;

#define RESOURCES_STR

TEST_CASE("Resources are parsed correctly", "[parser]") {
    p.data = "{\"Resources\": { \"c1\": { \"max\": [ 49, 23, 15 ], \"min\": [ 10, 0, 6 ] } }}"_json;
    vector<Resource> res = p.parseResources();
    REQUIRE(res.size() == 1);
    REQUIRE(res[0].name == "c1");
    REQUIRE(res[0].max == vector<int>{49, 23, 15});
    REQUIRE(res[0].min == vector<int>{10, 0, 6});
}

TEST_CASE("Seasons are parsed correctly", "[parser]") {
    p.data = "{\"Seasons\": { \"full\": [ 1, 2, 3 ] , \"winter\": [ 1, 3 ]}}"_json;
    vector<Season> res = p.parseSeasons();
    REQUIRE(res.size() == 2);
    REQUIRE(res[0].name == "full");
    REQUIRE(res[0].times == vector<int>{1, 2, 3});
    REQUIRE(res[1].name == "winter");
    REQUIRE(res[1].times == vector<int>{1, 3});
}

TEST_CASE("Interventions are parsed correctly", "[parser]") {
    vector<Resource> resources = {{"c1", 0, {49, 23, 15}, {10, 0, 6}}};
    p.data = "{\"Interventions\": { \"I1\": { \"tmax\": 1, \"Delta\": [ 3, 3, 2 ], \"workload\": {\"c1\": { \"1\": { \"1\": 31 }, \"2\": { \"1\": 0 }, \"3\": { \"1\": 8 } }} }}}"_json;
    vector<Intervention> res = p.parseInterventions(resources);
    REQUIRE(res.size() == 1);
    auto i1 = res[0];

    SECTION("General fields are correct") {

        REQUIRE(i1.name == "I1");
        REQUIRE(i1.tmax == 1);
        REQUIRE(i1.delta == vector<int>{3, 3, 2});

    }
    SECTION(" Workload parsed correctly") {

        REQUIRE(i1.workload.size() == 1);
        auto w1 = i1.workload[0];
        REQUIRE(w1.first.name == "c1");
        REQUIRE(w1.second == vector<vector<int>>{{31},
                                                 {0},
                                                 {8}});
    }
}

TEST_CASE("Exclusions are parsed correctly", "[parser]") {
    vector<Intervention> interventions = {Intervention(), Intervention(), Intervention()};
    vector<Season> seasons = {{"winter"},
                              {"full"}};
    p.data = "{\"Exclusions\": { \"E1\": [ \"I2\", \"I3\", \"full\" ], \"E2\": [ \"I1\", \"I3\", \"winter\" ] }}"_json;
    vector<Exclusion> res = p.parseExclusions(interventions, seasons);
    REQUIRE(res.size() == 2);

    SECTION("First exclusion parsed correctly") {
        REQUIRE(res[0].name == "E1");
        REQUIRE(res[0].season.name == seasons[1].name);
    }

    SECTION("Second exclusion parsed correctly"){
        REQUIRE(res[1].name == "E2");
        REQUIRE(res[1].season.name == seasons[0].name);
    }
}