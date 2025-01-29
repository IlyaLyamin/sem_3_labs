#include "base.h"
#include <vector>

struct CompressedMatrix {
    int* AN = nullptr;
    int* D = nullptr;
    int len_AN = 0;
    int size = 0;

    CompressedMatrix() {}

    CompressedMatrix(int* AN, int* D, int len_AN, int size) {
        this->AN = new int[len_AN];
        for (int i = 0; i < len_AN; i++) {
            this->AN[i] = AN[i];
        }
        this->D = new int[size];
        for (int i = 0; i < size; i++) {
            this->D[i] = D[i];
        }
        this->len_AN = len_AN;
        this->size = size;
    }

    ~CompressedMatrix() {
        clear();
    }

    void print() {
        cout << "AN: ";
        for (int i = 0; i < len_AN; i++) {
            cout << AN[i] << " ";
        }
        cout << endl << "D: ";
        for (int i = 0; i < size; i++) {
            cout << D[i] << " ";
        }
    }

    CompressedMatrix& operator= (const CompressedMatrix& rhs) {
        if (this == (&rhs)) {
            cout << "само присваивание" << "\n";
            return *this;
        }
        clear();

        this->size = rhs.size;
        this->len_AN = rhs.len_AN;
        this->AN = new int[len_AN];
        this->D = new int[size];

        for (int i = 0; i < len_AN; i++) {
            AN[i] = rhs.AN[i];
        }
        for (int i = 0; i < size; i++) {
            D[i] = rhs.D[i];
        }
        return *this;
    }

    void clear() {
        if (AN != nullptr) {
            delete[] AN;
            AN = nullptr;
        }
        if (D != nullptr) {
            delete[] D;
            D = nullptr;
        }
    }
};

void create_AN(int** matrix, int size, int* an_size, int** D, int** AN) {
    vector<int> an_loc;

    int glob_ind = 0;
    int last_ind = 0;// количнство элементов которые надо сохранить

    for (int i = 0; i < size; i++) {
        (*D)[i] = glob_ind;
        last_ind = i;
        for (int j = i; j < size; j++) {// смотрим до какого элемента добавлять
            if (matrix[i][j] != 0) last_ind = j;
        }
        for (int j = i; j < (last_ind + 1); j++) {
            an_loc.push_back(matrix[i][j]);
        }
        glob_ind += (last_ind - i) + 1;
    }

    (*an_size) = an_loc.size();
    (*AN) = new int[(*an_size)];
    for (int i = 0; i < (*an_size); i++) {
        (*AN)[i] = an_loc[i];
    }
}

CompressedMatrix matrix_packagin(Matrix* mat) {
    int AN_size = 0;
    int* D = new int[(*mat).rows];
    int* AN = nullptr;

    create_AN((*mat).array, (*mat).rows, &AN_size, &D, &AN);

    CompressedMatrix A(AN, D, AN_size, (*mat).rows);
    delete[] AN;
    delete[] D;
    return A;
}

Matrix matrix_unpackagin(CompressedMatrix* comp_mat) {
    Matrix ans(comp_mat->size);
    ans.un_zero = comp_mat->len_AN;
    cout << endl;
    
    int row = -1;
    int column = -1;
    int ind_D = 0;
    for (int i = 0; i < comp_mat->len_AN; i++) {
        column++;
        if (i == comp_mat->D[ind_D]) {
            row++;
            column = row;
            ind_D++;
        }
        ans.array[row][column] = comp_mat->AN[i];
        ans.array[column][row] = comp_mat->AN[i];
    }
    return ans;
}

CompressedMatrix sum_pack_matrix(CompressedMatrix* _1, CompressedMatrix* _2) {
    vector<int> *loc_sum = new vector<int>[_1->size];
    vector<int> result_AN;
    vector<int> result_D;
    int len_row_1 = 0;
    int len_row_2 = 0;
    int iter_row = 0;

    while (iter_row < (_1->size - 1)) {//size==rows
        len_row_1 = _1->D[iter_row + 1] - _1->D[iter_row];
        len_row_2 = _2->D[iter_row + 1] - _2->D[iter_row];
        for (int i = 0; i < max(len_row_1, len_row_2); i++) {
            loc_sum[iter_row].push_back(0);
            if (i < len_row_1) loc_sum[iter_row][loc_sum[iter_row].size() - 1] += _1->AN[_1->D[iter_row] + i];
            if (i < len_row_2) loc_sum[iter_row][loc_sum[iter_row].size() - 1] += _2->AN[_2->D[iter_row] + i];
        }
        iter_row++;
    }
    loc_sum[iter_row].push_back(0);
    loc_sum[iter_row][loc_sum[iter_row].size() - 1] += _1->AN[_1->D[iter_row]] + _2->AN[_2->D[iter_row]];
    int last_ind = 1; //инедекс до которого добавляем элемент

    for (int i = 0; i < _1->size; i++) {
        result_D.push_back(result_AN.size());
        //last_ind = 0;
        for (int el = 0; el < loc_sum[i].size(); el++) {
            if (loc_sum[i][el] != 0) last_ind = el;
        }
        for (int j = 0; j <= last_ind; j++) {
            result_AN.push_back(loc_sum[i][j]);
        }
    }

    int* D = new int[_1->size];
    int* AN = new int[result_AN.size()];
    for (int i = 0; i < result_AN.size(); i++) {
        AN[i] = result_AN[i];
    }
    for (int i = 0; i < _1->size; i++) {
        D[i] = result_D[i];
    }

    CompressedMatrix ans(AN, D, result_AN.size(), _1->size);
    return ans;
}