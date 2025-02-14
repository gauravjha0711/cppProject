#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

// Book Record Class
class Record {
public:
    string name;
    int number;
    string author;
    string isbn;
    bool issued; // Track if the book is issued

    Record() : issued(false) {}

    Record(string n, int num, string a, string i)
        : name(n), number(num), author(a), isbn(i), issued(false) {}
};

// Library Management System
class LMS {
private:
    vector<Record> books;
    string librarianPassword = "admin123";
    string userAPassword = "userA";  // User A - Issue books
    string userBPassword = "userB";  // User B - Return books

public:
    LMS() { loadBooks(); }

    // Load books from file
    void loadBooks() {
        ifstream file("books.txt");
        if (file) {
            books.clear();  // Clear existing book records
            string name, author, isbn;
            int number;
            bool issued;
            while (file >> number >> issued) {
                file.ignore();
                getline(file, name);
                getline(file, author);
                getline(file, isbn);
                books.push_back(Record(name, number, author, isbn));
                books.back().issued = issued;
            }
            file.close();
        }
    }

    // Save books to file
    void saveBooks() {
        ofstream file("books.txt");
        for (auto &book : books) {
            file << book.number << " " << book.issued << "\n"
                 << book.name << "\n"
                 << book.author << "\n"
                 << book.isbn << "\n";
        }
        file.close();
    }

    // Add a book
    void addBook() {
        string name, author, isbn;
        int number;

        cin.ignore(); // Clear input buffer
        cout << "Enter book name: ";
        getline(cin, name);  // Use getline to read full book name

        cout << "Enter book number: ";
        while (!(cin >> number)) { // Validate integer input
            cout << "Invalid input! Enter a valid number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cin.ignore(); // Clear buffer before next input
        cout << "Enter author: ";
        getline(cin, author);

        cout << "Enter ISBN: ";
        getline(cin, isbn);

        books.push_back(Record(name, number, author, isbn));
        saveBooks();
        cout << "✅ Book added successfully!\n";
    }

    // Remove a book
    void removeBook() {
        string isbn;
        cout << "Enter ISBN to remove book: ";
        cin >> isbn;

        for (auto it = books.begin(); it != books.end(); ++it) {
            if (it->isbn == isbn) {
                books.erase(it);
                saveBooks();
                cout << "✅ Book removed successfully!\n";
                return;
            }
        }
        cout << "❌ Book not found!\n";
    }

    // Display all books
    void displayBooks() {
        cout << "\nAvailable Books:\n";
        for (auto &book : books) {
            cout << "Name: " << book.name
                 << "\nNumber: " << book.number
                 << "\nAuthor: " << book.author
                 << "\nISBN: " << book.isbn
                 << "\nIssued: " << (book.issued ? "Yes" : "No")
                 << "\n-----------------------\n";
        }
    }

    // Issue a book (User A)
    void issueBook() {
        string isbn;
        cout << "Enter ISBN to issue a book: ";
        cin >> isbn;

        for (auto &book : books) {
            if (book.isbn == isbn && !book.issued) {
                book.issued = true;
                saveBooks();
                cout << "✅ Book issued successfully!\n";
                return;
            }
        }
        cout << "❌ Book not available or already issued!\n";
    }

    // Return a book (User B)
    void returnBook() {
        string isbn;
        cout << "Enter ISBN to return a book: ";
        cin >> isbn;

        for (auto &book : books) {
            if (book.isbn == isbn && book.issued) {
                book.issued = false;
                saveBooks();
                cout << "✅ Book returned successfully!\n";
                return;
            }
        }
        cout << "❌ Book not found or not issued!\n";
    }

    // Librarian Menu
    void librarianMenu() {
        string password;
        cout << "Enter Librarian Password: ";
        cin >> password;

        if (password != librarianPassword) {
            cout << "❌ Incorrect Password!\n";
            return;
        }

        int choice;
        do {
            cout << "\nLibrarian Menu:\n"
                 << "1. Add Book\n"
                 << "2. Remove Book\n"
                 << "3. Display Books\n"
                 << "4. Exit\n"
                 << "Enter choice: ";
            cin >> choice;

            switch (choice) {
                case 1: addBook(); break;
                case 2: removeBook(); break;
                case 3: displayBooks(); break;
                case 4: return;
                default: cout << "❌ Invalid choice!\n";
            }
        } while (choice != 4);
    }

    // User Menu
    void userMenu(char type) {
        string password;
        cout << "Enter User " << type << " Password: ";
        cin >> password;

        if ((type == 'A' && password != userAPassword) || (type == 'B' && password != userBPassword)) {
            cout << "❌ Incorrect Password!\n";
            return;
        }

        int choice;
        do {
            cout << "\nUser " << type << " Menu:\n"
                 << (type == 'A' ? "1. Issue Book\n" : "1. Return Book\n")
                 << "2. View Available Books\n"
                 << "3. Exit\n"
                 << "Enter choice: ";
            cin >> choice;

            if (type == 'A' && choice == 1) issueBook();
            if (type == 'B' && choice == 1) returnBook();
            if (choice == 2) displayBooks();
            if (choice == 3) return;
        } while (choice != 3);
    }
};

// Main function
int main() {
    LMS system;
    int choice;

    do {
        cout << "\nWelcome to Library Management System\n"
             << "1. Librarian\n"
             << "2. User A (Issue Books)\n"
             << "3. User B (Return Books)\n"
             << "4. Exit\n"
             << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: system.librarianMenu(); break;
            case 2: system.userMenu('A'); break;
            case 3: system.userMenu('B'); break;
            case 4: cout << "Exiting...\n"; break;
            default: cout << "❌ Invalid choice!\n";
        }
    } while (choice != 4);

    return 0;
}