#include "CalculatorDialog.h"
#include "ui_CalculatorDialog.h"

#include <QString>
#include <stdlib.h>
using std::string;
using std::vector;
using std::to_string;
using std::stod; using std::stoi;
#include <iostream>
using std::cout; using std::endl;

#include <vtkLookupTable.h>

CalculatorDialog::CalculatorDialog(QWidget *parent):QDialog(parent),ui(new Ui::CalculatorDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(emitSignal()));
}

CalculatorDialog::~CalculatorDialog()
{
    delete ui;
}

void CalculatorDialog::emitSignal()
{
    emit(setParameter(ui->textEdit->toPlainText(), ui->textEdit_2->toPlainText()));
}