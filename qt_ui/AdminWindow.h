#pragma once

#include <QWidget>

class SystemManager;
class QTabWidget;
class QTableWidget;
class QSpinBox;
class QLabel;
class QComboBox;
class QLineEdit;
class QPushButton;

// Forward declarations for management sub-widgets
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

    // Curriculum tab slots
    void onCurriculumFilter();
    void onCurriculumReset();

private:
    void setupUi();
    void setupCurriculumTab();
    void setupRegistrationsTab();
    void setupSettingsTab();

    SystemManager& m_system;

    QTabWidget*               m_tabWidget;
    StudentManagementWidget*  m_studentWidget;
    CourseManagementWidget*   m_courseWidget;
    SectionManagementWidget*  m_sectionWidget;

    // Tab Khung chuong trinh dao tao
    QWidget*                  m_curriculumTab;
    QComboBox*                m_currKhoaCombo;
    QLineEdit*                m_currKhoaHocEdit;
    QLineEdit*                m_currNganhEdit;
    QComboBox*                m_currHocKyCombo;
    QPushButton*              m_currFilterBtn;
    QPushButton*              m_currResetBtn;
    QTableWidget*             m_curriculumTable;

    // Tab Dang ky
    QWidget*                  m_registrationsTab;
    QTableWidget*             m_registrationsTable;
    QLabel*                   m_regTitle;

    // Tab Cai dat
    QWidget*                  m_settingsTab;
    QSpinBox*                 m_semesterSpinBox;
    QLabel*                   m_semesterBadge;
};
