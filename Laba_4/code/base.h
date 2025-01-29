#pragma once
#include <iostream>
#include <string>
#include <iomanip> 
#include <fstream>
#include "reading.h"
#include <clocale>
using namespace std;

class Matrix {
public:
    int rows;
    int columns;
    int un_zero = 0;
    int** array;

    Matrix() : rows(0), columns(0), array(nullptr) {}

    Matrix(int size) {
        this->rows = size;
        this->columns = size;
        array = new int* [size];
        for (int i = 0; i < size; i++) {
            array[i] = new int[size];
            for (int j = 0; j < size; j++) {
                array[i][j] = 0;
            }
        }
    }

    Matrix(int rows, int columns) {
        this->rows = rows;
        this->columns = columns;
        array = new int* [rows];
        for (int i = 0; i < rows; i++) {
            array[i] = new int[columns];
            for (int j = 0; j < columns; j++) {
                array[i][j] = 0;
            }
        }
    }

    ~Matrix() {
        clear();
    }

    void print() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                cout << setw(4) << array[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "ROWS: " << rows << "\n";
        cout << "COLUMNS: " << columns << "\n";
        cout << "UN_ZERO: " << un_zero << "\n";
    }

    Matrix& operator= (const Matrix& rhs) {
        if (this == (&rhs)) {
            cout << "само присваивание" << "\n";
            return *this;
        }
        clear();

        this->un_zero = rhs.un_zero;
        this->rows = rhs.rows;
        this->columns = rhs.columns;
        array = new int* [rows];

        for (int i = 0; i < rows; i++) {
            array[i] = new int[columns];
            for (int j = 0; j < columns; j++) {
                array[i][j] = rhs.array[i][j];
            }
        }
        return *this;
    }

    void clear() {
        if (array != nullptr) {
            for (int i = 0; i < rows; i++) {
                delete[] array[i];
            }
            delete[] array;
            array = nullptr;
        }
    }
};

Matrix usual_addition(Matrix* _1, Matrix* _2) {
    if ((_1->rows != _2->rows) or (_2->columns != _1->columns)) {
        cout << "Uncampatable sizes in addition: \n";
        exit(1);
    }
    Matrix ans(_1->rows, _1->columns);
    for (int i = 0; i < (*_1).rows; i++) {
        for (int j = 0; j < (*_2).rows; j++) {
            ans.array[i][j] = _1->array[i][j] + _2->array[i][j];
        }
    }
    return ans;
}

Matrix usual_multiplication(Matrix* array_l, Matrix* array_r) {
    Matrix new_ar(array_l->rows, array_r->columns);
    if (array_l->columns != array_r->rows) {
        cout << "Uncampatable sizes in multiplication: \n";
        exit(1);
    }

    for (int i = 0; i < array_l->rows; i++) {

        for (int j = 0; j < array_r->columns; j++) {
            for (int k = 0; k < array_l->columns; k++) {
                new_ar.array[i][j] += (array_l->array[i][k] * array_r->array[k][j]);
            }
            if (new_ar.array[i][j] != 0) new_ar.un_zero += 1;
        }
    }
    return new_ar;
}

int* parse(string str, int len, Matrix* mat) {
    int col = 0;
    for (char i : str)
        if (i == ';') col++;
    string* arr = new string[col];
    int iter = 0;
    for (char i : str) {
        if (i == ';') {
            iter++;
            continue;
        }
        arr[iter] += i;
    }
    if (iter == len) {
        int* ans = new int[iter];
        for (int i = 0; i < iter; i++) {
            ans[i] = str_int(arr[i]);
            if (ans[i] != 0) mat->un_zero += 1;
        }
        delete[] arr;
        return ans;
    }
    else {
        cout << "There are not enough numbers for a matrix of this size";
        exit(1);
    }
}

void read_len(string data, int* r, int* c) {
    bool FLAG = false;
    bool SQARE = true;
    string num_1 = "";
    string num_2 = "";
    for (int i = 0; i < data.size(); i++) {
        if (FLAG) {
            if (data[i] != '(') (num_1) += data[i];
            else SQARE = false;
            if (!(SQARE)) {
                i++;
                while (data[i] != ',') {
                    num_1 += data[i];
                    i++;
                }
                i++;
                while (data[i] != ')') {
                    num_2 += data[i];
                    i++;
                }
                break;
            }
        }
        else if (data[i] == ';') {
            FLAG = not(FLAG);
        }
    }
    (*r) = str_int(num_1);
    if (num_2 != "") (*c) = str_int(num_2);
    else (*c) = str_int(num_1);
}

Matrix read_matrix(string filename) {
    ifstream file(filename);
    string data;
    int rows = 0;
    int r = 0;
    int c = 0;
    if (file.is_open())
    {
        if (getline(file, data)) {
            if (data[0] == 'S') {
                read_len(data, &r, &c);// если там будет не число, то программа выведет ошибку
            }
        }
        Matrix matrix(r, c);
        // start reading the matrix
        while (getline(file, data)) {
            matrix.array[rows] = parse(data, c, &matrix);//////
            if (rows > r) {
                cout << "There are too many strings\n";
                exit(1);
            }
            rows++;
        }
        if (rows != r) {
            cout << "Amount of strings is not enoght\n";
            exit(1);
        }
        return matrix;
    }
    else {
        cout << "File read error\n";
        exit(1);
    }
}

void COMPARE(Matrix* _1, Matrix* _2) {
    for (int i = 0; i < _1->rows; i++) {
        for (int j = 0; j < _1->columns; j++) {
            if (_1->array[i][j] != _2->array[i][j]) {
                cout << "POSITOIN [" << i << "][" << j << "]: " << _1->array[i][j] << " != " << _2->array[i][j] << "\n";
            }
        }
    }
}