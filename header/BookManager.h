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
    void saved(Node* root, ofstream& fout);
    void saveToFile();

    // Helper method
    void clear(Node* p);

    //Check empty
    bool isEmpty();

    // Insert
    void insertBook();
    Node* insert(Book* b, Node* p); // recursion

    // Traversal & Display
    void displayBook(Node* p);
    void displayPreOrder(Node* root);
    void displayPostOrder(Node* root);
    void displayInOrder(Node* root);
    void displayBreadthFirst(Node* root);

    // Search
    Node* searchById(string key);
    void searchByTitle(Node* p, const string& title);

    // Deletion
    void deleteByCopyingLeft();
    void deleteByMerging();

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

    //Balacing tree
    void balanceBST();

     // Height calculation
    int getHeight(Node* p);
    void printTree();
    void printTreeHelper(
    Node* p,
    string prefix,
    bool isLast,
    char branch
);
};

#endif