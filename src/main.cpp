#include <iostream>
#include <fstream>
#include "../header/BookManager.h"
#include "../header/Subjects.h"

using namespace std;

#define BOOK_FILE "data/books.txt"

int main (){
    cout << "hello111";
    ifstream fin(BOOK_FILE);

    if (!fin.is_open()) {
        cout << "Cannot open file!\n";
        return 1;
    }

    BookManager_BST manager;
    manager.root = manager.loadFromFile(fin);

    
    cout << "===== In-order =====" << endl;
    manager.inOrder(manager.root);

    fin.close();

    return 0;
}