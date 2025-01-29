#include "stack.h"
#include <clocale>


void sort(int* st, int len) {//сортировка расчёсткой
    float factor = 1.247;
    int distance = (len) - 1;
    while (true) {
        for (int i = 0; (i + distance) < len; i++) {
            if ((st[i]) > (st[i + distance])) {
                (st[i]) += st[i + distance];
                (st[i + distance]) = st[i] - st[i + distance];
                (st[i]) -= (st[i + distance]);
            }
        }
        if (distance == 1) break;
        distance /= factor;
    }
}

int main()
{
    setlocale(LC_ALL, "rus");
    int len;
    cout << "Ввдите размер матрицы\n";
    cin >> len;

    int** MATRIX = new int* [len];
    for (int i = 0; i < len; i++) {
        MATRIX[i] = new int[len];
        for (int j = 0; j < len; j++) {
            if (i == j) MATRIX[i][j] = 0;
            else {
                cout << "Введите длину пути: " << i << " -> " << j << endl;
                cin >> MATRIX[i][j];
            }
        }
    }
    cout << "Получилась матрица:" << endl;
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            cout << MATRIX[i][j] << " ";
        }
        cout << endl;
    }

    setlocale(LC_ALL, "rus");
    Stack<int> towns(len);
    for (int i = 0; i < len; i++) {
        Push(&towns, i);
    }
    print(&towns);

    int* sp = new int[len];
    for (int i = 0; i < len; i++) {
        sp[i] = NULL;
    }

    int min_root = 100000;
    int beast[100];
    int i = 0;
    while (!(IsEmpty(&towns))) {
        int cur = Peek(&towns);
        Pop(&towns, &sp[i]);
        if (i > 0) {
            sort(sp, i + 1);
        }
        if (sp[i] != cur) {// Если есть элемент больше чем текцщий
            int added = 0;
            for (int j = 0; j <= i; j++) {
                if (sp[j] == cur) {
                    added = sp[j + 1];
                    break;
                }
            }
            if (added) {
                Push(&towns, added);
                for (int j = 0; j <= i; j++) {// добавляем оставшиеся по очереди
                    if (sp[j] != added) {
                        Push(&towns, sp[j]);
                    }
                    sp[j] = 0;
                }
            }
            print(&towns);
            // вычислем стоимость пути
            int cur_price = 0;
            for (int j = 0; j < (len - 1); j++) {
                cur_price += MATRIX[*(towns.array[j])][*(towns.array[j + 1])];
            }
            cout << cur_price << endl;
            if (cur_price < min_root) {
                for (int j = 0; j < len; j++) {
                    beast[j] = *(towns.array)[j];
                }
                min_root = cur_price;
            }
            cout << endl;
            i = 0;
            continue;
        }
        i++;
    }
    cout << "Лучший путь: " << min_root << endl;
    for (int j = 0; j < len; j++) {
        if ((j + 1) == len) {
            cout << beast[j];
            break;
        }
        cout << beast[j] << "->";
    }
    cout << endl;
}
