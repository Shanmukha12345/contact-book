#include "contact.h"
#include <iomanip>
#include <ctime>
#include <regex>

Contact::Contact() : id(0), isFavorite(false) {
    time_t now = time(0);
    lastModified = ctime(&now);
}

Contact::Contact(int id, string firstName, string lastName, string phone, string email,
                 string address, string city, string country, vector<string> labels,
                 string notes, string birthday, bool isFavorite, string profileImagePath)
    : id(id), firstName(firstName), lastName(lastName), phone(phone), email(email),
      address(address), city(city), country(country), labels(labels), notes(notes),
      birthday(birthday), isFavorite(isFavorite), profileImagePath(profileImagePath) {
    time_t now = time(0);
    lastModified = ctime(&now);
}

// Getters
int Contact::getId() const { return id; }
string Contact::getFirstName() const { return firstName; }
string Contact::getLastName() const { return lastName; }
string Contact::getPhone() const { return phone; }
string Contact::getEmail() const { return email; }
string Contact::getAddress() const { return address; }
string Contact::getCity() const { return city; }
string Contact::getCountry() const { return country; }
vector<string> Contact::getLabels() const { return labels; }
string Contact::getNotes() const { return notes; }
string Contact::getBirthday() const { return birthday; }
bool Contact::getIsFavorite() const { return isFavorite; }
string Contact::getLastModified() const { return lastModified; }
string Contact::getProfileImagePath() const { return profileImagePath; }

// Setters
void Contact::setId(int id) { this->id = id; }
void Contact::setFirstName(const string& fname) { this->firstName = fname; }
void Contact::setLastName(const string& lname) { this->lastName = lname; }
void Contact::setPhone(const string& phone) { this->phone = phone; }
void Contact::setEmail(const string& email) { this->email = email; }
void Contact::setAddress(const string& address) { this->address = address; }
void Contact::setCity(const string& city) { this->city = city; }
void Contact::setCountry(const string& country) { this->country = country; }
void Contact::setLabels(const vector<string>& labels) { this->labels = labels; }
void Contact::setNotes(const string& notes) { this->notes = notes; }
void Contact::setBirthday(const string& birthday) { this->birthday = birthday; }
void Contact::setIsFavorite(bool favorite) { this->isFavorite = favorite; }
void Contact::setLastModified(const string& modified) { this->lastModified = modified; }
void Contact::setProfileImagePath(const string& path) { this->profileImagePath = path; }

// Additional methods
void Contact::addLabel(const string& label) {
    labels.push_back(label);
}

void Contact::removeLabel(const string& label) {
    labels.erase(remove(labels.begin(), labels.end(), label), labels.end());
}

bool Contact::hasLabel(const string& label) const {
    return find(labels.begin(), labels.end(), label) != labels.end();
}

void Contact::displayContact() const {
    cout << "ID: " << id << "\n"
         << "Name: " << firstName << " " << lastName << "\n"
         << "Phone: " << phone << "\n"
         << "Email: " << email << "\n"
         << "Address: " << address << ", " << city << ", " << country << "\n"
         << "Birthday: " << birthday << "\n"
         << "Notes: " << notes << "\n"
         << "Labels: ";
    for (const auto& l : labels) cout << l << ", ";
    cout << "\nFavorite: " << (isFavorite ? "Yes" : "No") << "\n";
    cout << "Last Modified: " << lastModified << "\n";
    if (!profileImagePath.empty())
        cout << "Profile Image: " << profileImagePath << "\n";
}

bool Contact::matchesQuery(const string& query) const {
    string q = query;
    transform(q.begin(), q.end(), q.begin(), ::tolower);

    return firstName.find(q) != string::npos ||
           lastName.find(q) != string::npos ||
           phone.find(q) != string::npos ||
           email.find(q) != string::npos;
}

string Contact::toCSV() const {
    ostringstream oss;
    oss << id << ',' << firstName << ',' << lastName << ',' << phone << ',' << email << ','
        << address << ',' << city << ',' << country << ',' << birthday << ',' << isFavorite << ',';

    for (size_t i = 0; i < labels.size(); ++i) {
        oss << labels[i];
        if (i < labels.size() - 1) oss << '|';
    }

    oss << ',' << notes << ',' << profileImagePath;
    return oss.str();
}

Contact Contact::fromCSV(const string& csvLine) {
    stringstream ss(csvLine);
    string token;
    vector<string> fields;

    while (getline(ss, token, ',')) {
        fields.push_back(token);
    }

    vector<string> lbls;
    stringstream lblStream(fields[10]);
    while (getline(lblStream, token, '|')) {
        lbls.push_back(token);
    }

    return Contact(
        stoi(fields[0]), fields[1], fields[2], fields[3], fields[4],
        fields[5], fields[6], fields[7], lbls,
        fields[11], fields[8], fields[9] == "1",
        fields.size() > 12 ? fields[12] : ""
    );
}
