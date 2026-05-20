# startup-backend-cpp

A modular C++ backend and CLI-based internal management system developed for an early-stage startup that connects brands and creators.

This application is designed as an internal operational platform used by company members to manage business workflows, creator records, brand intelligence, deal tracking, analytics, and structured database operations.

The system focuses on secure authentication, modular backend architecture, normalized database design, and controlled access to operational data.

---

# 🚀 Purpose

This system allows internal company users to:

- Manage brand and creator records
- Track influencer outreach and deals
- Maintain structured operational data
- Enforce role-based permissions
- Perform advanced search and filtering
- Generate analytics insights
- Maintain secure authentication and credential handling

This is not a public-facing API, but a full internal operational backend system.

---

# 📦 Core Modules

## Brand Management
- Brand profile management
- Brand standards & insights
- Competitor tracking
- Revenue tracking
- Brand goals
- Head/contact management
- Funding/news tracking
- Past influencer campaign tracking

## Creator Management
- Creator profile management
- Niche and audience tracking
- Platform/profile account management
- Community reach analysis
- HPV audits
- Strategic notes
- Verdict & evaluation system
- Video L10 analytics tracking

## Deal Management
- Deal lifecycle tracking
- Lead → Connected → Deal Done workflow
- Post upload tracking
- Payment clearance tracking
- Validation-enforced state transitions

## Reference Management
Centralized management for:
- Genres
- Niches
- Platforms
- Target Audiences
- Positions

## Search & Filtering System
Supports:
- Brand search
- Creator search
- Genre filtering
- Niche filtering
- Audience-based filtering
- Cross-entity query workflows

## Analytics
- Monthly deal analytics
- CSV export pipeline
- Python + matplotlib integration

---

# 🔐 Authentication & Security

The system includes:

- Secure email-based authentication
- Argon2id password hashing (libsodium)
- Password reset/update flow
- Role-based authorization
- First-time login handling
- Structured logging using spdlog
- Separation of identity (`ADMIN`) and credentials (`LOGIN`)
- Normalized relational database schema

---

# 👥 Role-Based Access Control (RBAC)

Three roles are supported:

- `ADMIN`
- `DBA`
- `USER`

### Access Rules

- ADMIN & DBA → Full database access
- USER → Restricted from DELETE operations

Authorization checks are enforced at the service layer.

---

# 🏗 Architecture Overview

The project follows a layered modular backend architecture using:

- Repository Pattern
- Service Layer Architecture
- Aggregate-based data management
- Transaction management
- Modular CLI rendering architecture
- Structured dependency management

### Architectural Layers

- Authentication Layer
- Database/Repository Layer
- Business Logic Layer
- CLI/UI Layer
- Analytics Layer

---

# 🗄 Database Design

- MySQL relational database
- Normalized schema design
- Foreign key relationship enforcement
- Transaction-safe operations
- Soft delete architecture
- Aggregate persistence handling

A schema-only SQL dump (`schema.sql`) is included to recreate the database structure without test data.

---

# 🐳 Docker Support

The application supports containerized deployment using Docker and Docker Compose.

### Docker Features

- MySQL container integration
- Internal Docker networking
- Persistent database volumes
- Healthcheck-based startup sequencing
- Portable backend deployment workflow

---

# 🛠 Tech Stack

- C++17
- MySQL
- MySQL Connector/C++
- Docker
- Docker Compose
- libsodium (Argon2id hashing)
- spdlog
- fmt
- Python
- matplotlib

---

# 📂 Project Structure

```text
include/
├── Auth/
├── CLI/
├── Db/
├── Models/
├── Security/
├── Service/
└── Utils/

src/
├── Auth/
├── CLI/
├── Db/
├── Models/
├── Security/
├── Service/
└── Utils/

# ⚙️ Build Instructions

## Local Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

---

# 🐳 Docker Setup

## Build & Run

```bash
docker-compose up --build
```

---

# 🗄 Database Setup

To recreate the database schema:

```bash
mysql -u root -p SIMLIPAL < schema.sql
```

---

# 📊 Analytics

The system includes a lightweight analytics module that generates visual deal analytics using Python and matplotlib integration.

Current analytics support:
- Monthly deal tracking
- CSV export generation
- Graph plotting workflow

---

# 📌 Current Status

✔ Authentication & RBAC system completed  
✔ Brand management system completed  
✔ Creator management system completed  
✔ Deal management system completed  
✔ Reference management system completed  
✔ Search & filtering architecture completed  
✔ Analytics integration completed  
✔ Dockerized deployment support completed  
✔ Schema export & modular architecture finalized  

---

# 👩‍💻 About This Project

This backend system was developed as part of real startup work and ongoing backend engineering practice.

The project focuses heavily on:
- backend architecture
- systems programming
- modular design
- database engineering
- transaction-safe operations
- secure authentication systems

---

# 👩‍💻 Author

Shrishti

Undergraduate student passionate about systems programming, backend architecture, and database engineering.

Focused on building structured, secure, and scalable C++ systems with strong foundations in software architecture and relational database design.