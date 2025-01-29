#include <iostream>
#include <clocale>
using namespace std;

const int SIZE = 1000;

struct Turn{
    int capacity = 0;
    int col_elem = 0;
    int array[SIZE];
    int start = 0;
    int end = 0;

    Turn(int capacity) {
        this->capacity = capacity;
        for (int i = 0; i < capacity; i++) {
            this->array[i] = -1;
        }
    }
};


struct Stack {
    int capacity = 0;
    int col_elem = 0;
    int array[SIZE];
    int end = 0;
    int start = 0;

    Stack(int capacity) {
        this->capacity = capacity;
        for (int i = 0; i < capacity; i++) {
            this->array[i] = -1;
        }
    }
};

template<typename DS>
bool IsEmpty(DS* turn) {
    if (turn->col_elem == 0) return true;
    return false;
}

bool Pop(Stack* stack, int* contain) {
    if (IsEmpty(stack)) return false;
    *contain = stack->array[stack->end];
    stack->array[stack->end] = -1;
    stack->end -= 1;
    cout << "Number " << (*contain) << " extracted from stack" << endl;
    stack->col_elem -= 1;
    return true;
}

template<typename DS>
bool IsFull(DS* turn) {
    if (turn->capacity == turn->col_elem) return true;
    return false;
}


bool Push(Turn* turn, int* elem) {
    if (IsFull(turn)) {
        cout << "Array is full" << endl;
        return 0;
    }
    int iter;
    for (int i = turn->start; i <= (turn->start + turn->capacity - 1); i++) {
        iter  = i % turn->capacity;
        if (turn->array[iter] == -1) {
            if (IsEmpty(turn)) {
                cout << "Stack was empty" << endl;
                turn->col_elem += 1;
                turn->start = iter;
                turn->end = iter;
                turn->array[iter] = (*elem);
                return 1;
            }
            cout << "Stack was NOT empty" << endl;
            turn->col_elem += 1;
            turn->end = iter;
            turn->array[iter] = (*elem);
            return 1;
        }
    }
    return 0;
}

bool Push_stack(Stack* turn, int* elem) {
    if (IsFull(turn)) {
        cout << "Array is full" << endl;
        return 0;
    }
    int iter;
    for (int i = 0; i < turn->capacity; i++) {
        if (turn->array[i] == -1) {
            if (IsEmpty(turn)) {
                cout << "Stack was empty" << endl;
                turn->col_elem += 1;
                turn->end = i;
                turn->array[i] = (*elem);
                return 1;
            }
            cout << "Stack was NOT empty" << endl;
            turn->col_elem += 1;
            turn->end = i;
            turn->array[i] = (*elem);
            return 1;
        }
    }
    return 0;
}

bool Delete(Turn* turn, int* contain){
    if (IsEmpty(turn)) return false;
    if (turn->start == turn->end) {
        *contain = turn->array[turn->start];
        turn->array[turn->start] = -1;
        turn->start = 0;
        turn->end = 0;
        cout << "Number " << (*contain) << " extracted from stack, stack is now empty" << endl;
        turn->col_elem -= 1;
        return true;
    }
    *contain = turn->array[turn->start];
    turn->array[turn->start] = -1;
    turn->start = ((turn->start + 1) % turn->capacity);
    cout << "Number " << (*contain) << " extracted from stack" << endl;
    turn->col_elem -= 1;
    return true;
}

template<typename DS_2>
void print_basic_array(DS_2* turn) {
    for (int i = 0; i < turn->capacity; i++) {
        if ((i == turn->start) && (i == turn->end)) {
            cout << turn->array[i] << " --- BOTH" << endl;
            continue;
        }
        try
        {
            if (i == turn->start) {
                cout << turn->array[i] << " --- START" << endl;
                continue;
            }
        }
        catch (const std::exception&)
        {
            cout << "";
        }
        if (i == turn->end) {
            cout << turn->array[i] << " --- END" << endl;
            continue;
        }
        cout << turn->array[i] << endl;
    }
    cout << turn->col_elem << " --- elemnts" << endl;
}

template<typename DS_2>
void print(DS_2* turn) {
    int colis = 0;
    for (int i = 0; i < (turn->capacity); i++) {
        int ind = (i + turn->start) % turn->capacity;
        if (turn->array[ind] == -1) {
            colis += 1;
            continue;
        }
        cout << "[" << i - colis << "]" << " --- " << turn->array[ind] << endl;   
    }
    cout << turn->col_elem << " --- elemnts" << endl;
}

void read(int* number) {
    cout << "Введите число: ";

    while (true) {
        cin >> (*number);
        // Проверяем, если ввод не является числом
        if (cin.fail()) {
            cin.clear(); // Сбросить флаг ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистить буфер ввода
            cout << "Ошибка! Пожалуйста, введите корректное число: ";
        }
        else {
            break; // Если число введено корректно
        }
    }
}


void MODE_1() {
    cout << "You can use functions: Push, Delete, IsFull, IsEmpty, Leek" << endl;
    cout << "Put the len\n";
    int len;
    read(&len);
    while (len < 1) {
        int len;
        read(&len);
    }

    Turn st(len);
    int elem;
    cout << "Put the numbers\n";
    for (int i = 0; i < len; i++) {
        read(&elem);
        Push(&st, &elem);
        print(&st);
        print_basic_array(&st);
    }
    while (true)
    {
        string command;
        cin >> command;
        if (command == "Delete") {
            cout << "You will see the first added element, and it will extracted from the stack" << endl;
            int elem;
            if (!(Delete(&st, &elem))) cout << "Can't delete selected element" << endl;
            else {
                cout << "----------- " << elem << " ----------\n";
                print(&st);
                print_basic_array(&st);
            }
        }
        if (command == "Push") {
            cout << "Input the required element" << endl;
            int elem;
            read(&elem);
            if (!(Push(&st, &elem))) cout << "Can't add entered element" << endl;
            else {
                print(&st);
                print_basic_array(&st);
            }
        }
        if (command == "Leek") {
            print(&st);
            print_basic_array(&st);
        }
        if (command == "IsEmpty") {
            if (!(IsEmpty(&st))) cout << "No, it is't empty" << endl;
            else cout << "Yes, it is empty\n";
        }
        if (command == "IsFull") {
            if (!(IsFull(&st))) cout << "No, it is't full" << endl;///////////////////////////
            else {
                cout << "Yes, it is full\n";
            };
        }
    }
}

void MODE_2() {
    cout << "You can use functions: Push, Pop, IsFull, IsEmpty, Leek" << endl;
    cout << "Put the len\n";
    int len;
    read(&len);
    Stack st(len);
    int elem;
    cout << "Put the numbers\n";
    for (int i = 0; i < len; i++) {
        read(&elem);
        Push_stack(&st, &elem);
        print(&st);
        print_basic_array(&st);
    }
    while (true)
    {
        string command;
        cin >> command;
        if (command == "Pop") {
            cout << "You will see the last added element, and it will extracted from the stack" << endl;
            int elem;
            if (!(Pop(&st, &elem))) cout << "Can't delete selected element" << endl;
            else {
                cout << "----------- " << elem << " ----------\n";
                print(&st);
                print_basic_array(&st);
            }
        }
        if (command == "Push") {
            cout << "Input the required element" << endl;
            int elem;
            read(&elem);
            if (!(Push_stack(&st, &elem))) cout << "Can't add entered element" << endl;
            else {
                print(&st);
                print_basic_array(&st);
            }
        }
        if (command == "Leek") {
            print(&st);
            print_basic_array(&st);
        }
        if (command == "IsEmpty") {
            if (!(IsEmpty(&st))) cout << "No, it is't empty" << endl;
            else cout << "Yes, it is empty\n";
        }
        if (command == "IsFull") {
            if (!(IsFull(&st))) cout << "No, it is't full" << endl;///////////////////////////
            else {
                cout << "Yes, it is full\n";
            };
        }
    }
}


int main()
{
    setlocale(LC_ALL, "rus");
    cout << "1 - Turn\n2 - stack\n";
    int mode;
    while (true) {
        read(&mode);
        if (mode == 1) {
            MODE_1();
            break;
        }
        else if (mode == 2) {
            MODE_2();
        }
        else cout << "Write the correct number" << endl;
    }
}