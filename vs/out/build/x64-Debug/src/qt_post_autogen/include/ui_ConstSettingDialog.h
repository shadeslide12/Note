/********************************************************************************
** Form generated from reading UI file 'ConstSettingDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONSTSETTINGDIALOG_H
#define UI_CONSTSETTINGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ConstSettingDialog
{
public:
    QPushButton *GenerateButton;
    QLineEdit *HeightlineEdit;
    QLabel *label;
    QComboBox *comboBox;

    void setupUi(QDialog *ConstSettingDialog)
    {
        if (ConstSettingDialog->objectName().isEmpty())
            ConstSettingDialog->setObjectName(QString::fromUtf8("ConstSettingDialog"));
        ConstSettingDialog->resize(441, 343);
        GenerateButton = new QPushButton(ConstSettingDialog);
        GenerateButton->setObjectName(QString::fromUtf8("GenerateButton"));
        GenerateButton->setGeometry(QRect(250, 140, 80, 23));
        HeightlineEdit = new QLineEdit(ConstSettingDialog);
        HeightlineEdit->setObjectName(QString::fromUtf8("HeightlineEdit"));
        HeightlineEdit->setGeometry(QRect(130, 140, 113, 23));
        label = new QLabel(ConstSettingDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(80, 140, 41, 21));
        comboBox = new QComboBox(ConstSettingDialog);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(130, 90, 111, 21));

        retranslateUi(ConstSettingDialog);

        QMetaObject::connectSlotsByName(ConstSettingDialog);
    } // setupUi

    void retranslateUi(QDialog *ConstSettingDialog)
    {
        ConstSettingDialog->setWindowTitle(QCoreApplication::translate("ConstSettingDialog", "ConstSettingDialog", nullptr));
        GenerateButton->setText(QCoreApplication::translate("ConstSettingDialog", "Generate", nullptr));
        label->setText(QCoreApplication::translate("ConstSettingDialog", "Height", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConstSettingDialog: public Ui_ConstSettingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONSTSETTINGDIALOG_H
