//
// Created by rinat on 9/2/2020.
//

#ifndef ROADEF2020CHALLENGE_CHECKER_H
#define ROADEF2020CHALLENGE_CHECKER_H

#include "parser.h"
#include "abstract_checker.h"

class Checker : public AbstractChecker {
public:
    Checker(vector<int> schedule, const DataInstance &data);

    int checkAll() override;

    int checkResourceConstraint() override;

    int checkHorizon() override;

    int checkExclusions() override;

    vector<vector<double>> computeRiskDistribution();
    vector<double> computeMeanRisk(const vector<vector<double>> &risk);
    vector<double> computeQuantile(vector<vector<double>> &risk);
    double computeMetric();

    vector<int> schedule;
    vector<vector<float>> resource_consumption;
    const DataInstance &data;

    double computeObjective1(vector<double> mean_risk);

    double computeObjective2(vector<double> q, vector<double> mean_risk);

    tuple<vector<double>, vector<double>> getRiskDistribution();
};


#endif //ROADEF2020CHALLENGE_CHECKER_H
