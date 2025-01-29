#include <iostream>
#include <clocale>
#include <string>
using namespace std;

const int SIZE = 1000;


template<typename tp>
struct Stack {
    int capacity = 0;
    int col_elem = 0;
    tp* array[SIZE];
    int end = 0;
    int start = 0;

    Stack(int capacity) {
        this->capacity = capacity;
        for (int i = 0; i < capacity; i++) {
            this->array[i] = NULL;
        }
    }
};

template<typename tp>
bool IsEmpty(Stack<tp>* ds) {
    if (ds->col_elem == 0) return true;
    return false;
}

template<typename tp>
bool Pop(Stack<tp>* stack, tp* contain) {
    if (IsEmpty(stack)) return false;
    *contain = *(stack->array[stack->end]);
    stack->array[stack->end] = NULL;
    stack->end -= 1;
    //cout << "Number " << (*contain) << " extracted from stack" << endl;
    stack->col_elem -= 1;
    return true;
}

template<typename DS>
bool IsFull(DS* ds) {
    if (ds->capacity == ds->col_elem) return true;
    return false;
}

template<typename tp>
bool Push(Stack<tp>* st, tp elem) {
    if (IsFull(st)) {
        cout << "Array is full" << endl;
        return 0;
    }
    int iter;
    for (int i = 0; i < st->capacity; i++) {
        if (st->array[i] == NULL) {
            if (IsEmpty(st)) {
                //cout << "Stack was empty" << endl;
                st->col_elem += 1;
                st->end = i;
                st->array[i] = new tp;
                *(st->array[i]) = elem;
                return 1;
            }
            //cout << "Stack was NOT empty" << endl;
            st->col_elem += 1;
            st->end = i;
            st->array[i] = new tp;
            *(st->array[i]) = elem;
            return 1;
        }
    }
    return 0;
}

int Peek(Stack<int>* st) {
    return *(st->array[st->end]);
}

string Peek(Stack<string>* st) {
    return *(st->array[st->end]);
}

char Peek(Stack<char>* st) {
    return *(st->array[st->end]);
}

template<typename tp>
void print(Stack<tp>* st) {
    tp el;
    for (int i = 0; i < (st->end + 1); i++) {
        el = *(st->array[i]);
        cout << "[" << i + 1 << "]" << " --- " << el << endl;
    }
    cout << st->col_elem << " --- elemnts" << endl;
}