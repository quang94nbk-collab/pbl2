#pragma once
// ============================================================
//  StudentWindow.h
//  Presentation Layer - Qt GUI - Cua so chinh cho Sinh vien
//  Chi giao tiep voi BLL qua SystemManager.
// ============================================================

#include <QWidget>
#include <QString>

class SystemManager;
class QTabWidget;
class TimetableWidget;
class RegistrationWidget;
class QTableWidget;
class QLineEdit;
class QPushButton;
class QComboBox;
class Course;
template <typename T> class DoubleLinkedList;

class StudentWindow : public QWidget {
    Q_OBJECT

public:
    explicit StudentWindow(SystemManager& sys,
                           const QString& studentID,
                           QWidget* parent = nullptr);

signals:
    void logout();

private slots:
    void onLogout();
    void onTabChanged(int index);
    void onCurriculumFilter();
    void onStudentCourseMyProgram();
    void onStudentCourseReset();

private:
    void setupUi();
    void setupRegisteredTab();
    void setupProgressTab();
    void setupCreditTab();
    void setupCurriculumTab();
    void refreshRegisteredTab();
    void refreshProgressTab();
    void refreshCreditTab();
    void refreshCurriculumTab();
    void populateStudentCourseTable(DoubleLinkedList<Course>& list);
    void handleCancelSection(const QString& maLopHP);

    SystemManager&      m_sys;
    QString             m_studentID;

    QTabWidget*         m_tabs;
    TimetableWidget*    m_timetable;
    RegistrationWidget* m_registration;

    // Tab "Cac lop hoc phan da dang ky"
    QWidget*            m_registeredTab;
    // Tab "Tien do hoc tap"
    QWidget*            m_progressTab;
    // Tab "Tong ket tin chi"
    QWidget*            m_creditTab;
    // Tab "Khung chuong trinh dao tao"
    QWidget*            m_curriculumTab;
    QPushButton*        m_stuCourseMyProgramBtn;
    QComboBox*          m_stuFilterKhoaCombo;
    QLineEdit*          m_stuFilterKhoaHocEdit;
    QLineEdit*          m_stuFilterNganhEdit;
    QComboBox*          m_stuFilterHocKyCombo;
    QPushButton*        m_stuCurriculumFilterBtn;
    QPushButton*        m_stuResetFilterBtn;
    QTableWidget*       m_stuCourseTable;
};
