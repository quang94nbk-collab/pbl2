#pragma once
// ============================================================
//  RegistrationWidget.h
//  Presentation Layer - Qt GUI - Widget Dang ky Tin chi
//  Dung cho ca Admin (xem tat ca) va Sinh vien (chon theo nhom lop HP)
//  Chi giao tiep voi BLL qua SystemManager.
// ============================================================

#include <QWidget>
#include <QString>
#include <vector>

#include "../manager/system.h"

class QTableWidget;
class QPushButton;
class QLabel;

class RegistrationWidget : public QWidget {
    Q_OBJECT

public:
    // studentID rong = Admin mode; co studentID = Student mode
    explicit RegistrationWidget(SystemManager& sys,
                                 const QString& studentID = QString(),
                                 QWidget* parent = nullptr);

    void refreshTable();

signals:
    void registrationChanged();

private slots:
    void onRefresh();
    void onGroupSelectionChanged();
    void onRegisterSelectedGroup();

private:
    void setupAdminUi();
    void setupStudentUi();
    void populateAdminTable();
    void populateEligibleGroups();
    void populateSectionsForGroup(int groupIndex);

    void handleRegisterSection(const QString& maLopHP);

    SystemManager& m_sys;
    QString        m_studentID;   // rong neu la admin
    bool           m_isAdmin;

    // Admin mode widgets
    QTableWidget*  m_adminTable;

    // Student mode widgets
    QLabel*        m_infoLabel;
    QLabel*        m_groupStatusLabel;
    QLabel*        m_sectionTitleLabel;
    QPushButton*   m_registerGroupBtn;
    QPushButton*   m_refreshBtn;
    QTableWidget*  m_groupTable;
    QTableWidget*  m_sectionTable;

    std::vector<GroupInfo> m_eligibleGroups;
};
