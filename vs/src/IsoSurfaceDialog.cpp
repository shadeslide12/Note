#include "IsoSurfaceDialog.h"
#include "ui_IsoSurfaceDialog.h"

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

IsoSurfaceDialog::IsoSurfaceDialog(QWidget *parent): QDialog(parent),ui(new Ui::IsoSurfaceDialog)
{
    ui->setupUi(this);
    // connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(setParameters()));
    connect(ui->nameComboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(parameterChanged()));
    connect(ui->nameComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(flowNumberChanged()));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(textChanged(int)));
    connect(ui->horizontalSlider, SIGNAL(sliderReleased()), this, SLOT(valueChanged()));
    cout << "create iso-surface dialog" << endl;
}

IsoSurfaceDialog::~IsoSurfaceDialog()
{
    delete ui;
    cout << "destroy iso-surface dialog" << endl;
}

void IsoSurfaceDialog::setIsoSurfaceDialog(std::vector<vtkAesReader::FlowData> flows, int flowNumber, int floodNumber, double value)
{
    flowdata = flows;
    for (auto &x : flows)
    {
        ui->nameComboBox_2->addItem(x.name.c_str());
        ui->nameComboBox->addItem(x.name.c_str());
    }
    ui->textColorBarmin->setText(to_string(value).c_str());
    ui->nameComboBox->setCurrentIndex(flowNumber);
    ui->nameComboBox_2->setCurrentIndex(floodNumber);
}

void IsoSurfaceDialog::setParameters()
{
    // double value = stod(ui->textsurfaceValue->text().toStdString());
    // if (value < m || value > M)
    // {
    //     cout << "parameter value less than the min data or larger than the max data, please set the value again!" << endl;
    //     return;
    // }

    // emit(finishSetParameters(value));
}

void IsoSurfaceDialog::textChanged(int)
{
    auto range = this->flowdata[ui->nameComboBox->currentIndex()].range;
    double value = range[0] + (range[1]-range[0])*ui->horizontalSlider->value()/100.0;
    ui->textColorBarmin->setText(to_string(value).c_str());
}

void IsoSurfaceDialog::valueChanged()
{
    auto range = this->flowdata[ui->nameComboBox->currentIndex()].range;
    double value = range[0] + (range[1]-range[0])*ui->horizontalSlider->value()/100.0;
    ui->textColorBarmin->setText(to_string(value).c_str());
    emit(changeIsoSurfaceValue(value));
}

void IsoSurfaceDialog::parameterChanged()
{
    // int number = ui->comboBox->currentIndex();
    emit(changeFloodParameter(ui->nameComboBox_2->currentIndex()));
}

void IsoSurfaceDialog::flowNumberChanged()
{
    auto range = flowdata[ui->nameComboBox->currentIndex()].range;
    double value = range[0] + (range[1] - range[0]) * 0.5;
    ui->textColorBarmin->setText(to_string(value).c_str());
    ui->nameComboBox_2->setCurrentIndex(ui->nameComboBox->currentIndex());
    emit(changeFlowParameter(ui->nameComboBox->currentIndex()));
}