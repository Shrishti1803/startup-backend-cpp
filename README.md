# startup-backend-cpp

A C++ backend system developed for an early-stage startup that connects brands and influencers.

This application is designed as an internal management tool used by company members to manage operational data such as brand records, influencer information, and related business data.

The system focuses on secure authentication, structured database design, and controlled access to database operations.

---

## 🚀 Purpose

This backend application allows internal company users to:

- Insert, update, and manage brand data
- Maintain structured records
- Control access to database operations
- Enforce role-based permissions
- Maintain secure login and credential handling

It is not a public-facing API, but an internal operational system.

---

## 🔐 Authentication & Security

The system includes:

- Secure email-based authentication
- Argon2id password hashing (via libsodium)
- First-time login handling
- Separation of identity (`ADMIN`) and credentials (`LOGIN`)
- Normalized relational schema
- Structured logging using spdlog

---

## 👥 Role-Based Access Control (RBAC)

Three roles are supported:

- `ADMIN`
- `DBA`
- `USER`

Access control rules:

- ADMIN & DBA → Full database access (including DELETE)
- USER → Restricted from DELETE operations

Authorization checks are enforced at the service layer.

---

## 🏗 Architecture Overview

- C++17 backend architecture
- MySQL relational database
- Normalized schema with foreign key linkage via `user_id`
- Clear separation between:
  - Authentication layer
  - Query layer
  - Business logic layer
- Structured logging and error handling

---

## 🛠 Tech Stack

- C++17
- MySQL
- MySQL Connector/C++
- libsodium (Argon2id hashing)
- spdlog
- fmt

---

## 📂 Project Structure
include/
Auth/
Db/
Security/
Utils/

src/
Auth/
Db/
Security/
Utils/



---

## ⚙️ Build

Compile using:

```bash
g++ -std=c++17 -Iinclude $(find src -name "*.cpp") -o app -lmysqlcppconn -lsodium -lfmt


📌 Current Status

✔ Authentication system implemented
✔ Normalized schema design
🔜 Business data management modules in progress
🔜 Role-based access control in progress

👩‍💻 About This Project

This backend system is being developed as part of real startup work and ongoing backend engineering practice.

## 👩‍💻 Author

Shrishti  

Undergraduate student passionate about systems programming and backend architecture.  
Focused on building structured, secure C++ systems with strong foundations in database design and authentication.
