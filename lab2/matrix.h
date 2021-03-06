#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <intrin.h>
#include <emmintrin.h>

#define BLOCK 131072


using namespace std;

class Matrix {
public:
	int big_column;
	int big_row;
	int inner_column;
	int inner_row;
	double** matrix;
public:
	Matrix(int big_column, int big_row, int inner_column, int inner_row);
	void createMatrix();
	void fillMatrix(bool zero_fill);
	bool checkMatrix(Matrix matrix1);
	void vec_multiply(Matrix matrix1, Matrix matrix2);
	void no_vec_multiply(Matrix matrix1, Matrix matrix2);
	void manually_multiply(Matrix matrix1, Matrix matrix2, int maxBig1, int maxBig2, int minBig1, int minBig2);
	void print_result();
	void cache_multiply(Matrix matrix1, Matrix matrix2);

};
