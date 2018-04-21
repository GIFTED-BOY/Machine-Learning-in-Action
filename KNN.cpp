#include "KNN.h"

KNN::KNN(vector<vector<double> > x, vector<int> y, int m, int n) : dataSet(x), labels(y), sampleNum(m), featureNum(n) {}

int KNN::classify(vector<double> x, int k) { return getClass(getKNN(x, k), k); }

bool comp(vector<double> &a, vector<double> &b) { return a[0] < b[0]; }
vector<int> KNN::getKNN(vector<double> x, int k)
{
	vector<vector<double> > dists;
	for (int i = 0; i < sampleNum; i++)
	{
		double dist = 0.0;
		for (int j = 0; j < featureNum; j++) dist += (dataSet[i][j] - x[j]) * (dataSet[i][j] - x[j]);
		vector<double> v(2);
		v[0] = dist;
		v[1] = labels[i];
		dists.push_back(v);
	}
	sort(dists.begin(), dists.end(), comp);

	vector<int> knn(k);
	for (int i = 0; i < k; i++) knn[i] = dists[i][1];
	return knn;
}

int KNN::getClass(vector<int> knn, int k)
{
	sort(knn.begin(), knn.end());
	int currentCls = knn[0];
	int maxCls = knn[0];
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





void getDataSet(vector<vector<double> > &dataSet, vector<int> &labels)
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

void getDataSet(string dir, vector<vector<double> > &dataSet, vector<int> &labels)
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

void test(vector<vector<double> > trainingData, vector<int> trainingLabels, vector<vector<double> > testingData, vector<int> testingLabels)
{
	int m = trainingData.size();
	int testingNum = testingData.size();
	KNN knn(trainingData, trainingLabels, m, trainingData[0].size());
	int correctNum = 0;
	for (int i = 0; i < testingNum; i++)
	{
		int cls = knn.classify(testingData[i], 10);
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
	vector<int> labels;
	getDataSet(dataSet, labels);

	int m = dataSet.size();
	int featureNum = dataSet[0].size();
	normalize(dataSet, m, featureNum);

	int testingNum = m * 0.1;
	vector<vector<double> > trainingData(m - testingNum);
	vector<vector<double> > testingData(testingNum);
	copy(dataSet.begin(), dataSet.begin() + testingNum, testingData.begin());
	copy(dataSet.begin() + testingNum, dataSet.end(), trainingData.begin());
	vector<int> trainingLabels(m - testingNum);
	vector<int> testingLabels(testingNum);
	copy(labels.begin(), labels.begin() + testingNum, testingLabels.begin());
	copy(labels.begin() + testingNum, labels.end(), trainingLabels.begin());

	test(trainingData, trainingLabels, testingData, testingLabels);
}

void KNNTest2()
{
	vector<vector<double> > trainingData;
	vector<vector<double> > testingData;
	vector<int> trainingLabels;
	vector<int> testingLabels;
	getDataSet("dataset/Ch02/digits/trainingDigits", trainingData, trainingLabels);
	getDataSet("dataset/Ch02/digits/testDigits", testingData, testingLabels);

	test(trainingData, trainingLabels, testingData, testingLabels);
}