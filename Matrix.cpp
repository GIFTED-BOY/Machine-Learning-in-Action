#include "Matrix.h"

Matrix::Matrix() {}

Matrix::Matrix(int m, int n)
{
	rowNum = m;
	columnNum = n;
	data = new double[m * n];
	for (int i = 0; i < m * n; i++) data[i] = 0;
}

Matrix::Matrix(int m)
{
	rowNum = columnNum = m;
	data = new double[m * m];
	for (int i = 0; i < m; i++)
		for (int j = 0; j < m; j++) data[i * m + j] = i == j ? 1 : 0;
}

Matrix::Matrix(const Matrix &mMatrix)
{
	rowNum = mMatrix.rowNum;
	columnNum = mMatrix.columnNum;
	data = new double[rowNum * columnNum];
	for (int i = 0; i < rowNum * columnNum; i++) data[i] = mMatrix.data[i];
}

Matrix::Matrix(double *mData, int m, int n)
{
	rowNum = m;
	columnNum = n;
	data = new double[m * n];
	for (int i = 0; i < m * n; i++) data[i] = mData[i];
}

Matrix::Matrix(vector<double> mData, int m, int n)
{
	rowNum = m;
	columnNum = n;
	data = new double[m * n];
	for (int i = 0; i < m * n; i++) data[i] = mData[i];
}

Matrix::~Matrix() { if (data != NULL) delete[] data; }

double Matrix::getValue(int i, int j) const { return data[i * columnNum + j]; }

void Matrix::setValue(int i, int j, double value) { data[i * columnNum + j] = value; }

int Matrix::getRowNum() const { return rowNum; }

int Matrix::getColumnNum() const { return columnNum; }

Matrix Matrix::getRowValue(int i) const
{
	Matrix tmp(1, columnNum);
	for (int j = 0; j < columnNum; j++) tmp.data[j] = getValue(i, j);
	return tmp;
}

Matrix Matrix::getColumnValue(int i) const
{
	Matrix tmp(rowNum, 1);
	for (int j = 0; j < rowNum; j++) tmp.data[j] = getValue(j, i);
	return tmp;
}

int Matrix::getRowMaxValueIndex(int i) const
{
	double max = getValue(i, 0);
	double index = 0;
	for (int j = 1; j < columnNum; j++)
	{
		double d = getValue(i, j);
		if (d >= max)
		{
			max = d;
			index = j;
		}
	}
	return index;
}

int Matrix::getColumnMaxValueIndex(int i) const
{
	double max = getValue(0, i);
	double index = 0;
	for (int j = 1; j < rowNum; j++)
	{
		double d = getValue(j, i);
		if (d >= max)
		{
			max = d;
			index = j;
		}
	}
	return index;
}

double Matrix::getRowMaxValue(int i) const { return getValue(i, getRowMaxValueIndex(i)); }

double Matrix::getColumnMaxValue(int i) const { return getValue(getColumnMaxValueIndex(i), i); }

Matrix Matrix::transpose()
{
	Matrix transposeMatrix(columnNum, rowNum);
	for (int i = 0; i < columnNum; i++)
		for (int j = 0; j < rowNum; j++) transposeMatrix.setValue(i, j, getValue(j, i));
	return transposeMatrix;
}

Matrix Matrix::inverse()
{
	if (rowNum != columnNum || det() == 0)
	{
		cout << "cannot do inverse !" << endl;
		return *this;
	}

	Matrix tmp = *this;
	Matrix inverseMatrix(columnNum);

	for (int i = 0; i < rowNum; i++)
	{
		int maxRow = i;
		double max = fabs(tmp.getValue(i, i));
		for (int j = i; j < columnNum; j++)
		{
			if (fabs(tmp.getValue(j, i)) > max)
			{
				max = fabs(tmp.getValue(j, i));
				maxRow = j;
			}
		}
		
		if (maxRow != i)
		{
			inverseMatrix.swapRow(i, maxRow);
			tmp.swapRow(i, maxRow);
		}
		
		double multiple = 1.0 / tmp.getValue(i, i);
		tmp.rowTransform(i, -1, multiple);
		inverseMatrix.rowTransform(i, -1, multiple);
		
		for (int j = 0; j < rowNum; j++)
		{
			if (j != i)
			{
				multiple = -tmp.getValue(j, i);
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
		double means = getRowValue(i).rowMeans().getValue(0, 0);
		double sigma = 0.0;
		for (int j = 0; j < columnNum; j++) sigma += (getValue(i, j) - means) * (getValue(i, j) - means);
		stdMatrix.setValue(i, 0, sqrt(sigma / rowNum));
	}
	return stdMatrix;
}

Matrix Matrix::columnStd()
{
	Matrix stdMatrix(1, columnNum);
	for (int i = 0; i < columnNum; i++)
	{
		double means = getColumnValue(i).columnMeans().getValue(0, 0);
		double sigma = 0.0;
		for (int j = 0; j < rowNum; j++) sigma += (getValue(j, i) - means) * (getValue(j, i) - means);
		stdMatrix.setValue(0, i, sqrt(sigma / rowNum));
	}
	return stdMatrix;
}

Matrix Matrix::rowMeans()
{
	Matrix meansMatrix(rowNum, 1);
	for (int i = 0; i < rowNum; i++)
	{
		double sigma = 0.0;
		for (int j = 0; j < columnNum; j++) sigma += getValue(i, j);
		meansMatrix.setValue(i, 0, sigma / rowNum);
	}
	return meansMatrix;
}

Matrix Matrix::columnMeans()
{
	Matrix meansMatrix(1, columnNum);
	for (int i = 0; i < columnNum; i++)
	{
		double sigma = 0.0;
		for (int j = 0; j < rowNum; j++) sigma += getValue(j, i);
		meansMatrix.setValue(0, i, sigma / rowNum);
	}
	return meansMatrix;
}

double Matrix::det()
{
	if (rowNum != columnNum)
	{
		cout << "matrix not square��" << endl;
		return 0;
	}

	if (1 == rowNum) return data[0];

	Matrix tmp = *this;

	int swapTimes = 0;
	double res = 1.0;

	for (int i = 0; i < rowNum; i++)
	{
		if (tmp.getValue(i, i) == 0)
		{
			for (int j = i; j < rowNum; j++)
			{
				if (tmp.getValue(j, i) != 0)
				{
					tmp.swapRow(i, j);
					swapTimes++;
				}
			}
		}

		for (int k = i + 1; k < rowNum; k++)
		{
			double d = -1 * tmp.getValue(k, i) / tmp.getValue(i, i);

			for (int m = 0; m < rowNum; m++) tmp.setValue(k, m, tmp.getValue(k, m) + d * tmp.getValue(i, m));
		}
	}

	for (int i = 0; i < rowNum; i++)
	{
		double value = tmp.getValue(i, i);
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
		for (int j = 0; j < columnNum; j++) tmp.setValue(i, j, getValue(i, j) + mMatrix.getValue(i, j));
	return tmp;
}

Matrix Matrix::operator - (const Matrix &mMatrix)
{
	if (mMatrix.columnNum != columnNum || mMatrix.rowNum != rowNum) return *this;

	Matrix tmp = *this;
	for (int i = 0; i < rowNum; i++)
		for (int j = 0; j < columnNum; j++) tmp.setValue(i, j, getValue(i, j) - mMatrix.getValue(i, j));
	return tmp;
}

Matrix Matrix::operator * (const Matrix &mMatrix)
{
	if (columnNum != mMatrix.rowNum)
	{
		cout << "cannot do multiply��";
		return *this;
	}

	Matrix tmp(rowNum, mMatrix.getColumnNum());
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < mMatrix.columnNum; j++)
		{
			double sum = 0;
			for (int k = 0; k < mMatrix.rowNum; k++) sum += getValue(i, k) * mMatrix.getValue(k, j);
			tmp.setValue(i, j, sum);
		}
	}
	return tmp;
}

Matrix Matrix::operator * (const double multiple)
{
	Matrix tmp = *this;
	for (int i = 0; i < rowNum; i++)
		for (int j = 0; j < columnNum; j++) tmp.setValue(i, j, getValue(i, j) * multiple);
	return tmp;
}

Matrix& Matrix::operator = (const Matrix &mMatrix)
{
	rowNum = mMatrix.rowNum;
	columnNum = mMatrix.columnNum;
	if (data != NULL) delete[] data;
	data = new double[rowNum * columnNum];
	for (int i = 0; i < rowNum * columnNum; i++) data[i] = mMatrix.data[i];
	return *this;
}

void Matrix::swapRow(int i, int j)
{
	for (int k = 0; k < columnNum; k++)
	{
		double tmp = getValue(i, k);
		setValue(i, k, getValue(j, k));
		setValue(j, k, tmp);
	}
}

void Matrix::rowTransform(int i, int j, double multiple)
{
	if (j < 0)
		for (int k = 0; k < columnNum; k++) setValue(i, k, multiple * getValue(i, k));
	else
		for (int k = 0; k < columnNum; k++) setValue(j, k, multiple * getValue(i, k) + getValue(j, k));
}

ostream& operator << (ostream &os, const Matrix &mMatrix)
{
	for (int i = 0; i < mMatrix.rowNum; i++)
	{
		for (int j = 0; j < mMatrix.columnNum; j++) os << mMatrix.getValue(i, j) << "\t";
		os << "\n";
	}
	os.flush();
	return os;
}