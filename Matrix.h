// ==================================================
//
// Author: GIFTED-BOY
// Email: 120138380@qq.com
// Create Time: 2018.4.21
// Version: v1.0
//
// ==================================================

#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <vector>

using namespace std;

// --------------------------------------------------------------------------

class Row
{
	friend class Matrix;

private:
	int dataNum;
	double *data;

	Row();
	Row(int n);

public:
	Row(const Row &r);
	~Row();

	double& operator [] (const int i);
	Row& operator = (const Row &r);
	friend ostream& operator << (ostream &os, const Row &r);
};

// --------------------------------------------------------------------------

class Matrix
{
private:
	int rowNum;
	int columnNum;
	Row *rows;

public:
	Matrix();
	Matrix(int m, int n);
	Matrix(int m);
	Matrix(const Matrix &mMatrix);
	Matrix(double *mData, int m, int n);
	Matrix(vector<double> mData, int m, int n);
	Matrix(vector<vector<double> > mData, int m, int n);
	~Matrix();

	int getRowNum() const;
	int getColumnNum() const;

	Matrix row(int i) const;
	Matrix column(int i) const;

	int rowMaxValueIndex(int i) const;
	int columnMaxValueIndex(int i) const;
	double rowMaxValue(int i) const;
	double columnMaxValue(int i) const;

	int rowMinValueIndex(int i) const;
	int columnMinValueIndex(int i) const;
	double rowMinValue(int i) const;
	double columnMinValue(int i) const;

	Matrix T();
	Matrix i();

	Matrix rowStd();
	Matrix columnStd();
	Matrix rowMeans();
	Matrix columnMeans();

	Matrix rowSum();
	Matrix columnSum();
	double rowSum(int i);
	double columnSum(int i);

	double det();

	Matrix operator + (const Matrix &mMatrix);
	Matrix operator - (const Matrix &mMatrix);
	Matrix operator * (const Matrix &mMatrix);
	Matrix operator * (const double multiple);
	Matrix operator / (const double divide);
	Matrix& operator = (const Matrix &mMatrix);
	Row& operator [] (const int i);

	friend ostream& operator << (ostream &os, const Matrix &mMatrix);

	static Matrix ones(int m, int n);

private:
	void swapRow(int i, int j); // 交换i, j两行
	// 行变换：j < 0时，将第i行乘以multiple；j > 0时，将第i行的multiple倍加到第j行
	void rowTransform(int i, int j, double multiple);
};


Matrix sigmoid(Matrix matrix);
Matrix exp(Matrix matrix);
Matrix sign(Matrix matrix);
Matrix multiply(Matrix a, Matrix b);

#endif