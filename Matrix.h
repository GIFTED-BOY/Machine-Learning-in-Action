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

class Matrix
{
private:
	int rowNum;
	int columnNum;
	double *data;

public:
	Matrix();
	Matrix(int m, int n);
	Matrix(int m);
	Matrix(const Matrix &mMatrix);
	Matrix(double *mData, int m, int n);
	Matrix(vector<double> mData, int m, int n);
	Matrix(vector<vector<double> > mData, int m, int n);
	~Matrix();

	double getValue(int i, int j) const;
	void setValue(int i, int j, double value);
	int getRowNum() const;
	int getColumnNum() const;

	Matrix getRowValue(int i) const;
	Matrix getColumnValue(int i) const;

	int getRowMaxValueIndex(int i) const;
	int getColumnMaxValueIndex(int i) const;
	double getRowMaxValue(int i) const;
	double getColumnMaxValue(int i) const;

	int getRowMinValueIndex(int i) const;
	int getColumnMinValueIndex(int i) const;
	double getRowMinValue(int i) const;
	double getColumnMinValue(int i) const;

	Matrix transpose();
	Matrix inverse();

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