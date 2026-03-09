//
// Created by admin on 2023/12/21.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MeridionalPlaneDialog.h" resolved

#include "MeridionalPlaneDialog.h"
#include "ui_MeridionalPlaneDialog.h"

#include "iostream"

MeridionalPlaneDialog::MeridionalPlaneDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::MeridionalPlaneDialog) {
    ui->setupUi(this);
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFlow(int)));
}

MeridionalPlaneDialog::~MeridionalPlaneDialog() {
    delete ui;
}

void MeridionalPlaneDialog::setDialog(std::vector<vtkAesReader::FlowData> flows, int flowNumber)
{
    for (auto &x : flows)
    {
        ui->comboBox->addItem(x.name.c_str());
    }
    ui->comboBox->setCurrentIndex(flowNumber);
}

void MeridionalPlaneDialog::changeFlow(int number)
{
    int flow = ui->comboBox->currentIndex();
    emit(finishSetMeiridonalFlow(flow));
}