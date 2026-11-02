#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <limits>
using namespace std;

struct Student {
    string id, name, department;
    int semester = 1;
    double marks = 0.0;
    double studyHours = -1.0;
};

const string DATA_FILE = "students.csv";
const string REPORT_FILE = "student_report.txt";

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt(const string& prompt, int minValue, int maxValue) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minValue && value <= maxValue) {
            clearInput();
            return value;
        }
        cout << "Please enter a number from " << minValue << " to "
             << maxValue << ".\n";
        clearInput();
    }
}

double readDouble(const string& prompt, double minValue, double maxValue) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && isfinite(value) &&
            value >= minValue && value <= maxValue) {
            clearInput();
            return value;
        }
        cout << "Please enter a value from " << minValue << " to "
             << maxValue << ".\n";
        clearInput();
    }
}

string readText(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

bool validId(const string& id) {
    return id.size() == 6 &&
           all_of(id.begin(), id.end(), [](unsigned char c) {
               return c >= '0' && c <= '9';
           });
}

string readId(const string& prompt) {
    while (true) {
        string id = readText(prompt);
        if (validId(id)) return id;
        cout << "ID must contain exactly 6 digits.\n";
    }
}

int findById(const vector<Student>& students, const string& id) {
    for (int i = 0; i < (int)students.size(); ++i)
        if (students[i].id == id) return i;
    return -1;
}

string gradeFor(double marks) {
    if (marks >= 90) return "A+";
    if (marks >= 80) return "A";
    if (marks >= 70) return "B";
    if (marks >= 60) return "C";
    if (marks >= 50) return "D";
    return "F";
}

void saveData(const vector<Student>& students) {
    ofstream file(DATA_FILE);
    if (!file) {
        cout << "Could not save the file.\n";
        return;
    }

    // Write the header line first
    file << "ID,Name,Department,Semester,Marks,Study Hours\n";

    file << fixed << setprecision(2);
    for (const auto& s : students)
        file << s.id << ',' << s.name << ',' << s.department << ','
             << s.semester << ',' << s.marks << ',' << s.studyHours << '\n';
}

void loadData(vector<Student>& students) {
    ifstream file(DATA_FILE);
    string line;
    bool isFirstLine = true;
    
    while (getline(file, line)) {
        if (isFirstLine) {
            isFirstLine = false;
            continue; // Skip the header line ("ID,Name,...")
        }

        stringstream ss(line);
        Student s;
        string field;
        try {
            if (!getline(ss, s.id, ',') || !validId(s.id)) continue;
            if (!getline(ss, s.name, ',')) continue;
            if (!getline(ss, s.department, ',')) continue;
            if (!getline(ss, field, ',')) continue;
            s.semester = stoi(field);
            if (!getline(ss, field, ',')) continue;
            s.marks = stod(field);
            if (!getline(ss, field, ',')) continue;
            s.studyHours = stod(field);
            if (s.semester < 1 || s.semester > 8 ||
                s.marks < 0 || s.marks > 100 ||
                !isfinite(s.studyHours) || s.studyHours < -1 ||
                s.studyHours > 24) continue;
            if (findById(students, s.id) == -1) students.push_back(s);
        } catch (...) {
            // Skip incomplete or invalid rows.
        }
    }
}

void showStudent(const Student& s) {
    cout << "\nStudent ID: " << s.id
         << "\nName: " << s.name
         << "\nDepartment: " << s.department
         << "\nSemester: " << s.semester
         << "\nExam marks: " << s.marks << "/100"
         << "\nGrade: " << gradeFor(s.marks)
         << "\nStudy hours: ";
    if (s.studyHours < 0) cout << "Not entered\n";
    else cout << s.studyHours << '\n';
}

void printTable(const vector<Student>& students) {
    if (students.empty()) {
        cout << "No student records found.\n";
        return;
    }
    cout << left << setw(8) << "ID" << setw(21) << "Name"
         << setw(21) << "Department" << setw(10) << "Semester"
         << setw(8) << "Marks" << setw(7) << "Grade"
         << "Study Hours\n";
    cout << string(120, '-') << '\n';
    for (const auto& s : students) {
        cout << left << setw(8) << s.id
             << setw(21) << s.name.substr(0, 20)
             << setw(21) << s.department.substr(0, 20)
             << setw(10) << s.semester << setw(8) << fixed
             << setprecision(1) << s.marks << setw(7) << gradeFor(s.marks);
        if (s.studyHours < 0) cout << "Not entered\n";
        else cout << s.studyHours << '\n';
    }
}

void addStudent(vector<Student>& students) {
    Student s;
    s.id = readId("Enter 6-digit student ID: ");
    if (findById(students, s.id) != -1) {
        cout << "This ID already exists.\n";
        return;
    }
    s.name = readText("Enter student name: ");
    s.department = readText("Enter department: ");
    if (s.name.empty() || s.department.empty()) {
        cout << "Name and department cannot be empty.\n";
        return;
    }
    s.semester = readInt("Enter semester (1-8): ", 1, 8);
    s.marks = readDouble("Enter exam marks (0-100): ", 0, 100);
    s.studyHours = readDouble("Enter daily study hours (0-24): ", 0, 24);
    students.push_back(s);
    saveData(students);
    cout << "Student added successfully.\n";
}

void viewStudents(const vector<Student>& students) {
    cout << "\n=== ALL STUDENT RECORDS ===\n";
    printTable(students);
}

void searchStudent(const vector<Student>& students) {
    string id = readId("Enter student ID to search: ");
    int index = findById(students, id);
    if (index == -1) cout << "Student not found.\n";
    else showStudent(students[index]);
}

void updateStudent(vector<Student>& students) {
    string id = readId("Enter student ID to update: ");
    int index = findById(students, id);
    if (index == -1) {
        cout << "Student not found.\n";
        return;
    }
    Student& s = students[index];
    string newName = readText("Enter new name: ");
    string newDepartment = readText("Enter new department: ");
    if (newName.empty() || newDepartment.empty()) {
        cout << "Name and department cannot be empty. Update cancelled.\n";
        return;
    }
    s.name = newName;
    s.department = newDepartment;
    s.semester = readInt("Enter new semester (1-8): ", 1, 8);
    s.marks = readDouble("Enter new exam marks (0-100): ", 0, 100);
    s.studyHours = readDouble("Enter daily study hours (0-24): ", 0, 24);
    saveData(students);
    cout << "Student updated successfully.\n";
}

void deleteStudent(vector<Student>& students) {
    string id = readId("Enter student ID to delete: ");
    int index = findById(students, id);
    if (index == -1) {
        cout << "Student not found.\n";
        return;
    }
    string confirm = readText("Type YES to confirm deletion: ");
    if (confirm == "YES") {
        students.erase(students.begin() + index);
        saveData(students);
        cout << "Student deleted.\n";
    } else {
        cout << "Deletion cancelled.\n";
    }
}

void regression(const vector<Student>& students) {
    vector<const Student*> data;
    for (const auto& s : students)
        if (s.studyHours >= 0) data.push_back(&s);

    if (data.size() < 2) {
        cout << "Add at least two students with study hours first.\n";
        return;
    }

    double sx = 0, sy = 0, sxy = 0, sx2 = 0, sy2 = 0;
    for (auto s : data) {
        double x = s->studyHours, y = s->marks;
        sx += x; sy += y; sxy += x * y; sx2 += x * x; sy2 += y * y;
    }
    double n = static_cast<double>(data.size());
    double denominator = n * sx2 - sx * sx;
    if (fabs(denominator) < 1e-10) {
        cout << "Study hours must be different to calculate regression.\n";
        return;
    }

    double slope = (n * sxy - sx * sy) / denominator;
    double intercept = (sy - slope * sx) / n;
    double corrDenom = sqrt((n * sx2 - sx * sx) * (n * sy2 - sy * sy));
    double r = corrDenom > 1e-10 ? (n * sxy - sx * sy) / corrDenom : 0;

    cout << fixed << setprecision(3);
    cout << "\nLinear regression equation: Y = " << intercept
         << (slope >= 0 ? " + " : " - ") << fabs(slope) << "X\n";
    cout << "Pearson correlation (r): " << r << '\n';
    double hours = readDouble("Enter study hours for prediction (0-24): ", 0, 24);
    double prediction = intercept + slope * hours;
    prediction = max(0.0, min(100.0, prediction));
    cout << "Estimated exam marks: " << prediction << "/100\n";
    cout << "This is an estimate, not a guaranteed result.\n";
}

// Task 7: Rank students by marks, from highest to lowest.
void rankStudents(const vector<Student>& students) {
    if (students.empty()) {
        cout << "No student records to rank.\n";
        return;
    }
    vector<Student> ranked = students;
    stable_sort(ranked.begin(), ranked.end(), [](const Student& a, const Student& b) {
        return a.marks > b.marks;
    });
    cout << "\n=== STUDENT RANKING ===\n";
    cout << left << setw(8) << "Rank" << setw(8) << "ID"
         << setw(24) << "Name" << setw(10) << "Marks" << "Grade\n";
    cout << string(58, '-') << '\n';
    for (size_t i = 0; i < ranked.size(); ++i) {
        cout << left << setw(8) << i + 1 << setw(8) << ranked[i].id
             << setw(24) << ranked[i].name.substr(0, 23)
             << setw(10) << fixed << setprecision(1) << ranked[i].marks
             << gradeFor(ranked[i].marks) << '\n';
    }
}

// Task 8: Display a quick class summary.
void classDashboard(const vector<Student>& students) {
    if (students.empty()) {
        cout << "No student records available for the dashboard.\n";
        return;
    }
    double total = 0, highest = students[0].marks, lowest = students[0].marks;
    int pass = 0, fail = 0;
    for (const auto& s : students) {
        total += s.marks;
        highest = max(highest, s.marks);
        lowest = min(lowest, s.marks);
        if (s.marks >= 50) ++pass;
        else ++fail;
    }
    cout << fixed << setprecision(2);
    cout << "\n=== CLASS DASHBOARD ===\n"
         << "Total students : " << students.size() << '\n'
         << "Average marks  : " << total / students.size() << "/100\n"
         << "Highest marks  : " << highest << "/100\n"
         << "Lowest marks   : " << lowest << "/100\n"
         << "Passed (50+)   : " << pass << '\n'
         << "Failed (<50)   : " << fail << '\n';
}

// Task 9: Sort and display a copy, leaving saved record order unchanged.
void sortRecords(const vector<Student>& students) {
    if (students.empty()) {
        cout << "No student records to sort.\n";
        return;
    }
    cout << "\nSort by:\n1. Name (A-Z)\n2. Marks (high to low)\n3. Semester (low to high)\n";
    int option = readInt("Choose sorting method: ", 1, 3);
    vector<Student> sorted = students;
    if (option == 1) {
        stable_sort(sorted.begin(), sorted.end(), [](const Student& a, const Student& b) {
            return a.name < b.name;
        });
    } else if (option == 2) {
        stable_sort(sorted.begin(), sorted.end(), [](const Student& a, const Student& b) {
            return a.marks > b.marks;
        });
    } else {
        stable_sort(sorted.begin(), sorted.end(), [](const Student& a, const Student& b) {
            return a.semester < b.semester;
        });
    }
    cout << "\n=== SORTED STUDENT RECORDS ===\n";
    printTable(sorted);
}

// Task 10: Export all records and a summary to a text report.
void generateReport(const vector<Student>& students) {
    if (students.empty()) {
        cout << "No student records available for a report.\n";
        return;
    }
    ofstream report(REPORT_FILE);
    if (!report) {
        cout << "Could not create the report file.\n";
        return;
    }
    double total = 0, highest = students[0].marks, lowest = students[0].marks;
    int pass = 0, fail = 0;
    for (const auto& s : students) {
        total += s.marks;
        highest = max(highest, s.marks);
        lowest = min(lowest, s.marks);
        if (s.marks >= 50) ++pass;
        else ++fail;
    }

    report << "STUDENT RECORD MANAGEMENT SYSTEM - REPORT\n";
    report << "==========================================\n";
    report << "Total students: " << students.size() << '\n';
    report << fixed << setprecision(2)
           << "Average marks: " << total / students.size() << "/100\n"
           << "Highest marks: " << highest << "/100\n"
           << "Lowest marks: " << lowest << "/100\n"
           << "Passed (50+): " << pass << '\n'
           << "Failed (<50): " << fail << "\n\n";
    report << left << setw(8) << "ID" << setw(24) << "Name"
           << setw(44) << "Department" << setw(10) << "Semester"
           << setw(10) << "Marks" << setw(8) << "Grade"
           << "Study Hours\n";
    report << string(120, '-') << '\n';
    for (const auto& s : students) {
        report << left << setw(8) << s.id << setw(24) << s.name.substr(0, 23)
               << setw(44) << s.department.substr(0, 43)
               << setw(10) << s.semester << setw(10) << s.marks
               << setw(8) << gradeFor(s.marks);
        if (s.studyHours < 0) report << "Not entered\n";
        else report << s.studyHours << '\n';
    }
    report.close();
    cout << "Report generated successfully: " << REPORT_FILE << '\n';
}

int main() {
    vector<Student> students;
    loadData(students);
    int choice;
    do {
        cout << "\n=== STUDENT RECORD MANAGEMENT SYSTEM ===\n"
             << "1. Register student\n"
             << "2. View all students\n"
             << "3. Search student\n"
             << "4. Update student\n"
             << "5. Delete student\n"
             << "6. Linear regression and mark prediction\n"
             << "7. Student ranking\n"
             << "8. Class dashboard\n"
             << "9. Sort student records\n"
             << "10. Generate report\n"
             << "0. Save and exit\n";
        choice = readInt("Enter choice: ", 0, 10);
        switch (choice) {
            case 1: addStudent(students); break;
            case 2: viewStudents(students); break;
            case 3: searchStudent(students); break;
            case 4: updateStudent(students); break;
            case 5: deleteStudent(students); break;
            case 6: regression(students); break;
            case 7: rankStudents(students); break;
            case 8: classDashboard(students); break;
            case 9: sortRecords(students); break;
            case 10: generateReport(students); break;
            case 0: saveData(students); cout << "Data saved. Goodbye!\n"; break;
        }
    } while (choice != 0);
    return 0;
}