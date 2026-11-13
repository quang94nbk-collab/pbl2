#pragma once

#include <QWidget>

class SystemManager;
class QTabWidget;
class QTableWidget;
class QSpinBox;
class QLabel;

// Forward declarations for management sub-widgets
// (implemented by other workers in separate files)
class StudentManagementWidget;
class CourseManagementWidget;
class SectionManagementWidget;

class AdminWindow : public QWidget {
    Q_OBJECT

public:
    explicit AdminWindow(SystemManager& system, QWidget* parent = nullptr);

signals:
    void logout();
    void semesterChanged(int semester);

private slots:
    void onLogout();
    void onTabChanged(int index);
    void onApplySemester();
    void refreshRegistrationsTab();

private:
    void setupUi();
    void setupRegistrationsTab();
    void setupSettingsTab();

    SystemManager& m_system;

    QTabWidget*               m_tabWidget;
    StudentManagementWidget*  m_studentWidget;
    CourseManagementWidget*   m_courseWidget;
    SectionManagementWidget*  m_sectionWidget;
    QTableWidget*             m_registrationsTable;
    QLabel*                   m_regTitle;
    QSpinBox*                 m_semesterSpinBox;
    QLabel*                   m_semesterBadge;
};
