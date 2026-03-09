/********************************************************************************
** Form generated from reading UI file 'ControlPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLPANEL_H
#define UI_CONTROLPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ControlPanel
{
public:
    QVBoxLayout *mainLayout;
    QTableWidget *dataTable;
    QHBoxLayout *buttonLayout;
    QPushButton *Btn_Reset;
    QSpacerItem *buttonHorizontalSpacer;
    QPushButton *Btn_Apply;
    QPushButton *Btn_Close;

    void setupUi(QWidget *ControlPanel)
    {
        if (ControlPanel->objectName().isEmpty())
            ControlPanel->setObjectName(QString::fromUtf8("ControlPanel"));
        ControlPanel->resize(829, 264);
        mainLayout = new QVBoxLayout(ControlPanel);
        mainLayout->setSpacing(5);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        mainLayout->setContentsMargins(10, 10, 10, 10);
        dataTable = new QTableWidget(ControlPanel);
        if (dataTable->columnCount() < 7)
            dataTable->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        dataTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        dataTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        dataTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        dataTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        dataTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        dataTable->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        dataTable->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        if (dataTable->rowCount() < 30)
            dataTable->setRowCount(30);
        dataTable->setObjectName(QString::fromUtf8("dataTable"));
        dataTable->setAlternatingRowColors(true);
        dataTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        dataTable->setGridStyle(Qt::SolidLine);
        dataTable->setRowCount(30);
        dataTable->setColumnCount(7);

        mainLayout->addWidget(dataTable);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName(QString::fromUtf8("buttonLayout"));
        Btn_Reset = new QPushButton(ControlPanel);
        Btn_Reset->setObjectName(QString::fromUtf8("Btn_Reset"));

        buttonLayout->addWidget(Btn_Reset);

        buttonHorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttonLayout->addItem(buttonHorizontalSpacer);

        Btn_Apply = new QPushButton(ControlPanel);
        Btn_Apply->setObjectName(QString::fromUtf8("Btn_Apply"));

        buttonLayout->addWidget(Btn_Apply);

        Btn_Close = new QPushButton(ControlPanel);
        Btn_Close->setObjectName(QString::fromUtf8("Btn_Close"));

        buttonLayout->addWidget(Btn_Close);


        mainLayout->addLayout(buttonLayout);


        retranslateUi(ControlPanel);

        QMetaObject::connectSlotsByName(ControlPanel);
    } // setupUi

    void retranslateUi(QWidget *ControlPanel)
    {
        ControlPanel->setWindowTitle(QCoreApplication::translate("ControlPanel", "Zone Style", nullptr));
        QTableWidgetItem *___qtablewidgetitem = dataTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("ControlPanel", "Zone\n"
"Number", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = dataTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("ControlPanel", "Zone\n"
"Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = dataTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("ControlPanel", "Zone\n"
"Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = dataTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("ControlPanel", "Show\n"
"Zone", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = dataTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("ControlPanel", "Contour\n"
"Mode", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = dataTable->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("ControlPanel", "Transculency", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = dataTable->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("ControlPanel", "Zone\n"
"Delete", nullptr));
        dataTable->setStyleSheet(QCoreApplication::translate("ControlPanel", "QHeaderView::section {\n"
"    background-color: #4472C4;\n"
"    color: white;\n"
"    padding: 4px;\n"
"    border: 1px solid #2E5AAC;\n"
"    font-weight: bold;\n"
"}", nullptr));
        Btn_Reset->setText(QCoreApplication::translate("ControlPanel", "Reset", nullptr));
        Btn_Apply->setText(QCoreApplication::translate("ControlPanel", "Apply", nullptr));
        Btn_Close->setText(QCoreApplication::translate("ControlPanel", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ControlPanel: public Ui_ControlPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLPANEL_H
