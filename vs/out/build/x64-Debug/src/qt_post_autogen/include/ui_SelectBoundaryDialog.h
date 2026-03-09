/********************************************************************************
** Form generated from reading UI file 'SelectBoundaryDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTBOUNDARYDIALOG_H
#define UI_SELECTBOUNDARYDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_SelectBoundaryDialog
{
public:
    QGridLayout *gridLayout;
    QTreeWidget *treeWidget;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QDialog *SelectBoundaryDialog)
    {
        if (SelectBoundaryDialog->objectName().isEmpty())
            SelectBoundaryDialog->setObjectName(QString::fromUtf8("SelectBoundaryDialog"));
        SelectBoundaryDialog->resize(324, 530);
        gridLayout = new QGridLayout(SelectBoundaryDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        treeWidget = new QTreeWidget(SelectBoundaryDialog);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setMinimumSize(QSize(0, 0));

        gridLayout->addWidget(treeWidget, 0, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(217, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        pushButton = new QPushButton(SelectBoundaryDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 1, 1, 1, 1);


        retranslateUi(SelectBoundaryDialog);

        QMetaObject::connectSlotsByName(SelectBoundaryDialog);
    } // setupUi

    void retranslateUi(QDialog *SelectBoundaryDialog)
    {
        SelectBoundaryDialog->setWindowTitle(QCoreApplication::translate("SelectBoundaryDialog", "SelectActorDialog", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("SelectBoundaryDialog", "Actors", nullptr));
        pushButton->setText(QCoreApplication::translate("SelectBoundaryDialog", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SelectBoundaryDialog: public Ui_SelectBoundaryDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTBOUNDARYDIALOG_H
