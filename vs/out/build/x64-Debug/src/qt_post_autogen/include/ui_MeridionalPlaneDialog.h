/********************************************************************************
** Form generated from reading UI file 'MeridionalPlaneDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MERIDIONALPLANEDIALOG_H
#define UI_MERIDIONALPLANEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_MeridionalPlaneDialog
{
public:
    QComboBox *comboBox;

    void setupUi(QDialog *MeridionalPlaneDialog)
    {
        if (MeridionalPlaneDialog->objectName().isEmpty())
            MeridionalPlaneDialog->setObjectName(QString::fromUtf8("MeridionalPlaneDialog"));
        MeridionalPlaneDialog->resize(462, 368);
        QFont font;
        font.setPointSize(11);
        MeridionalPlaneDialog->setFont(font);
        comboBox = new QComboBox(MeridionalPlaneDialog);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(140, 110, 101, 21));

        retranslateUi(MeridionalPlaneDialog);

        QMetaObject::connectSlotsByName(MeridionalPlaneDialog);
    } // setupUi

    void retranslateUi(QDialog *MeridionalPlaneDialog)
    {
        MeridionalPlaneDialog->setWindowTitle(QCoreApplication::translate("MeridionalPlaneDialog", "MeridionalPlaneDialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MeridionalPlaneDialog: public Ui_MeridionalPlaneDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MERIDIONALPLANEDIALOG_H
