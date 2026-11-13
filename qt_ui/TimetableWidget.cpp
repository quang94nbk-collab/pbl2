// ============================================================
//  TimetableWidget.cpp
//  Presentation Layer - Qt GUI - Thoi Khoa Bieu (luoi gio-ngay)
//  Chi giao tiep voi BLL qua SystemManager.
//  KHONG co Qt code trong manager/, model/, dal/, DSA/.
// ============================================================

#include "TimetableWidget.h"

#include "../manager/system.h"
#include "../model/dtos.h"
#include "../model/student.h"
#include "../DSA/DoublyLinkedList.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFont>
#include <QColor>
#include <QBrush>
#include <map>
#include <string>

// Mapping ten thu -> chi so cot (0-based)
static int thuToCol(const std::string& thu)
{
    if (thu == "2" || thu == "Thu 2" || thu == "Thứ 2") return 0;
    if (thu == "3" || thu == "Thu 3" || thu == "Thứ 3") return 1;
    if (thu == "4" || thu == "Thu 4" || thu == "Thứ 4") return 2;
    if (thu == "5" || thu == "Thu 5" || thu == "Thứ 5") return 3;
    if (thu == "6" || thu == "Thu 6" || thu == "Thứ 6") return 4;
    if (thu == "7" || thu == "Thu 7" || thu == "Thứ 7") return 5;
    if (thu == "CN" || thu == "cn" || thu == "Chủ nhật") return 6;
    return -1;
}

// ---- Constructor ----
TimetableWidget::TimetableWidget(SystemManager& sys,
                                  const QString& studentID,
                                  QWidget* parent)
    : QWidget(parent)
    , m_sys(sys)
    , m_studentID(studentID)
    , m_grid(nullptr)
    , m_refreshBtn(nullptr)
    , m_infoLabel(nullptr)
{
    setupUi();
    populateTimetable();

    setStyleSheet(R"(
        TimetableWidget {
            background-color: #FFFFFF;
        }
        QTableWidget {
            background-color: #FFFFFF;
            color: #1E293B;
            gridline-color: #CBD5E1;
            font-size: 11px;
            selection-background-color: #DBEAFE;
            border: 1px solid #CBD5E1;
            border-radius: 6px;
        }
        QHeaderView::section {
            background-color: #2563EB;
            color: #FFFFFF;
            padding: 6px 4px;
            font-size: 12px;
            font-weight: bold;
            border: none;
            border-right: 1px solid #3B82F6;
            border-bottom: 1px solid #3B82F6;
        }
        QTableWidget::item {
            padding: 3px;
        }
        QPushButton {
            background-color: #2563EB;
            color: #FFFFFF;
            border: none;
            border-radius: 6px;
            padding: 6px 14px;
            font-size: 13px;
            font-weight: 600;
        }
        QPushButton:hover { background-color: #3B82F6; }
    )");
}

void TimetableWidget::setupUi()
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    // -- Header --
    auto* hdrRow = new QHBoxLayout;
    m_infoLabel = new QLabel;
    m_infoLabel->setStyleSheet("font-size:13px; color:#1E40AF; font-weight:bold; background:transparent;");
    m_refreshBtn = new QPushButton("Làm mới");
    m_refreshBtn->setCursor(Qt::PointingHandCursor);
    connect(m_refreshBtn, &QPushButton::clicked, this, &TimetableWidget::onRefresh);
    hdrRow->addWidget(m_infoLabel);
    hdrRow->addStretch();
    hdrRow->addWidget(m_refreshBtn);
    layout->addLayout(hdrRow);

    // -- Luoi thoi khoa bieu --
    const int TIET_COUNT = 15;
    const int DAY_COUNT  = 7;

    m_grid = new QTableWidget(TIET_COUNT, DAY_COUNT);
    m_grid->setHorizontalHeaderLabels({
        "Thứ 2", "Thứ 3", "Thứ 4", "Thứ 5", "Thứ 6", "Thứ 7", "Chủ nhật"
    });

    for (int t = 0; t < TIET_COUNT; ++t) {
        m_grid->setVerticalHeaderItem(t,
            new QTableWidgetItem(QString("Tiết %1").arg(t + 1)));
    }

    m_grid->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_grid->setSelectionMode(QAbstractItemView::NoSelection);
    m_grid->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_grid->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_grid->verticalHeader()->setMinimumWidth(68);
    m_grid->setShowGrid(true);

    layout->addWidget(m_grid, 1);

    // -- Chu thich --
    auto* legend = new QLabel(
        "<span style='background:#DBEAFE; border:1px solid #93C5FD; padding:2px 8px;'>&nbsp;&nbsp;&nbsp;</span>"
        " Lớp học phần đã đăng ký");
    legend->setStyleSheet("font-size:12px; color:#475569; background:transparent;");
    layout->addWidget(legend, 0, Qt::AlignRight);
}

void TimetableWidget::populateTimetable()
{
    if (!m_grid) return;

    for (int r = 0; r < m_grid->rowCount(); ++r) {
        for (int c = 0; c < m_grid->columnCount(); ++c) {
            m_grid->setSpan(r, c, 1, 1);
            m_grid->setItem(r, c, nullptr);
        }
    }

    const Student* sv = m_sys.findStudent(m_studentID.toStdString());
    if (sv && m_infoLabel) {
        m_infoLabel->setText(
            QString("Thời khóa biểu - %1 (%2)  |  Học kỳ %3")
                .arg(QString::fromStdString(sv->getHoTen()))
                .arg(m_studentID)
                .arg(m_sys.getCurrentSemester())
        );
    }

    DoubleLinkedList<TimetableEntry> entries;
    m_sys.getStudentTimetable(m_studentID.toStdString(), entries);

    // Bang mau pastel diu mat
    static const QColor COLORS[] = {
        QColor("#DBEAFE"),  // xanh duong nhat
        QColor("#DCFCE7"),  // xanh la nhat
        QColor("#FEF3C7"),  // vang kem
        QColor("#F3E8FF"),  // tim nhat
        QColor("#FFEDD5"),  // cam nhat
        QColor("#CCFBF1"),  // ngoc nhat
    };
    std::map<std::string, int> colorMap;
    int colorIdx = 0;

    auto* node = entries.getHead();
    while (node) {
        const TimetableEntry& e = node->data;
        int col = thuToCol(e.thu);
        int rowStart = e.tietBatDau - 1;
        int rowEnd   = e.tietKetThuc - 1;

        if (col < 0 || rowStart < 0 || rowEnd < rowStart
            || rowStart >= m_grid->rowCount() || rowEnd >= m_grid->rowCount()) {
            node = node->next;
            continue;
        }

        const std::string& maLop = e.maLopHP;
        if (colorMap.find(maLop) == colorMap.end())
            colorMap[maLop] = colorIdx++ % 6;
        QColor bg = COLORS[colorMap[maLop]];

        QString cellText = QString("%1\n%2\nPhòng: %3\nGV: %4")
            .arg(QString::fromStdString(e.maLopHP))
            .arg(QString::fromStdString(e.tenMon))
            .arg(QString::fromStdString(e.phong))
            .arg(QString::fromStdString(e.giangVien));
        auto* item = new QTableWidgetItem(cellText);
        item->setBackground(QBrush(bg));
        item->setForeground(QBrush(QColor("#1E293B")));
        item->setTextAlignment(Qt::AlignCenter);
        QFont f = item->font();
        f.setPointSize(9);
        f.setBold(true);
        item->setFont(f);

        m_grid->setItem(rowStart, col, item);

        int span = rowEnd - rowStart + 1;
        if (span > 1)
            m_grid->setSpan(rowStart, col, span, 1);

        node = node->next;
    }
}

void TimetableWidget::refresh()
{
    populateTimetable();
}

void TimetableWidget::onRefresh()
{
    populateTimetable();
}
