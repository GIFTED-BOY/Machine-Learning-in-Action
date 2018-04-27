// ==================================================
//
// Author: GIFTED-BOY
// Email: 120138380@qq.com
// Create Time: 2018.4.21
// Version: v1.0
//
// ==================================================

#ifndef _KNN_H_
#define _KNN_H_

#include <vector>
#include <string>
#include "Matrix.h"

using namespace std;

class KNN
{
private:
	Matrix dataSet;
	Matrix labels;
	int featureNum;
	int sampleNum;

public:
	KNN(Matrix x, Matrix y, int m, int n);

	double classify(Matrix x, int k);

private:
	vector<double> getKNN(Matrix x, int k);
	double getClass(vector<double> knn, int k);
};


void getDataSet_KNN(vector<vector<double> > &dataSet, vector<double> &labels);
void normalize(vector<vector<double> > &dataSet, int sampleNum, int featureNum);
void getDataSet_KNN(string dir, vector<vector<double> > &dataSet, vector<double> &labels);
void KNNTest1();
void KNNTest2();
void test(vector<vector<double> > trainingData, vector<double> trainingLabels, vector<vector<double> > testingData, vector<double> testingLabels);


#endif
