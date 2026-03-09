//
// Created by admin on 2023/12/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ConstHeightPlaneDialog.h" resolved

#include "ConstHeightPlaneDialog.h"
#include "ui_ConstHeightPlaneDialog.h"


ConstHeightPlaneDialog::ConstHeightPlaneDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::ConstHeightPlaneDialog) {
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()), this, SLOT(setParameters()));
}

ConstHeightPlaneDialog::~ConstHeightPlaneDialog() {
    delete ui;
}

void ConstHeightPlaneDialog::setParameters()
{
    height = stod(ui->heightLineEdit->text().toStdString());
    emit(finishSetParameters(height));
    this->close();
}

