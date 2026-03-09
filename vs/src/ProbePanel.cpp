#include "ProbePanel.h"
#include "ui_ProbePanel.h"
#include <QFont>
#include <QDebug>
#include <QHideEvent>

ProbePanel::ProbePanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProbePanel)
{
    ui->setupUi(this);
    
    // 设置表格属性
    ui->probeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 只读
    ui->probeTable->setColumnWidth(0, 150);  // Variable 列宽度
    
    // 连接关闭按钮
    connect(ui->closeButton, &QPushButton::clicked, this, &ProbePanel::hide);
    
    // 连接清空按钮
    connect(ui->clearButton, &QPushButton::clicked, this, &ProbePanel::ClearData);
    
    // 设置关闭按钮样式
    ui->closeButton->setStyleSheet(
        "QPushButton {"
        "    border: none;"
        "    color: #666;"
        "    font-weight: bold;"
        "    font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #e81123;"
        "    color: white;"
        "    border-radius: 3px;"
        "}"
    );
    
    // 初始化提示文本
    ClearData();
    
    qDebug() << "ProbePanel created";
}

ProbePanel::~ProbePanel()
{
    delete ui;
}

void ProbePanel::UpdateProbeData(const std::string& coordinates, 
                                  const std::vector<std::pair<std::string, double>>& data)
{
    // 清空现有数据
    ui->probeTable->setRowCount(0);
    
    // 添加坐标信息（x, y, z 分三行）
    QString coordStr = QString::fromStdString(coordinates);
    QStringList coordLines = coordStr.split('\n', Qt::SkipEmptyParts);
    
    for (const QString& line : coordLines) {
        QStringList parts = line.split(':');
        if (parts.size() == 2) {
            int row = ui->probeTable->rowCount();
            ui->probeTable->insertRow(row);
            
            QTableWidgetItem *varItem = new QTableWidgetItem(parts[0].trimmed());
            QTableWidgetItem *valItem = new QTableWidgetItem(parts[1].trimmed());
            
            // 坐标行用粗体显示
            QFont boldFont;
            boldFont.setBold(true);
            varItem->setFont(boldFont);
            
            ui->probeTable->setItem(row, 0, varItem);
            ui->probeTable->setItem(row, 1, valItem);
        }
    }
    
    // 添加所有物理量数据
    for (const auto& [varName, value] : data) {
        int row = ui->probeTable->rowCount();
        ui->probeTable->insertRow(row);
        
        QTableWidgetItem *varItem = new QTableWidgetItem(QString::fromStdString(varName));
        QTableWidgetItem *valItem = new QTableWidgetItem(QString::number(value, 'g', 10));
        
        ui->probeTable->setItem(row, 0, varItem);
        ui->probeTable->setItem(row, 1, valItem);
    }
    
    qDebug() << "ProbePanel updated with" << data.size() << "variables";
}

void ProbePanel::ClearData()
{
    ui->probeTable->setRowCount(0);
    
    // 显示初始提示信息
    int row = ui->probeTable->rowCount();
    ui->probeTable->insertRow(row);
    
    QTableWidgetItem *item = new QTableWidgetItem("No data selected");
    QFont italicFont;
    italicFont.setItalic(true);
    item->setFont(italicFont);
    
    ui->probeTable->setItem(row, 0, item);
    ui->probeTable->setSpan(row, 0, 1, 2);  // 合并两列
}

void ProbePanel::hideEvent(QHideEvent* event)
{
    QWidget::hideEvent(event);
    emit panelClosed();  // 发出面板关闭信号
}
