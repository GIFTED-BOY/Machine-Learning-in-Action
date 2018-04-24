// ==================================================
//
// Author: GIFTED-BOY
// Email: 120138380@qq.com
// Create Time: 2018.4.24
// Version: v1.0
//
// ==================================================

#ifndef _ADA_BOOST_H_
#define _ADA_BOOST_H_

#include <vector>
#include "Matrix.h"

using namespace std;

class DecisionStump
{
public:
	int featureIndex;
	double threshold;
	int ineq;
	double alpha;
};

class AdaBoost
{
private:
	vector<DecisionStump> classifiers;
	Matrix dataSet;
	Matrix labels;
	int sampleNum;
	int featureNum;
	int trainingRounds;

public:
	AdaBoost(Matrix x, Matrix y, int m, int n, int tr);

	void train();

	double classify(Matrix x);

private:
	DecisionStump buildStump(Matrix D, Matrix &classEst);
	void buildClassifiers();
	double getClass(Matrix x, int featureIndex, double threshold, int ineq);
};


void getDataSet2(string fileName, vector<vector<double> > &dataSet, vector<double> &labels);
void ABTest();

#endif