# 🍽️ Restaurant Orders Manager System

![C Language](https://img.shields.io/badge/Language-C99-blue?style=for-the-badge&logo=c)
![Build](https://img.shields.io/badge/Build-Makefile-orange?style=for-the-badge)
![Memory Sanity](https://img.shields.io/badge/Memory-Valgrind_Clean-green?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Completed-success?style=for-the-badge)

---

## 📖 Overview
This project is a high-performance **Restaurant Order Management System** written in pure C. It efficiently manages real-time customer table orders, kitchen workflows, dish state transitions, and financial billing calculations using optimized, nested data structures.

The system is designed with a strict modular programming paradigm to ensure maintainability, scalability, and memory safety.

---

## 🏗️ System Architecture & Data Structures
The core engine relies on a **nested singly linked list** architecture to represent the relationship between tables, orders, and dishes dynamically without wasting system heap memory.



* **Master List (`Order`):** Tracks current active and archived table sessions. Each order node stores transaction metadata (time, total price, status) and encapsulates a head pointer to its respective dish sub-list.
* **Sub-List (`Dish`):** Tracks the detailed granular state of each ordered portion inside a specific order session.

```txt
OrdersManager/
├── src/                # Implementation source files (.c)
├── include/            # Header files containing structures & prototypes (.h)
├── data/               # Persistent text database for end-of-day archiving
├── tests/              # Automated unit tests for data structures
└── Makefile            # Automated compilation script
```

## 💻 Core Features
- Dynamic Order Tracking: Create and append dishes to table orders chronologically (Tail Insertion).
- Kitchen State Engine: Real-time state transitions for dishes (MOI_TAO ➡️ DANG_LAM ➡️ DA_LAM_XONG ➡️ DA_TRA / BI_HUY).
- Strict Business Logic Verification: * Orders can only be cancelled if the kitchen has not started processing any associated dishes.
- Individual dishes can be cancelled mid-process with descriptive notes injected directly into the structure.
- Accurate Bill Aggregation: Traverses the active table structures upon checkout to compute final bills based strictly on kitchen-fulfilled and served counts.
- File Persistence: Automatic data serializing and parsing from persistent text files for archiving purposes.

## 🛠️ Compilation & Installation
This project uses Makefile to automate the compilation process, handle dependency tracking, and compile object files incrementally for optimized speed.

### Prerequisites
Ensure you have a C compiler (gcc) and make installed on your system.

### Build Instructions
Clone the repository and compile the system using the following automated hooks:

```bash
# 1. Clone the repository
git clone [https://github.com/kiendt207/OrdersManager-C.git](https://github.com/kiendt207/OrdersManager-C.git)
cd OrdersManager-C

# 2. Build the entire project (Compiles modules and links binaries)
make all

# 3. Launch the interactive Order Management CLI
./orders_manager
```

### Development and Automation Controls

```bash
# Run automated suite testing to validate structural logic integrity
make test

# Memory profiling (Detect and isolate memory leaks)
valgrind --leak-check=full --track-origins=yes ./orders_manager

# Clean temporary build artifacts and object files (.o)
make clean
```