// ==================================================
//
// Author: GIFTED-BOY
// Email: 120138380@qq.com
// Create Time: 2018.4.21
// Version: v1.0
//
// ==================================================

#ifndef _NAIVE_BAYES_H_
#define _NAIVE_BAYES_H_

#include <vector>
#include <string>

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
void getDataSet_NB(vector<vector<string> > &dataSet, vector<int> &labels);
void NBTest();


#endif