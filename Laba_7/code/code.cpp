#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdint>
using namespace std;

vector<unsigned long long> divs;
unsigned long limit = pow(2, 15);// ПОСТАВЬ ПОТОМ 22

void FIND_DIVS() {
    vector<bool> is_prime(limit + 1, true);
    is_prime[0] = is_prime[1] = false;

    for (unsigned long i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            divs.push_back(i);
            for (unsigned long j = 2 * i; j <= limit; j += i) {
                is_prime[j] = false;
            }
        }
    }
}

class Number_in_factors {
public:
    map<int, int> number{ pair<int, int>{1, 1} };
    int negative = 1;
    bool empty = 0;

    void convert_to_factors(string inp) {// здесь принимаем обычное число
        unsigned long long num_10 = 0;
        int digit = inp.size() - 1;
        for (int i = 0; i < inp.size(); i++) {
            if ((inp[i] == '-') and (negative == 1)) {
                negative = -1;
                digit--;
                continue;
            }
            else if (inp[i] == '-') {
                cout << "ERRROR!CHECK THE OPERATORS.\n";
                exit(1);
            }
            else if (inp[i] == '0') {
                digit--;
                continue;
            }
            else if (!isdigit(inp[i])) {
                cout << "ERROR\n";
                exit(2);
            }
            string loc(1, inp[i]);
            num_10 += stoi(loc) * pow(10, digit);
            digit--;
        }
        // переводим в формат множителей
        if (num_10 == 0) {
            empty = 1;
            number.erase(1);
            return;
        }
        empty = 0;
        int loc_ind = 0;
        while ((num_10 != 1) and (loc_ind < divs.size())) {
            if (num_10 % divs[loc_ind] == 0) {
                number[divs[loc_ind]] = 1;
                num_10 /= divs[loc_ind];
                while (num_10 % divs[loc_ind] == 0) {
                    number[divs[loc_ind]] += 1;
                    num_10 /= divs[loc_ind];
                }
            }
            loc_ind++;
        }
        if (loc_ind >= divs.size()) {
            cout << "IT IS TOO MUCH(PROGRAM STARTED EXTRA CALCULATES) ";
            for (unsigned long long int i = divs[loc_ind - 1]; i <= num_10; i++) {// если уже разделённое на все найденные делители число больше чем 2^64, то программа выдаст ошибку
                if (num_10 % i == 0) {
                    number[i] = 1;
                    num_10 /= i;
                    while (num_10 % i == 0) {
                        number[i] += 1;
                        num_10 /= i;
                    }
                    if (num_10 == 1) break;
                }
            }
        }
    }

    void print() {
        if (empty) cout << "0";
        else if (negative == -1) cout << "- ";
        for (auto it = number.begin(); it != number.end(); ++it) {
            cout << it->first << "^" << it->second << " ";
        }
        cout << "\n";
    }

    Number_in_factors operator*(const Number_in_factors _2) {
        Number_in_factors res;
        res.negative = this->negative * _2.negative;

        res.number = this->number;

        for (auto it = _2.number.begin(); it != _2.number.end(); ++it) {
            if (res.number.find(it->first) == res.number.end()) res.number[it->first] = it->second;
            else res.number[it->first] += it->second;
        }
        if ((empty or _2.empty)) {
            res.empty = 1;
            res.number.clear();
        }
        return res;
    }

    Number_in_factors operator+(const Number_in_factors _2) {
        Number_in_factors common_factors;
        long long small_sum_1 = this->empty == 1 ? 0 : 1 * this->negative;
        long long small_sum_2 = _2.empty == 1 ? 0 : 1 * _2.negative;

        for (auto it = _2.number.begin(); it != _2.number.end(); ++it) {
            if (this->number.find(it->first) != this->number.end()) {// разбираем общие делители
                common_factors.number[it->first] = min(this->number[it->first], it->second);
                if (this->number[it->first] > it->second) small_sum_1 *= pow(it->first, (this->number[it->first] - it->second));
                else if (this->number[it->first] < it->second) small_sum_2 *= pow(it->first, (it->second - this->number[it->first]));
            }
            else small_sum_2 *= pow(it->first, it->second);
        }
        for (auto it = this->number.begin(); it != this->number.end(); ++it) {
            if (_2.number.find(it->first) == _2.number.end()) {
                small_sum_1 *= pow(it->first, it->second);
            }
        }

        Number_in_factors small_sum;
        if ((small_sum_1 + small_sum_2) == 0) {
            small_sum.empty = 1;
        }
        if ((small_sum_1 + small_sum_2) < 0) small_sum.negative = -1;
        small_sum.convert_to_factors(to_string(abs(small_sum_1 + small_sum_2)));
        return common_factors*small_sum;
    }

    Number_in_factors operator-(const Number_in_factors _2) {
        Number_in_factors common_factors;
        long long small_sum_1 = this->empty == 1 ? 0 : 1*this->negative;
        long long small_sum_2 = _2.empty == 1 ? 0 : 1*_2.negative;

        for (auto it = _2.number.begin(); it != _2.number.end(); ++it) {
            if (this->number.find(it->first) != this->number.end()) {// разбираем общие делители
                common_factors.number[it->first] = min(this->number[it->first], it->second);
                if (this->number[it->first] > it->second) small_sum_1 *= pow(it->first, (this->number[it->first] - it->second));
                else if (this->number[it->first] < it->second) small_sum_2 *= pow(it->first, (it->second - this->number[it->first]));
            }
            else small_sum_2 *= pow(it->first, it->second);
        }
        for (auto it = this->number.begin(); it != this->number.end(); ++it) {
            if (_2.number.find(it->first) == _2.number.end()) {
                small_sum_1 *= pow(it->first, it->second);
            }
        }

        Number_in_factors small_sum;
        if ((small_sum_1 - small_sum_2) == 0) {
            small_sum.empty = 1;
            small_sum.number.clear();
            return small_sum;
        }
        if ((small_sum_1 - small_sum_2) < 0) small_sum.negative = -1;
        if (small_sum_1 > small_sum_2) small_sum.convert_to_factors(to_string(abs(small_sum_1 - small_sum_2)));
        else if (small_sum_1 < small_sum_2) small_sum.convert_to_factors(to_string(abs(small_sum_1 - small_sum_2)));
        return common_factors * small_sum;
    }
};

vector<unsigned long long> cancellation(Number_in_factors _1, Number_in_factors _2) {//num, num -> 10, 10 unsigned
    vector<unsigned long long> res(2, 1);
    int range = 0;
    res[0] = _1.empty == 1 ? 0 : 1;
    res[1] = _2.empty == 1 ? 0 : 1;

    try {
        for (auto it = _2.number.begin(); it != _2.number.end(); ++it) {
            if (_1.number.find(it->first) == _1.number.end())  res[1] *= pow(it->first, it->second);
            else {
                range = _1.number[it->first] - it->second;
                if (range > 0) res[0] *= pow(it->first, range);
                else if (range < 0) res[1] *= pow(it->first, abs(range));
                _1.number.erase(it->first);
            }
        }

        for (auto it = _1.number.begin(); it != _1.number.end(); ++it) {
            res[0] *= pow(it->first, it->second);
        }
    }
    catch (...) {
        cout << "ERROR IN DIV\n";
    }

    return res;
}

Number_in_factors div(Number_in_factors* _1, Number_in_factors* _2, string mode) {// целая часть от деления
    int sign = _1->negative * _2->negative;
    vector<unsigned long long> frac(2, 1);
    frac = cancellation(*_1, *_2);

    Number_in_factors res;
    if (_2->empty) {
        res.empty = 1;
        res.number.erase(1);
        cout << "DIVISION ERROR ";
        return res;
    }
    string a;
    if (mode == "div") {
        res.negative = sign;
        a = to_string(frac[0] / frac[1]);
        res.convert_to_factors(a);
    }
    else {
        res.negative = sign;
        a = to_string(frac[0] % frac[1]);
        res.convert_to_factors(a);
    }
    return res;
}

int compare(Number_in_factors _1, Number_in_factors _2) {// -1 -- <; 1 -- >; 0 -- ==
    if ((_1.negative < 0) and (_2.empty == 1)) return -1;// -1 0
    if ((_1.negative < 0) and (_2.negative > 0)) return -1;// -1 1

    if ((_1.empty == 1) and (_2.empty == 1)) return 0;// 0 0
    if ((_1.empty == 1) and (_2.negative > 0)) return -1;// 0 1
    if ((_1.empty == 1) and (_2.negative < 0)) return 1;// 0 -1

    if ((_1.negative > 0) and (_2.negative < 0)) return 1;// 1 -1
    if ((_1.negative > 0) and (_2.empty == 1)) return 1; // 1 0
    // -1 -1
    // 1 1

    vector<Number_in_factors> res(2);
    int range = 0;
    try {
        for (auto it = _2.number.begin(); it != _2.number.end(); ++it) {
            if (_1.number.find(it->first) == _1.number.end()) res[1].number[it->first] = it->second;
            else {
                range = _1.number[it->first] - it->second;
                if (range > 0) res[0].number[it->first] = range;
                else if (range < 0) res[1].number[it->first] = abs(range);
                _1.number.erase(it->first);
            }
        }

        for (auto it = _1.number.begin(); it != _1.number.end(); ++it) {
            res[0].number[it->first] = it->second;
        }
    }
    catch (...) {
        cout << "ERROR IN COMPARE\n";
    }

    // логорифмируем
    unsigned long lg_1 = 0;
    unsigned long lg_2 = 0;

    for (auto it = res[0].number.begin(); it != res[0].number.end(); ++it) {
        lg_1 += it->second * log(it->first);
    }
    for (auto it = res[1].number.begin(); it != res[1].number.end(); ++it) {
        lg_2 += it->second * log(it->first);
    }

    if (_1.negative == -1) {
        if (lg_1 > lg_2) return -1;
        if (lg_1 < lg_2) return 1;
    }
    else {
        if (lg_1 > lg_2) return 1;
        if (lg_1 < lg_2) return -1;
    }
    return 0;
    }

int main(){
    Number_in_factors A, B, C;
    string input_1, input_2;
    FIND_DIVS();// найдём все делители и сохраним их в вектор divs

    cout << "enter the first number: ";
    cin >> input_1;
    A.convert_to_factors(input_1);
    cout << "\nfirst number: ";
    A.print();

    cout << "enter the second number: ";
    cin >> input_2;
    B.convert_to_factors(input_2);
    cout << "second number: ";
    B.print();

    C = A * B;
    cout << "multiplication result: ";
    C.print();

    cout << "div: ";
    div(&A, &B, "div").print();

    cout << "mod: ";
    div(&A, &B, "mod").print();

    int r = compare(A, B);
    cout << "compare: ";
    if (r == -1) cout << "<\n";
    if (r == 1) cout << ">\n";
    if (r == 0) cout << "==\n";

    cout << "sum: ";
    C = A + B;
    C.print();

    cout << "diff: ";
    C = A - B;
    C.print();
}