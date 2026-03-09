#pragma once

#include <QDialog>

#include <string>
#include <vector>

#include "vtkAesReader.h"

#include <QTreeWidgetItem>

namespace Ui {
class SelectBoundaryDialog;
}

class SelectBoundaryDialog: public QDialog
{
    Q_OBJECT
    
public:
    SelectBoundaryDialog(QWidget *parent = nullptr);
    ~SelectBoundaryDialog();

    void setSelectBoundaryDialog(std::vector<std::vector<vtkAesReader::Boundary>> boundarys);
    void setSelectCutplaneItems();
    void addCutplaneItem();

private slots:
    void treeItemChanged(QTreeWidgetItem* item,int c);

signals:
    void setBoundarys(int,int, bool);
    void setCutplaneVisiable(int, bool);

private:
    Ui::SelectBoundaryDialog *ui;

    QTreeWidgetItem *cutplaneTopItem;
    int number = 1;
};