// ==================================================
//
// Author: GIFTED-BOY
// Email: 120138380@qq.com
// Create Time: 2018.4.21
// Version: v1.0
//
// ==================================================

#include "Matrix.h"
#include <iostream>
#include <math.h>

// -----------------------------------------------------------------

Row::Row() { data = NULL; }

Row::Row(int n) : dataNum(n) { data = new double[n]; }

Row::Row(const Row &r)
{
	dataNum = r.dataNum;
	data = new double[dataNum];
	for (int i = 0; i < dataNum; i++) data[i] = r.data[i];
}

Row::~Row() { if(data != NULL) delete[] data; }

double& Row::operator [] (const int i) { return data[i]; }

Row& Row::operator = (const Row &r)
{
	dataNum = r.dataNum;
	if (data != NULL) delete[] data;
	data = new double[dataNum];
	for (int i = 0; i < dataNum; i++) data[i] = r.data[i];
	return *this;
}

ostream& operator << (ostream &os, const Row &r)
{
	for (int i = 0; i < r.dataNum; i++) os << r.data[i] << "\t";
	os.flush();
	return os;
}

// -----------------------------------------------------------------

Matrix::Matrix() { rows = NULL; }

Matrix::Matrix(int m, int n)
{
	rowNum = m;
	columnNum = n;
	rows = new Row[m];
	for (int i = 0; i < m; i++)
	{
		rows[i] = Row(n);
		for (int j = 0; j < n; j++) rows[i].data[j] = 0;
	}
}

Matrix::Matrix(int m)
{
	rowNum = columnNum = m;
	rows = new Row[m];
	for (int i = 0; i < m; i++)
	{
		rows[i] = Row(m);
		for (int j = 0; j < m; j++) rows[i].data[j] = (i == j);
	}
}

Matrix::Matrix(const Matrix &mMatrix)
{
	rowNum = mMatrix.rowNum;
	columnNum = mMatrix.columnNum;
	rows = new Row[rowNum];
	for (int i = 0; i < rowNum; i++) rows[i] = mMatrix.rows[i];
}

Matrix::Matrix(double *mData, int m, int n)
{
	rowNum = m;
	columnNum = n;
	rows = new Row[m];
	for (int i = 0; i < m; i++)
	{
		rows[i] = Row(n);
		for (int j = 0; j < n; j++) rows[i].data[j] = mData[i * n + j];
	}
}

Matrix::Matrix(vector<double> mData, int m, int n)
{
	rowNum = m;
	columnNum = n;
	rows = new Row[m];
	for (int i = 0; i < m; i++)
	{
		rows[i] = Row(n);
		for (int j = 0; j < n; j++) rows[i].data[j] = mData[i * n + j];
	}
}

Matrix::Matrix(vector<vector<double> > mData, int m, int n)
{
	rowNum = m;
	columnNum = n;
	rows = new Row[m];
	for (int i = 0; i < m; i++)
	{
		rows[i] = Row(n);
		for (int j = 0; j < n; j++) rows[i].data[j] = mData[i][j];
	}
}

Matrix::~Matrix() { if (rows != NULL) delete[] rows; }

int Matrix::getRowNum() const { return rowNum; }

int Matrix::getColumnNum() const { return columnNum; }

Matrix Matrix::row(int i) const
{
	Matrix tmp(1, columnNum);
	tmp.rows[0] = rows[i];
	return tmp;
}

Matrix Matrix::column(int i) const
{
	Matrix tmp(rowNum, 1);
	for (int j = 0; j < rowNum; j++) tmp.rows[j].data[0] = rows[j].data[i];
	return tmp;
}

int Matrix::rowMaxValueIndex(int i) const
{
	double max = rows[i].data[0];
	int index = 0;
	for (int j = 1; j < columnNum; j++)
	{
		double d = rows[i].data[j];
		if (d >= max)
		{
			max = d;
			index = j;
		}
	}
	return index;
}

int Matrix::columnMaxValueIndex(int i) const
{
	double max = rows[0].data[i];
	int index = 0;
	for (int j = 1; j < rowNum; j++)
	{
		double d = rows[j].data[i];
		if (d >= max)
		{
			max = d;
			index = j;
		}
	}
	return index;
}

double Matrix::rowMaxValue(int i) const { return rows[i].data[rowMaxValueIndex(i)]; }

double Matrix::columnMaxValue(int i) const { return rows[columnMaxValueIndex(i)].data[i]; }

int Matrix::rowMinValueIndex(int i) const
{
	double min = rows[i].data[0];
	int index = 0;
	for (int j = 1; j < columnNum; j++)
	{
		double d = rows[i].data[j];
		if (d < min)
		{
			min = d;
			index = j;
		}
	}
	return index;
}

int Matrix::columnMinValueIndex(int i) const
{
	double min = rows[0].data[i];
	int index = 0;
	for (int j = 1; j < rowNum; j++)
	{
		double d = rows[j].data[i];
		if (d < min)
		{
			min = d;
			index = j;
		}
	}
	return index;
}

double Matrix::rowMinValue(int i) const { return rows[i].data[rowMinValueIndex(i)]; }

double Matrix::columnMinValue(int i) const { return rows[columnMinValueIndex(i)].data[i]; }

Matrix Matrix::T()
{
	Matrix transposeMatrix(columnNum, rowNum);
	for (int i = 0; i < columnNum; i++)
		for (int j = 0; j < rowNum; j++) transposeMatrix.rows[i].data[j] = rows[j].data[i];
	return transposeMatrix;
}

Matrix Matrix::i()
{
	if (rowNum != columnNum || det() == 0)
	{
		cout << "cannot do inverse!" << endl;
		return *this;
	}

	Matrix tmp = *this;
	Matrix inverseMatrix(columnNum);

	for (int i = 0; i < rowNum; i++)
	{
		int maxRow = i;
		double max = fabs(tmp.rows[i].data[i]);
		for (int j = i; j < columnNum; j++)
		{
			if (fabs(tmp.rows[j].data[i]) > max)
			{
				max = fabs(tmp.rows[j].data[i]);
				maxRow = j;
			}
		}
		
		if (maxRow != i)
		{
			inverseMatrix.swapRow(i, maxRow);
			tmp.swapRow(i, maxRow);
		}
		
		double multiple = 1.0 / tmp.rows[i].data[i];
		tmp.rowTransform(i, -1, multiple);
		inverseMatrix.rowTransform(i, -1, multiple);
		
		for (int j = 0; j < rowNum; j++)
		{
			if (j != i)
			{
				multiple = -tmp.rows[j].data[i];
				tmp.rowTransform(i, j, multiple);
				inverseMatrix.rowTransform(i, j, multiple);
			}
		}
	}

	return inverseMatrix;
}

Matrix Matrix::rowStd()
{
	Matrix stdMatrix(rowNum, 1);
	for (int i = 0; i < rowNum; i++)
	{
		double means = (row(i).rowMeans()).rows[0].data[0];
		double sigma = 0.0;
		for (int j = 0; j < columnNum; j++) sigma += (rows[i].data[j] - means) * (rows[i].data[j] - means);
		stdMatrix.rows[i].data[0] = sqrt(sigma / columnNum);
	}
	return stdMatrix;
}

Matrix Matrix::columnStd()
{
	Matrix stdMatrix(1, columnNum);
	for (int i = 0; i < columnNum; i++)
	{
		double means = (column(i).columnMeans()).rows[0].data[0];
		double sigma = 0.0;
		for (int j = 0; j < rowNum; j++) sigma += (rows[j].data[i] - means) * (rows[j].data[i] - means);
		stdMatrix.rows[0].data[i] = sqrt(sigma / rowNum);
	}
	return stdMatrix;
}

Matrix Matrix::rowMeans()
{
	Matrix meansMatrix(rowNum, 1);
	for (int i = 0; i < rowNum; i++) meansMatrix.rows[i].data[0] = rowSum(i) / columnNum;
	return meansMatrix;
}

Matrix Matrix::columnMeans()
{
	Matrix meansMatrix(1, columnNum);
	for (int i = 0; i < columnNum; i++) meansMatrix.rows[0].data[i] = columnSum(i) / rowNum;
	return meansMatrix;
}

Matrix Matrix::rowSum()
{
	Matrix sumMatrix(rowNum, 1);
	for (int i = 0; i < rowNum; i++) sumMatrix.rows[i].data[0] = rowSum(i);
	return sumMatrix;
}

Matrix Matrix::columnSum()
{
	Matrix sumMatrix(1, columnNum);
	for (int i = 0; i < columnNum; i++) sumMatrix.rows[0].data[i] = columnSum(i);
	return sumMatrix;
}

double Matrix::rowSum(int i)
{
	double sum = 0.0;
	for (int j = 0; j < columnNum; j++) sum += rows[i].data[j];
	return sum;
}

double Matrix::columnSum(int i)
{
	double sum = 0.0;
	for (int j = 0; j < rowNum; j++) sum += rows[j].data[i];
	return sum;
}

double Matrix::det()
{
	if (rowNum != columnNum)
	{
		cout << "matrix not square!" << endl;
		return 0;
	}

	if (1 == rowNum) return rows[0].data[0];

	Matrix tmp = *this;

	int swapTimes = 0;
	double res = 1.0;

	for (int i = 0; i < rowNum; i++)
	{
		if (tmp.rows[i].data[i] == 0)
		{
			for (int j = i; j < rowNum; j++)
			{
				if (tmp.rows[j].data[i] != 0)
				{
					tmp.swapRow(i, j);
					swapTimes++;
				}
			}
		}

		for (int k = i + 1; k < rowNum; k++)
		{
			double d = -1 * tmp.rows[k].data[i] / tmp.rows[i].data[i];

			for (int m = 0; m < rowNum; m++) tmp.rows[k].data[m] += d * tmp.rows[i].data[m];
		}
	}

	for (int i = 0; i < rowNum; i++)
	{
		double value = tmp.rows[i].data[i];
		if (0 == value) return 0;
		res *= value;
	}

	if (swapTimes & 1) res = -res;
	return res;
}

Matrix Matrix::operator + (const Matrix &mMatrix)
{
	if (mMatrix.columnNum != columnNum || mMatrix.rowNum != rowNum) return *this;

	Matrix tmp = *this;
	for (int i = 0; i < rowNum; i++)
		for (int j = 0; j < columnNum; j++) tmp.rows[i].data[j] = rows[i].data[j] + mMatrix.rows[i].data[j];
	return tmp;
}

Matrix Matrix::operator - (const Matrix &mMatrix)
{
	if (mMatrix.columnNum != columnNum || mMatrix.rowNum != rowNum) return *this;

	Matrix tmp = *this;
	for (int i = 0; i < rowNum; i++)
		for (int j = 0; j < columnNum; j++) tmp.rows[i].data[j] = rows[i].data[j] - mMatrix.rows[i].data[j];
	return tmp;
}

Matrix Matrix::operator * (const Matrix &mMatrix)
{
	if (columnNum != mMatrix.rowNum)
	{
		cout << "cannot do multiply!";
		return *this;
	}

	Matrix tmp(rowNum, mMatrix.getColumnNum());
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < mMatrix.columnNum; j++)
		{
			double sum = 0;
			for (int k = 0; k < mMatrix.rowNum; k++) sum += rows[i].data[k] * mMatrix.rows[k].data[j];
			tmp.rows[i].data[j] = sum;
		}
	}
	return tmp;
}

Matrix Matrix::operator * (const double multiple)
{
	Matrix tmp = *this;
	for (int i = 0; i < rowNum; i++)
		for (int j = 0; j < columnNum; j++) tmp.rows[i].data[j] = rows[i].data[j] * multiple;
	return tmp;
}

Matrix Matrix::operator / (const double divide)
{
	if (0 == divide)
	{
		cout << "cannot do divide!";
		return *this;
	}

	Matrix tmp = *this;
	for (int i = 0; i < rowNum; i++)
		for (int j = 0; j < columnNum; j++) tmp.rows[i].data[j] = rows[i].data[j] / divide;
	return tmp;
}

Matrix& Matrix::operator = (const Matrix &mMatrix)
{
	rowNum = mMatrix.rowNum;
	columnNum = mMatrix.columnNum;
	if (rows != NULL) delete[] rows;
	rows = new Row[rowNum];
	for (int i = 0; i < rowNum; i++) rows[i] = mMatrix.rows[i];
	return *this;
}

Row& Matrix::operator [] (const int i) { return rows[i]; }

void Matrix::swapRow(int i, int j) { swap(rows[i], rows[j]); }

void Matrix::rowTransform(int i, int j, double multiple)
{
	if (j < 0)
		for (int k = 0; k < columnNum; k++) rows[i].data[k] *= multiple;
	else
		for (int k = 0; k < columnNum; k++) rows[j].data[k] += multiple * rows[i].data[k];
}

ostream& operator << (ostream &os, const Matrix &mMatrix)
{
	for (int i = 0; i < mMatrix.rowNum; i++) os << mMatrix.rows[i] << "\n";
	os.flush();
	return os;
}

Matrix Matrix::ones(int m, int n)
{
	Matrix o(m, n);
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++) o.rows[i].data[j] = 1;
	return o;
}





Matrix sigmoid(Matrix matrix)
{
	int m = matrix.getRowNum();
	int n = matrix.getColumnNum();
	Matrix h(m, n);
	for (int i = 0; i < m; i++)
	{
		Row &hr = h[i];
		Row &mr = matrix[i];
		for (int j = 0; j < n; j++) hr[j] = 1.0 / (1.0 + exp(-mr[j]));
	}
	return h;
}

Matrix exp(Matrix matrix)
{
	int m = matrix.getRowNum();
	int n = matrix.getColumnNum();
	Matrix e(m, n);
	for (int i = 0; i < m; i++)
	{
		Row &er = e[i];
		Row &mr = matrix[i];
		for (int j = 0; j < n; j++) er[j] = exp(mr[j]);
	}
	return e;
}

Matrix sign(Matrix matrix)
{
	int m = matrix.getRowNum();
	int n = matrix.getColumnNum();
	Matrix s(m, n);
	for (int i = 0; i < m; i++)
	{
		Row &sr = s[i];
		Row &mr = matrix[i];
		for (int j = 0; j < n; j++) sr[j] = (mr[j] >= 0);
	}
	return s;
}

Matrix multiply(Matrix a, Matrix b)
{
	if (a.getRowNum() != b.getRowNum() || a.getColumnNum() != b.getColumnNum())
	{
		cout << "cannot do multiply!";
		return a;
	}

	int m = a.getRowNum();
	int n = a.getColumnNum();
	Matrix matrix(m, n);
	for (int i = 0; i < m; i++)
	{
		Row &ar = a[i];
		Row &br = b[i];
		Row &mr = matrix[i];
		for (int j = 0; j < n; j++) mr[j] = ar[j] * br[j];
	}
	return matrix;
}