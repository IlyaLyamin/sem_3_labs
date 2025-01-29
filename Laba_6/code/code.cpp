#include <cstring>  // Для функции strcmp
#include <iostream>
#include <queue>
#include <iomanip>
using namespace std;


struct Node {
    Node* L = nullptr;
    Node* R = nullptr;
    char* content = nullptr;
    int height;

    Node(char* el) {
        Node* L = new Node;
        Node* R = new Node;
        content = el;
        height = 1;
    }

    Node() {
        content = nullptr;
        height = 1;
    }

    Node& operator=(const Node& rhs) {
        L = rhs.L;
        R = rhs.R;
        content = rhs.content;
        height = rhs.height;
        return *this;
    }
};

int compare(char* str1, char* str2) {
    if (str1 == nullptr || str2 == nullptr) {
        if (str1 == nullptr && str2 == nullptr) return 0;
        return (str1 == nullptr) ? -1 : 1;
    }
    return strcmp(str1, str2);// -1 --- <; 0 -- ==;  1 --- >;
}

int get_height(Node* p) {
    return p ? p->height : 0;
}

int check_balance(Node* p) {// -2 - left; 0 - norm; 2 - right;
    return (get_height(p->L) - get_height(p->R)) == 0 ? 0 : (get_height(p->L) - get_height(p->R));
}

void fix_height(Node* p) {
    int a = get_height(p->L);
    int b = get_height(p->R);
    p->height = (a > b ? a : b) + 1;
}

Node* rot_right(Node* p) {
    Node* q = p->L;
    p->L = q->R;
    q->R = p;
    fix_height(p);
    fix_height(q);
    return q;
}

Node* rot_left(Node* root) {
    Node* p = root->R;
    root->R = p->L;
    p->L = root;
    fix_height(root);
    fix_height(p);
    return p;
}

Node* balance(Node* root) {
    if ((root->L) or (root->R)) fix_height(root);
    if (check_balance(root) == -2) {
        if (check_balance(root->R) > 0) root->R = rot_right(root->R);
        return rot_left(root);
    }
    if (check_balance(root) == 2) {
        if (check_balance(root->L) < 0)
            root->L = rot_left(root->L);
        return rot_right(root);
    }
    return root;
}

Node* ADD_ELEMENT(Node* root, char* content) {
    if (root == nullptr) return new Node(content);
    if (compare(content, root->content) == -1) root->L = ADD_ELEMENT(root->L, content);
    else if (compare(content, root->content) == 1) root->R = ADD_ELEMENT(root->R, content);
    else cout << "ERROR! This element have already exist.\n";
    return balance(root);
}

void printTree(Node* p) {
    cout << endl;
    if (p == nullptr) {
        cout << "Tree is empty..." << endl;
        return;
    }
    queue<Node*> q;
    q.push(p);
    int k = 128;
    int levelNodes = 1;
    Node* current = nullptr;
    while (!q.empty()) {
        k = k / 2;
        levelNodes = q.size();
        for (int i = 0; i < levelNodes; ++i) {
            current = q.front();
            q.pop();
            if (current != nullptr) {
                cout << setw(k) << current->content;

                q.push(current->L);
                q.push(current->R);
            }
            else {
                cout << setw(k) << "-";
            }
        }
        cout << endl;
    }
}

bool search_element(char* elem, Node* root) {
    cout << "Searching:\n";
    Node* cur = root;
    while (cur != nullptr) {
        if (compare(elem, cur->content) == -1) {
            cur = cur->L;
            cout << "left ";
        }
        else if (compare(elem, cur->content) == 1) {
            cur = cur->R;
            cout << "right ";
        }
        else if (compare(elem, cur->content) == 0){
            cout << "\nSearched: ";
            char loc = cur->content[0];
            int ind = 0;
            while (loc != '\0') {
                cout << loc;
                ind++;
                loc = cur->content[ind];
            }
            cout << "\n";
            return true;
        }
    }
    cout << "Element not found!!!!!\n";
    return false;
}
    
void MENU(Node* root) {
    cout << "Menu: " << endl;
    cout << "1-Exit" << endl;
    cout << "2-Insert element" << endl;
    cout << "3-Search element" << endl;
    cout << "4-Print tree" << endl;
    string el;
    int mode = 0;
    while (mode == 0) {
        cout << "Enter the number: \n";
        cin >> mode;
    }
    char* element = nullptr;
    if (mode == 1) exit(1);
    else if (mode == 2) {
        cout << "Enter the element:\n";
        cin >> el;
        char* element = new char[el.size()];
        for (int i = 0; i < el.size(); i++) {
            element[i] = el[i];
        }
        element[el.size()] = '\0';// Для отладки
        root = ADD_ELEMENT(root, element);
        printTree(root);
        MENU(root);
    }
    else if (mode == 3) {
        cout << "Enter the element for searching:\n";
        cin >> el;
        char* element = new char[el.size()];
        for (int i = 0; i < el.size(); i++) {
            element[i] = el[i];
        }
        element[el.size()] = '\0';// Для отладки
        search_element(element, root);
        MENU(root);
    }
    else if (mode == 4) {
        printTree(root);
        MENU(root);
    }
}

int main()
{
    setlocale(LC_ALL, "rus");
    Node* root = nullptr;
    MENU(root);
}

