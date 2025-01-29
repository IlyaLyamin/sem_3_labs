#include "Timer.h"
#include <vector>
#include <random>
unsigned long seed = 1;


// Функция генерации случайной строки заданной длины
string gen_random_str(int length) {
    string  characters = "abcdefghijklmnopqrstuvwxyz";
    random_device rd;  // Источник энтропии
    mt19937 generator(rd());  // Генератор случайных чисел
    uniform_int_distribution<> distribution(0, characters.size() - 1);

    string randomString;
    for (int i = 0; i < length; ++i) {
        randomString += characters[distribution(generator)];  // Выбираем случайный символ
    }
    return randomString;
}


void aloritm_Lev(matrix* m) {
    int n;
    for (int i = 0; i < (*m).ro(); i++) {
        for (int j = 0; j < (*m).co(); j++) {
            //formula for finding the Levenshtein distance
            if (i == 0) {
                (*m)[i][j] = j;
            }
            else if (j == 0) {
                (*m)[i][j] = i; //2 АБСОЛЮТНО БЕСПОЛЕЗНЫХ УСЛОВИЯ, ПОТОМУ ЧТО ПЕРВАЯ СТРОКА И СТОЛБЕЦ ПРИНИМАЮТ НУЖНЫЙ ВИД ЕЩЁ НА ЭТАПЕ ФОРМИРОВАНИЯ МАТРИЦЫ
            }
            else {
                if ((*m).str(2)[i] == (*m).str(1)[j]) n = 0;
                else n = 1;
                (*m)[i][j] = min(min((*m)[i][j - 1] + 1, (*m)[i - 1][j] + 1), ((*m)[i - 1][j - 1] + n));
            }
        }
    }
}

void algoritm_Dam_Lev(matrix* m) {
    int n;
    for (int i = 0; i < (*m).ro(); i++) {
        for (int j = 0; j < (*m).co(); j++) {
            //formula for finding the Damerau-Levenshtein distance
            if (i == 0) {
                (*m)[i][j] = j;
            }
            else if (j == 0) {
                (*m)[i][j] = i; //2 АБСОЛЮТНО БЕСПОЛЕЗНЫХ УСЛОВИЯ, ПОТОМУ ЧТО ПЕРВАЯ СТРОКА И СТОЛБЕЦ ПРИНИМАЮТ НУЖНЫЙ ВИД ЕЩЁ НА ЭТАПЕ ФОРМИРОВАНИЯ МАТРИЦЫ
            }
            else {
                if ((*m).str(2)[i] == (*m).str(1)[j]) n = 0;
                else n = 1;
                if ((j > 1) && (i > 1) && ((*m).str(2)[i] == (*m).str(1)[j - 1]) && ((*m).str(2)[i - 1] == (*m).str(1)[j])) {
                    (*m)[i][j] = min(min((*m)[i][j - 1] + 1, (*m)[i - 1][j] + 1), min((*m)[i - 1][j - 1] + n, (*m)[i - 2][j - 2] + 1));
                }
                else {
                    (*m)[i][j] = min(min((*m)[i][j - 1] + 1, (*m)[i - 1][j] + 1), (*m)[i - 1][j - 1] + n);
                }
            }
        }
    }
}

int alg_Lev_rec(matrix* m, int len_2, int len_1) {
    int n;

    //formula for finding the Levenshtein distance by recirsion
    if (len_1 == 0) {
        return len_2;
    }
    else if (len_2 == 0) {
        return len_1;
    }
    else {
        if ((*m).str(2)[len_2] == (*m).str(1)[len_1]) n = 0;
        else n = 1;
        return min(min(alg_Lev_rec(m, len_2, len_1 - 1) + 1, alg_Lev_rec(m, len_2 - 1, len_1) + 1), alg_Lev_rec(m, len_2 - 1, len_1 - 1) + n);
    }
}

int alg_Dam_Lev_rec(matrix* m, int len_2, int len_1) {
    int n;

    //formula for finding the Levenshtein distance by recirsion
    if (len_1 == 0) {
        return len_2;
    }
    else if (len_2 == 0) {
        return len_1;
    }
    else {
        if ((*m).str(2)[len_2] == (*m).str(1)[len_1]) n = 0;
        else n = 1;
        if ((len_1 > 1) && (len_2 > 1) && ((*m).str(2)[len_2] == (*m).str(1)[len_1 - 1]) && ((*m).str(2)[len_2 - 1] == (*m).str(1)[len_1])) {
            return min(min(alg_Dam_Lev_rec(m, len_2, len_1 - 1) + 1, alg_Dam_Lev_rec(m, len_2 - 1, len_1) + 1), min(alg_Dam_Lev_rec(m, len_2 - 1, len_1 - 1) + n, alg_Dam_Lev_rec(m, len_2 - 2, len_1 - 2) + 1));
        }
        return min(min(alg_Dam_Lev_rec(m, len_2, len_1 - 1) + 1, alg_Dam_Lev_rec(m, len_2 - 1, len_1) + 1), alg_Dam_Lev_rec(m, len_2 - 1, len_1 - 1) + n);
    }
}

int alg_Lev_rec_cash(matrix* m, int len_2, int len_1) {
    // Проверка, было ли значение уже рассчитано и сохранено в кэше
    if ((*m)[len_2][len_1] != -1) {
        return (*m)[len_2][len_1];
    }

    int n;
    if (len_1 == 0) {
        return len_2;
    }
    else if (len_2 == 0) {
        return len_1;
    }
    else {
        if ((*m).str(2)[len_2] == (*m).str(1)[len_1]) {
            n = 0;
        }
        else {
            n = 1;
        }

        // Вычисление расстояния Левенштейна с сохранением результата в кэш
        (*m)[len_2][len_1] = min(min(alg_Lev_rec_cash(m, len_2, len_1 - 1) + 1, alg_Lev_rec_cash(m, len_2 - 1, len_1) + 1), alg_Lev_rec_cash(m, len_2 - 1, len_1 - 1) + n);
        return (*m)[len_2][len_1];
    }
}

void MODE_1() {
    string a, b;
    cout << "Enter the first word:\n";
    cin >> a;
    cout << "Enter the second word:\n";
    cin >> b;
    matrix m_1(a, b);
    matrix m_2(a, b);
    matrix m_3(a, b);

    Timer x;

    x.start_time();
    aloritm_Lev(&m_1);
    cout << "Levenshtein algorithm result: " << m_1[m_1.ro() - 1][m_1.co() - 1] << endl;
    cout << "Resulting matrix:" << endl;
    x.stop_time();
    m_1.print();
    x.get_time();
    cout << endl;

    x.start_time();
    algoritm_Dam_Lev(&m_2);
    cout << "Damerau Levenshtein algorithm result: " << m_2[m_2.ro() - 1][m_2.co() - 1] << endl;
    cout << "Resulting matrix:" << endl;
    x.stop_time();
    m_2.print();
    x.get_time();
    cout << endl;

    if ((a.size() + b.size()) < 12) {
        x.start_time();
        cout << "Levenshtein recursion algorithm result: " << alg_Lev_rec(&m_3, b.size(), a.size()) << endl;
        x.stop_time();
        x.get_time();
        cout << endl;
        x.start_time();
        cout << "Damerau Levenshtein recursion algorithm result: " << alg_Dam_Lev_rec(&m_3, b.size(), a.size()) << endl;
        x.stop_time();
        x.get_time();
        cout << endl;
    }
    
    x.start_time();
    cout << "Levenshtein recursion with cash algorithm result: " << alg_Lev_rec_cash(&m_3, b.size(), a.size()) << endl;
    x.stop_time();
    x.get_time();
    cout << endl;
}

void MODE_2() {
    Timer x;
    string a, b;
    cout << "Comparison of recursive algorithms with ordinary" << endl;
    for (int i = 0; i <= 6; i += 2) {
        a = gen_random_str(i + 1);
        b = gen_random_str(i + 1);
        cout << endl << "--------------" << a << "--->" << b << "--------------" << endl;
        matrix m_1(a, b);
        matrix m_2(a, b);
        x.start_time();
        cout << "Levenshtein recursion algorithm result: " << alg_Lev_rec(&m_1, b.size() + 1, a.size() + 1) << endl;
        x.stop_time();
        cout << "Damerau Levenshtein recursion algorithm result: " << alg_Dam_Lev_rec(&m_1, b.size() + 1, a.size() + 1) << endl;
        cout << "Recursion time - ";
        x.get_time();

        x.start_time();
        aloritm_Lev(&m_2);
        x.stop_time();
        cout << "Ordinary time - ";
        x.get_time();
        cout << endl;
    }

    cout << endl << endl << "measurement of recursive algorithms with cash" << endl;
    for (int i = 200; i <= 600; i += 50) {
        a = gen_random_str(i + 1);
        b = gen_random_str(i + 1);
        matrix m_1(a, b);
        matrix m_2(a, b);
        matrix m_3(a, b);
        x.start_time();
        cout << "LEN: " << i << endl;
        cout << "Levenshtein algorithm with cash result: " << alg_Lev_rec_cash(&m_3, b.size(), a.size()) << endl;
        x.stop_time();
        x.get_time();

        x.start_time();
        aloritm_Lev(&m_1);
        cout << "Levenshtein algorithm result: " << m_1[m_1.ro() - 1][m_1.co() - 1] << endl;
        x.stop_time();
        x.get_time();

        x.start_time();
        algoritm_Dam_Lev(&m_2);
        cout << "Damerau Levenshtein algorithm result: " << m_2[m_2.ro() - 1][m_2.co() - 1] << endl;
        x.stop_time();
        x.get_time();
        cout << endl;
    }
}



int main()
{
    setlocale(LC_ALL, "rus");
    cout << "Choose the mode: 1 - single, 2 - mass" << endl;
    int ind;
    read(&ind);
    while (true){
        if (ind == 1) {
            MODE_1();
            break;
        }
        else if (ind == 2) {
            MODE_2();
            break;
        }
        else {
            cout << "Enter a valid number" << endl;
            read(&ind);
        }
    }
}
