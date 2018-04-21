// ==================================================
//
// Author: GIFTED-BOY
// Email: 120138380@qq.com
// Create Time: 2018.4.21
// Version: v1.0
//
// ==================================================

#ifndef _DECISION_TREE_H_
#define _DECISION_TREE_H_

#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

class TreeNode
{
public:
	int featureIndex;
	string value;
	unordered_map<string, TreeNode*> children;
};

class DecisionTree
{
private:
	vector<vector<string> > dataSet;
	int sampleNum;
	int featureNum;
	TreeNode *root;

public:
	DecisionTree(vector<vector<string> > x, int m, int n);

	void createTree();

	string classify(vector<string> x);

	void traverseTree();

private:
	double getEntropy(vector<vector<string> > mDataSet);
	vector<vector<string> > getSplitDataSet(vector<vector<string> > mDataSet, int m, int index, const string value);
	int getBestFeatureIndex(vector<vector<string> > mDataSet, int m, int n);
	string majorityVote(vector<vector<string> > mDataSet, int m, int n);
	void traverseTree(TreeNode *mRoot);
	TreeNode * createTree(vector<vector<string> > mDataSet, int m, int n);
	string getClass(TreeNode *mRoot, vector<string> x);
};


void getDataSet(vector<vector<string> > &mDataSet);
void DTTest();


#endif
