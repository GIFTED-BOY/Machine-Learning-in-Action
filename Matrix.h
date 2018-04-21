#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <iostream>
#include <vector>
#include <math.h>

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

	Matrix transpose();
	Matrix inverse();

	Matrix rowStd();
	Matrix columnStd();
	Matrix rowMeans();
	Matrix columnMeans();

	double det();

	Matrix operator + (const Matrix &mMatrix);
	Matrix operator - (const Matrix &mMatrix);
	Matrix operator * (const Matrix &mMatrix);
	Matrix operator * (const double multiple);
	Matrix& operator = (const Matrix &mMatrix);

	friend ostream& operator << (ostream &os, const Matrix &mMatrix);

private:
	void swapRow(int i, int j); // ����i, j����
	// �б任��j < 0ʱ������i�г���multiple��j > 0ʱ������i�е�multiple���ӵ���j��
	void rowTransform(int i, int j, double multiple);
};

#endif