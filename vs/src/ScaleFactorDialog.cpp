#include "ScaleFactorDialog.h"
#include "ui_ScaleFactorDialog.h"
#include <string>
using std::stod;
using std::string;
#include <iostream>
using std::cout;
using std::endl;

using std::vector;
ScaleFactorDialog::ScaleFactorDialog(QWidget *parent): QDialog(parent),ui(new Ui::ScaleFactorDialog)
{
    ui->setupUi(this);
    cout << "create scalefactor dialog" << endl;
    connect(ui->ApplyButton, SIGNAL(clicked()), this, SLOT(ApplyButtonClicked()));
    connect(ui->CloseButton, SIGNAL(clicked()), this, SLOT(close()));
}

ScaleFactorDialog::~ScaleFactorDialog()
{
    delete ui;
    cout << "destroy scalefactor dialog" << endl;
}

void ScaleFactorDialog::SetScaleTextEdit(double s)
{
    ui->scaleFactor->setText(std::to_string(s).c_str());
}

void ScaleFactorDialog::SetVectorComponentsComboBox(std::vector<vtkAesReader::FlowData> flows, int *vectorComponentsNumber)
{
    for (auto &x : flows)
    {
        ui->uComboBox->addItem(x.name.c_str());
        ui->vComboBox->addItem(x.name.c_str());
        ui->wComboBox->addItem(x.name.c_str());
    }
    ui->uComboBox->setCurrentIndex(vectorComponentsNumber[0]);
    ui->vComboBox->setCurrentIndex(vectorComponentsNumber[1]);
    ui->wComboBox->setCurrentIndex(vectorComponentsNumber[2]);
    connect(ui->uComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ComboBoxIndexChanged()));
    connect(ui->vComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ComboBoxIndexChanged()));
    connect(ui->wComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ComboBoxIndexChanged()));
}

void ScaleFactorDialog::ApplyButtonClicked()
{
    double scaleFactor = -1;
    try
    {
        scaleFactor = stod(ui->scaleFactor->toPlainText().toStdString());
    }
    catch (const std::exception& e)
    {
        cout << "catch exception while setting parameters, please check." << endl << 
        "exception is: " << e.what() << endl;
        this->close();
        return;
    }
    if (scaleFactor <= 0) 
    {
        cout << "error scale factor" << endl;
        return;
    }
    cout << "the scaleFactor is set to " << scaleFactor << endl;
    emit(SetScaleFactor(scaleFactor));
}

void ScaleFactorDialog::ComboBoxIndexChanged()
{
    int number[3];
    number[0] = ui->uComboBox->currentIndex();
    number[1] = ui->vComboBox->currentIndex();
    number[2] = ui->wComboBox->currentIndex();
    emit ChangeVectorComponentsNumber(number);
}