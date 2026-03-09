#pragma once

#include <QDialog>

#include <string>
using std::string;
using std::to_string;
using std::stod; 

#include "vtkAesReader.h"

namespace Ui {
class IsoSurfaceDialog;
}

class IsoSurfaceDialog: public QDialog
{
    Q_OBJECT
    
public:
    IsoSurfaceDialog(QWidget *parent = nullptr);
    ~IsoSurfaceDialog();

    void setIsoSurfaceDialog(std::vector<vtkAesReader::FlowData> flows, int flowNumber, int floodNumber, double value);

private slots:
    void setParameters();
    void parameterChanged();
    void flowNumberChanged();
    void valueChanged();
    void textChanged(int );

signals:
    void finishSetParameters(double);
    void changeFloodParameter(int);
    void changeFlowParameter(int);
    void changeIsoSurfaceValue(double);

private:
    std::vector<vtkAesReader::FlowData> flowdata;
    Ui::IsoSurfaceDialog *ui;
    double M;
    double m;
    double value;
};