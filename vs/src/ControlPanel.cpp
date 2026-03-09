// ═══════════════════════════════════════════════════════════════════════════════
//  ControlPanel.cpp
// ═══════════════════════════════════════════════════════════════════════════════

#include "ControlPanel.h"
#include "ui_ControlPanel.h"
#include <iostream>
#include <algorithm>
#include <QSet>

// ─────────────────────────────────────────────────────────────────────────────
//  Section 1: Construction & Destruction
// ─────────────────────────────────────────────────────────────────────────────

ControlPanel::ControlPanel(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ControlPanel)
{
    ui->setupUi(this);

    ui->dataTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->dataTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 右键菜单统一由 eventFilter 处理
    ui->dataTable->viewport()->installEventFilter(this);
    ui->dataTable->setContextMenuPolicy(Qt::NoContextMenu);
}

ControlPanel::~ControlPanel()
{
    delete ui;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Section 2: Table Setup
// ─────────────────────────────────────────────────────────────────────────────

void ControlPanel::setupTable(std::vector<std::vector<vtkAesReader::Boundary>> boundarys)
{
    // 设置列宽
    ui->dataTable->setColumnWidth(0, 70);    // Zone Number
    ui->dataTable->setColumnWidth(1, 200);   // Zone Name
    ui->dataTable->setColumnWidth(2, 120);   // Zone Type
    ui->dataTable->setColumnWidth(3, 70);    // Show Zone
    ui->dataTable->setColumnWidth(4, 100);   // Contour Mode
    ui->dataTable->setColumnWidth(5, 100);   // Transculency
    ui->dataTable->setColumnWidth(6, 100);   // Zone Delete

    // 清空之前的索引信息
    boundaryIndices.clear();
    cutplaneInfos.clear();
    rowTypes.clear();

    // 收集所有的 boundary 信息
    QStringList zoneNumbers;
    QStringList boundaryNames;
    int boundaryCount = 0;

    for (int i = 0; i < boundarys.size(); i++) {
        for (int j = 0; j < boundarys[i].size(); j++) {
            boundaryCount++;
            zoneNumbers.append(QString("%1*").arg(boundaryCount));

            QString zoneName = QString(boundarys[i][j].zoneName.c_str()).trimmed();
            QString boundaryName = QString(boundarys[i][j].name.c_str()).trimmed();
            boundaryNames.append(QString("%1->%2").arg(zoneName).arg(boundaryName));

            boundaryIndices.push_back(std::make_pair(i, j));
            rowTypes.push_back(BOUNDARY);
        }
    }

    // 填充表格
    ui->dataTable->setRowCount(boundaryCount);

    for (int row = 0; row < boundaryCount; ++row) {
        // Col 0: Zone Number
        QTableWidgetItem* zoneNumItem = new QTableWidgetItem(zoneNumbers[row]);
        zoneNumItem->setTextAlignment(Qt::AlignCenter);
        ui->dataTable->setItem(row, 0, zoneNumItem);

        // Col 1: Zone Name
        ui->dataTable->setItem(row, 1, new QTableWidgetItem(boundaryNames[row]));

        // Col 2: Zone Type
        QTableWidgetItem* groupNumItem = new QTableWidgetItem("Main Model");
        groupNumItem->setTextAlignment(Qt::AlignCenter);
        ui->dataTable->setItem(row, 2, groupNumItem);

        // Col 3: Show Zone (CheckBox)
        QWidget* checkBoxWidget = createCheckBoxWidget(true);
        QCheckBox* checkBox = checkBoxWidget->findChild<QCheckBox*>();
        if (checkBox) {
            checkBox->setProperty("boundaryRow", row);
            connect(checkBox, &QCheckBox::toggled, this, &ControlPanel::onShowZoneCheckBoxToggled);
        }
        ui->dataTable->setCellWidget(row, 3, checkBoxWidget);

        // Col 4: Contour Mode (ComboBox, boundary行不连接信号)
        ui->dataTable->setCellWidget(row, 4, createComboBoxWidget());

        // Col 5: Transculency (SpinBox)
        QWidget* transparencyWidget = createTransparencyWidget(0.5);
        QDoubleSpinBox* transSpinBox = transparencyWidget->findChild<QDoubleSpinBox*>();
        if (transSpinBox) {
            transSpinBox->setProperty("boundaryRow", row);
            connect(transSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                    this, &ControlPanel::onBoundaryTransparencyChanged);
        }
        ui->dataTable->setCellWidget(row, 5, transparencyWidget);

        // Col 6: Zone Delete (Button, boundary行不连接信号)
        ui->dataTable->setCellWidget(row, 6, createDeleteButtonWidget());
    }
}

void ControlPanel::addCutplaneToTable(int cutplaneIndex, double* origin, double* normal)
{
    // 存储 cutplane 信息
    CutplaneInfo info;
    info.index = cutplaneIndex;
    for (int i = 0; i < 3; i++) {
        info.origin[i] = origin[i];
        info.normal[i] = normal[i];
    }
    cutplaneInfos.push_back(info);

    // 新增一行
    int newRow = ui->dataTable->rowCount();
    ui->dataTable->setRowCount(newRow + 1);
    rowTypes.push_back(CUTPLANE);

    // Col 0: Zone Number
    QTableWidgetItem* zoneNumItem = new QTableWidgetItem(QString("S%1*").arg(cutplaneIndex + 1));
    zoneNumItem->setTextAlignment(Qt::AlignCenter);
    ui->dataTable->setItem(newRow, 0, zoneNumItem);

    // Col 1: Zone Name
    ui->dataTable->setItem(newRow, 1, new QTableWidgetItem(QString("Slice_%1").arg(cutplaneIndex + 1)));

    // Col 2: Zone Type
    QTableWidgetItem* groupNumItem = new QTableWidgetItem("Slice");
    groupNumItem->setTextAlignment(Qt::AlignCenter);
    ui->dataTable->setItem(newRow, 2, groupNumItem);

    // Col 3: Show Zone (CheckBox)
    QWidget* checkBoxWidget = createCheckBoxWidget(true);
    QCheckBox* checkBox = checkBoxWidget->findChild<QCheckBox*>();
    if (checkBox) {
        checkBox->setProperty("boundaryRow", newRow);
        connect(checkBox, &QCheckBox::toggled, this, &ControlPanel::onShowZoneCheckBoxToggled);
    }
    ui->dataTable->setCellWidget(newRow, 3, checkBoxWidget);

    // Col 4: Contour Mode (ComboBox, slice行连接信号)
    QWidget* comboWidget = createComboBoxWidget();
    QComboBox* comboBox = comboWidget->findChild<QComboBox*>();
    if (comboBox) {
        comboBox->setProperty("cutplaneIndex", cutplaneIndex);
        connect(comboBox, QOverload<const QString &>::of(&QComboBox::currentTextChanged),
                this, &ControlPanel::onSliceContourModeChanged);
    }
    ui->dataTable->setCellWidget(newRow, 4, comboWidget);

    // Col 5: Transculency (SpinBox)
    QWidget* transparencyWidget = createTransparencyWidget(1.0);
    QDoubleSpinBox* transSpinBox = transparencyWidget->findChild<QDoubleSpinBox*>();
    if (transSpinBox) {
        transSpinBox->setProperty("sliceIndex", cutplaneIndex);
        connect(transSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                this, &ControlPanel::onSliceTransparencyChanged);
    }
    ui->dataTable->setCellWidget(newRow, 5, transparencyWidget);

    // Col 6: Zone Delete (Button, slice行连接删除)
    QWidget* deleteWidget = createDeleteButtonWidget();
    QPushButton* deleteButton = deleteWidget->findChild<QPushButton*>();
    if (deleteButton) {
        deleteButton->setProperty("cutplaneIndex", cutplaneIndex);
        connect(deleteButton, &QPushButton::clicked, this, &ControlPanel::onSliceDeleteClicked);
    }
    ui->dataTable->setCellWidget(newRow, 6, deleteWidget);
}

void ControlPanel::removeCutplaneFromTable(int cutplaneIndex)
{
    // 找到对应的行号
    int rowToRemove = -1;
    int cutplaneCount = 0;

    for (int i = 0; i < rowTypes.size(); i++) {
        if (rowTypes[i] == CUTPLANE) {
            if (cutplaneInfos[cutplaneCount].index == cutplaneIndex) {
                rowToRemove = i;
                break;
            }
            cutplaneCount++;
        }
    }

    if (rowToRemove == -1) return;

    // 从表格和数据中删除
    ui->dataTable->removeRow(rowToRemove);
    rowTypes.erase(rowTypes.begin() + rowToRemove);

    int cutplaneInfoIndex = rowToRemove - boundaryIndices.size();
    if (cutplaneInfoIndex >= 0 && cutplaneInfoIndex < cutplaneInfos.size()) {
        cutplaneInfos.erase(cutplaneInfos.begin() + cutplaneInfoIndex);
    }

    // 重新编号 cutplane 行
    int newCutplaneNumber = 1;
    for (int i = 0; i < rowTypes.size(); i++) {
        if (rowTypes[i] != CUTPLANE) continue;

        int idx = i - boundaryIndices.size();
        if (idx < 0 || idx >= cutplaneInfos.size()) continue;

        cutplaneInfos[idx].index = newCutplaneNumber - 1;

        QTableWidgetItem* zoneNumberItem = ui->dataTable->item(i, 0);
        if (zoneNumberItem) zoneNumberItem->setText(QString("S%1*").arg(newCutplaneNumber));

        QTableWidgetItem* zoneNameItem = ui->dataTable->item(i, 1);
        if (zoneNameItem) zoneNameItem->setText(QString("Slice_%1").arg(newCutplaneNumber));

        newCutplaneNumber++;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  Section 3: Cell Widget Factories
// ─────────────────────────────────────────────────────────────────────────────

QWidget* ControlPanel::createCheckBoxWidget(bool checked)
{
    QWidget* widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(widget);
    QCheckBox* checkBox = new QCheckBox();
    checkBox->setChecked(checked);
    layout->addWidget(checkBox);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    return widget;
}

QWidget* ControlPanel::createComboBoxWidget()
{
    QWidget* widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(widget);
    QComboBox* comboBox = new QComboBox();
    comboBox->addItem("isolated");
    comboBox->addItem("sync with main");
    comboBox->setCurrentIndex(0);
    layout->addWidget(comboBox);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    return widget;
}

QWidget* ControlPanel::createTransparencyWidget(double value)
{
    QWidget* widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(widget);
    QDoubleSpinBox* spinBox = new QDoubleSpinBox();
    spinBox->setRange(0.0, 1.0);
    spinBox->setSingleStep(0.1);
    spinBox->setValue(value);
    spinBox->setDecimals(1);
    layout->addWidget(spinBox);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    return widget;
}

QWidget* ControlPanel::createDeleteButtonWidget()
{
    QWidget* widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(widget);
    QPushButton* deleteButton = new QPushButton("Delete");
    layout->addWidget(deleteButton);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    return widget;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Section 4: Visibility Control (Show Zone)
// ─────────────────────────────────────────────────────────────────────────────

void ControlPanel::onShowZoneCheckBoxToggled(bool checked)
{
    QCheckBox* checkBox = qobject_cast<QCheckBox*>(sender());
    if (!checkBox) return;

    int row = checkBox->property("boundaryRow").toInt();
    if (row < 0 || row >= rowTypes.size()) return;

    if (rowTypes[row] == BOUNDARY) {
        if (row >= boundaryIndices.size()) return;
        int meshNumber = boundaryIndices[row].first;
        int boundaryNumber = boundaryIndices[row].second;
        emit setBoundarys(meshNumber, boundaryNumber, checked);
    }
    else if (rowTypes[row] == CUTPLANE) {
        int cutplaneIndex = row - boundaryIndices.size();
        if (cutplaneIndex < 0 || cutplaneIndex >= cutplaneInfos.size()) return;
        emit setCutplaneVisiable(cutplaneInfos[cutplaneIndex].index, checked);
    }
}

void ControlPanel::showZoneContextMenu(const QPoint& pos)
{
    QList<int> selectedRows = getSelectedRows();
    if (selectedRows.isEmpty()) return;

    QMenu* menu = new QMenu(this);
    menu->setStyleSheet(
        "QMenu { background-color: white; border: 1px solid #ccc; }"
        "QMenu::item { padding: 5px 30px 5px 20px; }"
        "QMenu::item:selected { background-color: #0078d4; color: white; }"
    );

    QAction* actionActivate   = menu->addAction("Activate");
    QAction* actionDeactivate = menu->addAction("Deactivate");

    QAction* selectedAction = menu->exec(ui->dataTable->viewport()->mapToGlobal(pos));

    if (selectedAction == actionActivate) {
        setSelectedZonesVisibility(true);
    } else if (selectedAction == actionDeactivate) {
        setSelectedZonesVisibility(false);
    }

    delete menu;
}

void ControlPanel::setSelectedZonesVisibility(bool visible)
{
    QList<int> selectedRows = getSelectedRows();

    std::cout << "[Debug] Setting visibility " << visible
              << " for " << selectedRows.size() << " selected rows" << std::endl;

    for (int row : selectedRows) {
        if (row < 0 || row >= static_cast<int>(rowTypes.size())) continue;

        // 更新 CheckBox UI
        QWidget* checkBoxWidget = ui->dataTable->cellWidget(row, 3);
        if (!checkBoxWidget) continue;
        QCheckBox* checkBox = checkBoxWidget->findChild<QCheckBox*>();
        if (!checkBox) continue;

        checkBox->blockSignals(true);
        checkBox->setChecked(visible);
        checkBox->blockSignals(false);

        // 发送信号控制显示
        if (rowTypes[row] == BOUNDARY && row < static_cast<int>(boundaryIndices.size())) {
            int meshNumber = boundaryIndices[row].first;
            int boundaryNumber = boundaryIndices[row].second;
            emit setBoundarys(meshNumber, boundaryNumber, visible);
            std::cout << "[Debug] Set boundary [" << meshNumber << "][" << boundaryNumber
                      << "] visibility to " << visible << std::endl;
        }
        else if (rowTypes[row] == CUTPLANE) {
            int idx = row - static_cast<int>(boundaryIndices.size());
            if (idx >= 0 && idx < static_cast<int>(cutplaneInfos.size())) {
                emit setCutplaneVisiable(cutplaneInfos[idx].index, visible);
                std::cout << "[Debug] Set cutplane " << cutplaneInfos[idx].index
                          << " visibility to " << visible << std::endl;
            }
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  Section 5: Transparency Control
// ─────────────────────────────────────────────────────────────────────────────

void ControlPanel::onBoundaryTransparencyChanged(double value)
{
    QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(sender());
    if (!spinBox) return;

    int row = spinBox->property("boundaryRow").toInt();
    if (row < 0 || row >= boundaryIndices.size()) return;

    int meshNumber = boundaryIndices[row].first;
    int boundaryNumber = boundaryIndices[row].second;
    emit boundaryTransparencyChanged(meshNumber, boundaryNumber, value);

    std::cout << "[Debug] Boundary transparency changed: mesh=" << meshNumber
              << ", boundary=" << boundaryNumber << ", value=" << value << std::endl;
}

void ControlPanel::onSliceTransparencyChanged(double value)
{
    QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(sender());
    if (!spinBox) return;

    int sliceIndex = spinBox->property("sliceIndex").toInt();
    emit sliceTransparencyChanged(sliceIndex, value);

    std::cout << "[Debug] Slice transparency changed: slice=" << sliceIndex
              << ", value=" << value << std::endl;
}

void ControlPanel::setTransparencyControlsEnabled(bool enabled)
{
    for (int row = 0; row < ui->dataTable->rowCount(); row++) {
        QWidget* w = ui->dataTable->cellWidget(row, 5);
        if (!w) continue;
        QDoubleSpinBox* spinBox = w->findChild<QDoubleSpinBox*>();
        if (spinBox) spinBox->setEnabled(enabled);
    }
    std::cout << "[Debug] Transparency controls enabled: " << (enabled ? "true" : "false") << std::endl;
}

void ControlPanel::getAllBoundaryTransparencies(std::vector<std::vector<double>>& transparencies)
{
    transparencies.clear();
    if (boundaryIndices.empty()) return;

    // 找出最大的 mesh 索引
    int maxMeshIndex = 0;
    for (const auto& pair : boundaryIndices) {
        if (pair.first > maxMeshIndex) maxMeshIndex = pair.first;
    }
    transparencies.resize(maxMeshIndex + 1);

    for (int row = 0; row < rowTypes.size(); row++) {
        if (rowTypes[row] != BOUNDARY || row >= boundaryIndices.size()) continue;

        int meshNumber = boundaryIndices[row].first;
        int boundaryNumber = boundaryIndices[row].second;

        if (transparencies[meshNumber].size() <= boundaryNumber) {
            transparencies[meshNumber].resize(boundaryNumber + 1, 1.0);
        }

        QWidget* w = ui->dataTable->cellWidget(row, 5);
        if (!w) continue;
        QDoubleSpinBox* spinBox = w->findChild<QDoubleSpinBox*>();
        if (spinBox) transparencies[meshNumber][boundaryNumber] = spinBox->value();
    }
}

void ControlPanel::setAllBoundaryTransparencies(const std::vector<std::vector<double>>& transparencies)
{
    for (int row = 0; row < rowTypes.size(); row++) {
        if (rowTypes[row] != BOUNDARY || row >= boundaryIndices.size()) continue;

        int meshNumber = boundaryIndices[row].first;
        int boundaryNumber = boundaryIndices[row].second;

        if (meshNumber >= transparencies.size() ||
            boundaryNumber >= transparencies[meshNumber].size()) continue;

        QWidget* w = ui->dataTable->cellWidget(row, 5);
        if (!w) continue;
        QDoubleSpinBox* spinBox = w->findChild<QDoubleSpinBox*>();
        if (spinBox) {
            spinBox->blockSignals(true);
            spinBox->setValue(transparencies[meshNumber][boundaryNumber]);
            spinBox->blockSignals(false);
        }
    }
}

void ControlPanel::showTransparencyContextMenu(const QPoint& pos)
{
    QList<int> selectedRows = getSelectedRows();
    if (selectedRows.isEmpty()) return;

    QMenu* menu = new QMenu(this);
    menu->setStyleSheet(
        "QMenu { background-color: white; border: 1px solid #ccc; }"
        "QMenu::item { padding: 5px 30px 5px 20px; }"
        "QMenu::item:selected { background-color: #0078d4; color: white; }"
    );

    QAction* action0   = menu->addAction("0 (Transparent)");
    QAction* action20  = menu->addAction("20");
    QAction* action40  = menu->addAction("40");
    QAction* action60  = menu->addAction("60");
    QAction* action80  = menu->addAction("80");
    QAction* action100 = menu->addAction("100 (Opaque)");
    menu->addSeparator();
    QAction* actionCustom = menu->addAction("Enter...");

    QAction* selectedAction = menu->exec(ui->dataTable->viewport()->mapToGlobal(pos));

    if      (selectedAction == action0)      applyTransparencyToSelected(0.0);
    else if (selectedAction == action20)     applyTransparencyToSelected(0.2);
    else if (selectedAction == action40)     applyTransparencyToSelected(0.4);
    else if (selectedAction == action60)     applyTransparencyToSelected(0.6);
    else if (selectedAction == action80)     applyTransparencyToSelected(0.8);
    else if (selectedAction == action100)    applyTransparencyToSelected(1.0);
    else if (selectedAction == actionCustom) showCustomTransparencyDialog();

    delete menu;
}

void ControlPanel::applyTransparencyToSelected(double value)
{
    QList<int> selectedRows = getSelectedRows();

    std::cout << "[Debug] Applying transparency " << value
              << " to " << selectedRows.size() << " selected rows" << std::endl;

    for (int row : selectedRows) {
        if (row >= rowTypes.size() || rowTypes[row] != BOUNDARY) continue;

        QWidget* w = ui->dataTable->cellWidget(row, 5);
        if (!w) continue;
        QDoubleSpinBox* spinBox = w->findChild<QDoubleSpinBox*>();
        if (!spinBox) continue;

        spinBox->blockSignals(true);
        spinBox->setValue(value);
        spinBox->blockSignals(false);

        if (row < boundaryIndices.size()) {
            int meshNumber = boundaryIndices[row].first;
            int boundaryNumber = boundaryIndices[row].second;
            emit boundaryTransparencyChanged(meshNumber, boundaryNumber, value);
            std::cout << "[Debug] Set boundary [" << meshNumber << "][" << boundaryNumber
                      << "] transparency to " << value << std::endl;
        }
    }
}

void ControlPanel::showCustomTransparencyDialog()
{
    bool ok;
    double value = QInputDialog::getDouble(
        this, "Set Transparency",
        "Enter transparency value (0-100):\n0 = Transparent, 100 = Opaque",
        50.0, 0.0, 100.0, 1, &ok
    );
    if (ok) applyTransparencyToSelected(value / 100.0);
}

// ─────────────────────────────────────────────────────────────────────────────
//  Section 6: Slice Control
// ─────────────────────────────────────────────────────────────────────────────

void ControlPanel::onSliceDeleteClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int cutplaneIndex = button->property("cutplaneIndex").toInt();
    emit sliceDeleteRequested(cutplaneIndex);
    removeCutplaneFromTable(cutplaneIndex);
}

void ControlPanel::onSliceContourModeChanged(const QString &text)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if (!comboBox) return;

    int cutplaneIndex = comboBox->property("cutplaneIndex").toInt();
    std::cout << "[Debug] Slice " << cutplaneIndex + 1
              << " contour mode changed to: " << text.toStdString() << std::endl;

    emit sliceContourModeChanged(text);
    syncSliceContourMode(text, cutplaneIndex);
}

void ControlPanel::syncSliceContourMode(const QString &text, int excludeCutplaneIndex)
{
    for (int i = 0; i < rowTypes.size(); i++) {
        if (rowTypes[i] != CUTPLANE) continue;

        int idx = i - boundaryIndices.size();
        if (idx < 0 || idx >= cutplaneInfos.size()) continue;
        if (cutplaneInfos[idx].index == excludeCutplaneIndex) continue;

        QWidget* comboWidget = ui->dataTable->cellWidget(i, 4);
        if (!comboWidget) continue;
        QComboBox* comboBox = comboWidget->findChild<QComboBox*>();
        if (!comboBox) continue;

        comboBox->blockSignals(true);
        comboBox->setCurrentText(text);
        comboBox->blockSignals(false);

        std::cout << "[Debug] Synced Slice " << cutplaneInfos[idx].index + 1
                  << " contour mode to: " << text.toStdString() << std::endl;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  Section 7: Event Handling
// ─────────────────────────────────────────────────────────────────────────────

QList<int> ControlPanel::getSelectedRows()
{
    QSet<int> rowSet;
    for (QTableWidgetItem* item : ui->dataTable->selectedItems()) {
        rowSet.insert(item->row());
    }
    QList<int> rows = rowSet.values();
    std::sort(rows.begin(), rows.end());
    return rows;
}

bool ControlPanel::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == ui->dataTable->viewport() && event->type() == QEvent::ContextMenu) {
        QContextMenuEvent* contextEvent = static_cast<QContextMenuEvent*>(event);
        QPoint pos = contextEvent->pos();
        int row = ui->dataTable->rowAt(pos.y());
        int col = ui->dataTable->columnAt(pos.x());

        if (row < 0 || getSelectedRows().isEmpty()) return true;

        if (col == 3) {
            showZoneContextMenu(pos);         // Show Zone → Activate/Deactivate
        }
        else if (col == 5) {
            showTransparencyContextMenu(pos); // Transculency → 透明度预设
        }
        // col 4 (ContourMode), col 6 (Zone Delete) 及其他列：不弹菜单

        return true;  // 拦截所有右键事件
    }

    return QDialog::eventFilter(obj, event);
}
