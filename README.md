# 📚 Contact Book – DSA Project

A full-stack Contact Book application built using **Flask (Python)** for the backend and **HTML/CSS/JavaScript** for the frontend. This project integrates **core DSA (Data Structures & Algorithms)** such as **Trie** and **Hash Maps** for efficient data search and management.

---

## 🚀 Features

- 🔍 Full-text **search** using Trie & filters
- ➕ **Add**, 🖊️ **Edit**, ❌ **Delete** contacts
- ⭐ Mark/unmark **favorites**
- 🗂️ **Sort & Filter** by category, country
- ⬇️ **Import/Export** contacts via CSV
- 🎂 **Birthday Reminder**
- 📸 Upload contact **image URLs**
- 🧠 **Duplicate detection** via hashing (email & phone)
- 🖥️ **Local Storage** fallback (for offline use)

---

## 🧠 DSA Concepts Used

| Concept     | Usage |
|-------------|-------|
| **Trie**    | Fast prefix-based searching (first/last names) |
| **Hash Map**| Quick lookups for email, phone, and ID |
| **Array/List** | In-memory contact storage |
| **File Handling** | Persistent storage using `.txt` and `.csv` |

---

## 🛠️ Tech Stack

| Layer      | Tools |
|------------|-------|
| Frontend   | HTML, CSS, JavaScript |
| Backend    | Python, Flask |
| Storage    | CSV File, TXT File |
| Algorithms | Trie, Hash Map |

---

## 🗂️ Folder Structure

📁 contact-book/
├── 📂 backend/
│ └── app.py # Flask backend with DSA logic
├── 📂 templates/
│ └── index.html # Web frontend
├── 📂 static/
│ ├── script.js # JS logic
│ └── style.css # Styling
├── 📁 data/
│ └── contacts.txt # Contact data storage
├── README.md # You are here!


---

## 📦 How to Run the Project

```bash
git clone https://github.com/Shanmukha12345/contact-book
cd contact-book/backend
python app.py
