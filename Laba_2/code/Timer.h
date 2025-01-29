#include <string>
#include <iomanip> 
#include <iostream>
#include <clocale>
#include <chrono>

using namespace std;


class Timer {
public:
    Timer() {
        start = chrono::high_resolution_clock::now();
        end = chrono::high_resolution_clock::now();
    }

    void start_time() {
        start = chrono::high_resolution_clock::now();
    }
    void stop_time() {
        end = chrono::high_resolution_clock::now();
    }

    void get_time() {
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);


        // Выведем результат в микросекундах
        std::cout << "Elapsed time: " << duration.count() << " nanoseconds" << std::endl;

    }

private:
    chrono::time_point<chrono::high_resolution_clock> start;
    chrono::time_point<chrono::high_resolution_clock> end;
};


struct matrix {
public:
    matrix(string str_1, string str_2) {
        this->rows = (str_2.size() + 1);
        this->columns = (str_1.size() + 1);
        this->str_1 = "_" + str_1;
        this->str_2 = "_" + str_2;
        this->c = new int* [rows];
        prepair_matrix();
    }

    ~matrix() {
        delete[] c;
    }

    void print() {
        cout << setiosflags(ios::left);

        for (int i = 0; i < rows; i++) {
            cout << setw(2) << "|";
            for (int j = 0; j < columns; j++) {
                cout << setw(3) << c[i][j] << " ";
            }
            cout << "|" << endl;
        }
    }

    int* operator[] (int row) {
        return (this->c[row]);
    }

    string str(int ind) {
        if (ind == 1) {
            return str_1;
        }
        return str_2;
    }

    int ro() {
        return rows;
    }

    int co() {
        return columns;
    }

private:
    string str_1;
    string str_2;
    int rows;
    int columns;
    int** c;

    void prepair_matrix() {
        for (int i = 0; i < rows; i++) {
            c[i] = new int[columns];
            for (int j = 0; j < (columns); j++) {
                if (i == 0) {
                    c[i][j] = j;
                }
                else if (j == 0) {
                    c[i][j] = i;
                }
                else {
                    c[i][j] = -1;
                }
            }
        }
    }
};


void read(int* number) {
    cout << "Enter the number: ";

    while (true) {
        cin >> (*number);
        // Проверяем, если ввод не является числом
        if (cin.fail()) {
            cin.clear(); // Сбросить флаг ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистить буфер ввода
            cout << "Error! Please, input the valid number: ";
        }
        else {
            break; // Если число введено корректно
        }
    }
}