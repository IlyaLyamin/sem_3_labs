#include "stack.h"
#include <cctype>
#include<map>
#include <clocale>
#include <stdlib.h>
#include <cmath>
#include <iomanip>
const double PI = acos(-1.0);
string ERROR;


template <typename tp>
void read(tp* number) {

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

string nums = "1234567890x.";
map<string, unsigned> operators{{"-u", 2},
                                {"cos", 4},
                                {"sin", 4},
                                {"tg", 4},
                                {"ctg", 4},
                                {"acos", 4},
                                {"asin", 4},
                                {"atg", 4},
                                {"actg", 4},
                                {"^", 3},
                                {"*", 2},
                                { "/", 2 },
                                { "+", 1 },
                                { "-", 1 }
};

map<string, int> operands{ {"-u", 1},
                                {"cos", 1},
                                {"sin", 1},
                                {"tg", 1},
                                {"ctg", 1},
                                {"acos", 1},
                                {"asin", 1},
                                {"atg", 1},
                                {"actg", 1},
                                {"^", 2},
                                {"*", 2},
                                { "/", 2 },
                                { "+", 2 },
                                { "-", 2 }
};

// include x
bool is_num(char el) {
    for (int i = 0; i < 12; i++) {
        if (el == nums[i]) return true;
    }    
    return false;
}

bool is_num(string el) {
    for (int i = 0; i < 11; i++) {
        if (el[0] == nums[i]) return true;
    }
    return false;
}

bool is_oper(char el) {
    string operators = "-+/*^";
    for (int i = 0; i < 6; i++) {
        if (el == operators[i]) return true;
    }
    return false;
}

bool is_brack(char el) {
    if ((el == ')') or (el == '(')) return true;
    return false;
}

int ord(string el) {
    return operators[el];
}

bool check_valid(string* str) {

    // lower
    int len = (*str).size();
    for (int i = 0; i < len; i++) {
        (*str)[i] = tolower((*str)[i]);
    }

    if ((*str) == "") return false;

    // checking the brackets
    int count = 0;
    for (int i = 0; i < len; i++) {
        if ((*str)[i] == '(') count += 1;
        if ((*str)[i] == ')') count -= 1;
        if (count < 0) {
            cout << "Check the brackets!\n";
            return false;
        }
    }
    if (count != 0) {
        cout << "Check the brackets!\n";
        return false;
    }


    string tg = "tg";
    string _3[4] = { "sin", "cos", "ctg", "atg" };
    string _4[3] = { "asin", "acos", "actg" };


    for (int i = 0; i < len; i++) {
        char elem = (*str)[i];
        if (is_num(elem)) {
            if (i == 0) {// рядом с числом может стоять только другое число, опер или точка
                if (!((is_num((*str)[1]) or is_oper((*str)[1])))) {
                    if (((*str)[1] == '(') or ((*str)[1] == '.')) continue;
                    cout << "Unable symbol next to the first number\nTry again\n";
                    return false;
                }
            }
            else if (i == (len - 1)) {
                if (!((is_num((*str)[len - 2]) or is_oper((*str)[len - 2])))) {
                    if (((*str)[len - 2] == '(') or ((*str)[len - 2] == '.')) continue;///////////
                    cout << "Unable symbol next to the last number\nTry again\n";
                    return false;
                }
            }
            else {
                if (!((is_num((*str)[i - 1]) or is_oper((*str)[i - 1])) or ((*str)[i - 1] == '(')) or (!((is_num((*str)[i + 1]) or is_oper((*str)[i + 1]) or ((*str)[i + 1] == ')'))))) {
                    if ((*str)[i - 1] == '.' or (*str)[i + 1] == '.') continue;
                    cout << "Unable symbol next to the number\nTry again\n";
                    return false;
                }
                continue;
            }
        }
        else if (is_oper(elem)) {

            // оператор на последнем месте
            if ((i + 1) >= len) {
                cout << "!!!'" << elem << "' can't be the last symbol!!!\nTry again\n";
                return false;
            }

            // несколько бинарных операторов подряд
            if ((i + 1) < len) {
                if (is_oper((*str)[i + 1])) {
                    cout << "!!!check the the binary operator repetition!!!\n End try again\n";
                    return false;
                }
            }

            // унарный минус
            if (elem == '-') {
                if ((i + 1) >= len) {
                    cout << "!!!'-' can't be the last symbol!!!\nTry again\n";
                    return false;
                }
                continue;
            }
            // БИНАРНЫЕ операторы не могут быть первыми
            if ((i == 0)) {
                cout << "Binary operator can't be the first\nEnd try again\n";
                return false;
            }
            else {
                if (((*str)[i - 1] == '(') or ((*str)[i + 1] == ')')) {// случаи (+, +), +, где + -- любая бинарная операция кроме минуса
                    cout << "Check the correction of arguments for binary operator\nEnd try again\n";
                    return false;
                }
                else if ((elem == '/') and ((*str)[i + 1] == '0')) {// деление на ноль
                    cout << "division by zero\nTry again\n";
                    return false;
                }
            }


        }
        else if (is_brack(elem)) continue;
        else {// проверяем, на корректность тригонометрических функций
            bool indic = false;
            for (const auto [oper, od] : operators) 
                if (oper[0] == elem) {
                    if ((i + oper.size()) > len) {// проверка на то что название функции не длиннее остатка строки
                        continue;
                    }
                    string cur_func = "";
                    for (int j = 0; j < oper.size(); j++) {
                        cur_func += (*str)[i + j];
                    }
                    if (!(cur_func == oper)) {
                        continue;
                    }
                    cur_func += (*str)[i + oper.size()];
                    if (cur_func != (oper + '(')) {
                        cout << "!!!You must write the '(' after functions!!!\nTry again\n";
                        return false;
                    }
                    else {
                        i += (oper.size() - 1);
                        indic = true;
                        break;
                    }
                }
            if (indic) continue;
            cout << "!!!Such a function does not exist!!!\nTry again\n";
            return false;
        }
    }
    return true;
}

string* create_arr(string* str, int* ln) {
    int len = (*str).size();

    int ind_arr = 0;
    string* arr = new string[len];

    string cur = "";
    for (int i = 0; i < len; i++) {
        cur += (*str)[i];
        if (is_oper((*str)[i])) {//операторы
            if (i == 0) arr[ind_arr] = "-u";
            else if ((i > 0) and ((*str)[i - 1] == '(')) arr[ind_arr] = "-u";
            else arr[ind_arr] = cur;
            cur = "";
            ind_arr++;
        }
        else if (is_num((*str)[i])) {
            if (len > (i + 1)) {
                if (!(is_num((*str)[i + 1]))) {
                    arr[ind_arr] = cur;
                    ind_arr++;
                    cur = "";
                }
            }
            else {
                arr[ind_arr] = cur;
                ind_arr++;
                cur = "";
            }
        }
        else if (is_brack((*str)[i])) {
            arr[ind_arr] = cur;
            ind_arr++;
            cur = "";
        }
        else {
            if (len > (i + 1)) {
                if ((*str)[i + 1] == '(') {
                    arr[ind_arr] = cur;
                    ind_arr++;
                    cur = "";
                }
            }
            else {
                arr[ind_arr] = cur;
                ind_arr++;
                cur = "";
            }
        }
    }
    (*ln) = ind_arr;
    return arr;
}

string* create_postf_arr(string* st_arr, int *len) {
    Stack<string> stack(*len);// с запасом
    string* postf_arr = new string[(*len)];

    int ind = 0;// для польской записи

    for (int i = 0; i < (*len); i++) {// i - cursor
        //1.
        if (is_num(st_arr[i])) {
            postf_arr[ind] = st_arr[i];
            ind++;
        }
        //2.
        else if ((st_arr[i]) == "(") {
            Push(&stack, st_arr[i]);
        }
        //3.
        else if ((st_arr[i]) == ")") {
            while (!(IsEmpty(&stack)) and (Peek(&stack) != "(")) {
                Pop(&stack, &((postf_arr)[ind]));
                ind++;
            }
            if (Peek(&stack) == "(") {
                Del(&stack);
            }
            //3.2
             else if (IsEmpty(&stack)) {
                cout << "ОШИБКА РАЗБОРА\n";
                break;
             }
        }
        //4.
        else {
            while (true) {
                //4.1
                if (IsEmpty(&stack)) {
                    Push(&stack, st_arr[i]);
                    break;
                }
                //4.2
                else if ((Peek(&stack) == "(")) {
                    Push(&stack, st_arr[i]);
                    break;
                }
                //4.3
                else if (ord(st_arr[i]) > ord(Peek(&stack))) {
                    Push(&stack, st_arr[i]);
                    break;
                }
                //4.4
                else {
                    Pop(&stack, &((postf_arr)[ind]));
                    ind++;
                }
            }
        }
    }

    while (!(IsEmpty(&stack))) {
        Pop(&stack, &((postf_arr)[ind]));
        ind++;
    }
    (*len) = ind;
    return postf_arr;
}

float arithm(float num_1, float num_2, string oper) {
    //так как элементы достаются из стека берём цифры в обратном порядке
    if (oper == "/") {
        if (num_1 != 0) {
            return num_2 / num_1;
        }
        ERROR = "!!!Division by zero!!!";
    }
    if (oper == "*") return num_2 * num_1;
    if (oper == "-") return num_2 - num_1;
    if (oper == "+") return num_2 + num_1;
    if (oper == "^") {
        if ((num_1 == 0) and (num_2 < 0)) {
            ERROR = "!!!Arithmetic error!!!";
        }
        else if ((num_1 == 0) and (num_2 == 0)) return 1;
        else if ((num_2 < 0) and (round(num_1) != num_1)) {
            ERROR = "!!!Arithmetic error!!!";
        }
        else if (num_1 == 0) return 1;
        return pow(num_2, num_1);
    }
}

float arithm(float num, string oper) {
    if (oper == "-u") return -num;
    if (oper == "tg") return tan(num / 180 * PI);
    if (oper == "ctg") return pow(tan(num / 180 * PI), -1);
    if (oper == "sin") return sin(num / 180 * PI);
    if (oper == "cos") return cos(num / 180 * PI);
    if (oper == "acos") return acos(num / 180 * PI);
    if (oper == "asin") return asin(num);
    if (oper == "atg") return atan(num);
    if (oper == "actg") return pow(atan(num), -1);
}

void calculate(string s, float left, float right, float step) {
    while ((left > right) || (step <= 0)){
        cout << "Enter correct dates" << endl;
        cout << "LEFT: ";
        cin >> left;
        cout << "RIGHT: ";
        cin >> right;
        cout << "STEP: ";
        cin >> step;
    }
    cin.ignore();
    cout << "Enter the arithmetic expression: ";
    getline(std::cin, s);
    string s_read;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] != ' ') s_read += s[i];
    }
    int ans = check_valid(&s_read);
    while (!(ans)) {
        cout << "Enter expression again\n";
        getline(std::cin, s);
        s_read.clear();
        for (int i = 0; i < s.size(); i++) {
            if (s[i] != ' ') s_read += s[i];
        }
        ans = check_valid(&s_read);
    }
    int len;
    string* arr = create_arr(&s_read, &len);// запарсили

    string* post_arr = create_postf_arr(arr, &len);// привели к постфиксной
    cout << endl;
    cout << "LEN: " << len << endl;
    cout << "POSTFIX: ";
    for (int i = 0; i < len; i++) {
        cout << post_arr[i] << "_";
    }
    cout << endl;

    Stack<float> nums(len);
    cout << "Value in the interval from " << left << " to " << right << " with step " << step << ":";
    for (float i = left - step; i <= right; i += step) {
        i += step;
        if (i > right) i = right;
        int ind = 0;
        for (int j = 0; j < len; j++) {
            if (is_num(post_arr[j])) {
                if (post_arr[j] == "x") {
                    Push(&nums, i);
                }
                else {
                    Push(&nums, stof(post_arr[j]));
                }
            }
            else {
                if (nums.col_elem < operands[post_arr[j]]) {
                    cout << "!!!Insufficient number of arguments!!!";
                    exit(1);
                }
                else {
                    if (operands[post_arr[j]] == 1) {
                        float _1;
                        Pop(&nums, &_1);
                        Push(&nums, arithm(_1, post_arr[j]));
                    }
                    else {
                        float _1, _2;
                        Pop(&nums, &_1);
                        Pop(&nums, &_2);
                        Push(&nums, arithm(_1, _2, post_arr[j]));
                    }
                }
            }
        }
        float ans;
        Pop(&nums, &ans);
        cout << endl;
        if (ERROR == "") cout << "|x = " << i << setw(4) << "| " << ans << "|" << endl;
        else cout << "|x = " << i << setw(4) << "| " << ERROR << "|" << endl;
        ERROR = "";
        if (i < right) i -= step;
    }
}

int main()
{
    //setlocale(LC_ALL, "rus");
    string s;
    float left = 0;
    float right = 0;
    float step = 0;
    cout << "Enter the LEFT boundary of the segment: ";
    read<float>(&left);
    cout << "Enter the RIGHT boundary of the segment: ";
    read<float>(&right);
    cout << "Enter the STEP: ";
    read<float>(&step);
    calculate(s, left, right, step);
}
// 1+2-3/4*tg(x)
// 3+5*x-7
// sin(x)+cos(45)-tg(x)*ctg(45)
// -3^x+acos(1/2)
// (-3)^x+acos(1/2)