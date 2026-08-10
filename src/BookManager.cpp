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
#include <algorithm>


#define BOOK_FILE "data/books.txt"

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

void BookManager_BST::saved(Node* root, ofstream& fout){
    if (root == nullptr){
        fout << "#\n";
        return;
    }

    fout << root->data->getId() << "|"
         << root->data->getTitle() << "|"
         << root->data->getAuthor() << "|"
         << root->data->getYear() << '\n';

    saved(root->left, fout);
    saved(root->right, fout);
}

void  BookManager_BST:: saveToFile (){
    // Save BST to file
    ofstream fout(BOOK_FILE);

    if (!fout) {
        cout << "Cannot open file.\n";
        return;
    }

    saved(root, fout);
    fout.close();
}

void BookManager_BST::displayBook(Node* p) {
    if (p == nullptr) return;
    cout << p->data->getId() << " | "
         << p->data->getTitle() << " | "
         << p->data->getAuthor() << " | "
         << p->data->getYear() << endl;
}

// Search by ID
Node* BookManager_BST::searchById(string key) {
    Node* p = root;

    while (p != nullptr) {
        if (key == p->data->getId())
            return p;
        else if (key < p->data->getId())
            p = p->left;
        else
            p = p->right;
    }

    return nullptr;
}

//Insert, recursive
void BookManager_BST::insertBook() {
    Book b = inputBook();
    if (searchById(b.getId()) != nullptr) {
        cout << "Book ID already exists.\n";
        return;
    }
    Book *book = new Book(b);
    root = insert(book, root);
    
    saveToFile();
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
Node* delMerging(Node* root, const string& id){
    if (root == nullptr) return nullptr;

    //Finding
    if (id < root->data->getId()){
        root->left = delMerging(root->left, id);
    }
    else if (id > root->data->getId()){
        root->right = delMerging(root->right, id);
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
void BookManager_BST::deleteByMerging(){
    string id = inputString("Enter book ID: ");

    if (isEmpty()){
        cout << "Library has no books.\n";
        return;
    }
    if (!searchById(id)){
        cout << "Book not found.\n";
        return;
    }

    root = delMerging(root, id);

    saveToFile();

    cout << "Book with ID " << id << " deleted by Merging" << endl;
}
// Delete by Copying////////////////////////////////////////////////////////
void BookManager_BST::delCopying(Node* p, bool copyLeft)
{
    if (p == nullptr)
        return;

    Node* father;
    Node* cur;

    //cpying left 
    if (copyLeft)
    {
        // Find the rightmost node
        // in the left subtree
        father = p;
        cur = p->left;

        while (cur->right != nullptr)
        {
            father = cur;
            cur = cur->right;
        }

        // Transfer Book ownership
        Book* oldData = p->data;

        p->data = cur->data;
        cur->data = oldData;

        // Remove cur from the tree
        if (father == p)
        {
            p->left = cur->left;
        }
        else
        {
            father->right = cur->left;
        }

        // cur now owns oldData.
        // Node destructor will delete oldData.
        delete cur;

        return;
    }

    // coyping right
    // Find the leftmost node
    // in the right subtree
    father = p;
    cur = p->right;

    while (cur->left != nullptr)
    {
        father = cur;
        cur = cur->left;
    }

    // Transfer Book ownership
    Book* oldData = p->data;

    p->data = cur->data;
    cur->data = oldData;

    // Remove cur from the tree
    if (father == p)
    {
        p->right = cur->right;
    }
    else
    {
        father->left = cur->right;
    }

    // cur now owns oldData.
    // Node destructor will delete oldData.
    delete cur;
}

// Delete Book by Copying/////////////////////////////////////////////////////////
void BookManager_BST::deleteByCopying()
{
    // Check empty tree
    if (isEmpty())
    {
        cout << "Library has no books.\n";
        return;
    }
    // Input Book ID
    string id = inputString("Enter book ID: ");
    // Search for the node
    Node* p = searchById(id);
    if (p == nullptr)
    {
        cout << "Book not found.\n";
        return;
    }
    // Copying is used for a node
    // with two children
    if (p->left == nullptr || p->right == nullptr)
    {
        cout << "Cannot delete this book by copying.\n";
        cout << "The selected node must have two children.\n";
        return;
    }
    // Choose copying direction
    int choice;
    cout << "\n===== DELETE BY COPYING =====\n";
    cout << "1. Copying Left\n";
    cout << "2. Copying Right\n";
    inputIntegerInRange(
        choice,
        1,
        2,
        "Enter your choice: "
    );
    bool copyLeft = (choice == 1);
    // Perform deletion
    delCopying(p, copyLeft);
    cout << "\nBook with ID " << id
         << " deleted by ";

    if (copyLeft)
    {
        cout << "Copying Left.\n";
    }
    else
    {
        cout << "Copying Right.\n";
    }

    // Save updated tree
    saveToFile();
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
    saveToFile();
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

//Sort for balancing tree
void storeInOrder(Node* root, vector<Node*>& nodes) {
    if (!root) return;

    storeInOrder(root->left, nodes);
    nodes.push_back(root);
    storeInOrder(root->right, nodes);
}

Node* buildBalancedBST(vector<Node*>& nodes, int left, int right) {
    if (left > right)
        return nullptr;

    int mid = (left + right) / 2;

    Node* root = nodes[mid];

    root->left = buildBalancedBST(nodes, left, mid - 1);
    root->right = buildBalancedBST(nodes, mid + 1, right);

    return root;
}

Node* balancingBST(Node* root) {
    vector<Node*> nodes;

    storeInOrder(root, nodes);

    return buildBalancedBST(nodes, 0, nodes.size() - 1);
}

void BookManager_BST::balanceBST() {
    root = balancingBST(root);
}
// Height calculation
int BookManager_BST::getHeight(Node* p) {
    if (p == nullptr) {
        return 0; // Base case: height of an empty tree is 0
    }
    
    int leftHeight = getHeight(p->left);
    int rightHeight = getHeight(p->right);

    return 1 + std::max(leftHeight, rightHeight);
}
// left rotation
void BookManager_BST::leftRotate(Node* p) {
    if (p== nullptr || p->right == nullptr) return;
    Node* q = p->right;
    swap(p->data, q->data);
    p->right = q->right;
    q->right = q->left;
    q->left = p->left;
    p->left = q;
}
// right rotation
void BookManager_BST::rightRotate(Node* p) {
    if (p== nullptr || p->left == nullptr) return;
    Node* q = p->left;
    swap(p->data, q->data);
   p->left = q->left;
    q->left = q->right;
    q->right = p->right;
    p->right = q;
}
//left right rotation
void BookManager_BST::leftRightRotate(Node* p) {
    if (p== nullptr || p->left == nullptr) return;
    leftRotate(p->left);
    rightRotate(p);
}
//right left rotation
void BookManager_BST::rightLeftRotate(Node* p) {
    if (p== nullptr || p->right == nullptr) return;
    rightRotate(p->right);
    leftRotate(p);
}