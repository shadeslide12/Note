#include "PropertyPanel.h"
#include <QHeaderView>
#include <QPushButton>
#include <QColorDialog>
#include <QDebug>

PropertyPanel::PropertyPanel(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupConnections();
}

PropertyPanel::~PropertyPanel()
{
}

void PropertyPanel::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // 创建标题标签
    titleLabel = new QLabel("属性", this);
    titleLabel->setStyleSheet("QLabel { font-weight: bold; padding: 8px; background-color: #f5f5f5; border-bottom: 1px solid #d0d0d0; }");
    mainLayout->addWidget(titleLabel);
    
    // 创建属性表格
    propertyTable = new QTableWidget(this);
    propertyTable->setColumnCount(2);
    propertyTable->setHorizontalHeaderLabels({"属性", "值"});
    propertyTable->horizontalHeader()->setStretchLastSection(true);
    propertyTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    propertyTable->verticalHeader()->setVisible(false);
    propertyTable->setAlternatingRowColors(true);
    propertyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    propertyTable->setSelectionMode(QAbstractItemView::SingleSelection);
    propertyTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 使用自定义编辑器
    
    mainLayout->addWidget(propertyTable);
    
    setLayout(mainLayout);
}

void PropertyPanel::setupConnections()
{
    connect(propertyTable, &QTableWidget::cellChanged,
            this, &PropertyPanel::onCellChanged);
}

void PropertyPanel::clearProperties()
{
    propertyTable->setRowCount(0);
    propertyTypes.clear();
    propertyRows.clear();
}

void PropertyPanel::addStringProperty(const QString &name, const QString &value)
{
    int row = propertyTable->rowCount();
    propertyTable->insertRow(row);
    
    QTableWidgetItem *nameItem = new QTableWidgetItem(name);
    nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
    propertyTable->setItem(row, 0, nameItem);
    
    QTableWidgetItem *valueItem = new QTableWidgetItem(value);
    propertyTable->setItem(row, 1, valueItem);
    
    propertyTypes[name] = StringType;
    propertyRows[name] = row;
}

void PropertyPanel::addIntProperty(const QString &name, int value, int min, int max)
{
    int row = propertyTable->rowCount();
    propertyTable->insertRow(row);
    
    QTableWidgetItem *nameItem = new QTableWidgetItem(name);
    nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
    propertyTable->setItem(row, 0, nameItem);
    
    QSpinBox *spinBox = new QSpinBox(this);
    spinBox->setMinimum(min);
    spinBox->setMaximum(max);
    spinBox->setValue(value);
    connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this, name](int val) {
        emit propertyValueChanged(name, val);
    });
    
    propertyTable->setCellWidget(row, 1, spinBox);
    
    propertyTypes[name] = IntType;
    propertyRows[name] = row;
}

void PropertyPanel::addDoubleProperty(const QString &name, double value, double min, double max, int decimals)
{
    int row = propertyTable->rowCount();
    propertyTable->insertRow(row);
    
    QTableWidgetItem *nameItem = new QTableWidgetItem(name);
    nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
    propertyTable->setItem(row, 0, nameItem);
    
    QDoubleSpinBox *spinBox = new QDoubleSpinBox(this);
    spinBox->setMinimum(min);
    spinBox->setMaximum(max);
    spinBox->setDecimals(decimals);
    spinBox->setValue(value);
    connect(spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this, name](double val) {
        emit propertyValueChanged(name, val);
    });
    
    propertyTable->setCellWidget(row, 1, spinBox);
    
    propertyTypes[name] = DoubleType;
    propertyRows[name] = row;
}

void PropertyPanel::addBoolProperty(const QString &name, bool value)
{
    int row = propertyTable->rowCount();
    propertyTable->insertRow(row);
    
    QTableWidgetItem *nameItem = new QTableWidgetItem(name);
    nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
    propertyTable->setItem(row, 0, nameItem);
    
    QCheckBox *checkBox = new QCheckBox(this);
    checkBox->setChecked(value);
    connect(checkBox, &QCheckBox::toggled, [this, name](bool checked) {
        emit propertyValueChanged(name, checked);
    });
    
    QWidget *container = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->addWidget(checkBox);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    container->setLayout(layout);
    
    propertyTable->setCellWidget(row, 1, container);
    
    propertyTypes[name] = BoolType;
    propertyRows[name] = row;
}

void PropertyPanel::addColorProperty(const QString &name, const QColor &value)
{
    int row = propertyTable->rowCount();
    propertyTable->insertRow(row);
    
    QTableWidgetItem *nameItem = new QTableWidgetItem(name);
    nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
    propertyTable->setItem(row, 0, nameItem);
    
    QPushButton *colorButton = new QPushButton(this);
    colorButton->setStyleSheet(QString("background-color: %1; border: 1px solid #d0d0d0;").arg(value.name()));
    colorButton->setMaximumHeight(30);
    
    connect(colorButton, &QPushButton::clicked, [this, name, colorButton]() {
        QColor currentColor = colorButton->palette().button().color();
        QColor newColor = QColorDialog::getColor(currentColor, this, "选择颜色");
        if (newColor.isValid()) {
            colorButton->setStyleSheet(QString("background-color: %1; border: 1px solid #d0d0d0;").arg(newColor.name()));
            emit propertyValueChanged(name, newColor);
        }
    });
    
    propertyTable->setCellWidget(row, 1, colorButton);
    
    propertyTypes[name] = ColorType;
    propertyRows[name] = row;
}

void PropertyPanel::addComboProperty(const QString &name, const QStringList &options, const QString &currentValue)
{
    int row = propertyTable->rowCount();
    propertyTable->insertRow(row);
    
    QTableWidgetItem *nameItem = new QTableWidgetItem(name);
    nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
    propertyTable->setItem(row, 0, nameItem);
    
    QComboBox *comboBox = new QComboBox(this);
    comboBox->addItems(options);
    comboBox->setCurrentText(currentValue);
    connect(comboBox, &QComboBox::currentTextChanged, [this, name](const QString &text) {
        emit propertyValueChanged(name, text);
    });
    
    propertyTable->setCellWidget(row, 1, comboBox);
    
    propertyTypes[name] = ComboType;
    propertyRows[name] = row;
}

QVariant PropertyPanel::getPropertyValue(const QString &name) const
{
    if (!propertyRows.contains(name)) {
        return QVariant();
    }
    
    int row = propertyRows[name];
    PropertyType type = propertyTypes[name];
    
    switch (type) {
        case StringType: {
            QTableWidgetItem *item = propertyTable->item(row, 1);
            return item ? item->text() : QString();
        }
        case IntType: {
            QSpinBox *spinBox = qobject_cast<QSpinBox*>(propertyTable->cellWidget(row, 1));
            return spinBox ? spinBox->value() : 0;
        }
        case DoubleType: {
            QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox*>(propertyTable->cellWidget(row, 1));
            return spinBox ? spinBox->value() : 0.0;
        }
        case BoolType: {
            QWidget *container = propertyTable->cellWidget(row, 1);
            QCheckBox *checkBox = container ? container->findChild<QCheckBox*>() : nullptr;
            return checkBox ? checkBox->isChecked() : false;
        }
        case ColorType: {
            QPushButton *button = qobject_cast<QPushButton*>(propertyTable->cellWidget(row, 1));
            return button ? button->palette().button().color() : QColor();
        }
        case ComboType: {
            QComboBox *comboBox = qobject_cast<QComboBox*>(propertyTable->cellWidget(row, 1));
            return comboBox ? comboBox->currentText() : QString();
        }
        default:
            return QVariant();
    }
}

void PropertyPanel::setPropertyValue(const QString &name, const QVariant &value)
{
    if (!propertyRows.contains(name)) {
        return;
    }
    
    int row = propertyRows[name];
    PropertyType type = propertyTypes[name];
    
    switch (type) {
        case StringType: {
            QTableWidgetItem *item = propertyTable->item(row, 1);
            if (item) {
                item->setText(value.toString());
            }
            break;
        }
        case IntType: {
            QSpinBox *spinBox = qobject_cast<QSpinBox*>(propertyTable->cellWidget(row, 1));
            if (spinBox) {
                spinBox->setValue(value.toInt());
            }
            break;
        }
        case DoubleType: {
            QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox*>(propertyTable->cellWidget(row, 1));
            if (spinBox) {
                spinBox->setValue(value.toDouble());
            }
            break;
        }
        case BoolType: {
            QWidget *container = propertyTable->cellWidget(row, 1);
            QCheckBox *checkBox = container ? container->findChild<QCheckBox*>() : nullptr;
            if (checkBox) {
                checkBox->setChecked(value.toBool());
            }
            break;
        }
        case ColorType: {
            QPushButton *button = qobject_cast<QPushButton*>(propertyTable->cellWidget(row, 1));
            if (button && value.canConvert<QColor>()) {
                QColor color = value.value<QColor>();
                button->setStyleSheet(QString("background-color: %1; border: 1px solid #d0d0d0;").arg(color.name()));
            }
            break;
        }
        case ComboType: {
            QComboBox *comboBox = qobject_cast<QComboBox*>(propertyTable->cellWidget(row, 1));
            if (comboBox) {
                comboBox->setCurrentText(value.toString());
            }
            break;
        }
    }
}

void PropertyPanel::setObjectName(const QString &name)
{
    titleLabel->setText("属性 - " + name);
}

void PropertyPanel::onCellChanged(int row, int column)
{
    if (column != 1) {
        return;
    }
    
    // 查找属性名称
    QString propertyName;
    for (auto it = propertyRows.constBegin(); it != propertyRows.constEnd(); ++it) {
        if (it.value() == row) {
            propertyName = it.key();
            break;
        }
    }
    
    if (propertyName.isEmpty()) {
        return;
    }
    
    // 只有StringType使用cellChanged信号
    if (propertyTypes[propertyName] == StringType) {
        QTableWidgetItem *item = propertyTable->item(row, 1);
        if (item) {
            emit propertyValueChanged(propertyName, item->text());
        }
    }
}
