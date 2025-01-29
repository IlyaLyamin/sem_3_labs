#include "base.h"
#include <algorithm>
#include <stdexcept>
#include <numeric>
#include <clocale>
#include "to_tex.h"

double a, b, q;// + в особенности b
int DAYS = 500;// +
int MODE;// +
int MODE_2;// +
int N; // количество городов и муравьёв+
Matrix<int> D; // матрица смежности +
Matrix<double> T; // матрица ферормонов от D+
double Q; // квота феромона от D+
double l = 0.00000001; //константа для феромона

double calculateMedian(vector<int>& v) {
    if (v.empty()) {
        throw invalid_argument("Вектор пуст, медиана не может быть вычислена.\n");
    }
    sort(v.begin(), v.end());
    size_t size = v.size();
    if (size % 2 == 1) {
        return v[size / 2];
    }
    return (v[size / 2 - 1] + v[size / 2]) / 2.0;
}

double calculateMean(const std::vector<int>& v) {
    if (v.empty()) {
        throw invalid_argument("Вектор пуст, среднее арифметическое не может быть вычислено.\n");
    }

    double sum = accumulate(v.begin(), v.end(), 0.0);
    return sum / v.size();
}

void show_route(vector<int>* nodes) {
    cout << "---\n";
    for (int i = 0; i < (nodes->size() - 1); i++) {
        cout << (*nodes)[i]  << "->" << (*nodes)[i + 1] << ": " << D.array[(*nodes)[i]][(*nodes)[i + 1]] << endl;
    }
    if (MODE == 1) cout << (*nodes)[(nodes->size() - 1)] << "->" << (*nodes)[0] << ": " << D.array[(*nodes)[(nodes->size() - 1)]][(*nodes)[0]] << endl;// цикл
}

int get_length(vector<int>* nodes) {
    int length = 0;
    for (int i = 0; i < (nodes->size() - 1); i++) {
        length += D.array[(*nodes)[i]][(*nodes)[i + 1]];
    }
    if (MODE == 1) length += D.array[(*nodes)[(nodes->size() - 1)]][(*nodes)[0]];
    return length;
}

void count_Q() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Q += D.array[i][j];
        }
    }
    if (MODE == 1) Q /= N;
    else Q /= (N - 1);
}

void upgrate_pheromon(vector<vector<int>>* routes, int L_best) {
    //cout << "\nAMOUNT OF THE BEST ROUTES: " << routes->size();
    for (int i = 0; i < routes->size(); i++) {
        for (int j = 0; j < N; j++) {
            T.array[i][j] += Q / L_best;
        }
    }
}

int algo(int m) {
    vector<int> L(N, 0);// массив всех длин
    int L_b = 100000000000; // наилучший путь
    vector<vector<int>> best_routes;// массив для лучших путей
    int cur; // текущий город муравья
    double n;// 1/на расстояние между рёбрами
    double choose;// выбор муравья (рандомное число)
    double sum_prob = 0;// ограничение веротности
    Matrix<int> M_h(N);// матрица хранящая историю ходов муравья (ОДНОГО) за день
    Matrix<double> local_T(N);// феромон выделяющийся за день

    for (int t = 0; t < DAYS; t++) {//дни
        vector<vector<int>> routes(N, vector<int>(0));
        for (int k = 0; k < N; k++) {//город начала/муравьи
            vector<double> P(N, 0);// вероятности
            cur = k;
            while (routes[k].size() < N) {
                sum_prob = 0;
                routes[k].push_back(cur);

                for (int g = 0; g < N; g++) {// вероятности
                    if (find(routes[k].begin(), routes[k].end(), g) != routes[k].end()) P[g] = 0;// если город уже посещался
                    else {
                        n = 1.0 / D.array[cur][g];// k->g
                        P[g] = pow(n, a) * pow(T.array[cur][g], b);
                        sum_prob += P[g];
                    }
                }
                if (sum_prob == 0) break;
                for (int g = 0; g < N; g++) {
                    if (find(routes[k].begin(), routes[k].end(), g) != routes[k].end()) P[g] = 0;// если город уже посещался
                    else {
                        P[g] /= sum_prob;
                    }
                }
                choose = get_random_number(1.0);//выборы следующего города
                for (int i = 0; i < N; i++) {
                    choose -= P[i];
                    if (choose < 0) {
                        M_h.array[cur][i] = 1;
                        cur = i;// перешёл в город i
                        break;
                    }
                }
            }
            //обновление феромона локальной матрице
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    if (M_h.array[i][j] != 0) local_T.array[i][j] = Q / get_length(&routes[k]);
                }
            }

            M_h.zero();
        }

        // обновление лучшего результата (вечер)
        for (int i = 0; i < N; i++) {
            if (get_length(&routes[i]) < L_b) {
                L_b = get_length(&routes[i]);
                best_routes.clear();
                best_routes.push_back(routes[i]);
            }
            else if (get_length(&routes[i]) == L_b) {
                if (best_routes.front() != routes[i]) best_routes.push_back(routes[i]);
            }
        }
        if (m == 1) {
            cout << "\n" << L_b << ":\n";
            for (int i = 0; i < best_routes.size(); i++)
            {
                show_route(&best_routes[i]);
            }
        }

        // общее обновление феромона (ночь)
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                T.array[i][j] = T.array[i][j] * (1.0 - q) + local_T.array[i][j];
                if (T.array[i][j] < l) T.array[i][j] = l;// проверка феромона на константу
            }
        }
        // чистка феромона
        local_T.zero();
        
        // модернизация с элитными муравьями
        if (MODE_2 == 2) upgrate_pheromon(&best_routes, L_b);
    }
    return L_b;
}

void researching() {
    string fileName = "result.txt";
    ofstream outFile(fileName);

    if (!outFile.is_open()) {
        cerr << "ERROR! UNABLE TO OPEN THE FILE.\n";
    }

    int L_BEST = 1000000000;
    vector<string> database = {"tests/_5_15.txt", "tests/_4_10.txt", "tests/_2_10.txt"};
    int number_of_file;
    cout << "ENTER THE NUMBER OF FILE(1, 2, 3): ";
    cin >> number_of_file;
    string FILE = database[number_of_file - 1];
    D = read_matrix(FILE);
    N = D.size;
    count_Q();
    Matrix<double> new_m(N);
    T = new_m;
    int local_best = 100000;
    cout << "ENTER THE MODE(CYCLE OR ROUTE): ";
    cin >> MODE;
    cout << "ENTER THE MODE_2(USUAL OR WUTH ELITE ANTS): ";
    cin >> MODE_2;
    vector<int> res;
    for (double i = 0.200001; i <= 0.9999; i += 0.25) {
        a = i;
        for (double j = 0.200001; j <= 0.9999; j += 0.25) {
            q = j;
            outFile << "a: " << a << "\nq: " << q << "\n";
            cout << "a: " << a << "\nq: " << q << "\n";
            for (int d = 200; d < 1000; d += 300) {
                cout << "DAYS--------------------------" << d << "\n";
                outFile << "DAYS--------------------------" << d << "\n";
                DAYS = d;
                b = 1.0 - a;
                for (int t = 0; t < 5; t++) {
                    res.push_back(algo(2));
                    cout << (t + 1) << ") MATRIX: " << FILE << "\nMODE: " << MODE << "\nMODE_2: " << MODE_2 << "\nRESULT: " << res.back() << "\n";
                    if (local_best > res.back()) local_best = res.back();
                    if (L_BEST > res.back()) L_BEST = res.back();
                }
                cout << "BEST:" << local_best << "\n";
                cout << "AVERAGE: " << calculateMean(res) << "++++++++++++++++++++++++++++\n";
                cout << "MEDIANA: " << calculateMedian(res) << "++++++++++++++++++++++++++++\n";
                cout << "\n";
                outFile << "BEST:" << local_best << "\n";
                outFile << "AVERAGE:" << calculateMean(res) << "\n";
                outFile << "MEDIANA:" << calculateMedian(res) << "\n";
                outFile << "\n";
                res.clear();
                local_best = 1000000;
            }
            cout << "\n";
        }
    }
    cout << "\n THE BEST RESULT: " << L_BEST;
    outFile << L_BEST;
    outFile.close();
    create_tex();
}

void prgram(int m) {
    if (m == 1){
        D = read_matrix("tests/_2_10.txt");
        D.print();
        cout << "ENTER THE FIRST PARAMETR:\n";
        read_num(&a);
        b = 1 - a;
        cout << "a = " << a << "\nb = " << b << "\n";
        cout << "ENTER THE EVAPORATION COEFFICIENT:\n";
        read_num(&q);
        N = D.size;
        Matrix<double> new_m(N);
        T = new_m;

        cout << "CHOOSE THE MODE:\n1 - HAMILTONIAN CYCLE\n2 - SHORTEST ROUTE\n";
        while (MODE == 0) {
            cin >> MODE;
            if ((MODE != 1) and (MODE != 2)) {
                cout << "ERROR, TRY AGAIN!\n";
                MODE = 0;
            }
        }

        cout << "CHOOSE THE MODE:\n1- USUAL ALGORITM\n2- ALGORITM WITH ELITE ANTS\n";
        while (MODE_2 == 0) {
            cin >> MODE_2;
            if ((MODE_2 != 1) and (MODE_2 != 2)) {
                cout << "ERROR, TRY AGAIN!\n";
                MODE_2 = 0;
            }
        }

        count_Q();
        algo(1);
    } 
    else {
        N = D.size;
        Matrix<double> new_m(N);
        T = new_m;

        count_Q();
        algo(2);
    }
    
}

int main()
{
    //setlocale(LC_ALL, "rus");
    int m = 0;
    cout << "1 - program\n2 - research\n";
    while (m == 0) {
        cin >> m;
        if ((m != 1) and (m != 2)) {
            cout << "ERROR, TRY AGAIN!\n";
            m = 0;
        }
    }
    if (m == 1) prgram(1);
    else researching();
}
