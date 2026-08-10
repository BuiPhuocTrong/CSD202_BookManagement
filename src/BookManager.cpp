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

#define LAST true
#define NOT_LAST false

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

//Delete by copying left
void delCopyingLeft(Node*p){
    if (p == nullptr || p->left == nullptr) return;
    //Internal node don't have rightmost leave
    if (p->left->right == nullptr) {
        Node* tmp = p->left;
        p->data = tmp->data;
        tmp->data = nullptr;
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
void BookManager_BST::deleteByCopyingLeft(){
    string id = inputString("Enter book ID: ");

    if (isEmpty()){
        cout << "Library has no books.\n";
        return;
    }
    Node *temp = searchById(id);
    if (!temp){
        cout << "Book not found.\n";
        return;
    }

    delCopyingLeft(temp);
    cout << "Book with ID " << id << " deleted by Copying left" << endl;
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

// ==================== FEATURE 15: BALANCE BST ====================

// Helper function: Traverse tree in In-order and store nodes into a vector
static void storeInOrderNodes(Node* root, vector<Node*>& nodes) {
    if (!root) return;
    storeInOrderNodes(root->left, nodes);
    nodes.push_back(root);
    storeInOrderNodes(root->right, nodes);
}

// Helper function: Rebuild a perfectly balanced BST from sorted nodes vector
static Node* buildBalancedBSTHelper(vector<Node*>& nodes, int start, int end) {
    if (start > end) return nullptr;

    int mid = start + (end - start) / 2;
    Node* p = nodes[mid];

    // Safely disconnect and reassign left and right child pointers
    p->left = buildBalancedBSTHelper(nodes, start, mid - 1);
    p->right = buildBalancedBSTHelper(nodes, mid + 1, end);

    return p;
}

// Main function to execute Feature 15
void BookManager_BST::balanceBST() {
    // 1. Guard clause: Check if tree is empty
    if (isEmpty()) {
        cout << "Library is empty. Nothing to balance!\n";
        return;
    }

    // Get tree height prior to balancing for comparison
    int oldHeight = getHeight(root);

    // 2. Retrieve all nodes in ascending order of ID (In-order traversal)
    vector<Node*> nodes;
    storeInOrderNodes(root, nodes);

    // 3. Reconstruct tree into a balanced BST
    root = buildBalancedBSTHelper(nodes, 0, static_cast<int>(nodes.size()) - 1);

    // Get updated tree height after balancing
    int newHeight = getHeight(root);

    // 4. Save updated tree structure to persistent storage
    saveToFile();

    // 5. Output operation results
    cout << "==> Tree balanced successfully!\n";
    cout << " - Old Tree Height: " << oldHeight << "\n";
    cout << " - New Tree Height: " << newHeight << "\n";
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

// Tree visualization
void BookManager_BST::printTree()
{
    if (root == nullptr){
        cout << "Library is empty.\n";
        return;
    }

    cout << "\n================ BOOK TREE ================\n\n";
    
    // Print root
    cout << root->data->getId() << endl;

    //Case 1: root have both L/R
    if (root->left != nullptr && root->right != nullptr){
        // Left is NOT the last branch
        printTreeHelper(root->left, "", NOT_LAST, 'L');
        // Right IS the last branch
        printTreeHelper(root->right, "", LAST, 'R');
    }
    //Case 2: Root only have left
    else if (root->left != nullptr){
        printTreeHelper(root->left, "", LAST, 'L');
    }

    //Case 3: Root only have right
    else if (root->right != nullptr){
        printTreeHelper(root->right, "", LAST, 'R');
    }

    cout << "\n============================================\n";
}

void BookManager_BST::printTreeHelper(Node* p, string prefix, bool isLast, char branch){
    if (p == nullptr) return;

    //Print current node
    cout << prefix;

    if (isLast)
        cout << "└── ";
    else
        cout << "├── ";

    cout << branch << ": " << p->data->getId() << endl;

    // Leaf node, don't have both L/R
    if (p->left == nullptr && p->right == nullptr) return;

    // Case 1: Current node has both L/R
    if (p->left != nullptr && p->right != nullptr){
        string newPrefix;
        newPrefix = (isLast) ? prefix + "    " : prefix + "│   ";
        // Left not last
        printTreeHelper(p->left, newPrefix, NOT_LAST, 'L');
        //Right is last
        printTreeHelper(p->right, newPrefix, LAST, 'R');
    }
    // Case 2: Cur node only have Left
    else if (p->left != nullptr){
        string newPrefix;
        newPrefix = (isLast) ? prefix + "    " : prefix + "│   ";
        printTreeHelper(p->left, newPrefix, LAST, 'L');
    }
    // Case 3: Cur node only have Right
    else if (p->right != nullptr){
        string newPrefix;
        newPrefix = (isLast) ? prefix + "    " : prefix + "│   ";
        printTreeHelper(p->right, newPrefix, LAST, 'R');
    }
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

void BookManager_BST::rotateRight (){
    cin.ignore(10000, '\n');
    
    string id = inputString("Enter book ID to rotate right: ");
    Node* p = searchById(id);

    if (p == nullptr) {
        cout << "Book ID not found" << endl;
        return;
    } 
    else if (p->left == nullptr) {
        cout << "Node " << id << " has no left child to rotate right" << endl;
        return;
    } 
    else {
        rightRotate(p);
        cout << "Rotation successful at node " << id << endl;
        return;
    }

}

void BookManager_BST::rotateLeft(){
    cin.ignore(10000, '\n');

    string id = inputString("Enter book ID to rotate right: ");
    Node* p = searchById(id);

    if (p == nullptr) {
        cout << "Book ID not found" << endl;
        return;
    } 
    else if (p->right == nullptr) {
        cout << "Node " << id << " has no right child to rotate left" << endl;
        return;
    } 
    else {
        leftRotate(p);
        cout << "Rotation successful at node " << id << endl;
        return;
    }

} 