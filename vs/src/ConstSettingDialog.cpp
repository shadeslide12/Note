//
// Created by admin on 2024/6/18.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ConstSettingDialog.h" resolved

#include "ConstSettingDialog.h"
#include "ui_ConstSettingDialog.h"
#include "iostream"

ConstSettingDialog::ConstSettingDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::ConstSettingDialog) {
    ui->setupUi(this);
    connect(ui->GenerateButton, SIGNAL(clicked()),this, SLOT(setHeight()));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)),this, SLOT(changeFlow(int)));
}

ConstSettingDialog::~ConstSettingDialog() {
    delete ui;
}

void ConstSettingDialog::setHeight()
{
    height = stod(ui->HeightlineEdit->text().toStdString());
    emit(finishSetHeight(height));
}

void ConstSettingDialog::setConstSettingDialog(std::vector<vtkAesReader::FlowData> flows, int flowNumber)
{
    for(auto &x : flows)
    {
        ui->comboBox->addItem(x.name.c_str());
    }
    ui->comboBox->setCurrentIndex(flowNumber);
}

void ConstSettingDialog::changeFlow(int number)
{
    int flow = ui->comboBox->currentIndex();
    emit(finishSetFlow(flow));
}