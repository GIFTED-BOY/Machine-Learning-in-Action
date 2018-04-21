#ifndef _KNN_H_
#define _KNN_H_

#include <vector>
#include <algorithm>
#include <iostream>
#include <io.h>
#include <fstream>
#include <string>

using namespace std;

class KNN
{
private:
	vector<vector<double> > dataSet;
	vector<int> labels;
	int featureNum;
	int sampleNum;

public:
	KNN(vector<vector<double> > x, vector<int> y, int m, int n);

	int classify(vector<double> x, int k);

private:
	vector<int> getKNN(vector<double> x, int k);
	int getClass(vector<int> knn, int k);
};


void getDataSet(vector<vector<double> > &dataSet, vector<int> &labels);
void normalize(vector<vector<double> > &dataSet, int sampleNum, int featureNum);
void getDataSet(string dir, vector<vector<double> > &dataSet, vector<int> &labels);
void KNNTest1();
void KNNTest2();
void test(vector<vector<double> > trainingData, vector<int> trainingLabels, vector<vector<double> > testingData, vector<int> testingLabels);


#endif
