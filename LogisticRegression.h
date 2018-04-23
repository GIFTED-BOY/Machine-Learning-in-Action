// ==================================================
//
// Author: GIFTED-BOY
// Email: 120138380@qq.com
// Create Time: 2018.4.23
// Version: v1.0
//
// ==================================================

#ifndef _LOGISTIC_REGRESSION_H_
#define _LOGISTIC_REGRESSION_H_

#include "Matrix.h"

class LogisticRegression
{
private:
	Matrix dataSet;
	Matrix labels;
	Matrix weights;
	int sampleNum;
	double alpha;
	int trainingRounds;

public:
	LogisticRegression(Matrix x, Matrix y, int m, double a, int tr);

	void train();

	int classify(Matrix x);

private:
	void batchGradientDescent();
	void stochasticGradientDescent();
	int getClass(Matrix x);
};


void getDataSet(string fileName, vector<vector<double> > &dataSet, vector<double> &labels);
void LRTest();

#endif