#include "Dgeninks.h"
#include "KRM.h"
// Файл заполняется в виде SIZE;s; \n матрица[s][s]
// Либо если матрица не квадратная, то вместо s, пишем (r,c), где r - количество строк, а c - количество столбцов
// C++17
//KRM:
// _1 И _2 для проверки суммы равной нулю
// _3 и _4 для проверки из презентации (Только для сложения)
// умножение 0 на 0 _5 и _6 


void DJ_MOD() {
    string name_1 = "Tests/test_1.txt";
    string name_2 = "Tests/test_2.txt";
    Matrix matrix_1;
    Matrix matrix_2;
    matrix_1 = read_matrix(name_1);
    matrix_2 = read_matrix(name_2);
    cout << "FIRST:\n";
    matrix_1.print();

    cout << "SECOND:\n";
    matrix_2.print();

    Matrix res_mat(matrix_1.rows);
    res_mat = usual_addition(&matrix_1, &matrix_2);

    CompressedMatrix A, B, C;
    A = matrix_packagin(&matrix_1);
    B = matrix_packagin(&matrix_2);
    cout << "FIRST_PACKED:\n";
    A.print();
    cout << "\nSECOND_PACKED:\n";
    B.print();
    cout << "\nUNPUCKED_SECOND\n";
    Matrix mat_2 = matrix_unpackagin(&B);
    mat_2.print();
    C = sum_pack_matrix(&A, &B);
    cout << "\nSUM_PACKED:\n";
    C.print();
    Matrix result;
    result = matrix_unpackagin(&C);
    cout << "\nUNPACKED_MATRIX:\n";
    result.print();
    cout << "\nIDYLE_MATRIX:\n";
    res_mat.print();
    COMPARE(&res_mat, &result);
}

void KRM_MOD() {
    string name_3 = "KnutTests/_1.txt";
    Matrix knut_mat;
    knut_mat = read_matrix(name_3);
    cout << "PACKED_1:";

    KRMCompressedMatrix comp_knut(&knut_mat);
    cout << endl;
    comp_knut.print();

    Matrix unpacked_1 = unpacking_KRM(&comp_knut);
    cout << "\n\nUNPACKED_1:\n";
    unpacked_1.print();


    string name_4 = "KnutTests/_2.txt";
    Matrix knut_mat_2;
    knut_mat_2 = read_matrix(name_4);

    KRMCompressedMatrix comp_knut_2(&knut_mat_2);
    cout << "\nPACKED_2\n";
    comp_knut_2.print();

    Matrix unpacked_2 = unpacking_KRM(&comp_knut_2);
    cout << "\nUNPACKED_2\n";
    unpacked_2.print();
    cout << "\nCHOOSE THE MODE: 1 - ADDITION, 2 - MULTIPLICATION.\n";
    int mode = -1;

    while (mode == -1) {
        cin >> mode;
        if (mode == 1) {
            if ((unpacked_1.columns != unpacked_2.columns) || (unpacked_1.rows != unpacked_2.rows)) {
                cout << "Please, check the matrix. Uncampatable size.";
                exit(0);
            }
            cout << "\nIDYLE_ADDITION_RESULT:\n";
            Matrix mult;
            mult = usual_addition(&knut_mat, &knut_mat_2);
            mult.print();

            cout << "KRM ADDITION:";
            KRMCompressedMatrix add_krm_mat;
            add_krm_mat = KRM_addition(&comp_knut, &comp_knut_2);
            cout << endl;
            add_krm_mat.print();

            Matrix unpacked_add = unpacking_KRM(&add_krm_mat);
            cout << "\nUNPACKED_ADD\n";
            unpacked_add.print();
            break;
        }
        else if (mode == 2) {
            if (unpacked_1.columns != unpacked_2.rows) {
                cout << "Please, check the matrix. Uncampatable size.";
                exit(0);
            }
            cout << "\nIDYLE_MULTIPLICATION_RESULT:\n";
            Matrix mult;
            mult = usual_multiplication(&knut_mat, &knut_mat_2);
            mult.print();

            cout << "KRM MULTIPLICATION:";
            KRMCompressedMatrix mult_krm_mat;
            mult_krm_mat = KRM_multiplication(&comp_knut, &comp_knut_2);
            cout << endl;
            mult_krm_mat.print();

            Matrix unpacked_mult = unpacking_KRM(&mult_krm_mat);
            cout << "\nUNPACKED_MULT\n";
            unpacked_mult.print();
            break;
        }
        else {
            cout << "ENTER VALID NUMBER: ";
            mode = -1;
        }
    }
}

int main() {
    setlocale(LC_ALL, "rus");
    int mode = -1;
    cout << "ENTER THE MODE:\n1-DJENINX\n2-KRM\n";
    while (mode == -1) {
        cin >> mode;
        if (mode == 1) {
            DJ_MOD();
            break;
        }
        else if (mode == 2) {
            KRM_MOD();
            break;
        }
        else {
            cout << "ENTER VALID NUMBER: ";
            mode = -1;
        }
    }
}