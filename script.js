// DOM Elements
const contactForm = document.getElementById("contactForm");
const contactList = document.getElementById("contactList");
const searchQuery = document.getElementById("searchQuery");
const csvInput = document.getElementById("csvInput");

const API = "http://127.0.0.1:5000";
let contacts = [];
let originalContacts = []; // For restoring after import
let sortAsc = true;

// Load Contacts from Backend
window.onload = async () => {
  try {
    const res = await fetch(`${API}/contacts`);
    contacts = await res.json();
    renderContacts();
    birthdayReminder();
  } catch (error) {
    console.error("Failed to load contacts from backend:", error);
    loadContactsFromLocal();
    birthdayReminder();
  }
};

// Add Contact
contactForm.addEventListener("submit", async function (e) {
  e.preventDefault();

  const newContact = {
    id: document.getElementById("id").value.trim(),
    firstName: document.getElementById("firstName").value.trim(),
    lastName: document.getElementById("lastName").value.trim(),
    phone: document.getElementById("phone").value.trim(),
    email: document.getElementById("email").value.trim(),
    category: document.getElementById("category").value.trim(),
    country: document.getElementById("country").value.trim(),
    birthday: document.getElementById("birthday")?.value.trim() || "",
    address: document.getElementById("address")?.value.trim() || "",
    city: document.getElementById("city")?.value.trim() || "",
    labels: (document.getElementById("labels")?.value || "")
            .split(",")
            .map(l => l.trim())
            .filter(Boolean),
    notes: document.getElementById("notes")?.value.trim() || "",
    imageUrl: document.getElementById("imageUrl")?.value.trim() || "",
    favorite: false
  };

  if (hasDuplicate(newContact)) {
    alert("Duplicate contact (same email & phone) not allowed.");
    return;
  }

  contacts.push(newContact);
  saveContactsToLocal();
  renderContacts();
  contactForm.reset();

  try {
    await fetch(`${API}/add`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(newContact)
    });
  } catch (error) {
    console.error("Failed to add contact to backend:", error);
  }
});

// Render Contact List
function renderContacts(list = contacts) {
  contactList.innerHTML = "";
  list.forEach((contact, index) => {
    const row = document.createElement("tr");
    row.innerHTML = `
      <td>${contact.favorite ? '★' : '☆'}</td>
      <td>${contact.id}</td>
      <td>
        <div style="display:flex; align-items:center; gap:8px;">
          ${contact.imageUrl ? `<img src="${contact.imageUrl}" alt="Img" width="40" height="40" style="border-radius:50%">` : ''}
          <span>${contact.firstName}</span>
        </div>
      </td>
      <td>${contact.lastName}</td>
      <td>${contact.phone}</td>
      <td>${contact.email}</td>
      <td>${contact.category}</td>
      <td>${contact.country}</td>
      <td>${contact.address || ''}</td>
      <td>${contact.city || ''}</td>
      <td>${contact.birthday || ''}</td>
      <td>${contact.labels?.join(", ") || ''}</td>
      <td>${contact.notes || ''}</td>
      <td>
        <button onclick="editContact(${index})">Edit</button>
        <button onclick="deleteContact(${index})">Delete</button>
        <button onclick="toggleFavorite(${index})">${contact.favorite ? '★' : '☆'}</button>
      </td>
    `;
    contactList.appendChild(row);
  });
}

// Search Contacts
function searchContact() {
  const normalize = str => (str || "").toLowerCase().trim();
  const qNorm = normalize(searchQuery.value);

  const filtered = contacts.filter(c =>
    normalize(c.firstName).includes(qNorm) ||
    normalize(c.lastName).includes(qNorm) ||
    normalize(c.phone).includes(qNorm) ||
    normalize(c.email).includes(qNorm) ||
    normalize(c.category).includes(qNorm) ||
    normalize(c.country).includes(qNorm) ||
    normalize(c.address).includes(qNorm) ||
    normalize(c.city).includes(qNorm) ||
    normalize(c.birthday).includes(qNorm) ||
    normalize((Array.isArray(c.labels) ? c.labels.join(" ") : "")).includes(qNorm) ||
    normalize(c.notes).includes(qNorm)
  );

  renderContacts(filtered);
}

// Edit Contact
function editContact(index) {
  const c = contacts[index];
  document.getElementById("id").value = c.id;
  document.getElementById("firstName").value = c.firstName;
  document.getElementById("lastName").value = c.lastName;
  document.getElementById("phone").value = c.phone;
  document.getElementById("email").value = c.email;
  document.getElementById("category").value = c.category;
  document.getElementById("country").value = c.country;
  document.getElementById("birthday").value = c.birthday || "";
  document.getElementById("address").value = c.address || "";
  document.getElementById("city").value = c.city || "";
  document.getElementById("labels").value = c.labels?.join(", ") || "";
  document.getElementById("notes").value = c.notes || "";
  document.getElementById("imageUrl").value = c.imageUrl || "";
  deleteContact(index);
}

// Delete Contact
async function deleteContact(index) {
  const contact = contacts[index];
  contacts.splice(index, 1);
  saveContactsToLocal();
  renderContacts();

  try {
    await fetch(`${API}/delete/${contact.id}`, {
      method: "DELETE"
    });
  } catch (error) {
    console.error("Failed to delete contact from backend:", error);
  }
}

// Toggle Favorite
async function toggleFavorite(index) {
  contacts[index].favorite = !contacts[index].favorite;
  saveContactsToLocal();
  renderContacts();

  try {
    await fetch(`${API}/update/${contacts[index].id}`, {
      method: "PUT",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(contacts[index])
    });
  } catch (error) {
    console.error("Failed to update favorite in backend:", error);
  }
}

// Export to CSV
function exportCSV() {
  let csv = "ID,First Name,Last Name,Phone,Email,Category,Country,Birthday,Address,City,Labels,Notes,ImageURL\n";
  contacts.forEach(c => {
    csv += `${c.id},${c.firstName},${c.lastName},${c.phone},${c.email},${c.category},${c.country},${c.birthday},${c.address},${c.city},"${c.labels?.join("; ")}",${c.notes},${c.imageUrl}\n`;
  });
  const blob = new Blob([csv], { type: "text/csv" });
  const a = document.createElement("a");
  a.href = URL.createObjectURL(blob);
  a.download = "contacts.csv";
  a.click();
}

// Import CSV
csvInput?.addEventListener("change", function (event) {
  const file = event.target.files[0];
  const reader = new FileReader();
  reader.onload = function (e) {
    originalContacts = [...contacts]; // Backup original
    contacts = [];
    const lines = e.target.result.split("\n");
    lines.slice(1).forEach(line => {
      const [id, firstName, lastName, phone, email, category, country, birthday, address, city, labels, notes, imageUrl] = line.split(",");
      if (id && firstName) {
        contacts.push({
          id, firstName, lastName, phone, email, category, country,
          birthday, address, city, notes, imageUrl,
          labels: labels?.split(";").map(l => l.trim()) || [],
          favorite: false
        });
      }
    });
    saveContactsToLocal();
    renderContacts();
  };
  reader.readAsText(file);
});

// Restore Recent (Original)
function showOriginalContacts() {
  if (originalContacts.length > 0) {
    contacts = [...originalContacts];
    renderContacts();
  } else {
    alert("No recent (original) contacts found.");
  }
}
// Sort
function sortContactsByName() {
  contacts.sort((a, b) => {
    const nameA = (a.firstName + a.lastName).toLowerCase();
    const nameB = (b.firstName + b.lastName).toLowerCase();
    return sortAsc ? nameA.localeCompare(nameB) : nameB.localeCompare(nameA);
  });
  sortAsc = !sortAsc;
  renderContacts();
}

// Filters
function filterByCategory(category) {
  renderContacts(contacts.filter(c => c.category.toLowerCase() === category.toLowerCase()));
}
function filterByCountry(country) {
  renderContacts(contacts.filter(c => c.country.toLowerCase() === country.toLowerCase()));
}
function filterFavorites() {
  const favoriteContacts = contacts.filter(c => c.favorite);
  if (favoriteContacts.length === 0) alert("No favorite contacts found.");
  else renderContacts(favoriteContacts);
}
function showAllContacts() {
  renderContacts(contacts);
}

// Birthday Reminder
function birthdayReminder() {
  const today = new Date().toISOString().slice(5, 10);
  const upcoming = contacts.filter(c => c.birthday?.slice(5, 10) === today);
  if (upcoming.length) {
    alert(`🎂 Today's birthdays:\n${upcoming.map(c => c.firstName + " " + c.lastName).join("\n")}`);
  }
}

// Duplicates
function hasDuplicate(newContact) {
  return contacts.some(c => c.email === newContact.email && c.phone === newContact.phone);
}

// Local Storage
function saveContactsToLocal() {
  localStorage.setItem('contacts', JSON.stringify(contacts));
}
function loadContactsFromLocal() {
  const data = localStorage.getItem('contacts');
  if (data) {
    contacts = JSON.parse(data);
    renderContacts();
  }
}
