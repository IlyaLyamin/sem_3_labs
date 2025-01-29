#include <string>
#include <iomanip> 
#include <iostream>
#include <stdlib.h>

using namespace std;

int str_int(string arg) {
    return atoi(arg.c_str());
}

void read_num(int* number) {
    while (true) {
        cin >> (*number);
        // Проверяем, если ввод не является числом
        if (cin.fail()) {
            cin.clear(); // Сбросить флаг ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистить буфер ввода
            cout << "Error! There is not valid number after 'SIZE'\n";
            exit(1);
        }
        else {
            break; // Если число введено корректно
        }
    }
}