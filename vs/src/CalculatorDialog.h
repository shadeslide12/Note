#pragma once

#include <QDialog>

#include <string>

namespace Ui {
class CalculatorDialog;
}

class CalculatorDialog: public QDialog
{
    Q_OBJECT
    
public:
    CalculatorDialog(QWidget *parent = nullptr);
    ~CalculatorDialog();

private slots:
    void emitSignal();
    
signals:
    void setParameter(QString, QString);

private:
    Ui::CalculatorDialog *ui;

};