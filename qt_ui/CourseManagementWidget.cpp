#include "CourseManagementWidget.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QSpinBox>

#include "../manager/exception.h"

// ─── constructor ──────────────────────────────────────────────────────────────

CourseManagementWidget::CourseManagementWidget(SystemManager& sys, QWidget* parent)
    : QWidget(parent), m_sys(sys)
{
    setupUi();
    refreshTable();

    setStyleSheet(R"(
        CourseManagementWidget { background-color: #FFFFFF; }
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

void CourseManagementWidget::setupUi()
{
    // --- search bar ---
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("Nhập mã môn hoặc tên môn để tìm kiếm...");
    m_searchBtn  = new QPushButton("Tìm kiếm", this);
    m_refreshBtn = new QPushButton("Làm mới",  this);

    auto* searchLayout = new QHBoxLayout;
    searchLayout->addWidget(m_searchEdit);
    searchLayout->addWidget(m_searchBtn);
    searchLayout->addWidget(m_refreshBtn);

    // --- table ---
    m_table = new QTableWidget(this);
    m_table->setColumnCount(7);
    m_table->setHorizontalHeaderLabels({
        "Mã môn", "Tên môn", "Số TC", "Khoa", "Khoá học", "Ngành", "Học kỳ"
    });
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->setVisible(false);

    // --- action buttons ---
    m_addBtn    = new QPushButton("Thêm môn học", this);
    m_editBtn   = new QPushButton("Sửa môn học",  this);
    m_deleteBtn = new QPushButton("Xoá môn học",  this);

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
    connect(m_searchBtn,  &QPushButton::clicked, this, &CourseManagementWidget::onSearch);
    connect(m_refreshBtn, &QPushButton::clicked, this, &CourseManagementWidget::onRefresh);
    connect(m_addBtn,     &QPushButton::clicked, this, &CourseManagementWidget::onAddCourse);
    connect(m_editBtn,    &QPushButton::clicked, this, &CourseManagementWidget::onEditCourse);
    connect(m_deleteBtn,  &QPushButton::clicked, this, &CourseManagementWidget::onDeleteCourse);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &CourseManagementWidget::onSearch);
}

// ─── helpers ─────────────────────────────────────────────────────────────────

void CourseManagementWidget::populateTable(DoubleLinkedList<Course>& list)
{
    m_table->setRowCount(0);

    auto node = list.getHead();
    while (node) {
        const Course& c = node->data;
        int row = m_table->rowCount();
        m_table->insertRow(row);

        auto setCell = [&](int col, const QString& val) {
            auto* item = new QTableWidgetItem(val);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            m_table->setItem(row, col, item);
        };

        setCell(0, QString::fromStdString(c.getMaMon()));
        setCell(1, QString::fromStdString(c.getTenMon()));
        setCell(2, QString::number(c.getSoTinChi()));
        setCell(3, QString::fromStdString(c.getKhoa()));
        setCell(4, QString::fromStdString(c.getKhoaHoc()));
        setCell(5, QString::fromStdString(c.getNganhHoc()));
        setCell(6, QString::number(c.getHocKy()));

        node = node->next;
    }

    m_table->resizeColumnsToContents();
}

void CourseManagementWidget::refreshTable()
{
    DoubleLinkedList<Course> list;
    m_sys.getAllCourses(list);
    populateTable(list);
}

// ─── slots ───────────────────────────────────────────────────────────────────

void CourseManagementWidget::onSearch()
{
    QString kw = m_searchEdit->text().trimmed();
    if (kw.isEmpty()) {
        refreshTable();
        return;
    }
    DoubleLinkedList<Course> list;
    m_sys.searchCourses(kw.toStdString(), list);
    populateTable(list);
}

void CourseManagementWidget::onRefresh()
{
    m_searchEdit->clear();
    refreshTable();
}

void CourseManagementWidget::onAddCourse()
{
    showCourseDialog(nullptr);
}

void CourseManagementWidget::onEditCourse()
{
    int row = m_table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Thông báo", "Vui lòng chọn một môn học để sửa.");
        return;
    }
    QString maMon = m_table->item(row, 0)->text();
    Course* c = m_sys.findCourse(maMon.toStdString());
    if (!c) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy môn học.");
        return;
    }
    showCourseDialog(c);
}

void CourseManagementWidget::onDeleteCourse()
{
    int row = m_table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Thông báo", "Vui lòng chọn một môn học để xoá.");
        return;
    }

    QString maMon  = m_table->item(row, 0)->text();
    QString tenMon = m_table->item(row, 1)->text();

    auto res = QMessageBox::question(
        this, "Xác nhận xoá",
        QString("Bạn có chắc muốn xoá môn học %1 (%2) không?").arg(maMon, tenMon),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No
    );
    if (res != QMessageBox::Yes) return;

    try {
        std::string msg;
        bool ok = m_sys.deleteCourse(maMon.toStdString(), &msg);
        if (ok) {
            if (!msg.empty()) {
                QMessageBox::information(this, "Thông báo", QString::fromStdString(msg));
            }
            refreshTable();
        } else {
            QString errMsg = msg.empty()
                ? "Không thể xoá môn học."
                : QString::fromStdString(msg);
            QMessageBox::warning(this, "Lỗi", errMsg);
        }
    } catch (const AppException& e) {
        QMessageBox::warning(this, "Lỗi", QString::fromStdString(e.what()));
    }
}

// ─── dialog ──────────────────────────────────────────────────────────────────

void CourseManagementWidget::showCourseDialog(Course* existing)
{
    bool isEdit = (existing != nullptr);

    QDialog dlg(this);
    dlg.setWindowTitle(isEdit ? "Sửa môn học" : "Thêm môn học");
    dlg.setMinimumWidth(380);

    auto* form      = new QFormLayout;
    auto* maEdit    = new QLineEdit(&dlg);
    auto* tenEdit   = new QLineEdit(&dlg);
    auto* tcSpin    = new QSpinBox(&dlg);
    auto* khoaEdit  = new QLineEdit(&dlg);
    auto* khoaHocEdit = new QLineEdit(&dlg);
    auto* nganhEdit = new QLineEdit(&dlg);
    auto* hkSpin    = new QSpinBox(&dlg);

    tcSpin->setRange(1, 10);
    tcSpin->setValue(3);
    hkSpin->setRange(1, 2);
    hkSpin->setValue(1);

    if (isEdit) {
        maEdit->setText(QString::fromStdString(existing->getMaMon()));
        maEdit->setEnabled(false); // cannot change course code
        tenEdit->setText(QString::fromStdString(existing->getTenMon()));
        tcSpin->setValue(existing->getSoTinChi());
        khoaEdit->setText(QString::fromStdString(existing->getKhoa()));
        khoaHocEdit->setText(QString::fromStdString(existing->getKhoaHoc()));
        nganhEdit->setText(QString::fromStdString(existing->getNganhHoc()));
        hkSpin->setValue(existing->getHocKy());
    }

    form->addRow("Mã môn:",     maEdit);
    form->addRow("Tên môn:",    tenEdit);
    form->addRow("Số tín chỉ:", tcSpin);
    form->addRow("Khoa:",       khoaEdit);
    form->addRow("Khoá học:",   khoaHocEdit);
    form->addRow("Ngành:",      nganhEdit);
    form->addRow("Học kỳ:",     hkSpin);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    auto* vlay = new QVBoxLayout(&dlg);
    vlay->addLayout(form);
    vlay->addWidget(buttons);

    if (dlg.exec() != QDialog::Accepted) return;

    std::string ma      = maEdit->text().trimmed().toStdString();
    std::string ten     = tenEdit->text().trimmed().toStdString();
    int         tinChi  = tcSpin->value();
    std::string khoa    = khoaEdit->text().trimmed().toStdString();
    std::string khoaHoc = khoaHocEdit->text().trimmed().toStdString();
    std::string nganh   = nganhEdit->text().trimmed().toStdString();
    int         hocKy   = hkSpin->value();

    try {
        if (isEdit) {
            // For edit: re-create or update fields via setters if available.
            // Since Course may not expose individual setters, we delete and re-add.
            // But to be safe and not break references, we update the object in-place
            // by recreating with the same maMon (edit dialog disables maEdit).
            std::string existingMa = existing->getMaMon();
            std::string outMsg;
            m_sys.deleteCourse(existingMa, &outMsg);
            Course updated(existingMa, ten, tinChi, khoa, khoaHoc, nganh, hocKy);
            bool ok = m_sys.addCourse(updated);
            if (!ok) {
                QMessageBox::warning(this, "Lỗi", "Không thể cập nhật môn học.");
                return;
            }
            refreshTable();
        } else {
            Course c(ma, ten, tinChi, khoa, khoaHoc, nganh, hocKy);
            bool ok = m_sys.addCourse(c);
            if (!ok) {
                QMessageBox::warning(this, "Lỗi", "Mã môn học đã tồn tại.");
                return;
            }
            refreshTable();
        }
    } catch (const AppException& e) {
        QMessageBox::warning(this, "Lỗi", QString::fromStdString(e.what()));
    }
}
