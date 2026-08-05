#ifndef SUBJECTS_H
#define SUBJECTS_H

#include <string>

using namespace std;

class Book {
    private:
        string id;
        string title;
        string author;
        int year;

    public:
        //Constructor
        Book(string id, string title, string author, int year) {
            this->id = id;
            this->title = title;
            this->author = author;
            this->year = year;
        }

        //Getters
        string getId() {
            return id;
        }
        string getTitle() {
            return title;
        }
        string getAuthor() {
            return author;
        }
        int getYear() {
            return year;
        }    

        //Setters
        void setId (string id) {
            this->id = id;
        }
        void setTitle (string title) {
            this->title = title;
        }
        void setAuthor (string author) {
            this->author = author;
        }
        void setYear (int year) {
            this->year = year;
        }
};

class Node {
    public:
        Book *data;
        Node* left;
        Node* right;

        Node(Book *value) {
            data = value;
            left = right = nullptr;
        }
        ~Node() {
            delete data;
        }
};

#endif 
