//
// Created by rinat on 8/27/2020.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <string>
#include <parser.h>
#include <catch.hpp>
#include <checker.h>

Parser p;


TEST_CASE("Resources are parsed correctly", "[structs]") {
    p.data = "{\"Resources\": { \"c1\": { \"max\": [ 49, 23, 15 ], \"min\": [ 10, 0, 6 ] } }}"_json;
    vector<Resource> res = p.parseResources();
    REQUIRE(res.size() == 1);
    REQUIRE(res[0].name == "c1");
    REQUIRE(res[0].max == vector<float>{49, 23, 15});
    REQUIRE(res[0].min == vector<float>{10, 0, 6});
}

TEST_CASE("Seasons are parsed correctly", "[structs]") {
    p.data = "{\"Seasons\": { \"full\": [ 1, 2, 3 ] , \"winter\": [ 1, 3 ]}}"_json;
    vector<Season> res = p.parseSeasons();
    REQUIRE(res.size() == 2);
    REQUIRE(res[0].name == "full");
    REQUIRE(res[0].times == vector<int>{1, 2, 3});
    REQUIRE(res[1].name == "winter");
    REQUIRE(res[1].times == vector<int>{1, 3});
}

TEST_CASE("Interventions are parsed correctly", "[structs]") {
    vector<Resource> resources = {{"c1", 0, {49, 23, 15}, {10, 0, 6}}};
    p.data = "{\"Interventions\": { \"I1\": { \"tmax\": \"1\", \"Delta\": [ 3, 3, 2 ], \"workload\": {\"c1\": { \"1\": { \"1\": 31 }, \"2\": { \"1\": 0 }, \"3\": { \"1\": 8 } }} }}}"_json;
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
        //REQUIRE(w1.second == vector<vector<double>>{{31, 0, 8}});
    }
}

TEST_CASE("Exclusions are parsed correctly", "[structs]") {
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

TEST_CASE("scenarious are parsed correctly", "[structs]") {
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
    Checker c(d);
    c.schedule = vector<int>{5, 7, 2, 9};
    REQUIRE(c.checkHorizon() == 2);
}

TEST_CASE("Risk distribution is parsed correctly") {
    p.data = "{\"risk\":{ \"1\":{ \"1\":[ 1.1, 2.2, 3.3, 4.4, 5.5, 6.6 ]}, \"2\":{ \"1\":[ 1.11, 2.22, 3.33], \"2\":[ 1.2, 2.74, 3.94, 4.58, 5.62, 6.39, 7.7 ]}}}"_json;
    riskVector risk = p.parseRisk(p.data);
    REQUIRE(risk.size() == 2);
    REQUIRE(risk[0].second.size() == 1);

    REQUIRE((risk[0].second[0].size() == 6 && risk[1].second[0].size() == 3 && risk[1].second[1].size() == 7));
    REQUIRE(risk[1].second[0] == vector<double>{1.11, 2.22, 3.33});
}

TEST_CASE("Resource consumption is correct") {
//    vector<Resource> resources = {{"c1", 0, {20, 30, 30}, {10, 20, 20}},
//                                  {"c2", 1, {10, 20, 20}, {10, 0,  10}}};
//
//    workloadVec workload1{{resources[0], {{20, 20, 20}, {5, 5}, {10, 10}}}};
//
//    workloadVec workload2{{resources[1], {{10, 10,}, {5, 9}, {10, 10}}}};
//
//    workloadVec workload3{{resources[1], {{5}, {1}, {2}}}};
//
//    Intervention int1 = {.delta = {3, 2, 2}, .workload = workload1};
//    Intervention int2 = {.delta = {2, 2, 2}, .workload = workload2};
//    Intervention int3 = {.delta = {1, 1, 1}, .workload = workload3};
//    vector<Intervention> interventions = {int1, int2, int3};
//
//    DataInstance d{.T = 3, .interventions = interventions, .resources = resources,};
//
//    SECTION("First slot") {
//        Checker c(vector<int>{0, 0, 0}, d);
//        int res = c.checkResourceConstraint();
//        REQUIRE(res == 2);
//    }
//
//    SECTION("Second slot") {
//        Checker c(vector<int>{0, 1, 1}, d);
//        int res = c.checkResourceConstraint();
//        REQUIRE(res == 2);
//    }
}

TEST_CASE("Exclusions are correct") {
    Intervention int1 = {.id = 0};
    Intervention int2 = {.id = 1};
    Intervention int3 = {.id = 2};

    Season s1 = {.name = "first", .times=vector<int>{0, 1}};
    Season s2 = {.name = "second", .times=vector<int>{2}};

    Exclusion exc1 = {.id = 0, .int1 = int1, .int2 = int2, .season = s1};
    Exclusion exc2 = {.id = 1, .int1 = int2, .int2 = int3, .season = s2};
    vector<Exclusion> exclusions = {exc1, exc2};

    DataInstance d{.exclusions = exclusions};

    Checker c(vector<int>{0, 1, 2}, d);
    int res = c.checkExclusions();
    REQUIRE(res == 1);

    c.schedule = vector<int>{2, 0, 0};
    res = c.checkExclusions();
    REQUIRE(res == 0);

    c.schedule = vector<int>{0, 2, 2};
    res = c.checkExclusions();
    REQUIRE(res == 1);
}

TEST_CASE("Objective function is computed correctly"){
    Parser parser("../A_set/A_09.json");
    auto d = parser.parseJsonToSchedule();
    vector<int> schedule {2, 10, 4, 14, 6, 1, 1, 12, 3, 12, 13, 6, 8, 4, 9, 15, 7, 15};
    for (auto& inter:schedule)
        inter--;
    Checker c(schedule, d);
    double res = c.computeMetric();
    REQUIRE(abs(res - 1772.57) < 0.01);
}

TEST_CASE("cc"){
    Parser parser("../A_set/A_04.json");
    auto data = parser.parseJsonToSchedule();
    ifstream schedule_06_wrong("../outputs/output_04.txt");
    vector<int> schedule;
    for (int i = 0; i < data.interventions.size(); i++)
    {
        int time;
        schedule_06_wrong >> time;
        schedule.push_back(time-1);
    }
    Checker checker(data);
    cout << checker.checkAll(schedule) << endl;
    cout << "done" << endl;
}