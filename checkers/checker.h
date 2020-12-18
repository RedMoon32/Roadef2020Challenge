//
// Created by rinat on 9/2/2020.
//

#ifndef ROADEF2020CHALLENGE_CHECKER_H
#define ROADEF2020CHALLENGE_CHECKER_H

#include "parser.h"
#include "abstract_checker.h"

class Checker : public AbstractChecker {
public:
    Checker(const DataInstance &data);
    Checker(const vector<int>& schedule, const DataInstance & data);

    int checkAll(vector<int> schedule) override;

    int checkResourceConstraint() override;

    int checkHorizon() override;

    int checkExclusions() override;

    vector<vector<double>> computeRiskDistribution();
    vector<double> computeMeanRisk(const vector<vector<double>> &risk);
    vector<double> computeQuantile(vector<vector<double>> &risk);
    double computeMetric();

    vector<int> wrong_exclusion;
    vector<int> wrong_resource_intervention;

    vector<int> schedule;
    vector<vector<float>> resource_consumption;
    DataInstance data;

    double computeObjective1(vector<double> mean_risk);

    double computeObjective2(vector<double> q, vector<double> mean_risk);

    tuple<vector<double>, vector<double>> getRiskDistribution();
};

class FastChecker: public Checker {
public:
    using Checker::Checker;
    int checkResourceConstraint() override;
    vector<int> computeChange();
    void setConsumption(int resource_id, int time, double value);
    vector<int> prevSchedule;
    vector<vector<pair<bool, bool>>> wrongResource;
    int prevWrong;
};
#endif //ROADEF2020CHALLENGE_CHECKER_H
