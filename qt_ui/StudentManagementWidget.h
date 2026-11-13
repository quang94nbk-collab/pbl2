#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "../manager/system.h"

class StudentManagementWidget : public QWidget {
    Q_OBJECT

public:
    explicit StudentManagementWidget(SystemManager& sys, QWidget* parent = nullptr);
    void refreshTable();

private slots:
    void onSearch();
    void onRefresh();
    void onAddStudent();
    void onEditStudent();
    void onDeleteStudent();

private:
    SystemManager& m_sys;

    QLineEdit*    m_searchEdit;
    QPushButton*  m_searchBtn;
    QPushButton*  m_refreshBtn;
    QTableWidget* m_table;
    QPushButton*  m_addBtn;
    QPushButton*  m_editBtn;
    QPushButton*  m_deleteBtn;

    void setupUi();
    void populateTable(DoubleLinkedList<Student>& list);
    void showStudentDialog(Student* existing = nullptr);
};
