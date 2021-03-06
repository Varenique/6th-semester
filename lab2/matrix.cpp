#include "matrix.h"

#pragma intrinsic(__rdtsc)

Matrix::Matrix(int big_column, int big_row, int inner_column, int inner_row) {
	this->big_column = big_column;
	this->inner_column = inner_column;
	this->big_row = big_row;
	this->inner_row = inner_row;
}

void Matrix::createMatrix() {
	double** matrix = new double*[sizeof(double)*(this->big_column*this->big_row)];
	for (int i = 0; i < (this->big_column*this->big_row); i++) {
		matrix[i] = new double[sizeof(double)*(this->inner_column*this->inner_row)];
	}
	this->matrix = matrix;
}

void Matrix::fillMatrix(bool zero_fill) {
	for (int i = 0; i < (this->big_column*this->big_row); i++) {
		for (int j = 0; j < (this->inner_column*this->inner_row); j++) {
			if (zero_fill) {
				matrix[i][j] = 0;
			}
			else {
				matrix[i][j] = (double)rand() / 1000;
			}
			//cout << matrix[i][j] << " ";
		}
		//cout << endl;
	}
	//cout << endl;
}

bool Matrix::checkMatrix(Matrix matrix1) {
	if ((this->big_column*this->big_row) == (matrix1.big_column*matrix1.big_row) && (this->inner_column*this->inner_row) == (matrix1.inner_column*matrix1.inner_row)) {
		for (int big = 0; big < (this->big_column*this->big_row); big++) {
			for (int inner = 0; inner < (this->inner_column*this->inner_row); inner++) {
				if (round(matrix[big][inner] * 10) / 10 != round(matrix1.matrix[big][inner] * 10) / 10) {
					return false;
				}
			}
		}
		return true;
	}
	return false;
}


void Matrix::manually_multiply(Matrix matrix1, Matrix matrix2, int maxBig1, int maxBig2, int minBig1, int minBig2) {
	unsigned __int64 time = __rdtsc();
	int row = 0, column = 0;
	int i = 0, j = 0, big1 = 0, big2 = 0, end1 = 0, end2 = 0, inner1 = 0, inner2 = 0;
	for (big1 = minBig1; big1 < maxBig1; big1++) {
		if (big1 % matrix1.big_column == 0) {
			big2 = minBig2;
		}
		else {
			big2 = matrix2.big_column*(big1 % matrix1.big_column) + minBig2;
		}

		if (maxBig2 < matrix2.big_column) {
			end1 = big2 + maxBig2;
		}
		else {
			end1 = big2 + matrix2.big_column;
		}
		for (big2; big2 < end1; big2++) {


			__m128d reg0;
			__m128d reg1;
			__m128d reg2;
			__m128d reg4;


			for (row = 0; row < 36; row += matrix1.inner_column)
			{
				for (column = 0; column < 6; column += 2)
				{

					reg0 = _mm_set1_pd(matrix1.matrix[big1][row]);
					reg1 = _mm_load_pd(&matrix2.matrix[big2][column]);
					reg2 = _mm_mul_pd(reg0, reg1);
					for (int k = 1, z = 6; k < 6, z<36; k++, z += 6)
					{

						reg0 = _mm_set1_pd(matrix1.matrix[big1][row + k]);
						reg1 = _mm_load_pd(&matrix2.matrix[big2][z + column]);
						reg2 = _mm_add_pd(reg2, _mm_mul_pd(reg0, reg1));



					}
					reg4 = _mm_load_pd(&matrix[((big1 / matrix1.big_column)*matrix1.big_column) +
						(big2 % matrix2.big_column)][row + column]);
					reg2 = _mm_add_pd(reg2, reg4);
					_mm_store_pd(&matrix[((big1 / matrix1.big_column)*matrix1.big_column) +
						(big2 % matrix2.big_column)][row + column], reg2);


				}
			}

		}
	}
	time = __rdtsc() - time;
	cout << "Manually vectorization: " << time << endl;
}

void Matrix::cache_multiply(Matrix matrix1, Matrix matrix2) {
	unsigned __int64 time = __rdtsc();
	int row = 0, column = 0;
	
	for (int i = 0; i < (matrix1.big_column * matrix1.big_row); i += BLOCK) {
		for (int j = 0; j < (matrix2.big_column * matrix2.big_row); j += BLOCK) {
			int minBig1 = i;
			int maxBig1 = i + BLOCK;
			int minBig2 = j;
			int maxBig2 = j + BLOCK;
			manually_multiply(matrix1, matrix2, maxBig1, maxBig2, minBig1, minBig2);
		}
	}

	time = __rdtsc() - time;
	cout << "Cache vectorization: " << time << endl;
}

void Matrix::print_result() {
	cout << "Your matrix: " << endl;
	for (int i = 0; i < (this->big_column*this->big_row); i++) {
		for (int j = 0; j < (this->inner_column*this->inner_row); j++) {
			cout << this->matrix[i][j] << " ";
		}
		cout << endl;
	}
}