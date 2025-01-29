#include <string>
#include <iomanip> 
#include <iostream>
#include <stdlib.h>
#include <random>

using namespace std;


double get_random_number(double d) {
    random_device rd; // Источник случайности
    mt19937 gen(rd()); // Генератор чисел (Mersenne Twister)
    uniform_real_distribution<> dis(0.0, d-0.0000000001); // Диапазон [0.0, 1.0]
    return dis(gen); // Генерируем случайное число
}

int str_int(string arg) {
    return atoi(arg.c_str());
}

void read_num(double* number) {
    while (true) {
        cin >> (*number);
        // Проверяем, если ввод не является числом
        if (cin.fail()) {
            cin.clear(); // Сбросить флаг ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистить буфер ввода
            cout << "Error! There is not valid number. Try again:\n";
            continue;
        }
        else if ((*number >= 1) or (*number <= 0)) {
            cout << "Error! There is not valid number. Try again:\n";
            continue;
        }
        else {
            break; // Если число введено корректно
        }
    }
}