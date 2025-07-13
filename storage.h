// storage.h
#ifndef STORAGE_H
#define STORAGE_H

#include "contact.h"
#include <vector>
#include <string>
#include <map>
#include <set>
using namespace std;

class Storage {
public:
    // Core File Operations
    static vector<Contact> loadContacts(const string& filename);
    static void saveContacts(const string& filename, const vector<Contact>& contacts);
    static void backupContacts(const string& filename, const string& backupFilename);
    static void restoreFromBackup(const string& backupFilename, const string& originalFilename);

    // CRUD
    static void addContact(const string& filename, const Contact& contact);
    static void deleteContact(const string& filename, int contactId);
    static void updateContact(const string& filename, const Contact& updatedContact);

    // Search and Filter
    static vector<Contact> searchContacts(const vector<Contact>& contacts, const string& query);
    static vector<Contact> filterByCategory(const vector<Contact>& contacts, const string& category);
    static vector<Contact> filterByCountry(const vector<Contact>& contacts, const string& country);
    static vector<Contact> searchByPhone(const vector<Contact>& contacts, const string& phone);

    // Sort and Count
    static void sortContactsByName(vector<Contact>& contacts);
    static void sortContactsByDate(vector<Contact>& contacts); // assuming date field
    static int countContacts(const vector<Contact>& contacts);
    static map<string, int> countByCategory(const vector<Contact>& contacts);

    // Utilities
    static bool contactExists(const vector<Contact>& contacts, const Contact& contact);
    static bool isValidEmail(const string& email);
    static bool isValidPhone(const string& phone);
    static void exportToCSV(const vector<Contact>& contacts, const string& csvFile);
    static Contact undoLastDelete(const string& filename, const string& tempFile);

    // Favorites Feature
    static void markAsFavorite(vector<Contact>& contacts, int contactId);
    static void unmarkFavorite(vector<Contact>& contacts, int contactId);
    static vector<Contact> listFavorites(const vector<Contact>& contacts);
};

#endif // STORAGE_H
