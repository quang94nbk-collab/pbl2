#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "../manager/system.h"

class SectionManagementWidget : public QWidget {
    Q_OBJECT

public:
    explicit SectionManagementWidget(SystemManager& sys, QWidget* parent = nullptr);
    void refreshTable();

private slots:
    void onSearch();
    void onRefresh();
    void onAddSection();
    void onDeleteSection();
    void onViewStudents();

private:
    SystemManager& m_sys;

    QLineEdit*    m_searchEdit;
    QPushButton*  m_searchBtn;
    QPushButton*  m_refreshBtn;
    QTableWidget* m_table;
    QPushButton*  m_addBtn;
    QPushButton*  m_deleteBtn;
    QPushButton*  m_viewStudentsBtn;

    void setupUi();
    void populateTable(DoubleLinkedList<ClassSection>& list);
};
