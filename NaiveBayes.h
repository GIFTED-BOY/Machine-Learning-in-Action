#ifndef _NAIVE_BAYES_H_
#define _NAIVE_BAYES_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <math.h>
#include <fstream>
#include <time.h>
#include <iostream>

using namespace std;

class NaiveBayes
{
private:
	vector<vector<string> > dataSet;
	vector<int> labels;
	int sampleNum;
	int classKindNum;

	vector<string> vocabList;
	int vocabNum;

	vector<vector<double> > wordsProb;
	vector<double> classProb;

public:
	NaiveBayes(vector<vector<string> > x, vector<int> y, int m, int c);

	void train();

	int classify(vector<string> x);

private:
	vector<string> getVocabList();
	vector<int> getSetOfWordsVector(vector<string> x, int n);
	vector<vector<int> > getSetOfWordsMatrix();
	void calcClassProb();
	void calcWordsProb(vector<vector<int> > setOfWordsMatrix);
	int getClass(vector<string> x, int n);
};


vector<string> getWordsVector(string s);
string getSample(string fileName);
void getDataSet(vector<vector<string> > &dataSet, vector<int> &labels);
void NBTest();


#endif