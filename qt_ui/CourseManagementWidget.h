#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "../manager/system.h"

class CourseManagementWidget : public QWidget {
    Q_OBJECT

public:
    explicit CourseManagementWidget(SystemManager& sys, QWidget* parent = nullptr);
    void refreshTable();

private slots:
    void onSearch();
    void onRefresh();
    void onAddCourse();
    void onEditCourse();
    void onDeleteCourse();

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
    void populateTable(DoubleLinkedList<Course>& list);
    void showCourseDialog(Course* existing = nullptr);
};
