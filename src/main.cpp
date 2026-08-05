#include <iostream>
#include <fstream>
#include "../header/BookManager.h"
#include "../header/Subjects.h"

using namespace std;

#define BOOK_FILE "data/books.txt"

int main (){
    cout << "hello111";
    ifstream fin(BOOK_FILE);

    BookManager_BST manager;
    manager.root = manager.loadFromFile(fin);
}