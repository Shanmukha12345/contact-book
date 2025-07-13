#ifndef CONTACT_H
#define CONTACT_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <ctime>
using namespace std;

class Contact {
private:
    int id;
    string firstName;
    string lastName;
    string phone;
    string email;
    string address;
    string city;
    string country;
    vector<string> labels;
    string notes;
    string birthday;
    bool isFavorite;
    string lastModified;
    string profileImagePath;

public:
    Contact();
    Contact(int id, string firstName, string lastName, string phone, string email,
            string address, string city, string country, vector<string> labels,
            string notes = "", string birthday = "", bool isFavorite = false);

    // Getters
    int getId() const;
    string getFirstName() const;
    string getLastName() const;
    string getPhone() const;
    string getEmail() const;
    string getAddress() const;
    string getCity() const;
    string getCountry() const;
    vector<string> getLabels() const;
    string getNotes() const;
    string getBirthday() const;
    bool getIsFavorite() const;
    string getLastModified() const;
    string getProfileImagePath() const;

    // Setters
    void setId(int id);
    void setFirstName(const string& fname);
    void setLastName(const string& lname);
    void setPhone(const string& phone);
    void setEmail(const string& email);
    void setAddress(const string& address);
    void setCity(const string& city);
    void setCountry(const string& country);
    void setLabels(const vector<string>& labels);
    void setNotes(const string& notes);
    void setBirthday(const string& birthday);
    void setIsFavorite(bool favorite);
    void setProfileImagePath(const string&);
    void updateTimestamp();

    // Tag Management
    void addLabel(const string& label);
    void removeLabel(const string& label);
    bool hasLabel(const string& label) const;

    // Comparison
    bool operator<(const Contact& other) const;
    bool operator==(const Contact& other) const;

    // Validation
    bool isValidPhone() const;
    bool isValidEmail() const;

    // Additional Features
    void displayContact() const;
    bool matchesQuery(const string& query) const;
    string toCSV() const;
    static Contact fromCSV(const string& csvLine);
};

#endif // CONTACT_H
