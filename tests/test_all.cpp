//
// Created by rinat on 8/27/2020.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "../parser.h"
#include "../catch.hpp"
#include "../checker.h"
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

    }SECTION(" Workload parsed correctly") {

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

    SECTION("Second exclusion parsed correctly") {
        REQUIRE(res[1].name == "E2");
        REQUIRE(res[1].season.name == seasons[0].name);
    }
}

TEST_CASE("scenarious are parsed correctly", "[parser]") {
    p.data = "{\"Scenarios_number\": [ 3, 1, 2 ] }"_json;
    vector<int> scenarious = p.parseScenarious();
    REQUIRE(scenarious.size() == 3);
    REQUIRE(scenarious == vector<int>{3, 1, 2});
}


TEST_CASE("Time horizon is correct") {
    vector<Intervention> interventions = {Intervention{.tmax = 5},
                                          Intervention{.tmax = 6},
                                          Intervention{.tmax = 4},
                                          Intervention{.tmax = 7}};

    DataInstance d{.interventions = interventions,};
    Checker c(vector<int>{5, 7, 2, 9}, d);
    REQUIRE(c.checkHorizon() == 2);
}

TEST_CASE("Resource consumption is correct") {
    vector<Resource> resources = {{"c1", 0, {20, 30, 30}, {10, 20, 20}},
                                  {"c2", 1, {10, 20, 20}, {10, 0,  10}}};

    workloadVec workload1{pair<Resource &, vector<vector<int>>>(resources[0], vector<vector<int>>{{10,},
                                                                                                  {10, 30,},
                                                                                                  {20, 10, 10}})};

    workloadVec workload2{pair<Resource &, vector<vector<int>>>(resources[1], vector<vector<int>>{{0,},
                                                                                                  {10, 20,},})};

    workloadVec workload3{pair<Resource &, vector<vector<int>>>(resources[1], vector<vector<int>>{{0,  0, 10},
                                                                                                  {10, 20,},})};

    Intervention int1 = {.workload = workload1};
    Intervention int2 = {.workload = workload2};
    Intervention int3 = {.workload = workload3};
    vector<Intervention> interventions = {int1, int2, int3};

    DataInstance d{.T = 3, .interventions = interventions, .resources = resources, };
    Checker c(vector<int>{0, 1, 1}, d);

    int res = c.checkResourceConstraint();
    REQUIRE(res == 4);
}

TEST_CASE("Exclusions are correct") {
    Intervention int1 = {.id = 0};
    Intervention int2 = {.id = 1};
    Intervention int3 = {.id = 2};

    Season s1 =  {.name = "first", .times=vector<int>{0, 1}};
    Season s2 =  {.name = "second", .times=vector<int>{2}};

    Exclusion exc1 = {.id = 0, .int1 = int1, .int2 = int2, s1};
    Exclusion exc2 = {.id = 1, .int1 = int2, .int2 = int3, s2};
    vector<Exclusion> exclusions = {exc1, exc2};

    DataInstance d{.exclusions = exclusions};

    Checker c(vector<int>{0, 1, 2}, d);
    int res = c.checkExclusions();
    REQUIRE(res == 1);

    c.schedule = vector<int> {2, 0, 0};
    res = c.checkExclusions();
    REQUIRE(res == 0);

    c.schedule = vector<int> {0, 2, 2};
    res = c.checkExclusions();
    REQUIRE(res == 1);

}
