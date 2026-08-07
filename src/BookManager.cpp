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

void BookManager_BST::inOrder(Node* root)
{
    if (root == nullptr)
        return;

    inOrder(root->left);

    cout << root->data->getId() << " | "
         << root->data->getTitle() << " | "
         << root->data->getAuthor() << " | "
         << root->data->getYear() << endl;

    inOrder(root->right);
}

//Insert, recursive
void BookManager_BST::insert(Book* b) {
    root = insert(b, root);
}
Node* BookManager_BST::insert(Book* b, Node* p) {
    if (p == nullptr) return new Node(b);
    
    if (b->getId() < p->data->getId()) 
        p->left = insert(b, p->left);
    else if (b->getId() > p->data->getId())
        p->right = insert(b, p->right);

    return p;
}

//Delete by Merging Left
Node* BookManager_BST::deleteByMerging(Node* root, const string& id)
{
    if (root == nullptr) return nullptr;

    //Finding
    if (id < root->data->getId()){
        root->left = deleteByMerging(root->left, id);
    }
    else if (id > root->data->getId()){
        root->right = deleteByMerging(root->right, id);
    }
    else{
        //Found
        Node* tmp = root;

        // left subtree is empty
        if (root->left == nullptr) root = root->right;

        // right subtree is empty
        else if (root->right == nullptr) root = root->left;

        // have both subtrees
        else{
            Node* leftSubTree = root->left;
            Node* rightMost = leftSubTree;

            // find rightmost node of left subtree
            while (rightMost->right != nullptr){
                rightMost = rightMost->right;
            }

            // merge right subtree
            rightMost->right = root->right;

            // update new root
            root = leftSubTree;
        }

        delete tmp;
    }

    return root;
}

void BookManager_BST::deleteByCopyingLeft(Node*p){
    if (p == nullptr || p->left == nullptr) return;
    //Internal node don't have rightmost leave
    if (p->left->right == nullptr) {
        Node* tmp = p->left;
        p->data = tmp->data;
        p->left = tmp->left;
        delete tmp;
    } else {
        //have rightmost leave
        Node* father = p->left;
        Node* cur = father->right;
        while (cur->right) {
            father = cur;
            cur = cur->right;
        }
        p->data = cur->data;
        cur->data = nullptr; // Bcs cur and p point to same Book address, need to set cur point to nullptr to avoid double delete
        father->right = cur->left;
        delete cur;
    }
}

//Search
Node* BookManager_BST::Search(string key){
    Node* cur = root;
    while (cur){
        if (cur->data->getId() == key) return cur;
        cur = (cur->data->getId() < key) ? cur->right : cur->left;
    }
    return nullptr;
}