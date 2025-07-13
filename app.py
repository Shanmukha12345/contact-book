from flask import Flask, request, jsonify, render_template, send_file
from flask_cors import CORS
import os
import csv
from io import StringIO

app = Flask(__name__, static_folder="static", template_folder="templates")
CORS(app)

DATA_FILE = "../data/contacts.txt"

FIELDS = [
    "id", "firstName", "lastName", "phone", "email",
    "category", "country", "birthday", "address",
    "city", "labels", "notes", "imageUrl", "favorite"
]

# ============ DSA Structures ============
class TrieNode:
    def __init__(self):
        self.children = {}
        self.contact_ids = set()

class Trie:
    def __init__(self):
        self.root = TrieNode()

    def insert(self, word, contact_id):
        node = self.root
        for char in word.lower():
            if char not in node.children:
                node.children[char] = TrieNode()
            node = node.children[char]
            node.contact_ids.add(contact_id)

    def search(self, prefix):
        node = self.root
        for char in prefix.lower():
            if char not in node.children:
                return set()
            node = node.children[char]
        return node.contact_ids

name_trie = Trie()
email_map = {}      #hash maps
phone_map = {}
id_map = {}

def load_ds_maps(contacts):
    name_trie.root = TrieNode()
    email_map.clear()
    phone_map.clear()
    id_map.clear()
    for c in contacts:
        name_trie.insert(c["firstName"], c["id"])
        name_trie.insert(c["lastName"], c["id"])
        email_map[c["email"]] = c
        phone_map[c["phone"]] = c
        id_map[c["id"]] = c

# ============ File Operations ============
def read_contacts():
    if not os.path.exists(DATA_FILE):
        return []
    with open(DATA_FILE, 'r', encoding="utf-8") as f:
        lines = [line.strip().split(',') for line in f.readlines()]
    contacts = []
    for parts in lines:
        if len(parts) >= 7:
            # Fill missing fields and parse labels & favorite
            extended = parts + [""] * (len(FIELDS) - len(parts))
            contact = dict(zip(FIELDS, extended))
            contact["labels"] = contact["labels"].split(";") if contact["labels"] else []
            contact["favorite"] = contact.get("favorite", "False").lower() == "true"
            contacts.append(contact)
    load_ds_maps(contacts)
    return contacts

def write_contacts(contact_list):
    with open(DATA_FILE, 'w', encoding="utf-8") as f:
        for c in contact_list:
            line = ','.join([
                c.get("id", ""),
                c.get("firstName", ""),
                c.get("lastName", ""),
                c.get("phone", ""),
                c.get("email", ""),
                c.get("category", ""),
                c.get("country", ""),
                c.get("birthday", ""),
                c.get("address", ""),
                c.get("city", ""),
                ";".join(c.get("labels", [])) if isinstance(c.get("labels"), list) else c.get("labels", ""),
                c.get("notes", ""),
                c.get("imageUrl", ""),
                str(c.get("favorite", False))
            ])
            f.write(line + '\n')

# ============ Routes ============
@app.route("/")
def index():
    return render_template("index.html")

@app.route("/contacts", methods=["GET"])
def get_contacts():
    return jsonify(read_contacts())

@app.route("/add", methods=["POST"])
def add_contact():
    data = request.json
    contacts = read_contacts()
    if data["email"] in email_map or data["phone"] in phone_map:
        return jsonify({"status": "duplicate", "message": "Contact already exists."}), 409
    contacts.append(data)
    write_contacts(contacts)
    return jsonify({"status": "added"})

@app.route("/delete/<id>", methods=["DELETE"])
def delete_contact(id):
    contacts = read_contacts()
    contacts = [c for c in contacts if c['id'] != id]
    write_contacts(contacts)
    return jsonify({"status": "deleted"})

@app.route("/search", methods=["GET"])
def search_contacts():
    query = request.args.get("q", "").lower()
    contacts = read_contacts()
    matched_ids = name_trie.search(query)
    results = [c for c in contacts if
        c["id"] in matched_ids or
        query in c["firstName"].lower() or
        query in c["lastName"].lower() or
        query in c["phone"].lower() or
        query in c["email"].lower() or
        query in c["category"].lower() or
        query in c["country"].lower() or
        query in c["address"].lower() or
        query in c["city"].lower() or
        query in c["notes"].lower() or
        any(query in label.lower() for label in c.get("labels", []))
    ]
    return jsonify(results)

@app.route("/export", methods=["GET"])
def export_contacts():
    contacts = read_contacts()
    output = StringIO()
    writer = csv.DictWriter(output, fieldnames=FIELDS)
    writer.writeheader()
    for c in contacts:
        c["labels"] = ";".join(c["labels"]) if isinstance(c["labels"], list) else c.get("labels", "")
        c["favorite"] = str(c.get("favorite", False))
        writer.writerow(c)
    output.seek(0)
    return send_file(output, mimetype='text/csv', as_attachment=True, download_name="contacts.csv")

if __name__ == "__main__":
    app.run(debug=True)
