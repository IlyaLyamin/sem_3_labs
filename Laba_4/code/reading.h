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
        // ���������, ���� ���� �� �������� ������
        if (cin.fail()) {
            cin.clear(); // �������� ���� ������
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // �������� ����� �����
            cout << "Error! There is not valid number after 'SIZE'\n";
            exit(1);
        }
        else {
            break; // ���� ����� ������� ���������
        }
    }
}