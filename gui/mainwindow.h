#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <string>

#include "../manager/system.h"
#include "../manager/filemanager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    SystemManager& system;
    FileManager& fileManager;
    std::string currentStudentID;
    bool isAdmin;

    // UI Widgets
    QTabWidget* tabWidget;
    QLabel* lblUserInfo;
    QPushButton* btnSaveData;
    QPushButton* btnLogout;

    // Tab 1: CTDT
    QComboBox* cboFacultyFilter;
    QTableWidget* tableCurriculum;

    // Tab 2: Dang ky hoc phan
    QTableWidget* tableAvailableSections;
    QPushButton* btnRegister;

    // Tab 3: Lop da dang ky
    QTableWidget* tableRegisteredSections;
    QPushButton* btnCancelRegistration;

    // Tab 4: Quan tri vien (Admin)
    QWidget* tabAdmin;
    QTableWidget* tableAllStudents;
    QLineEdit* txtAddStudentID;
    QLineEdit* txtAddStudentName;
    QLineEdit* txtAddStudentPhone;
    QLineEdit* txtAddStudentEmail;
    QLineEdit* txtAddStudentClass;
    QLineEdit* txtAddStudentMajor;
    QPushButton* btnAddStudent;

    // Admin them CTDT
    QComboBox* cboAdminFaculty;
    QLineEdit* txtAdminCourseID;
    QSpinBox* spnAdminSemester;
    QCheckBox* chkAdminCompulsory;
    QPushButton* btnAddCurriculumItem;

    void setupUI();
    void loadUserInfo();
    void loadCurriculumTable();
    void loadAvailableSectionsTable();
    void loadRegisteredSectionsTable();
    void loadAdminStudentTable();

private slots:
    void onCurriculumFacultyChanged(int index);
    void onRegisterClicked();
    void onCancelRegistrationClicked();
    void onSaveDataClicked();
    void onAddStudentClicked();
    void onAddCurriculumItemClicked();

public:
    MainWindow(SystemManager& sys, FileManager& fm, const std::string& studentID,
               bool adminMode = false, QWidget* parent = nullptr);
};

#endif

