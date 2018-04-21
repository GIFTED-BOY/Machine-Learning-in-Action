#include "DecisionTree.h"

DecisionTree::DecisionTree(vector<vector<string> > x, int m, int n) : dataSet(x), sampleNum(m), featureNum(n) {}

void DecisionTree::createTree() { root = createTree(dataSet, sampleNum, featureNum); }

string DecisionTree::classify(vector<string> x) { return getClass(root, x); }

void DecisionTree::traverseTree() { traverseTree(root); }

double DecisionTree::getEntropy(vector<vector<string> > mDataSet)
{
	unordered_map<string, int> labelsFreq;
	int m = mDataSet.size();
	int n = mDataSet[0].size();
	for (int i = 0; i < m; i++)
		labelsFreq.count(mDataSet[i][n - 1]) ? labelsFreq[mDataSet[i][n - 1]]++ : labelsFreq[mDataSet[i][n - 1]] = 1;

	double ent = 0.0;
	for (auto iter = labelsFreq.begin(); iter != labelsFreq.end(); iter++)
	{
		double prob = (double) (iter->second) / m;
		ent -= prob * log2(prob);
	}
	return ent;
}

vector<vector<string> > DecisionTree::getSplitDataSet(vector<vector<string> > mDataSet, int m, int index, const string value)
{
	vector<vector<string> > subDataSet;
	for (int i = 0; i < m; i++)
	{
		if (value == mDataSet[i][index])
		{
			mDataSet[i].erase(mDataSet[i].begin() + index);
			subDataSet.push_back(mDataSet[i]);
		}
	}
	return subDataSet;
}

int DecisionTree::getBestFeatureIndex(vector<vector<string> > mDataSet, int m, int n)
{
	double bestInfoGain = 0.0;
	int bestFeatureIndex = -1;
	double baseEnt = getEntropy(mDataSet);
	unordered_set<string> featureValueSet;
	for (int j = 0; j < n; j++)
	{
		featureValueSet.clear();
		double ent = 0.0;
		for (int i = 0; i < m; i++)
		{
			if (featureValueSet.count(mDataSet[i][j])) continue;
			featureValueSet.insert(mDataSet[i][j]);
			vector<vector<string> > subDataSet = getSplitDataSet(mDataSet, m, j, mDataSet[i][j]);
			double prob = (double) (subDataSet.size()) / m;
			ent += prob * getEntropy(subDataSet);
		}
		double infoGain = baseEnt - ent;
		if (infoGain > bestInfoGain)
		{
			bestInfoGain = infoGain;
			bestFeatureIndex = j;
		}
	}
	return bestFeatureIndex;
}

bool comp(vector<string> &a, vector<string> &b) { return a[a.size() - 1] < b[b.size() - 1]; }

string DecisionTree::majorityVote(vector<vector<string> > mDataSet, int m, int n)
{
	sort(mDataSet.begin(), mDataSet.end(), comp);
	string currentCls = mDataSet[0][n];
	string maxCls = mDataSet[0][n];
	int currentFreq = 1;
	int maxFreq = 1;
	for (int i = 1; i < m; i++)
	{
		if (mDataSet[i][n] == currentCls) currentFreq++;
		else
		{
			if (currentFreq > maxFreq)
			{
				maxFreq = currentFreq;
				maxCls = currentCls;
			}
			currentCls = mDataSet[i][n];
			currentFreq = 1;
		}
	}
	return currentFreq > maxFreq ? currentCls : maxCls;
}

TreeNode * DecisionTree::createTree(vector<vector<string> > mDataSet, int m, int n)
{
	TreeNode *mRoot = new TreeNode;
	mRoot->featureIndex = -1;
	mRoot->value = "";
	int i = 1;
	for (; i < m; i++)
		if (mDataSet[i][n] != mDataSet[i - 1][n]) break;
	if (i == m) mRoot->value = mDataSet[0][n];
	else if (0 == n) mRoot->value = majorityVote(mDataSet, m, n);
	else
	{
		mRoot->featureIndex = getBestFeatureIndex(mDataSet, m, n);
		for (i = 0; i < m; i++)
		{
			if (mRoot->children.count(mDataSet[i][mRoot->featureIndex])) continue;
			vector<vector<string> > subDataSet = getSplitDataSet(mDataSet, m, mRoot->featureIndex, mDataSet[i][mRoot->featureIndex]);
			mRoot->children[mDataSet[i][mRoot->featureIndex]] = createTree(subDataSet, subDataSet.size(), subDataSet[0].size() - 1);
		}
	}
	return mRoot;
}

void DecisionTree::traverseTree(TreeNode *mRoot)
{
	if (-1 == mRoot->featureIndex) cout << "class: " << mRoot->value;
	else
	{
		cout << "best feature: " << mRoot->featureIndex << "{";
		for (auto iter = mRoot->children.begin(); iter != mRoot->children.end(); iter++)
		{
			cout << "subTree: " << iter->first << "[";
			traverseTree(iter->second);
			cout << "], ";
		}
		cout << "}";
	}
}

string DecisionTree::getClass(TreeNode *mRoot, vector<string> x)
{
	string cls = mRoot->value;
	if (mRoot->featureIndex != -1)
	{
		for (auto iter = mRoot->children.begin(); iter != mRoot->children.end(); iter++)
		{
			if (iter->first == x[mRoot->featureIndex])
			{
				x.erase(x.begin() + mRoot->featureIndex);
				cls = getClass(iter->second, x);
				break;
			}
		}
	}
	return cls;
}





void getDataSet(vector<vector<string> > &mDataSet)
{
	ifstream in("dataset/Ch03/lenses.txt");
	string str;
	char *p = NULL;
	while (getline(in, str))
	{
		vector<string> x;
		p = strtok((char *) str.c_str(), "\t");
		while (p != NULL)
		{
			string s = p;
			x.push_back(s);
			p = strtok(NULL, "\t");
		}
		mDataSet.push_back(x);
	}
	delete[] p;
	in.close();
}

void DTTest()
{
	vector<vector<string> > trainingData;
	getDataSet(trainingData);
	DecisionTree dt(trainingData, trainingData.size(), trainingData[0].size() - 1);
	dt.createTree();
	dt.traverseTree();
	vector<string> testingData;
	testingData.push_back("young");
	testingData.push_back("hyper");
	testingData.push_back("yes");
	testingData.push_back("normal");
	cout << dt.classify(testingData) << endl;
}