#pragma once
#include <iostream>
#include <string>
#include <iomanip> 
#include <fstream>
#include "reading.h"
#include <clocale>
#include <math.h>
using namespace std;

template <typename dt>
class Matrix {
public:
    int size;
    dt** array;

    Matrix() : size(0), array(nullptr) {}

    Matrix(int size) {
        this->size = size;
        array = new dt* [size];
        for (int i = 0; i < size; i++) {
            array[i] = new dt[size];
            for (int j = 0; j < size; j++) {
                array[i][j] = 0.000001;
            }
        }
    }

    Matrix(int rows, int columns) {
        this->size = rows;
        array = new dt* [rows];
        for (int i = 0; i < rows; i++) {
            array[i] = new dt[columns];
            for (int j = 0; j < columns; j++) {
                array[i][j] = 0.0000001;
            }
        }
    }

    ~Matrix() {
        clear();
    }

    void print() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                cout << setw(4) << array[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "SIZE: " << size << "\n";
    }

    void zero() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                array[i][j] = 0.000001;
            }
        }
    }

    Matrix& operator= (const Matrix& rhs) {
        if (this == (&rhs)) {
            cout << "само присваивание" << "\n";
            return *this;
        }
        clear();

        this->size = rhs.size;
        array = new dt* [size];

        for (int i = 0; i < size; i++) {
            array[i] = new dt[size];
            for (int j = 0; j < size; j++) {
                array[i][j] = rhs.array[i][j];
            }
        }
        return *this;
    }

    void clear() {
        if (array != nullptr) {
            for (int i = 0; i < size; i++) {
                delete[] array[i];
            }
            delete[] array;
            array = nullptr;
        }
    }
};

int* parse(string str, int len, Matrix<int>* mat) {
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

Matrix<int> read_matrix(string filename) {
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
        Matrix<int> matrix(r, c);
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
