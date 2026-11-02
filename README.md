# Student Record Management System (SRMS) — Academic Analytics & Ledger

**Portfolio Project** is a robust C++17 console application designed for student record management, academic performance tracking, and statistical analytics. Developed with the assistance of AI tools, the application features rigorous input validation, multi-criteria sorting, data persistence via CSV, and a linear regression model to predict exam scores based on study hours.

---

## 🚀 Key Features

* **Student Lifecycle Management:** Register, view, search, update, and delete student records with strict 6-digit ID validation and duplicate protection.
* **Automated Grading:** Instantly calculates letter grades (`A+`, `A`, `B`, `C`, `D`, `F`) based on exam marks.
* **Predictive Analytics (Linear Regression):** Computes linear regression equations ($Y = a + bX$) and Pearson correlation coefficients ($r$) to estimate exam performance relative to daily study hours.
* **Class Dashboard & Ranking:** Generates quick class summaries (average, highest, lowest marks, pass/fail counts) and ranks students by academic performance.
* **Flexible Sorting:** Sort records dynamically by Name, Marks, or Semester without altering the saved file sequence.
* **Data Persistence & Reporting:** Automatically saves and loads records via `students.csv` (complete with header rows) and exports comprehensive text summary reports to `student_report.txt`.

---

## 🛠️ Technical Specifications

* **Language Standard:** C++17
* **Data Structures:** Uses STL vectors (`std::vector`), structures (`struct Student`), and string streams (`std::stringstream`) for parsing CSV rows.
* **Robust Input Validation:** Implements custom safe-reading functions (`readInt`, `readDouble`, `readId`) with stream clearing and range checking to prevent infinite loops on invalid input.
* **File I/O:** Standard library file streams (`std::ifstream` and `std::ofstream`) with error handling for persistent local storage.

---

## 📂 Project Structure

```text
StudentRecordSystem/
│
├── project_2.cpp         # Main source file containing application logic, UI, and algorithms
├── students.csv          # Local persistent data storage with header rows
├── student_report.txt    # Exported text summary report
└── README.md             # Project documentation

⚙️ Compilation & Build Instructions
You can compile and run the project using GCC/G++ inside an approved C++17 development environment (such as VS Code with MSYS2 on Windows).

1. Compile the Source Code
Open your terminal in the project directory and run:

Bash
g++ project_2.cpp -o student_system
2. Run the Executable
On Windows (PowerShell / Command Prompt):

Bash
.\student_system.exe
On macOS / Linux:

Bash
./student_system
🎮 Interactive Menu Options
Upon launching the program, you will be presented with a numbered console menu (0–10):

Register student — Add a new student record (requires a unique 6-digit ID).

View all students — Display all records formatted in a structured tabular view.

Search student — Look up a specific student profile using their 6-digit ID.

Update student — Modify existing student details and recalculate metrics.

Delete student — Remove a student record from the system with confirmation.

Linear regression and mark prediction — Analyze the correlation between study hours and exam scores, and forecast potential grades.

Student ranking — View a sorted leaderboard of students from highest to lowest marks.

Class dashboard — Display aggregate class metrics (class average, highest/lowest scores, and pass/fail distribution).

Sort student records — Temporarily sort and view records by Name, Marks, or Semester.

Generate report — Export all records and summary statistics to student_report.txt.

Save and exit — Flushes memory changes to students.csv, terminates execution, and exits cleanly.

Verifying Exit Status
After exiting the program via option 0, you can verify that the program terminated with a successful exit code: