#include "StudentManagementWidget.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QMessageBox>
#include <QHeaderView>

#include "../manager/exception.h"

// ─── constructor ──────────────────────────────────────────────────────────────

StudentManagementWidget::StudentManagementWidget(SystemManager& sys, QWidget* parent)
    : QWidget(parent), m_sys(sys)
{
    setupUi();
    refreshTable();

    setStyleSheet(R"(
        StudentManagementWidget { background-color: #FFFFFF; }
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
    m_table->setAlternatingRowColors(true);
    m_deleteBtn->setStyleSheet("QPushButton{background:#EF4444;} QPushButton:hover{background:#DC2626;}");
}

// ─── setup UI ────────────────────────────────────────────────────────────────

void StudentManagementWidget::setupUi()
{
    // --- search bar ---
    m_searchEdit  = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("Nhập mã SV hoặc tên để tìm kiếm...");
    m_searchBtn   = new QPushButton("Tìm kiếm", this);
    m_refreshBtn  = new QPushButton("Làm mới",  this);

    auto* searchLayout = new QHBoxLayout;
    searchLayout->addWidget(m_searchEdit);
    searchLayout->addWidget(m_searchBtn);
    searchLayout->addWidget(m_refreshBtn);

    // --- table ---
    m_table = new QTableWidget(this);
    m_table->setColumnCount(8);
    m_table->setHorizontalHeaderLabels({
        "Mã SV", "Họ tên", "Lớp", "Ngành", "Khoa", "Khoá học", "SĐT", "Email"
    });
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->setVisible(false);

    // --- action buttons ---
    m_addBtn    = new QPushButton("Thêm sinh viên", this);
    m_editBtn   = new QPushButton("Sửa sinh viên",  this);
    m_deleteBtn = new QPushButton("Xoá sinh viên",  this);

    auto* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_addBtn);
    btnLayout->addWidget(m_editBtn);
    btnLayout->addWidget(m_deleteBtn);
    btnLayout->addStretch();

    // --- main layout ---
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(m_table);
    mainLayout->addLayout(btnLayout);

    // --- connections ---
    connect(m_searchBtn,  &QPushButton::clicked, this, &StudentManagementWidget::onSearch);
    connect(m_refreshBtn, &QPushButton::clicked, this, &StudentManagementWidget::onRefresh);
    connect(m_addBtn,     &QPushButton::clicked, this, &StudentManagementWidget::onAddStudent);
    connect(m_editBtn,    &QPushButton::clicked, this, &StudentManagementWidget::onEditStudent);
    connect(m_deleteBtn,  &QPushButton::clicked, this, &StudentManagementWidget::onDeleteStudent);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &StudentManagementWidget::onSearch);
}

// ─── helpers ─────────────────────────────────────────────────────────────────

void StudentManagementWidget::populateTable(DoubleLinkedList<Student>& list)
{
    m_table->setRowCount(0);

    auto node = list.getHead();
    while (node) {
        const Student& s = node->data;
        int row = m_table->rowCount();
        m_table->insertRow(row);

        auto setCell = [&](int col, const std::string& val) {
            auto* item = new QTableWidgetItem(QString::fromStdString(val));
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            m_table->setItem(row, col, item);
        };

        setCell(0, s.getMaSV());
        setCell(1, s.getHoTen());
        setCell(2, s.getLop());
        setCell(3, s.getNganhHoc());
        setCell(4, s.getKhoa());
        setCell(5, s.getKhoaHoc());
        setCell(6, s.getSoDienThoai());
        setCell(7, s.getEmail());

        node = node->next;
    }

    m_table->resizeColumnsToContents();
}

void StudentManagementWidget::refreshTable()
{
    DoubleLinkedList<Student> list;
    m_sys.getAllStudents(list);
    populateTable(list);
}

// ─── slots ───────────────────────────────────────────────────────────────────

void StudentManagementWidget::onSearch()
{
    QString kw = m_searchEdit->text().trimmed();
    if (kw.isEmpty()) {
        refreshTable();
        return;
    }
    DoubleLinkedList<Student> list;
    m_sys.searchStudents(kw.toStdString(), list);
    populateTable(list);
}

void StudentManagementWidget::onRefresh()
{
    m_searchEdit->clear();
    refreshTable();
}

void StudentManagementWidget::onAddStudent()
{
    showStudentDialog(nullptr);
}

void StudentManagementWidget::onEditStudent()
{
    int row = m_table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Thông báo", "Vui lòng chọn một sinh viên để sửa.");
        return;
    }
    QString maSV = m_table->item(row, 0)->text();
    Student* s = m_sys.findStudent(maSV.toStdString());
    if (!s) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy sinh viên.");
        return;
    }
    showStudentDialog(s);
}

void StudentManagementWidget::onDeleteStudent()
{
    int row = m_table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Thông báo", "Vui lòng chọn một sinh viên để xoá.");
        return;
    }

    QString maSV   = m_table->item(row, 0)->text();
    QString hoTen  = m_table->item(row, 1)->text();

    auto res = QMessageBox::question(
        this, "Xác nhận xoá",
        QString("Bạn có chắc muốn xoá sinh viên %1 (%2) không?").arg(maSV, hoTen),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No
    );
    if (res != QMessageBox::Yes) return;

    try {
        DoubleLinkedList<std::string> waitlistNotices;
        bool ok = m_sys.deleteStudent(maSV.toStdString(), &waitlistNotices);
        if (ok) {
            // Show waitlist notices if any
            auto node = waitlistNotices.getHead();
            while (node) {
                QMessageBox::information(this, "Thông báo danh sách chờ",
                    QString::fromStdString(node->data));
                node = node->next;
            }
            refreshTable();
        } else {
            QMessageBox::warning(this, "Lỗi", "Không thể xoá sinh viên.");
        }
    } catch (const AppException& e) {
        QMessageBox::warning(this, "Lỗi", QString::fromStdString(e.what()));
    }
}

// ─── dialog ──────────────────────────────────────────────────────────────────

void StudentManagementWidget::showStudentDialog(Student* existing)
{
    bool isEdit = (existing != nullptr);

    QDialog dlg(this);
    dlg.setWindowTitle(isEdit ? "Sửa sinh viên" : "Thêm sinh viên");
    dlg.setMinimumWidth(380);

    auto* form      = new QFormLayout;
    auto* maSVEdit  = new QLineEdit(&dlg);
    auto* tenEdit   = new QLineEdit(&dlg);
    auto* lopEdit   = new QLineEdit(&dlg);
    auto* nganhEdit = new QLineEdit(&dlg);
    auto* khoaEdit  = new QLineEdit(&dlg);
    auto* khoaHocEdit = new QLineEdit(&dlg);
    auto* sdtEdit   = new QLineEdit(&dlg);
    auto* emailEdit = new QLineEdit(&dlg);

    if (isEdit) {
        maSVEdit->setText(QString::fromStdString(existing->getMaSV()));
        maSVEdit->setEnabled(false); // cannot change ID
        tenEdit->setText(QString::fromStdString(existing->getHoTen()));
        lopEdit->setText(QString::fromStdString(existing->getLop()));
        nganhEdit->setText(QString::fromStdString(existing->getNganhHoc()));
        khoaEdit->setText(QString::fromStdString(existing->getKhoa()));
        khoaHocEdit->setText(QString::fromStdString(existing->getKhoaHoc()));
        sdtEdit->setText(QString::fromStdString(existing->getSoDienThoai()));
        emailEdit->setText(QString::fromStdString(existing->getEmail()));
    }

    form->addRow("Mã SV:",      maSVEdit);
    form->addRow("Họ tên:",     tenEdit);
    form->addRow("Lớp:",        lopEdit);
    form->addRow("Ngành học:",  nganhEdit);
    form->addRow("Khoa:",       khoaEdit);
    form->addRow("Khoá học:",   khoaHocEdit);
    form->addRow("SĐT:",        sdtEdit);
    form->addRow("Email:",      emailEdit);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    auto* vlay = new QVBoxLayout(&dlg);
    vlay->addLayout(form);
    vlay->addWidget(buttons);

    if (dlg.exec() != QDialog::Accepted) return;

    std::string maSV    = maSVEdit->text().trimmed().toStdString();
    std::string ten     = tenEdit->text().trimmed().toStdString();
    std::string lop     = lopEdit->text().trimmed().toStdString();
    std::string nganh   = nganhEdit->text().trimmed().toStdString();
    std::string khoa    = khoaEdit->text().trimmed().toStdString();
    std::string khoaHoc = khoaHocEdit->text().trimmed().toStdString();
    std::string sdt     = sdtEdit->text().trimmed().toStdString();
    std::string email   = emailEdit->text().trimmed().toStdString();

    try {
        if (isEdit) {
            existing->setHoTen(ten);
            existing->setLop(lop);
            existing->setNganhHoc(nganh);
            existing->setKhoa(khoa);
            existing->setKhoaHoc(khoaHoc);
            existing->setSoDienThoai(sdt);
            existing->setEmail(email);
            refreshTable();
        } else {
            Student s(maSV, ten, sdt, email, lop, nganh, khoa, khoaHoc);
            bool ok = m_sys.addStudent(s);
            if (!ok) {
                QMessageBox::warning(this, "Lỗi", "Mã sinh viên đã tồn tại.");
                return;
            }
            refreshTable();
        }
    } catch (const AppException& e) {
        QMessageBox::warning(this, "Lỗi", QString::fromStdString(e.what()));
    }
}
