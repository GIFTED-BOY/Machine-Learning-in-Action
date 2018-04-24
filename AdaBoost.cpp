// ==================================================
//
// Author: GIFTED-BOY
// Email: 120138380@qq.com
// Create Time: 2018.4.24
// Version: v1.0
//
// ==================================================

#include "AdaBoost.h"
#include <fstream>
#include <string>
#include <iostream>

AdaBoost::AdaBoost(Matrix x, Matrix y, int m, int n, int tr)
	: dataSet(x), labels(y), sampleNum(m), featureNum(n), trainingRounds(tr) {}

void AdaBoost::train() { buildClassifiers(); }

double AdaBoost::classify(Matrix x)
{
	int classifierNum = classifiers.size();
	double clsEst = 0.0;
	for (int i = 0; i < classifierNum; i++)
		clsEst += classifiers[i].alpha * getClass(x, classifiers[i].featureIndex, classifiers[i].threshold, classifiers[i].ineq);
	return clsEst >= 0 ? 1 : -1;
}

DecisionStump AdaBoost::buildStump(Matrix D, Matrix &classEst)
{
	int steps = 10;
	DecisionStump ds;
	double minError = LDBL_MAX;
	for (int i = 0; i < featureNum; i++)
	{
		double minValue = dataSet.getColumnMinValue(i);
		double maxValue = dataSet.getColumnMaxValue(i);
		double stepSize = (maxValue - minValue) / steps;
		for (int j = -1; j <= steps; j++)
		{
			for (int k = 0; k <= 1; k++)
			{
				double threshold = minValue + stepSize * j;
				Matrix predictedLabels(sampleNum, 1);
				double weightedError = 0.0;
				for (int p = 0; p < sampleNum; p++)
				{
					double cls = getClass(dataSet.getRowValue(p), i, threshold, k);
					weightedError += D.getValue(p, 0) * (cls != labels.getValue(p, 0));
					predictedLabels.setValue(p, 0, cls);
				}
				if (weightedError < minError)
				{
					minError = weightedError;
					classEst = predictedLabels;
					ds.featureIndex = i;
					ds.threshold = threshold;
					ds.ineq = k;
					ds.alpha = 0.5 * log((1 - minError) / (0 == minError ? 1e-16 : minError));
				}
			}
		}
	}
	return ds;
}

void AdaBoost::buildClassifiers()
{
	Matrix D = Matrix::ones(sampleNum, 1) * (1.0 / sampleNum);
	Matrix aggClassEst(sampleNum, 1);
	for (int i = 0; i < trainingRounds; i++)
	{
		Matrix classEst(sampleNum, 1);
		DecisionStump ds = buildStump(D, classEst);
		classifiers.push_back(ds);
		Matrix expon = multiply(labels * -ds.alpha, classEst);
		D = multiply(D, exp(expon));
		D = D / D.columnSum(0);
		aggClassEst = aggClassEst + classEst * ds.alpha;
		Matrix s = sign(aggClassEst);
		int errorClassNum = 0;
		for (int j = 0; j < sampleNum; j++) errorClassNum += (s.getValue(j, 0) != labels.getValue(j, 0));
		double errorRate = (double) errorClassNum / sampleNum;
		if (0 == errorRate) break;
	}
}

double AdaBoost::getClass(Matrix x, int featureIndex, double threshold, int ineq)
{
	if (0 == ineq) return x.getValue(0, featureIndex) <= threshold ? -1 : 1;
	return x.getValue(0, featureIndex) > threshold ? -1 : 1;
}





void getDataSet2(string fileName, vector<vector<double> > &dataSet, vector<double> &labels)
{
	ifstream in("dataset/Ch07/" + fileName);
	string str;
	char *p = NULL;
	while (getline(in, str))
	{
		//cout << str << endl;
		vector<double> x;
		p = strtok((char *)str.c_str(), "\t");
		while (p != NULL)
		{
			x.push_back(atof(p));
			p = strtok(NULL, "\t");
		}
		labels.push_back(x[x.size() - 1]);
		x.pop_back();
		dataSet.push_back(x);
	}
	delete[] p;
	in.close();
}

void ABTest()
{
	vector<vector<double> > trainingDataSet;
	vector<double> trainingLabels;
	getDataSet2("horseColicTraining2.txt", trainingDataSet, trainingLabels);
	Matrix trainingX(trainingDataSet, trainingDataSet.size(), trainingDataSet[0].size());
	Matrix trainingY(trainingLabels, trainingLabels.size(), 1);
	AdaBoost ab(trainingX, trainingY, trainingDataSet.size(), trainingDataSet[0].size(), 10);

	vector<vector<double> > testingDataSet;
	vector<double> testingLabels;
	getDataSet2("horseColicTest2.txt", testingDataSet, testingLabels);
	int testingNum = testingDataSet.size();
	Matrix testingX(testingDataSet, testingNum, testingDataSet[0].size());

	ab.train();
	int correctNum = 0;
	for (int j = 0; j < testingNum; j++)
	{
		double cls = ab.classify(testingX.getRowValue(j));
		if (cls == testingLabels[j]) correctNum++;
		cout << "ab: " << cls << "\treal: " << testingLabels[j] << endl;
	}
	cout << "testing num: " << testingNum << endl;
	cout << "correct num: " << correctNum << endl;
	cout << "correct rate: " << (double) correctNum / testingNum << endl << endl;
}