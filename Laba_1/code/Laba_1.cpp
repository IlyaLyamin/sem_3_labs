#include <Windows.h>
#include <iostream>
#include <clocale>
#include <chrono>
using namespace std;
unsigned long seed = 1;
const int FIX_ARR_SIZE = 10000;


class Timer {
public:
    Timer() {
        start = chrono::high_resolution_clock::now();
        end = chrono::high_resolution_clock::now();
    }

    void start_time() {
        start = chrono::high_resolution_clock::now();
    }
    void stop_time() {
        end = chrono::high_resolution_clock::now();
    }

    void get_time() {
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        // Выведем результат в микросекундах
        std::cout << "Elapsed time: " << duration.count() << " nanoseconds" << std::endl;

    }
private:
    chrono::time_point<chrono::high_resolution_clock> start;
    chrono::time_point<chrono::high_resolution_clock> end;
};


int get_num() {
    seed = seed * 1103515245 + 12345;
    return (seed / 65536) % 1000;  
}


template <class type>
class Mass {
public:
    Mass(): height(0), wide(0){
        fill_arr(0);
    }

    Mass(type height, type wide) {
        this->height = height;
        this->wide = wide;
        fill_arr(0);
    }

    ~Mass() {
        for (int i = 0; i < height; i++) {
            delete[] array[i];
        }
        delete[] array;
    }

    void fill_arr(int flag) {// если flag == 0 то заполняем нулями в ином случае пользователь заполняет его сам
        if (!(flag)) array = new type * [height];
        for (int i = 0; i < height; i++) {
            if (!(flag)) array[i] = new type[wide];
            for (int j = 0; j < wide; j++) {
                if (!(flag)) array[i][j] = 0;
                else cin >> array[i][j];
            }
        }
    }

    void print() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < wide; j++) {
                cout << array[i][j] << " ";
            }
            cout << "\n";
        }
    }

    int get_wide() {
        return wide;
    }

    int get_height() {
        return height;
    }

    Mass<type>& operator= (const Mass<type>& rhs) {
        if (this == (&rhs)) {
            cout << "само присваивание" << "\n";
            return *this;
        }
        clear();

        this->height = rhs.height;
        this->wide = rhs.wide;
        array = new type*[height];
        
        for (int i = 0; i < height; i++) {
            array[i] = new type[wide];
            for (int j = 0; j < wide; j++) {
                array[i][j] = rhs.array[i][j];
            }
        }
        return *this;
    }

    type* operator[] (int row) {
        return (this->array[row]);
    }

private:
    int height;
    int wide;
    type** array;

    void clear() {
        if (array) {
            for (int i = 0; i < height; i++) {
                delete[] array[i];
            }
            delete[] array;
            array = nullptr;
        }
    }
};

template<class dtype>
Mass<dtype> multiplication(Mass<dtype>& array_l, Mass<dtype>& array_r) {
    Mass<dtype> new_ar(array_l.get_height(), array_r.get_wide());
    Timer x;
    x.start_time();
    for (int i = 0; i < array_l.get_height(); i++) {
        for (int j = 0; j < array_r.get_wide(); j++) {
            for (int k = 0; k < array_l.get_wide(); k++) {
                new_ar[i][j] = new_ar[i][j] + (array_l[i][k] * array_r[k][j]);
            }
        }
    }
    x.stop_time();
    x.get_time();
    return new_ar;
}

///////////////////vinograd///////////////////

template<class dtype>
void prepair_arrays(dtype* array_rows, dtype* array_columns, Mass<dtype>& array_l, Mass<dtype>& array_r, int l_h, int l_w, int r_h, int r_w) {

    for (int i = 0; i < l_h; i++) {
        array_rows[i] = 0;
        for (int j = 0; j < l_w; j++) {
            array_rows[i] += array_l[i][j] * array_l[i][j + 1];
            j++;
        }
        //cout << "строка " << i << " " << array_rows[i] << "\n";
    }
    array_rows[l_h] = -1;
    for (int i = 0; i < r_w; i++) {
        array_columns[i] = 0;
        for (int j = 0; j < r_h; j++) {
            array_columns[i] += array_r[j][i] * array_r[j + 1][i];
            j++;
        }
        //cout << "столбец " << i << " " << array_columns[i] << "\n";
    }
    array_columns[r_w] = -1;
}

template<class dtype>
Mass<dtype> vinograd_multiplication(Mass<dtype>& array_l, Mass<dtype>& array_r) {
    Mass<dtype> new_ar(array_l.get_height(), array_r.get_wide());
    dtype fix_rows[FIX_ARR_SIZE];
    dtype fix_columns[FIX_ARR_SIZE];

    Timer x;
    x.start_time();

    BOOL A = TRUE;
    int l_h = array_l.get_height();
    int l_w = array_l.get_wide();
    int r_h = array_r.get_height();
    int r_w = array_r.get_wide();

    if ((array_l.get_wide() % 2) != 0) {
        A = FALSE;
        l_w -= 1;
        r_h -= 1;
    }


    prepair_arrays(fix_rows, fix_columns, array_l, array_r, l_h, l_w, r_h, r_w);// l_h, l_w, r_h, r_w помещены в аргумент

    for (int i = 0; i < l_h; i++) {
        for (int j = 0; j < r_w; j++) {
            for (int k = 0; k < (r_h / 2); k++) {
                new_ar[i][j] += (array_l[i][2*k] + array_r[(2*k) + 1][j]) * (array_l[i][(2*k) + 1] + array_r[(2*k)][j]);
            }
            if (!(A)) {// если нечётная
                new_ar[i][j] += array_l[i][l_w] * array_r[r_h][j];
            }
            new_ar[i][j] -= (fix_rows[i] + fix_columns[j]);
        }
    }

    x.stop_time();
    x.get_time();

    return new_ar;
}

///////////////////optimized_vin///////////////////

template<class dtype>
void opt_prepair_arrays(dtype* array_rows, dtype* array_columns, Mass<dtype>& array_l, Mass<dtype>& array_r, int l_h, int l_w, int r_h, int r_w) {
    int lim = max(l_h, r_w);
    int counter = 0;
    bool L = true;
    bool R = true;  
    while (counter < lim) {
        if (L) {
            array_rows[counter] = 0;
        }
        if (R) {
            array_columns[counter] = 0;
        }
        if ((l_w != 1) && (l_h != 1)) {/// добавил
            array_rows[counter] += array_l[counter][0] * array_l[counter][1];
            array_columns[counter] += array_r[0][counter] * array_r[1][counter];
        }
        for (int j = 1; j < (l_w / 2); j++) {
            if (L) {
                array_rows[counter] += array_l[counter][(j << 1)] * array_l[counter][(j << 1) + 1];
            }
            if (R) {
                array_columns[counter] += array_r[(j << 1)][counter] * array_r[(j << 1) + 1][counter];
            }
        }
        counter++;
        if (counter >= l_h) L = false;
        if (counter >= r_w) R = false;
    }
    array_rows[l_h] = -1;
    array_columns[r_w] = -1;
}


template<class dtype>
Mass<dtype> optimized_vinograd_multiplication(Mass<dtype>& array_l, Mass<dtype>& array_r) {
    Mass<dtype> new_ar(array_l.get_height(), array_r.get_wide());
    dtype fix_rows[FIX_ARR_SIZE];
    dtype fix_columns[FIX_ARR_SIZE];

    Timer x;
    x.start_time();

    bool A = true;
    int l_h = array_l.get_height();
    int l_w = array_l.get_wide();
    int r_h = array_r.get_height();
    int r_w = array_r.get_wide();


    if ((array_l.get_wide() % 2) != 0) {
        A = FALSE;
        l_w -= 1;
        r_h -= 1;
    }

    opt_prepair_arrays(fix_rows, fix_columns, array_l, array_r, l_h, l_w, r_h, r_w);// l_h, l_w, r_h, r_w помещены в аргумент
    if (!(A)) {
        for (int i = 0; i < l_h; i++) {
            for (int j = 0; j < r_w; j++) {
                for (int k = 0; k < (r_h / 2); k++) {
                    new_ar[i][j] += (array_l[i][k << 1] + array_r[(k << 1) + 1][j]) * (array_l[i][(k << 1) + 1] + array_r[(k << 1)][j]);
                }
                new_ar[i][j] -= (fix_rows[i] + fix_columns[j] - array_l[i][l_w] * array_r[r_h][j]);
            }
        }
    }
    else {
        for (int i = 0; i < l_h; i++) {
            for (int j = 0; j < r_w; j++) {
                for (int k = 0; k < (r_h / 2); k++) {
                    new_ar[i][j] += (array_l[i][k << 1] + array_r[(k << 1) + 1][j]) * (array_l[i][(k << 1) + 1] + array_r[(k << 1)][j]);
                }
                new_ar[i][j] -= (fix_rows[i] + fix_columns[j]);
            }
        }
    }

    x.stop_time();
    x.get_time();

    return new_ar;
}

///////////////////MODES///////////////////

void MODE_1() {
    int l_hei, l_wid;
    cout << "Введите размеры первой матрицы" << "\n";
    bool norm = false;
    while (!(norm)) {
        cin >> l_hei;
        cin >> l_wid;
        if ((l_hei < 1) || (l_wid < 1)) {
            cout << "Введите нормальные размеры матрицы" << endl;
        }
        else norm = true;
    }
    Mass<int> A(l_hei, l_wid);
    Mass<int> C;
    cout << "Заполните первую матрицу" << "\n";
    A.fill_arr(1);
    A.print();

    int r_hei, r_wid;
    cout << "Введите размеры второй матрицы" << "\n";
    norm = false;
    while (!(norm)) {
        cin >> r_hei;
        cin >> r_wid;
        if ((r_hei < 1) || (r_wid < 1) || (l_wid != r_hei)) {
            cout << "Введите нормальные размеры матрицы" << endl;
        }
        else norm = true;
    }
    Mass<int> B(r_hei, r_wid);
    cout << "Заполните вторую матрицу" << "\n";
    B.fill_arr(1);
    cout << "Обычный:\n";
    C = multiplication(A, B);
    C.print();
    C = vinograd_multiplication(A, B);
    cout << "Виноград:\n";
    C.print();
    cout << "Оптимизированный виноград:\n";
    C = optimized_vinograd_multiplication(A, B);
    C.print();
}


void MODE_2() {
    srand(time(0));
    int size;

    for (int i = 0; i < 10; i++) {
        size = 10 + 10 * i;
        cout << "SIZE: " << size << endl;
        Mass<int> A(size, size);
        Mass<int> B(size, size);
        Mass<int> C(size, size);
        for (int r = 0; r < size; r++) {
            for (int c = 0; c < size; c++) {
                A[r][c] = get_num();
                B[r][c] = get_num();
            }
        }
        cout << "-----A*B----- ";
        C = multiplication(A, B);
        //C.print();
        cout << "-----A*B_vin----- ";
        C = vinograd_multiplication(A, B);
        //C.print();
        cout << "-----A*B_opt_vin----- ";
        C = optimized_vinograd_multiplication(A, B);
        //C.print();
    }
}

int main()
{
    /////////////////
    setlocale(LC_ALL, "rus");
    int mode;
    cout << "Введите режим(1 - одиночный, 2 - массовый)\n";
    cin >> mode;
    if (mode == 1) MODE_1();
    else MODE_2();
}
