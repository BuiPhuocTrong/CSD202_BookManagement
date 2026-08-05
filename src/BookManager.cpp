#include "../header/BookManager.h"
#include "../header/Subjects.h"


#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

BookManager_BST::BookManager_BST() {
    root = nullptr;
}
BookManager_BST::~BookManager_BST() {
    clear(root);
}

void BookManager_BST::clear(Node* p) {
    if (p != nullptr) {
        clear(p->left);
        clear(p->right);
        delete p;
    }
}

//Check empty
bool BookManager_BST::isEmpty() {
    return root == nullptr;
}

Node* BookManager_BST::loadFromFile(ifstream& fin){
    string line;

    if (!getline(fin, line))
        return nullptr;

    if (line == "#")
        return nullptr;

    stringstream ss(line);

    string id, title, author, year;

    getline(ss, id, '|');
    getline(ss, title, '|');
    getline(ss, author, '|');
    getline(ss, year);

    int yearInt = stoi(year);

    Book* b = new Book(id, title, author, yearInt);

    Node* p = new Node(b);

    p->left = loadFromFile(fin);
    p->right = loadFromFile(fin);

    return p;
}

void BookManager_BST:: saveToFile(Node* root, ofstream& fout)
{
    if (root == nullptr)
    {
        fout << "#\n";
        return;
    }

    fout << root->data->getId() << "|"
         << root->data->getTitle() << "|"
         << root->data->getAuthor() << "|"
         << root->data->getYear() << '\n';

    saveToFile(root->left, fout);
    saveToFile(root->right, fout);
}