// storage.cpp
#include "storage.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <iomanip>
using namespace std;

vector<Contact> Storage::loadContacts(const string& filename) {
    vector<Contact> contacts;
    ifstream file(filename);
    if (!file) return contacts;

    string line;
    while (getline(file, line)) {
        contacts.push_back(Contact::fromCSV(line));
    }
    return contacts;
}

void Storage::saveContacts(const string& filename, const vector<Contact>& contacts) {
    ofstream file(filename);
    for (const auto& c : contacts) {
        file << c.toCSV() << "\n";
    }
}

void Storage::addContact(const string& filename, const Contact& contact) {
    vector<Contact> contacts = loadContacts(filename);
    contacts.push_back(contact);
    saveContacts(filename, contacts);
}

void Storage::deleteContact(const string& filename, int contactId) {
    vector<Contact> contacts = loadContacts(filename);
    contacts.erase(remove_if(contacts.begin(), contacts.end(),
        [contactId](const Contact& c) { return c.getId() == contactId; }), contacts.end());
    saveContacts(filename, contacts);
}

void Storage::updateContact(const string& filename, const Contact& updatedContact) {
    vector<Contact> contacts = loadContacts(filename);
    for (auto& c : contacts) {
        if (c.getId() == updatedContact.getId()) {
            c = updatedContact;
            break;
        }
    }
    saveContacts(filename, contacts);
}

vector<Contact> Storage::searchContacts(const vector<Contact>& contacts, const string& query) {
    vector<Contact> results;
    for (const auto& c : contacts) {
        if (c.matchesQuery(query)) {
            results.push_back(c);
        }
    }
    return results;
}

vector<Contact> Storage::searchByPhone(const vector<Contact>& contacts, const string& phone) {
    vector<Contact> results;
    for (const auto& c : contacts) {
        if (c.getPhone().find(phone) != string::npos)
            results.push_back(c);
    }
    return results;
}

vector<Contact> Storage::filterByCategory(const vector<Contact>& contacts, const string& category) {
    vector<Contact> results;
    copy_if(contacts.begin(), contacts.end(), back_inserter(results),
        [&category](const Contact& c) {
            const auto& labels = c.getLabels();
            return find(labels.begin(), labels.end(), category) != labels.end();
        });
    return results;
}

vector<Contact> Storage::filterByCountry(const vector<Contact>& contacts, const string& country) {
    vector<Contact> results;
    copy_if(contacts.begin(), contacts.end(), back_inserter(results),
        [&country](const Contact& c) { return c.getCountry() == country; });
    return results;
}

void Storage::sortContactsByName(vector<Contact>& contacts) {
    sort(contacts.begin(), contacts.end(), [](const Contact& a, const Contact& b) {
        return a.getFirstName() < b.getFirstName();
    });
}

int Storage::countContacts(const vector<Contact>& contacts) {
    return contacts.size();
}

map<string, int> Storage::countByCategory(const vector<Contact>& contacts) {
    map<string, int> categoryCount;
    for (const auto& c : contacts) {
        for (const auto& label : c.getLabels()) {
            categoryCount[label]++;
        }
    }
    return categoryCount;
}

bool Storage::contactExists(const vector<Contact>& contacts, const Contact& contact) {
    return any_of(contacts.begin(), contacts.end(), [&contact](const Contact& c) {
        return c.getFirstName() == contact.getFirstName() &&
               c.getLastName() == contact.getLastName() &&
               c.getPhone() == contact.getPhone();
    });
}

bool Storage::isValidEmail(const string& email) {
    return email.find('@') != string::npos && email.find('.') != string::npos;
}

bool Storage::isValidPhone(const string& phone) {
    return phone.length() >= 10 && all_of(phone.begin(), phone.end(), ::isdigit);
}

void Storage::exportToCSV(const vector<Contact>& contacts, const string& csvFile) {
    ofstream file(csvFile);
    file << "ID,FirstName,LastName,Phone,Email,Address,City,Country,Birthday,Favorite,Labels,Notes,ProfileImage\n";
    for (const auto& c : contacts) {
        file << c.toCSV() << "\n";
    }
}

void Storage::backupContacts(const string& filename, const string& backupFilename) {
    ifstream src(filename, ios::binary);
    ofstream dst(backupFilename, ios::binary);
    dst << src.rdbuf();
}

void Storage::restoreFromBackup(const string& backupFilename, const string& originalFilename) {
    ifstream src(backupFilename, ios::binary);
    ofstream dst(originalFilename, ios::binary);
    dst << src.rdbuf();
}

Contact Storage::undoLastDelete(const string& filename, const string& tempFile) {
    ifstream temp(tempFile);
    string line;
    Contact lastDeleted;
    if (getline(temp, line)) {
        lastDeleted = Contact::fromCSV(line);
        addContact(filename, lastDeleted);
    }
    return lastDeleted;
}

void Storage::markAsFavorite(vector<Contact>& contacts, int contactId) {
    for (auto& c : contacts) {
        if (c.getId() == contactId) {
            c.setIsFavorite(true);
            break;
        }
    }
}

void Storage::unmarkFavorite(vector<Contact>& contacts, int contactId) {
    for (auto& c : contacts) {
        if (c.getId() == contactId) {
            c.setIsFavorite(false);
            break;
        }
    }
}

vector<Contact> Storage::listFavorites(const vector<Contact>& contacts) {
    vector<Contact> favorites;
    copy_if(contacts.begin(), contacts.end(), back_inserter(favorites), [](const Contact& c) {
        return c.getIsFavorite();
    });
    return favorites;
}