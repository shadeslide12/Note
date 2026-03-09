#pragma once

#include <QDialog>

#include <vector>
#include <vtkSmartPointer.h>
#include <vtkPlane.h>

#include "vtkAesReader.h"

namespace Ui {
class ScaleFactorDialog;
}

class ScaleFactorDialog: public QDialog
{
    Q_OBJECT
    
public:
    ScaleFactorDialog(QWidget *parent = nullptr);
    ~ScaleFactorDialog();

    void SetScaleTextEdit(double);
    void SetVectorComponentsComboBox(std::vector<vtkAesReader::FlowData> flows,int *vectorComponentsNumber);

private slots:
    void ApplyButtonClicked();
    void ComboBoxIndexChanged();

signals:
    void SetScaleFactor(double scale);
    void ChangeVectorComponentsNumber(int *vectorComponentsNumber);

private:
    Ui::ScaleFactorDialog *ui;


};