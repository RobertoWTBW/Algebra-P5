#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

ifstream fin("date.in");
ofstream fout("date.out");

long long getNrOfReducedEchelon(int m, int n) { //this is here only as what's the closest I got to the formula, this gives an error of 4 for both 34 and 44, and 56 for both 45 and 55
	/*
	* This function calculates the number of matrices of size n and m in Z2 in reduced echelon form
	* @param m - nr of lines of such matrices
	* @param n - nr of columns of such matrices
	* @return int
	*/
	long long nrOfReducedEchelon = 0;
	int sum = 0;
	int tp = 0;
	for (int k = 0; k <= min(m, n); k++) { //number of rows with pivot
		for (int i = 0; i <= k * (n - k); i++) {
			nrOfReducedEchelon += (1 << i);
		}
	}
	return nrOfReducedEchelon;
}

void printNrOfReducedEchelon(long long nrOfReducedEchelon, int m, int n) {
	/*
	* This function prints the number of matrices in reduced echelon form as per the sentence
	* @param nrOfReducedEchelon - nr of matrices in reduced echelon form
	* @param m - number of lines of the matrices
	* @param n - number of columns of the matrices
	* @return
	*/
	fout << "1. The number of matrices A belonging to M" << m << n << "(Z2) in reduced echelon form is: " << nrOfReducedEchelon << "\n";
}

void printMatrix(vector<vector<int>>& re) {
	/*
	* This function prints a matrix
	* @param re - current matrix
	* @return
	*/
	int m = re.size() - 1;
	int n = re[1].size() - 1;
	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= n; j++)
			fout << re[i][j] << " ";
		fout << "\n";
	}
	fout << "\n";
}

bool onSameColumn(int j, vector<int> prevp, int k) {
	/*
	* Checks if on the same column there is a pivot
	* @param j - the line where the number we are checking is
	* @param prevp - the history of the positions of the pivots in the current line
	* @return true if the 1 is on the same column as a previous pivot, false otherwise
	*/
	int n = prevp.size() - 1;
	for (int i = 1; i <= n; i++) {
		if (prevp[i] == j and i != k)
			return true;
	}
	return false;
}

void completeMatrix(vector<vector<int>>& re, vector<int>& prevp, int m, int n, int &nrOfReducedEchelon, int i = 1, int j = 1, int czero = 0) {
	/*
	* This completes the matrix as per the prevp vector
	* @param re - current matrix
	* @param prevp - the history of the positions of the pivots in the current line
	* @param m - nr of lines of the matrix
	* @param n - nr of columns of the matrix
	* @param i - current line
	* @param j - current column
	* @param czero - counter of zeros in the current line
	* @return
	*/
	if (j > n) { //we need to go to the next line
		if (czero == n and prevp[i] != n + 1) { //we check this to not have the O matrix multiple times
			return;
		}
		completeMatrix(re, prevp, m, n, nrOfReducedEchelon, i + 1, 1);
	}
	else {
		if (i > m) { //if we finished to complete it, we print it
			if (nrOfReducedEchelon == -1)
				printMatrix(re);
			else
				nrOfReducedEchelon += 1;
		}
		else {
			if (!onSameColumn(j, prevp, i) and j >= prevp[i] and j >= i) //pretty obvious
				re[i][j] = 1, completeMatrix(re, prevp, m, n, nrOfReducedEchelon, i, j + 1, czero);
			if (j != prevp[i])
				re[i][j] = 0, completeMatrix(re, prevp, m, n, nrOfReducedEchelon, i, j + 1, czero + 1);
		}
	}
}

void genReducedEchelon(int m, int n, vector<vector<int>> &re, vector<int> &prevp, int &nrOfReducedEchelon, int i = 1) { //prevp = column of the previous pivots
	/*
	* This generates the prevp vector needed to complete the matrix
	* @param m - nr of lines of the matrix
	* @param n - nr of columns of the matrix
	* @param re - current matrix
	* @param nrOfReducedEchelon - needed to count the matrices, if it is -1, then I only print them
	* @param prevp - the history of the positions of the pivots in the current line
	* @param i - current line
	*/
	if (i > m) { //if we finished to fill the prevp vector, then we need to complete the matrix
		completeMatrix(re, prevp, m, n, nrOfReducedEchelon);
	}
	else {
		re[i] = vector<int>(n + 1, 0);
		for (int j = prevp[i - 1] + 1; j <= n; j++) { // we generate poisitions for the pivot in the current line
			prevp[i] = j;
			genReducedEchelon(m, n, re, prevp, nrOfReducedEchelon, i + 1);
		}
		prevp[i] = n + 1; //it can also get the value n + 1, meaning the line does not have a pivot
		genReducedEchelon(m, n, re, prevp, nrOfReducedEchelon, i + 1);
	}
}

void printSecondSentence(int m, int n) {
	/*
	* Prints the second sentence of the problem as in the document
	* @param m - nr of lines of the matrix
	* @param n - nr of columns of the matrix
	* @return
	*/
	fout << "2. The matrices A belonging to M" << m << n <<"(Z2) in reduced echelon form are:\n";
}

int main() {
	int m, n;
	fin >> m >> n;
	int nrOfReducedEchelon = 0;
	vector<vector<int>> re = vector<vector<int>>(m + 1);
	vector<int> prevp = vector<int>(m + 1, 0);
	genReducedEchelon(m, n, re, prevp, nrOfReducedEchelon);
	printNrOfReducedEchelon(nrOfReducedEchelon, m, n);
	if (m >= 2 and m <= 5 and n >= 2 and n <= 5) {
		printSecondSentence(m, n);
		int placeholder = -1;
		genReducedEchelon(m, n, re, prevp, placeholder);
	}
	fin.close(), fout.close();
}
