// ==================================================
//
// Author: GIFTED-BOY
// Email: 120138380@qq.com
// Create Time: 2018.4.23
// Version: v1.0
//
// ==================================================

#include "LogisticRegression.h"
#include <vector>
#include <time.h>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

LogisticRegression::LogisticRegression(Matrix x, Matrix y, int m, double a, int tr)
	: dataSet(x), labels(y), sampleNum(m), alpha(a), trainingRounds(tr) {}

void LogisticRegression::train()
{
	weights = Matrix::ones(dataSet.getColumnNum(), 1);
	//batchGradientDescent();
	stochasticGradientDescent();
}

int LogisticRegression::classify(Matrix x) { return getClass(x); }

void LogisticRegression::batchGradientDescent()
{
	for (int i = 0; i < trainingRounds; i++)
		weights = weights + dataSet.transpose() * (labels - Matrix::sigmoid(dataSet * weights)) * alpha;
}

void LogisticRegression::stochasticGradientDescent()
{
	srand((unsigned) time(NULL));

	vector<int> v(sampleNum);
	for (int i = 0; i < sampleNum; i++) v[i] = i;
	for (int i = 0; i < trainingRounds; i++)
	{
		vector<int> indexes = v;
		for (int j = 0; j < sampleNum; j++)
		{
			alpha = 4.0 / (1 + j + i) + 0.01;
			int r = rand() % indexes.size();
			weights = weights
				+ dataSet.getRowValue(indexes[r]).transpose()
				* (labels.getValue(indexes[r], 0) - Matrix::sigmoid(dataSet.getRowValue(indexes[r]) * weights).getValue(0, 0))
				* alpha;
			swap(indexes[r], indexes[indexes.size() - 1]);
			indexes.pop_back();
		}
	}
}

int LogisticRegression::getClass(Matrix x) { return Matrix::sigmoid(x * weights).getValue(0, 0) >= 0.5 ? 1 : 0; }





void getDataSet(string fileName, vector<vector<double> > &dataSet, vector<double> &labels)
{
	ifstream in("dataset/Ch05/" + fileName);
	string str;
	char *p = NULL;
	while (getline(in, str))
	{
		//cout << str << endl;
		vector<double> x;
		p = strtok((char *) str.c_str(), "\t");
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

void LRTest()
{
	vector<vector<double> > trainingDataSet;
	vector<double> trainingLabels;
	getDataSet("horseColicTraining.txt", trainingDataSet, trainingLabels);
	Matrix trainingX(trainingDataSet, trainingDataSet.size(), trainingDataSet[0].size());
	Matrix trainingY(trainingLabels, trainingLabels.size(), 1);
	LogisticRegression lr(trainingX, trainingY, trainingDataSet.size(), 0.01, 1000);

	vector<vector<double> > testingDataSet;
	vector<double> testingLabels;
	getDataSet("horseColicTest.txt", testingDataSet, testingLabels);
	int testingNum = testingDataSet.size();
	Matrix testingX(testingDataSet, testingNum, testingDataSet[0].size());

	int testTimes = 10;
	double totalCorretRate = 0.0;
	for (int i = 0; i < testTimes; i++)
	{
		cout << "round " << i << ":" << endl;
		lr.train();
		int correctNum = 0;
		for (int j = 0; j < testingNum; j++)
		{
			int cls = lr.classify(testingX.getRowValue(j));
			if (cls == testingLabels[j]) correctNum++;
			cout << "lr: " << cls << "\treal: " << testingLabels[j] << endl;
		}
		cout << "testing num: " << testingNum << endl;
		cout << "correct num: " << correctNum << endl;
		cout << "correct rate: " << (double) correctNum / testingNum << endl << endl;
		totalCorretRate += (double) correctNum / testingNum;
	}
	cout << "avg correct rate: " << totalCorretRate / testTimes << endl;
}