#include <string>
#include <iomanip> 
#include <iostream>
#include <stdlib.h>
#include <random>

using namespace std;


double get_random_number(double d) {
    random_device rd; // �������� �����������
    mt19937 gen(rd()); // ��������� ����� (Mersenne Twister)
    uniform_real_distribution<> dis(0.0, d-0.0000000001); // �������� [0.0, 1.0]
    return dis(gen); // ���������� ��������� �����
}

int str_int(string arg) {
    return atoi(arg.c_str());
}

void read_num(double* number) {
    while (true) {
        cin >> (*number);
        // ���������, ���� ���� �� �������� ������
        if (cin.fail()) {
            cin.clear(); // �������� ���� ������
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // �������� ����� �����
            cout << "Error! There is not valid number. Try again:\n";
            continue;
        }
        else if ((*number >= 1) or (*number <= 0)) {
            cout << "Error! There is not valid number. Try again:\n";
            continue;
        }
        else {
            break; // ���� ����� ������� ���������
        }
    }
}