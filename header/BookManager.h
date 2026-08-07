#ifndef BOOKMANAGER_H
#define BOOKMANAGER_H

#include "Subjects.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

class BookManager_BST{
public:
    Node* root;

    // Constructor & Destructor
    BookManager_BST();
    ~BookManager_BST();

    //File Management
    Node* loadFromFile(ifstream& fin);
    void saveToFile(Node* root, ofstream& fout);

    // Helper method
    void clear(Node* p);

    //Check empty
    bool isEmpty();

    // Insert
    void insert(Book* b);
    Node* insert(Book* b, Node* p); // recursion

    // Traversal & Display
    void visit(Node* p);
    void preOrder(Node* p);
    void postOrder(Node* p);
    void inOrder(Node* p);
    void BreadthFirst();

    // Search
    Node* searchById(string key);
    void searchByTitle(Node* p, const string& title);

    // Deletion
    void deleteByCopyingLeft(Node*p);
    Node* deleteByMerging(Node* root, const string& id);

    //Update book by ID
    void updateBook();

    // Rotations
    void leftRotate(Node* p);
    void rightRotate(Node* p);
    void leftRightRotate(Node* p);
    void rightLeftRotate(Node* p);

    // Statistics
    int countTotalBooks(Node* p);
    void countBooksByYear(Node* p, map<int, int>& yearMap);
};

#endif