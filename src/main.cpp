#include <iostream>
#include <fstream>
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
         << "15. Automatically balance tree\n"
         << "16. Tree visualization\n"
         << "0. Exit\n"
         << "============================================\n";
}

void loopMenu() {
    int choice;

    do {
        showMenu();

        inputIntegerInRange(choice, 0, 16, "==> Enter your selection: ");

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
        case 11:
            //library.rotateLeft();
            break;

        // ROTATE RIGHT
        case 12:
            //library.rotateRight();
            break;

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

        // Automatically balance 
        case 15:
            waitEnter();
            break;    

        // EXIT
        case 0:
            isRunning = false;
            cout << "Exiting the program...\n";
            break;

        default:
            cout << "Invalid choice.\n";
        }

    } while (isRunning);
}

int main (){
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