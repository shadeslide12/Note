/********************************************************************************
** Form generated from reading UI file 'ScaleFactorDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCALEFACTORDIALOG_H
#define UI_SCALEFACTORDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_ScaleFactorDialog
{
public:
    QGridLayout *gridLayout;
    QTextEdit *scaleFactor;
    QComboBox *vComboBox;
    QLabel *uLabel;
    QComboBox *uComboBox;
    QLabel *vLabel;
    QLabel *scaleFactorLabel;
    QLabel *wLabel;
    QComboBox *wComboBox;
    QPushButton *ApplyButton;
    QPushButton *CloseButton;

    void setupUi(QDialog *ScaleFactorDialog)
    {
        if (ScaleFactorDialog->objectName().isEmpty())
            ScaleFactorDialog->setObjectName(QString::fromUtf8("ScaleFactorDialog"));
        ScaleFactorDialog->resize(402, 370);
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        ScaleFactorDialog->setFont(font);
        gridLayout = new QGridLayout(ScaleFactorDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        scaleFactor = new QTextEdit(ScaleFactorDialog);
        scaleFactor->setObjectName(QString::fromUtf8("scaleFactor"));
        scaleFactor->setMinimumSize(QSize(150, 30));
        scaleFactor->setMaximumSize(QSize(150, 30));

        gridLayout->addWidget(scaleFactor, 3, 1, 1, 1);

        vComboBox = new QComboBox(ScaleFactorDialog);
        vComboBox->setObjectName(QString::fromUtf8("vComboBox"));
        vComboBox->setMinimumSize(QSize(150, 30));
        vComboBox->setMaximumSize(QSize(150, 30));

        gridLayout->addWidget(vComboBox, 1, 1, 1, 1);

        uLabel = new QLabel(ScaleFactorDialog);
        uLabel->setObjectName(QString::fromUtf8("uLabel"));
        uLabel->setMinimumSize(QSize(150, 30));
        uLabel->setMaximumSize(QSize(150, 30));
        uLabel->setFont(font);

        gridLayout->addWidget(uLabel, 0, 0, 1, 1);

        uComboBox = new QComboBox(ScaleFactorDialog);
        uComboBox->setObjectName(QString::fromUtf8("uComboBox"));
        uComboBox->setMinimumSize(QSize(150, 30));
        uComboBox->setMaximumSize(QSize(150, 30));

        gridLayout->addWidget(uComboBox, 0, 1, 1, 1);

        vLabel = new QLabel(ScaleFactorDialog);
        vLabel->setObjectName(QString::fromUtf8("vLabel"));
        vLabel->setMinimumSize(QSize(150, 30));
        vLabel->setMaximumSize(QSize(150, 30));
        vLabel->setFont(font);

        gridLayout->addWidget(vLabel, 1, 0, 1, 1);

        scaleFactorLabel = new QLabel(ScaleFactorDialog);
        scaleFactorLabel->setObjectName(QString::fromUtf8("scaleFactorLabel"));
        scaleFactorLabel->setMinimumSize(QSize(150, 30));
        scaleFactorLabel->setMaximumSize(QSize(150, 30));
        scaleFactorLabel->setFont(font);

        gridLayout->addWidget(scaleFactorLabel, 3, 0, 1, 1);

        wLabel = new QLabel(ScaleFactorDialog);
        wLabel->setObjectName(QString::fromUtf8("wLabel"));
        wLabel->setMinimumSize(QSize(150, 30));
        wLabel->setMaximumSize(QSize(150, 30));
        wLabel->setFont(font);

        gridLayout->addWidget(wLabel, 2, 0, 1, 1);

        wComboBox = new QComboBox(ScaleFactorDialog);
        wComboBox->setObjectName(QString::fromUtf8("wComboBox"));
        wComboBox->setMinimumSize(QSize(150, 30));
        wComboBox->setMaximumSize(QSize(150, 30));

        gridLayout->addWidget(wComboBox, 2, 1, 1, 1);

        ApplyButton = new QPushButton(ScaleFactorDialog);
        ApplyButton->setObjectName(QString::fromUtf8("ApplyButton"));

        gridLayout->addWidget(ApplyButton, 4, 0, 1, 1);

        CloseButton = new QPushButton(ScaleFactorDialog);
        CloseButton->setObjectName(QString::fromUtf8("CloseButton"));

        gridLayout->addWidget(CloseButton, 4, 1, 1, 1);


        retranslateUi(ScaleFactorDialog);

        QMetaObject::connectSlotsByName(ScaleFactorDialog);
    } // setupUi

    void retranslateUi(QDialog *ScaleFactorDialog)
    {
        ScaleFactorDialog->setWindowTitle(QCoreApplication::translate("ScaleFactorDialog", "ScaleFactorDialog", nullptr));
        uLabel->setText(QCoreApplication::translate("ScaleFactorDialog", "           U", nullptr));
        vLabel->setText(QCoreApplication::translate("ScaleFactorDialog", "           V", nullptr));
        scaleFactorLabel->setText(QCoreApplication::translate("ScaleFactorDialog", "    ScaleFactor:", nullptr));
        wLabel->setText(QCoreApplication::translate("ScaleFactorDialog", "           W", nullptr));
        ApplyButton->setText(QCoreApplication::translate("ScaleFactorDialog", "Apply", nullptr));
        CloseButton->setText(QCoreApplication::translate("ScaleFactorDialog", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScaleFactorDialog: public Ui_ScaleFactorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCALEFACTORDIALOG_H
