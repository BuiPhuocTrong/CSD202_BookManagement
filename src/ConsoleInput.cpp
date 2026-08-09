#include "../header/ConsoleInput.h"
#include "../header/Validation.h"
#include "../header/BookManager.h"
#include "../header/Subjects.h"

#include <iostream>
#include <string>

using namespace std;

//Input integer
void inputIntegerInRange(int& target, int min, int max, const string& prompt) {
    string input;

    while (true) {
        cout << prompt;
        getline(cin, input);

        if (!isIntegerBelongRange(input, min, max)) {
            cout << "Only enter a number from "
                 << min << " to " << max << "." << endl;
            continue;
        }

        target = stoi(input);
        return;
    }
}

//Input string
//Enter prompt, return input from user
string inputString(const string& prompt)
{
    string input;

    while (true)
    {
        cout << prompt;
        getline(cin, input);

        if (input.empty())
        {
            cout << "Input cannot be empty.\n";
            continue;
        }

        return input;
    }
}

// Input Book
Book inputBook() {
    string id, title, author;
    int year;

    id = inputString("Enter book ID: ");

    title = inputString("Enter book title: ");

    author = inputString("Enter book author: ");

    inputIntegerInRange(year, 0, 2026, "Enter book year (0000-2026): ");

    Book b(id, title, author, year);
    return b;
}

void waitEnter(){
    cout << "\nPress Enter to back...";
    cin.get();
}