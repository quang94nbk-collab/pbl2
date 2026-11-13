// ============================================================
//  SectionManagementWidget.cpp
//  Presentation Layer - Qt GUI - Quan ly Lop Hoc Phan (Admin)
//  Chi giao tiep voi BLL qua SystemManager.
//  KHONG co Qt code trong manager/, model/, dal/, DSA/.
// ============================================================

#include "SectionManagementWidget.h"

#include "../manager/exception.h"
#include "../model/classsection.h"
#include "../model/dtos.h"
#include "../DSA/DoublyLinkedList.h"

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QSpinBox>
#include <QTableWidgetItem>

// ---- Constructor ----
SectionManagementWidget::SectionManagementWidget(SystemManager& sys, QWidget* parent)
    : QWidget(parent), m_sys(sys)
{
    setupUi();
    refreshTable();

    setStyleSheet(R"(
        SectionManagementWidget {
            background-color: #FFFFFF;
        }
        QTableWidget {
            background-color: #FFFFFF;
            color: #1E293B;
            gridline-color: #E2E8F0;
            font-size: 12px;
            selection-background-color: #DBEAFE;
            selection-color: #1E3A8A;
            alternate-background-color: #F8FAFC;
            border: 1px solid #CBD5E1;
            border-radius: 6px;
        }
        QHeaderView::section {
            background-color: #2563EB;
            color: #FFFFFF;
            padding: 7px 8px;
            font-size: 12px;
            font-weight: bold;
            border: none;
            border-right: 1px solid #3B82F6;
        }
        QPushButton {
            background-color: #2563EB;
            color: #FFFFFF;
            border: none;
            border-radius: 6px;
            padding: 7px 16px;
            font-size: 13px;
            font-weight: 600;
        }
        QPushButton:hover { background-color: #3B82F6; }
        QPushButton:pressed { background-color: #1D4ED8; }
        QLineEdit {
            background-color: #FFFFFF;
            color: #1E293B;
            border: 1px solid #CBD5E1;
            border-radius: 6px;
            padding: 6px 10px;
            font-size: 13px;
        }
        QLineEdit:focus { border: 1.5px solid #2563EB; }
    )");
}

void SectionManagementWidget::setupUi()
{
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(10);

    // -- Thanh tim kiem --
    auto* searchLayout = new QHBoxLayout;
    m_searchEdit = new QLineEdit;
    m_searchEdit->setPlaceholderText("Tìm kiếm theo mã lớp HP, mã môn, nhóm, giảng viên...");
    m_searchBtn  = new QPushButton("Tìm kiếm");
    m_refreshBtn = new QPushButton("Làm mới");
    searchLayout->addWidget(m_searchEdit, 1);
    searchLayout->addWidget(m_searchBtn);
    searchLayout->addWidget(m_refreshBtn);
    mainLayout->addLayout(searchLayout);

    // -- Table --
    m_table = new QTableWidget;
    m_table->setColumnCount(11);
    m_table->setHorizontalHeaderLabels({
        "Mã lớp HP", "Mã môn", "Giảng viên", "Thứ",
        "Tiết BĐ", "Tiết KT", "Phòng",
        "Sĩ số TĐ", "Đã ĐK", "Học kỳ", "Nhóm"
    });
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setAlternatingRowColors(true);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->setVisible(false);
    mainLayout->addWidget(m_table, 1);

    // -- Nut hanh dong --
    auto* btnLayout = new QHBoxLayout;
    m_addBtn          = new QPushButton("Thêm lớp HP");
    m_deleteBtn       = new QPushButton("Xóa lớp HP");
    m_viewStudentsBtn = new QPushButton("Xem DS sinh viên");
    m_deleteBtn->setStyleSheet("QPushButton{background:#EF4444;} QPushButton:hover{background:#DC2626;}");
    m_viewStudentsBtn->setStyleSheet("QPushButton{background:#16A34A;} QPushButton:hover{background:#15803D;}");
    btnLayout->addWidget(m_addBtn);
    btnLayout->addWidget(m_deleteBtn);
    btnLayout->addWidget(m_viewStudentsBtn);
    btnLayout->addStretch();
    mainLayout->addLayout(btnLayout);

    // -- Ket noi signal/slot --
    connect(m_searchBtn,       &QPushButton::clicked, this, &SectionManagementWidget::onSearch);
    connect(m_refreshBtn,      &QPushButton::clicked, this, &SectionManagementWidget::onRefresh);
    connect(m_addBtn,          &QPushButton::clicked, this, &SectionManagementWidget::onAddSection);
    connect(m_deleteBtn,       &QPushButton::clicked, this, &SectionManagementWidget::onDeleteSection);
    connect(m_viewStudentsBtn, &QPushButton::clicked, this, &SectionManagementWidget::onViewStudents);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &SectionManagementWidget::onSearch);
}

void SectionManagementWidget::populateTable(DoubleLinkedList<ClassSection>& list)
{
    m_table->setRowCount(0);
    auto* node = list.getHead();
    while (node) {
        const ClassSection& sec = node->data;
        int row = m_table->rowCount();
        m_table->insertRow(row);

        auto setItem = [&](int col, const QString& text) {
            auto* item = new QTableWidgetItem(text);
            item->setTextAlignment(Qt::AlignCenter);
            m_table->setItem(row, col, item);
        };

        QString thuDisplay = QString::fromStdString(sec.getThu());
        if (thuDisplay != "CN" && !thuDisplay.startsWith("Th")) {
            thuDisplay = "Thứ " + thuDisplay;
        }

        setItem(0,  QString::fromStdString(sec.getMaLopHP()));
        setItem(1,  QString::fromStdString(sec.getMaMon()));
        setItem(2,  QString::fromStdString(sec.getGiangVien()));
        setItem(3,  thuDisplay);
        setItem(4,  QString::number(sec.getTietBatDau()));
        setItem(5,  QString::number(sec.getTietKetThuc()));
        setItem(6,  QString::fromStdString(sec.getPhong()));
        setItem(7,  QString::number(sec.getSiSoToiDa()));
        setItem(8,  QString::number(sec.getSoLuongDaDangKy()));
        setItem(9,  QString("Kỳ %1").arg(sec.getHocKy()));
        setItem(10, QString::fromStdString(sec.getNhomLopHP()));

        if (!sec.conCho()) {
            for (int c = 0; c < m_table->columnCount(); ++c) {
                if (m_table->item(row, c))
                    m_table->item(row, c)->setBackground(QColor("#FEE2E2"));
            }
        }

        node = node->next;
    }
    m_table->resizeColumnsToContents();
}

void SectionManagementWidget::refreshTable()
{
    DoubleLinkedList<ClassSection> list;
    m_sys.getAllSections(list);
    populateTable(list);
}

void SectionManagementWidget::onSearch()
{
    QString kw = m_searchEdit->text().trimmed();
    if (kw.isEmpty()) { refreshTable(); return; }
    DoubleLinkedList<ClassSection> list;
    m_sys.searchSections(kw.toStdString(), list);
    populateTable(list);
}

void SectionManagementWidget::onRefresh()
{
    m_searchEdit->clear();
    refreshTable();
}

void SectionManagementWidget::onAddSection()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Thêm lớp học phần mới");
    dlg.setMinimumWidth(420);

    auto* form = new QFormLayout(&dlg);
    form->setLabelAlignment(Qt::AlignRight);

    auto* maLopEdit  = new QLineEdit;
    auto* maMonEdit  = new QLineEdit;
    auto* gvEdit     = new QLineEdit;
    auto* thuCombo   = new QComboBox;
    thuCombo->addItem("Thứ 2", "2");
    thuCombo->addItem("Thứ 3", "3");
    thuCombo->addItem("Thứ 4", "4");
    thuCombo->addItem("Thứ 5", "5");
    thuCombo->addItem("Thứ 6", "6");
    thuCombo->addItem("Thứ 7", "7");
    thuCombo->addItem("Chủ nhật", "CN");

    auto* phongEdit  = new QLineEdit;
    auto* nhomEdit   = new QLineEdit;

    auto* tietDauSpin  = new QSpinBox; tietDauSpin->setRange(1, 15); tietDauSpin->setValue(1);
    auto* tietCuoiSpin = new QSpinBox; tietCuoiSpin->setRange(1, 15); tietCuoiSpin->setValue(3);
    auto* siSoSpin     = new QSpinBox; siSoSpin->setRange(1, 200);   siSoSpin->setValue(50);
    auto* hkSpin       = new QSpinBox; hkSpin->setRange(1, 2);       hkSpin->setValue(m_sys.getCurrentSemester());

    form->addRow("Mã lớp HP (*):", maLopEdit);
    form->addRow("Mã môn (*):",    maMonEdit);
    form->addRow("Giảng viên:",    gvEdit);
    form->addRow("Thứ:",           thuCombo);
    form->addRow("Tiết bắt đầu:",  tietDauSpin);
    form->addRow("Tiết kết thúc:", tietCuoiSpin);
    form->addRow("Phòng:",         phongEdit);
    form->addRow("Sĩ số tối đa:",  siSoSpin);
    form->addRow("Học kỳ:",        hkSpin);
    form->addRow("Nhóm lớp HP:",   nhomEdit);

    auto* btns = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form->addRow(btns);
    connect(btns, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btns, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted) return;

    QString maLop = maLopEdit->text().trimmed();
    QString maMon = maMonEdit->text().trimmed();
    if (maLop.isEmpty() || maMon.isEmpty()) {
        QMessageBox::warning(this, "Thiếu thông tin", "Mã lớp HP và Mã môn là bắt buộc.");
        return;
    }

    try {
        ClassSection sec(
            maLop.toStdString(),
            maMon.toStdString(),
            gvEdit->text().trimmed().toStdString(),
            thuCombo->currentData().toString().toStdString(),
            tietDauSpin->value(),
            tietCuoiSpin->value(),
            phongEdit->text().trimmed().toStdString(),
            siSoSpin->value(),
            hkSpin->value(),
            nhomEdit->text().trimmed().toStdString()
        );
        if (!m_sys.addClassSection(sec)) {
            QMessageBox::warning(this, "Lỗi", "Không thể thêm lớp học phần (mã có thể bị trùng hoặc mã môn chưa tồn tại).");
        } else {
            refreshTable();
        }
    } catch (const AppException& e) {
        QMessageBox::warning(this, "Lỗi nghiệp vụ", QString::fromStdString(e.what()));
    }
}

void SectionManagementWidget::onDeleteSection()
{
    int row = m_table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Chưa chọn", "Vui lòng chọn một lớp học phần để xóa.");
        return;
    }
    QString maLop = m_table->item(row, 0)->text();

    auto res = QMessageBox::question(this, "Xác nhận xóa",
        QString("Bạn có chắc muốn xóa lớp học phần '%1'?\nViệc này sẽ hủy tất cả đăng ký liên quan.").arg(maLop),
        QMessageBox::Yes | QMessageBox::No);
    if (res != QMessageBox::Yes) return;

    try {
        if (!m_sys.deleteClassSection(maLop.toStdString())) {
            QMessageBox::warning(this, "Lỗi", "Không thể xóa lớp học phần này.");
        } else {
            refreshTable();
        }
    } catch (const AppException& e) {
        QMessageBox::warning(this, "Lỗi", QString::fromStdString(e.what()));
    }
}

void SectionManagementWidget::onViewStudents()
{
    int row = m_table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Chưa chọn", "Vui lòng chọn một lớp học phần.");
        return;
    }
    QString maLop = m_table->item(row, 0)->text();

    ClassSection sec;
    DoubleLinkedList<SectionStudentInfo> students;
    if (!m_sys.getStudentsInSection(maLop.toStdString(), sec, students)) {
        QMessageBox::warning(this, "Lỗi", QString("Không tìm thấy lớp học phần: %1").arg(maLop));
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle(QString("Danh sách sinh viên - %1 (%2 / %3 chỗ)")
                       .arg(maLop)
                       .arg(sec.getSoLuongDaDangKy())
                       .arg(sec.getSiSoToiDa()));
    dlg.resize(620, 420);

    auto* layout = new QVBoxLayout(&dlg);
    auto* table  = new QTableWidget;
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"Mã SV", "Họ tên", "Lớp", "Ngày ĐK"});
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);

    auto* node = students.getHead();
    while (node) {
        int r = table->rowCount();
        table->insertRow(r);
        const SectionStudentInfo& info = node->data;
        table->setItem(r, 0, new QTableWidgetItem(QString::fromStdString(info.maSV)));
        table->setItem(r, 1, new QTableWidgetItem(QString::fromStdString(info.hoTen)));
        table->setItem(r, 2, new QTableWidgetItem(QString::fromStdString(info.lop)));
        table->setItem(r, 3, new QTableWidgetItem(QString::fromStdString(info.ngayDangKy)));
        node = node->next;
    }
    table->resizeColumnsToContents();
    layout->addWidget(table);

    auto* closeBtn = new QPushButton("Đóng");
    connect(closeBtn, &QPushButton::clicked, &dlg, &QDialog::accept);
    layout->addWidget(closeBtn, 0, Qt::AlignRight);
    dlg.exec();
}
