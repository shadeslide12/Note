#include "SelectBoundaryDialog.h"
#include "ui_SelectBoundaryDialog.h"

#include <string>
using std::string;
using std::stoi;
using std::to_string;

SelectBoundaryDialog::SelectBoundaryDialog(QWidget *parent):QDialog(parent),ui(new Ui::SelectBoundaryDialog)
{
    ui->setupUi(this);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(close()));
    cout << "create select boundary dialog" << endl;
}

SelectBoundaryDialog::~SelectBoundaryDialog()
{
    cout << "delete select boundary dialog" << endl;
    delete ui;
}

void SelectBoundaryDialog::setSelectBoundaryDialog(std::vector<std::vector<vtkAesReader::Boundary>> boundarys)
{
    QTreeWidgetItem *boundarysTopItem = new QTreeWidgetItem(ui->treeWidget);
    boundarysTopItem->setText(0, "Boundarys");
    boundarysTopItem->setCheckState(0, Qt::Checked);
    boundarysTopItem->setExpanded(true);

    ui->treeWidget->addTopLevelItem(boundarysTopItem);
    for (int i = 0; i < boundarys.size(); i++)
    {
        QTreeWidgetItem *meshItem = new QTreeWidgetItem(boundarysTopItem);
        meshItem->setText(0, boundarys[i][0].zoneName.c_str());
        meshItem->setCheckState(0, Qt::Checked);
        meshItem->setExpanded(true);
        for (int j = 0; j < boundarys[i].size(); j++)
        {
            QTreeWidgetItem *boundaryItem = new QTreeWidgetItem(meshItem);
            boundaryItem->setText(0, boundarys[i][j].name.c_str());
            boundaryItem->setCheckState(0, Qt::Checked);
        }
    }

    connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(treeItemChanged(QTreeWidgetItem*,int)));
}

void SelectBoundaryDialog::setSelectCutplaneItems()
{
    cutplaneTopItem = new QTreeWidgetItem(ui->treeWidget);
    cutplaneTopItem->setText(0, "Cutplanes");
    cutplaneTopItem->setCheckState(0,Qt::Checked);
    cutplaneTopItem->setExpanded(true);
    ui->treeWidget->addTopLevelItem(cutplaneTopItem);

        string name = "cutplane";
        name += to_string(number);
        number++;
        QTreeWidgetItem *cutplaneItem = new QTreeWidgetItem(cutplaneTopItem);
        cutplaneItem->setText(0, name.c_str());
        cutplaneItem->setCheckState(0, Qt::Checked);
}

void SelectBoundaryDialog::addCutplaneItem()
{
        string name = "cutplane";
        name += to_string(number);
        number++;
        QTreeWidgetItem *cutplaneItem = new QTreeWidgetItem(cutplaneTopItem);
        cutplaneItem->setText(0, name.c_str());
        cutplaneItem->setCheckState(0, Qt::Checked);
}

void SelectBoundaryDialog::treeItemChanged(QTreeWidgetItem* item, int c)
{
    if (item->childCount() == 0)
    {
        if (item->parent() == nullptr) return;
        if (item->parent()->parent() != nullptr)
        {
            int nmesh = item->parent()->parent()->indexOfChild(item->parent());
            int nboundary = item->parent()->indexOfChild(item);
            bool flag = (item->checkState(0) == Qt::Checked);
            emit(setBoundarys(nmesh, nboundary, flag));
        }
        else if (item->parent()->parent() == nullptr)
        {
            bool flag = (item->checkState(0) == Qt::Checked);
            emit(setCutplaneVisiable(item->parent()->indexOfChild(item), flag));
        }
    }
    else if (item->childCount() != 0)
    {
        for (int i = 0; i < item->childCount(); i++)
        {
            item->child(i)->setCheckState(0, item->checkState(0));
        }
    }
}
