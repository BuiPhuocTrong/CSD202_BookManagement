#include <iostream>
#include <fstream>
#include <windows.h>

#include "../header/BookManager.h"
#include "../header/Subjects.h"
#include "../header/ConsoleInput.h"

using namespace std;

#define BOOK_FILE "data/books.txt"

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

BookManager_BST library;
bool isRunning = true;


void showMenu() {
    clearScreen();
    cout << "============ LIBRARY MANAGEMENT ============\n"
         << "1. Add new book\n"
         << "2. Delete book (copying)\n"
         << "3. Delete book (merging)\n"
         << "4. Search book by ID\n"
         << "5. Search book by title\n"
         << "6. Update book info\n"
         << "7. Display books (in-order)\n"
         << "8. Display books (pre-order)\n"
         << "9. Display books (post-order)\n"
         << "10. Display books (breadth-first traversal / level-order)\n"
         << "11. Rotate left at a node\n"
         << "12. Rotate right at a node\n"
         << "13. Show total number of books\n"
         << "14. Number of books per publication year\n"
         << "15. Check height of tree and balance check\n"
         << "16. Tree visualization\n"
         << "17. Manual rotation at specific nodes\n"
         << "18. Automatically balance tree\n"
         << "0. Exit\n"
         << "============================================\n";
}

void loopMenu() {
    int choice;

    do {
        showMenu();

        inputIntegerInRange(choice, 0, 17, "==> Enter your selection: ");

        clearScreen();

        switch (choice) {

        // ADD BOOK
        case 1:
            library.insertBook();
            break;

        // DELETE BOOK (COPYING)
        case 2:
            //library.deleteByCopying();
            break;

        // DELETE BOOK (MERGING)
        case 3:
            library.deleteByMerging();
            break;

        // SEARCH BY ID
        case 4:
        {
            string id = inputString ("Enter book ID: ");
            Node* result = library.searchById(id);

            if (!result) {
                cout << "Book not found.\n";
            }
            else {
                cout << "Book found: \n";
                library.displayBook(result);
            }

            waitEnter();
            break;
        }

        // SEARCH BY TITLE
        case 5:
        {
            string title = inputString("Enter title to search: \n");
            library.searchByTitle(library.root, title);
            break;
        }
            
        // UPDATE BOOK
        case 6:
            library.updateBook();
            break;

        // DISPLAY IN-ORDER
        case 7:
            library.displayInOrder(library.root);
            waitEnter();
            break;

        // DISPLAY PRE-ORDER
        case 8:
            library.displayPreOrder(library.root);
            waitEnter();
            break;

        // DISPLAY POST-ORDER
        case 9:
            library.displayPostOrder(library.root);
            waitEnter();
            break;

        // DISPLAY breadth-first
        case 10:
            library.displayBreadthFirst(library.root);
            waitEnter();
            break;

        // ROTATE LEFT
        case 11:{
        cin.ignore(10000, '\n');

   string id;
    cout << "Enter book ID to rotate left: ";
    cin >> id;

    Node* p = library.searchById(id);

    if (p == nullptr) {
        cout << "Book ID not found" << endl;
    } else if (p->right == nullptr) {
        cout << "Node " << id << " has no right child to rotate left" << endl;
    } else {
        library.leftRotate(p);
        cout << "Rotation successful at node " << id << endl;
    }

    cout << "\nPress Enter to return to menu...";
    cin.ignore(10000, '\n');
    cin.get();
    break;
}

        // ROTATE RIGHT
        case 12:{
        cin.ignore(10000, '\n');
         string id;
    cout << "Enter book ID to rotate right: ";
    cin >> id;

    Node* p = library.searchById(id);

    if (p == nullptr) {
        cout << "Book ID not found" << endl;
    } else if (p->left == nullptr) {
        cout << "Node " << id << " has no left child to rotate right" << endl;
    } else {
        library.rightRotate(p);
        cout << "Rotation successful at node " << id << endl;
    }

    cout << "\nPress Enter to return to menu...";
    cin.ignore(10000, '\n');
    cin.get();
    break;
}
        // TOTAL BOOKS
        case 13:
            /*cout << "Total books: "
                 << library.countBooks() << endl;*/
            waitEnter();
            break;

        // BOOKS PER PUBLICATION YEAR
        case 14:
            //library.countBooksByYear();
            waitEnter();
            break;

        // Height and Balance check  (still need to check)
        case 15:
            int height = library.getHeight(library.root);
            cout << "Height of the tree: " << height << endl;
            waitEnter();
            break;  

        case 16:
            library.printTree();
            waitEnter();
            break;
            
        //Manual rotation at specific nodes    
        case 17:
        {
            int height = library.getHeight(library.root);
            cout << "Height of the tree: " << height << endl;
            waitEnter();
            break;
        }  

        //Auto balance
        case 18:
        {
            int height = library.getHeight(library.root);
            cout << "Height of the tree: " << height << endl;
            waitEnter();
            break;
        }  

        // EXIT
        case 0:
            isRunning = false;
            cout << "Exiting the program...\n";
            break;

        default:
            cout << "Invalid choice.\n";
        break;
    }

    } while (isRunning);
}

int main (){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    ifstream fin(BOOK_FILE);

    if (!fin.is_open()) {
        cout << "Cannot open file!\n";
        return 1;
    }
    library.root = library.loadFromFile(fin);

    fin.close();

    loopMenu();

    return 0;
}