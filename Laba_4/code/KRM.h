#include "base.h"
#include <map>


void create_arrays(Matrix* mat, int*& AN, int*& NR, int*& NC, int*& JR, int*& JC)
{
    int R = mat->rows;
    int C = mat->columns;
    int LEN_AN = mat->un_zero;
    int iter = -1;// Индекс в AN
    int cur_R = 0;
    int cur_C = 0;
    int loc_iter = 0;
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            if (mat->array[i][j] != 0) {
                iter++;
                if (iter >= LEN_AN) {
                    cout << "17!!!!!!!!!!!";
                    exit(1);
                }
                AN[iter] = mat->array[i][j];
                NR[iter] = iter;
                NC[iter] = iter;

                if ((JR[i] == -1) or (JC[j] == -1)) {
                    if (JR[i] == -1) JR[i] = iter;
                    if (JC[j] == -1) JC[j] = iter;
                }
                //NR
                cur_R = JR[i];
                while (cur_R < iter) {
                    NR[cur_R] = cur_R + 1;
                    cur_R += 1;
                }
                NR[iter] = JR[i];

                //NC
                cur_C = JC[j];
                while (cur_C < iter) {
                    if (NC[cur_C] == JC[j]) {
                        NC[cur_C] = iter;
                        cur_C = iter;
                    }
                    else if (NC[cur_C] != iter) {
                        cur_C = NC[cur_C];
                    }
                    else {
                        NC[cur_C] = iter;
                        cur_C = NC[cur_C];//iter
                    }
                }
                NC[iter] = JC[j];
            }
        }
    }
}

void create_arrays(map<pair<int, int>, int>* coords, int*& AN, int*& NR, int*& NC, int*& JR, int*& JC, int rows, int columns) {
    int R = rows;
    int C = columns;
    int LEN_AN = (*coords).size();

    int* IA = new int[LEN_AN];//
    int* JA = new int[LEN_AN];
    int iter = 0;// Индекс в AN
    int cur_R = 0;
    int cur_C = 0;
    for (const auto& pair : (*coords)) {
        if (pair.second != 0) {
            IA[iter] = (pair.first).first;
            JA[iter] = (pair.first).second;
            iter++;
        }
    }

    iter = -1;
    int loc_iter = 0;
    bool found_column = false;
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            if (coords->find({i, j}) != coords->end()) {
                iter++;
                if (iter >= LEN_AN) {
                    cout << "17!!!!!!!!!!!";
                    exit(1);
                }
                AN[iter] = (*coords)[{i, j}];
                NR[iter] = iter;
                NC[iter] = iter;

                if ((JR[i] == -1) or (JC[j] == -1)) {
                    if (JR[i] == -1) JR[i] = iter;
                    if (JC[j] == -1) JC[j] = iter;
                }
                //NR
                cur_R = JR[i];
                while (cur_R < iter) {
                    NR[cur_R] = cur_R + 1;
                    cur_R += 1;
                }
                NR[iter] = JR[i];

                //NC
                cur_C = JC[j];
                while (cur_C < iter) {
                    if (NC[cur_C] == JC[j]) {
                        NC[cur_C] = iter;
                        cur_C = iter;
                    }
                    else if (NC[cur_C] != iter) {
                        cur_C = NC[cur_C];
                    }
                    else {
                        NC[cur_C] = iter;
                        cur_C = NC[cur_C];//iter
                    }
                }
                NC[iter] = JC[j];
            }
        }
    }
}

class KRMCompressedMatrix {
public:
    int* AN = nullptr;// массив ненулевых элементов
    int* NR = nullptr;// индекс следующих элементов по строке в AN
    int* NC = nullptr;// индекс следующих элементов по столбцу
    int* JR = nullptr;// точки входа в строки
    int* JC = nullptr;// точки входа в столбцы
    int len_AN = 0;
    int rows = 0;
    int columns = 0;

    KRMCompressedMatrix() {}

    KRMCompressedMatrix(Matrix* mat) {
        this->len_AN = mat->un_zero;
        this->rows = mat->rows;
        this->columns = mat->columns;
        AN = new int[len_AN];// массив ненулевых элементов
        NR = new int[len_AN];// индекс следующих элементов по строке в AN
        NC = new int[len_AN];// индекс следующих элементов по столбцу
        JR = new int[rows];// точки входа в строки
        JC = new int[columns];// точки входа в столбцы
        for (int i = 0; i < len_AN; i++) {
            AN[i] = 0;
            NR[i] = -1;
            NC[i] = -1;
        }
        for (int i = 0; i < rows; i++) {
            JR[i] = -1;
        }
        for (int i = 0; i < columns; i++) {
            JC[i] = -1;
        }
        create_arrays(mat, AN, NR, NC, JR, JC);
    }

    KRMCompressedMatrix(map<pair<int, int>, int>* coords, int rows, int columns) {
        this->len_AN = (*coords).size();
        this->rows = rows;
        this->columns = columns;
        
        AN = new int[len_AN];// массив ненулевых элементов
        NR = new int[len_AN];// индекс следующих элементов по строке в AN
        NC = new int[len_AN];// индекс следующих элементов по столбцу
        JR = new int[rows];// точки входа в строки
        JC = new int[columns];// точки входа в столбцы

        for (int i = 0; i < len_AN; i++) {
            AN[i] = 0;
        }
        for (int i = 0; i < rows; i++) {
            JR[i] = -1;
        }
        for (int i = 0; i < columns; i++) {
            JC[i] = -1;
        }

        create_arrays(coords, AN, NR, NC, JR, JC, rows, columns);
    }

    KRMCompressedMatrix(int len_AN, int rows, int columns, int*& AN, int*& NR, int*& NC, int*& JR, int*& JC) {
        this->len_AN = len_AN;
        this->rows = rows;
        this->columns = columns;
        this->AN = AN;
        this->NR = NR;
        this->NC = NC;
        this->JR = JR;
        this->JC = JC;
    }

    ~KRMCompressedMatrix() {
        clear();
    }

    void print() {
        cout << "AN: ";
        for (int i = 0; i < len_AN; i++) {
            cout << setw(3) << AN[i] << " ";
        }

        cout << endl << "NR: ";
        for (int i = 0; i < len_AN; i++) {
            cout << setw(3) << NR[i] << " ";
        }

        cout << endl << "NC: ";
        for (int i = 0; i < len_AN; i++) {
            cout << setw(3) << NC[i] << " ";
        }

        cout << endl << "JR: ";
        for (int i = 0; i < rows; i++) {
            cout << setw(3) << JR[i] << " ";
        }

        cout << endl << "JC: ";
        for (int i = 0; i < columns; i++) {
            cout << setw(3) << JC[i] << " ";
        }
    }

    KRMCompressedMatrix& operator= (const KRMCompressedMatrix& rhs) {
        if (this == (&rhs)) {
            cout << "само присваивание" << "\n";
            return *this;
        }
        clear();

        this->rows = rhs.rows;
        this->columns = rhs.columns;
        this->len_AN = rhs.len_AN;
        this->AN = new int[len_AN];
        this->NR = new int[len_AN];
        this->NC = new int[len_AN];
        this->JR = new int[rows];
        this->JC = new int[columns];
        
        for (int i = 0; i < len_AN; i++) {
            this->AN[i] = rhs.AN[i];
            this->NR[i] = rhs.NR[i];
            this->NC[i] = rhs.NC[i];
        }
        for (int i = 0; i < rows; i++) {
            this->JR[i] = rhs.JR[i];
        }
        for (int i = 0; i < columns; i++) {
            this->JC[i] = rhs.JC[i];
        }
        return *this;
    }

    void clear() {
        if (AN != nullptr) {
            delete[] AN;
            AN = nullptr;
        }
        if (NR != nullptr) {
            delete[] NR;
            NR = nullptr;
        }
        if (NC != nullptr) {
            delete[] NC;
            NC = nullptr;
        }
        if (JR != nullptr) {
            delete[] JR;
            JR = nullptr;
        }
        if (JC != nullptr) {
            delete[] JC;
            JC = nullptr;
        }
    }
};

int* get_IA(KRMCompressedMatrix* _1) {//rows numbers
    int* IA = new int[_1->len_AN];
    for (int i = 0; i < _1->len_AN; i++) {
        IA[i] = -1;
    }
    int cur;
    for (int i = 0; i < _1->len_AN; i++) {
        if (IA[i] == -1) {
            for (int r = 0; r < _1->rows; r++) {
                if (i == _1->JR[r]) {
                    IA[i] = r;
                    cur = _1->NR[i];
                    while (cur != _1->JR[r]) {
                        IA[cur] = r;
                        cur = _1->NR[cur];
                        //if (cur >= _1->len_AN) cout << "294!!!!!!!!!!!!!!!!!\n";
                    }
                }
            }
        }
    }
    return IA;
}

int*& get_JA(KRMCompressedMatrix* _1) {// get columns numbers
    int* JA = new int[_1->len_AN];
    for (int i = 0; i < _1->len_AN; i++) {
        JA[i] = -1;
    }
    int cur;
    for (int i = 0; i < _1->len_AN; i++) {
        if (JA[i] == -1) {
            for (int c = 0; c < _1->columns; c++) {
                if (i == _1->JC[c]) {
                    JA[i] = c;
                    cur = _1->NC[i];
                    while (cur != _1->JC[c]) {
                        JA[cur] = c;
                        cur = _1->NC[cur];
                        //if (cur >= _1->len_AN) cout << "318!!!!!!!!!!!!!!!!!\n";
                    }
                }
            }
        }
    }
    return JA;
}

Matrix unpacking_KRM(KRMCompressedMatrix* krm) {
    int* IA = get_IA(krm);
    int* JA = get_JA(krm);

    Matrix res(krm->rows, krm->columns);
    for (int i = 0; i < krm->len_AN; i++) {
        res.array[IA[i]][JA[i]] = krm->AN[i];
    }
    res.un_zero = krm->len_AN;
    if (IA != nullptr) delete[] IA;
    if (JA != nullptr) delete[] JA;
    return res;
}

KRMCompressedMatrix KRM_addition(KRMCompressedMatrix* _1, KRMCompressedMatrix* _2) {
    if (_1->columns != _2->columns or _1->rows != _2->rows) {
        cout << "Uncampatable sizes in addition: \n";
        exit(1);
    }
    int rows = max(_1->rows, _2->rows);
    int columns = max(_1->columns, _2->columns);
    
    // coords arrays
    int* IA_1 = get_IA(_1);
    int* JA_1 = get_JA(_1);


    int* IA_2 = get_IA(_2);
    int* JA_2 = get_JA(_2);

    map <pair<int, int>, int> coords_3;
    for (int i = 0; i < _1->len_AN; i++) {
        if (coords_3.find({IA_1[i], JA_1[i]}) != coords_3.end()) {
            coords_3[{IA_1[i], JA_1[i]}] += _1->AN[i];
        }
        else {
            coords_3[{IA_1[i], JA_1[i]}] = _1->AN[i]; 
        }
    }

    for (int i = 0; i < _2->len_AN; i++) {
        if (coords_3.find({ IA_2[i], JA_2[i] }) != coords_3.end()) {
            coords_3[{IA_2[i], JA_2[i]}] += _2->AN[i];
        }
        else {
            coords_3[{IA_2[i], JA_2[i]}] = _2->AN[i];
        }
    }

    //removal the zeros
    for (auto it = coords_3.begin(); it != coords_3.end(); ) {
        if (it->second == 0) {
            it = coords_3.erase(it); // removal the element and renewing the iterator
        }
        else {
            ++it; // move to the next
        }
    }

    int len_AN = coords_3.size();
    
    cout << "\n";
    
    KRMCompressedMatrix res(&coords_3, rows, columns);
    if (JA_1 != nullptr) delete[] JA_1;
    if (IA_1 != nullptr) delete[] IA_1;
    if (JA_2 != nullptr) delete[] JA_2;
    if (IA_2 != nullptr) delete[] IA_2;

    return res;
}

KRMCompressedMatrix KRM_multiplication(KRMCompressedMatrix* _1, KRMCompressedMatrix* _2) {
    int* IA_1 = get_IA(_1);
    int* JA_1 = get_JA(_1);

    // transponiruem
    int* IA_2 = get_IA(_2);
    int* JA_2 = get_JA(_2);

    map <pair<int, int>, int> coords_1;
    for (int i = 0; i < _1->len_AN; i++) {
        coords_1[{IA_1[i], JA_1[i]}] = _1->AN[i];
    }

    map <pair<int, int>, int> coords_2;
    for (int i = 0; i < _2->len_AN; i++) {
        coords_2[{IA_2[i], JA_2[i]}] = _2->AN[i];
    }

    int R = _1->rows;
    int C = _2->columns;

    int sum = 0;
    map <pair<int, int>, int> res_coords;
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            for (int k = 0; k < _1->columns; k++) {
                if ((coords_1.find({i, k}) != coords_1.end()) and (coords_2.find({k, j}) != coords_2.end())) {
                    sum += coords_1[{i, k}] * coords_2[{k, j}];
                }
            }
            // find sum
            if (sum != 0) {
                res_coords[{i, j}] = sum;
                sum = 0;
            }       
        }
    }

    KRMCompressedMatrix a(&res_coords, R, C);

    if (JA_1 != nullptr) delete[] JA_1;
    if (IA_1 != nullptr) delete[] IA_1;
    if (JA_2 != nullptr) delete[] JA_2;
    if (IA_2 != nullptr) delete[] IA_2;
    return a;
}

