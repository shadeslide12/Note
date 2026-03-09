/********************************************************************************
** Form generated from reading UI file 'ConstHeightPlaneDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONSTHEIGHTPLANEDIALOG_H
#define UI_CONSTHEIGHTPLANEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ConstHeightPlaneDialog
{
public:
    QPushButton *pushButton;
    QLabel *label_2;
    QLineEdit *heightLineEdit;

    void setupUi(QDialog *ConstHeightPlaneDialog)
    {
        if (ConstHeightPlaneDialog->objectName().isEmpty())
            ConstHeightPlaneDialog->setObjectName(QString::fromUtf8("ConstHeightPlaneDialog"));
        ConstHeightPlaneDialog->resize(400, 300);
        pushButton = new QPushButton(ConstHeightPlaneDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(140, 210, 91, 21));
        QFont font;
        font.setPointSize(11);
        pushButton->setFont(font);
        label_2 = new QLabel(ConstHeightPlaneDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(50, 110, 101, 16));
        label_2->setFont(font);
        heightLineEdit = new QLineEdit(ConstHeightPlaneDialog);
        heightLineEdit->setObjectName(QString::fromUtf8("heightLineEdit"));
        heightLineEdit->setGeometry(QRect(170, 100, 113, 23));

        retranslateUi(ConstHeightPlaneDialog);

        QMetaObject::connectSlotsByName(ConstHeightPlaneDialog);
    } // setupUi

    void retranslateUi(QDialog *ConstHeightPlaneDialog)
    {
        ConstHeightPlaneDialog->setWindowTitle(QCoreApplication::translate("ConstHeightPlaneDialog", "ConstHeightPlaneDialog", nullptr));
        pushButton->setText(QCoreApplication::translate("ConstHeightPlaneDialog", "Generate", nullptr));
        label_2->setText(QCoreApplication::translate("ConstHeightPlaneDialog", "Height", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConstHeightPlaneDialog: public Ui_ConstHeightPlaneDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONSTHEIGHTPLANEDIALOG_H
