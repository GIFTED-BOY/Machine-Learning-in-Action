// ==================================================
//
// Author: GIFTED-BOY
// Email: 120138380@qq.com
// Create Time: 2018.4.21
// Version: v1.0
//
// ==================================================

#include "KNN.h"
#include <algorithm>
#include <iostream>
#include <io.h>
#include <fstream>

KNN::KNN(Matrix x, Matrix y, int m, int n) : dataSet(x), labels(y), sampleNum(m), featureNum(n) {}

double KNN::classify(Matrix x, int k) { return getClass(getKNN(x, k), k); }

bool comp(vector<double> &a, vector<double> &b) { return a[0] < b[0]; }
vector<double> KNN::getKNN(Matrix x, int k)
{
	vector<vector<double> > dists;
	for (int i = 0; i < sampleNum; i++)
	{
		Matrix diff = dataSet.row(i) - x;
		double dist = (diff * diff.T())[0][0];
		vector<double> v(2);
		v[0] = dist;
		v[1] = labels[i][0];
		dists.push_back(v);
	}
	sort(dists.begin(), dists.end(), comp);

	vector<double> knn(k);
	for (int i = 0; i < k; i++) knn[i] = dists[i][1];
	return knn;
}

double KNN::getClass(vector<double> knn, int k)
{
	sort(knn.begin(), knn.end());
	double currentCls = knn[0];
	double maxCls = knn[0];
	int currentFreq = 1;
	int maxFreq = 1;
	for (int i = 1; i < k; i++)
	{
		if (knn[i] == currentCls) currentFreq++;
		else
		{
			if (currentFreq > maxFreq)
			{
				maxFreq = currentFreq;
				maxCls = currentCls;
			}
			currentCls = knn[i];
			currentFreq = 1;
		}
	}
	return currentFreq > maxFreq ? currentCls : maxCls;
}





void getDataSet_KNN(vector<vector<double> > &dataSet, vector<double> &labels)
{
	ifstream in("dataset/Ch02/datingTestSet2.txt");
	string str;
	char *p = NULL;
	while (getline(in, str))
	{
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

void normalize(vector<vector<double> > &dataSet, int sampleNum, int featureNum)
{
	vector<double> maxValue(featureNum, 0.0);
	vector<double> minValue(featureNum, LDBL_MAX);
	for (int j = 0; j < featureNum; j++)
	{
		for (int i = 0; i < sampleNum; i++)
		{
			if (dataSet[i][j] > maxValue[j]) maxValue[j] = dataSet[i][j];
			if (dataSet[i][j] < minValue[j]) minValue[j] = dataSet[i][j];
		}

		for (int i = 0; i < sampleNum; i++) dataSet[i][j] = (dataSet[i][j] - minValue[j]) / (maxValue[j] - minValue[j]);
	}
}

vector<double> getSample(string fileName)
{
	vector<double> x;
	ifstream in(fileName);
	string str;
	while (getline(in, str))
	{
		int size = str.size();
		//cout << str << endl;
		for (int i = 0; i < size; i++) x.push_back(str[i] - '0');
	}
	in.close();
	return x;
}

void getDataSet_KNN(string dir, vector<vector<double> > &dataSet, vector<double> &labels)
{
	long handle;
	_finddata_t fileInfo;
	string pattern = dir + "\\*.txt";

	if ((handle = _findfirst(pattern.c_str(), &fileInfo)) != -1)
	{
		do
		{
			string fileName = fileInfo.name;
			//cout << "file name: " << fileName << endl;
			dataSet.push_back(getSample(dir + "\\" + fileName));
			//cout << endl;
			char digit = fileName[0];
			labels.push_back(digit - '0');
		} while (_findnext(handle, &fileInfo) == 0);
		_findclose(handle);
	}
}

void test(vector<vector<double> > trainingData, vector<double> trainingLabels, vector<vector<double> > testingData, vector<double> testingLabels)
{
	int m = trainingData.size();
	int testingNum = testingData.size();
	Matrix trainingX(trainingData, m, trainingData[0].size());
	Matrix trainingY(trainingLabels, m, 1);
	KNN knn(trainingX, trainingY, m, trainingData[0].size());

	Matrix testingX(testingData, testingNum, testingData[0].size());
	int correctNum = 0;
	for (int i = 0; i < testingNum; i++)
	{
		double cls = knn.classify(testingX.row(i), 10);
		if (cls == testingLabels[i]) correctNum++;
		cout << "knn: " << cls << "\treal: " << testingLabels[i] << endl;
	}
	cout << "testing num: " << testingNum << endl;
	cout << "correct num: " << correctNum << endl;
	cout << "correct rate: " << (double) correctNum / testingNum << endl;
}

void KNNTest1()
{
	vector<vector<double> > dataSet;
	vector<double> labels;
	getDataSet_KNN(dataSet, labels);

	int m = dataSet.size();
	int featureNum = dataSet[0].size();
	normalize(dataSet, m, featureNum);

	int testingNum = m * 0.1;
	vector<vector<double> > trainingData(m - testingNum);
	vector<vector<double> > testingData(testingNum);
	copy(dataSet.begin(), dataSet.begin() + testingNum, testingData.begin());
	copy(dataSet.begin() + testingNum, dataSet.end(), trainingData.begin());
	vector<double> trainingLabels(m - testingNum);
	vector<double> testingLabels(testingNum);
	copy(labels.begin(), labels.begin() + testingNum, testingLabels.begin());
	copy(labels.begin() + testingNum, labels.end(), trainingLabels.begin());

	test(trainingData, trainingLabels, testingData, testingLabels);
}

void KNNTest2()
{
	vector<vector<double> > trainingData;
	vector<vector<double> > testingData;
	vector<double> trainingLabels;
	vector<double> testingLabels;
	getDataSet_KNN("dataset/Ch02/digits/trainingDigits", trainingData, trainingLabels);
	getDataSet_KNN("dataset/Ch02/digits/testDigits", testingData, testingLabels);

	test(trainingData, trainingLabels, testingData, testingLabels);
}