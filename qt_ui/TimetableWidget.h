#pragma once
// ============================================================
//  TimetableWidget.h
//  Presentation Layer - Qt GUI - Thoi Khoa Bieu sinh vien
//  Chi giao tiep voi BLL qua SystemManager.
// ============================================================

#include <QWidget>
#include <QString>

class SystemManager;
class QTableWidget;
class QPushButton;
class QLabel;

class TimetableWidget : public QWidget {
    Q_OBJECT

public:
    explicit TimetableWidget(SystemManager& sys,
                              const QString& studentID,
                              QWidget* parent = nullptr);

    void refresh();

private slots:
    void onRefresh();

private:
    void setupUi();
    void populateTimetable();

    SystemManager& m_sys;
    QString        m_studentID;

    QTableWidget*  m_grid;
    QPushButton*   m_refreshBtn;
    QLabel*        m_infoLabel;
};
