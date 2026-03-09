/********************************************************************************
** Form generated from reading UI file 'CalculatorDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALCULATORDIALOG_H
#define UI_CALCULATORDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_CalculatorDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QTextEdit *textEdit;
    QLabel *label_2;
    QTextEdit *textEdit_2;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CalculatorDialog)
    {
        if (CalculatorDialog->objectName().isEmpty())
            CalculatorDialog->setObjectName(QString::fromUtf8("CalculatorDialog"));
        CalculatorDialog->resize(401, 252);
        gridLayout = new QGridLayout(CalculatorDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(CalculatorDialog);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(11);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        textEdit = new QTextEdit(CalculatorDialog);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setMinimumSize(QSize(382, 0));
        textEdit->setMaximumSize(QSize(382, 16777215));
        textEdit->setFont(font);

        gridLayout->addWidget(textEdit, 1, 0, 1, 1);

        label_2 = new QLabel(CalculatorDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        textEdit_2 = new QTextEdit(CalculatorDialog);
        textEdit_2->setObjectName(QString::fromUtf8("textEdit_2"));
        textEdit_2->setMinimumSize(QSize(382, 30));
        textEdit_2->setMaximumSize(QSize(382, 30));
        textEdit_2->setFont(font);

        gridLayout->addWidget(textEdit_2, 3, 0, 1, 1);

        buttonBox = new QDialogButtonBox(CalculatorDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 4, 0, 1, 1);


        retranslateUi(CalculatorDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), CalculatorDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CalculatorDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(CalculatorDialog);
    } // setupUi

    void retranslateUi(QDialog *CalculatorDialog)
    {
        CalculatorDialog->setWindowTitle(QCoreApplication::translate("CalculatorDialog", "CalculatorDialog", nullptr));
        label->setText(QCoreApplication::translate("CalculatorDialog", "Expression:", nullptr));
        label_2->setText(QCoreApplication::translate("CalculatorDialog", "Variable Name:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CalculatorDialog: public Ui_CalculatorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALCULATORDIALOG_H
