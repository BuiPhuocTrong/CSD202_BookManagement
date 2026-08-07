#include "../header/BookManager.h"
#include "../header/Subjects.h"
#include "../header/ConsoleInput.h"
#include "../header/Validation.h"


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

void displayBook(Node* p) {
    if (p == nullptr) return;
    cout << p->data->getId() << " | "
         << p->data->getTitle() << " | "
         << p->data->getAuthor() << " | "
         << p->data->getYear() << endl;
}

//Insert, recursive
void BookManager_BST::insertBook() {
    Book b = inputBook();
    Book *book = &b;
    root = insert(book, root);
}
Node* BookManager_BST::insert(Book* b, Node* p) {
    if (p == nullptr) return new Node(b);
    
    if (b->getId() < p->data->getId()) 
        p->left = insert(b, p->left);
    else if (b->getId() > p->data->getId())
        p->right = insert(b, p->right);

    return p;
}

//Delete by Merging Left (recursion)
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

//Search by ID
Node* BookManager_BST::searchById(string key){
    Node* cur = root;
    while (cur){
        if (cur->data->getId() == key) return cur;
        cur = (cur->data->getId() < key) ? cur->right : cur->left;
    }
    return nullptr;
}

// Check 'sub' is in 'str' or not (ignore case)
bool isSubstringIgnoreCase(string str, string sub) {
    if (sub.empty()) return true;

    //lowercase
    for (char &c : str) c = tolower(c);
    for (char &c : sub) c = tolower(c);

    // Check sub string
    //find() -> return first index of sub in str, if not found return string::npos
    //string::npos -> not found (const value)
    return str.find(sub) != string::npos;
}

// Search by title (Traverse entire tree)
void BookManager_BST::searchByTitle(Node* root, const string& title){
    if (root == nullptr) return;

    searchByTitle(root->left, title);

    if (isSubstringIgnoreCase(root->data->getTitle(), title)){
        displayBook(root);
    }

    searchByTitle(root->right, title);
}

//Update Book by ID
void BookManager_BST::updateBook(){
    if (isEmpty()) {
        cout << "The book list is empty." << endl;
        return;
    }
    
    string id = inputString("Enter book ID to update: ");

    Node* p = searchById(id);
    if (p == nullptr) {
        cout << "Book not found." << endl;
        return;
    }

    cout << "Current book details:" << endl
         << "ID: " << p->data->getId() << endl
         << "Title: " << p->data->getTitle() << endl
         << "Author: " << p->data->getAuthor() << endl
         << "Year: " << p->data->getYear() << endl;


    int choice;
    cout << "What would you like to update?" << endl
         << "1. Title" << endl
         << "2. Author" << endl
         << "3. Year" << endl
         << "4. All" << endl;     
    inputIntegerInRange(choice, 1, 4, "Enter your choice (1-4): ");
    
    switch (choice) {
        case 1: {
            string newTitle = inputString("Enter new title: ");
            p->data->setTitle(newTitle);
            break;
        }
        case 2: {
            string newAuthor = inputString("Enter new author: ");
            p->data->setAuthor(newAuthor);
            break;
        }
        case 3: {
            int newYear;
            inputIntegerInRange(newYear, 0, 9999, "Enter new year: ");
            p->data->setYear(newYear);
            break;
        }
        case 4: {
            string newTitle = inputString("Enter new title: ");
            string newAuthor = inputString("Enter new author: ");
            int newYear;
            inputIntegerInRange(newYear, 0, 9999, "Enter new year: ");

            p->data->setTitle(newTitle);
            p->data->setAuthor(newAuthor);
            p->data->setYear(newYear);
            break;
        }
    }
    cout << "Book updated successfully." << endl;
}

//display In Order traversal
//left -> root -> right
void BookManager_BST::displayInOrder(Node* root){
    if (root == nullptr)
        return;

    displayInOrder(root->left);

    displayBook(root);

    displayInOrder(root->right);
}

//display Pre Order traversal
//root -> left -> right
void BookManager_BST::displayPreOrder(Node* root){
    if (root == nullptr) return;

    displayBook(root);

    displayPreOrder(root->left);
    displayPreOrder(root->right);
}

//display Post Order traversal
//left -> right -> root
void BookManager_BST::displayPostOrder(Node* root){
    if (root == nullptr) return;

    displayPostOrder(root->left);
    displayPostOrder(root->right);

    displayBook(root);
}

//display breadth first traversal
void BookManager_BST::displayBreadthFirst(Node *root) {
    if (!root) return;

    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* cur = q.front();
        q.pop();

        displayBook(cur);

        if (cur->left) q.push(cur->left);
        if (cur->right) q.push(cur->right);
    }
}
