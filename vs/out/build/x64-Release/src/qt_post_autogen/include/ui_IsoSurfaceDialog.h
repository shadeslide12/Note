/********************************************************************************
** Form generated from reading UI file 'IsoSurfaceDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ISOSURFACEDIALOG_H
#define UI_ISOSURFACEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_IsoSurfaceDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer;
    QSlider *horizontalSlider;
    QLabel *labelParameterName;
    QLabel *lableColorBarmin;
    QComboBox *nameComboBox;
    QLabel *labelParameterName_2;
    QSpacerItem *horizontalSpacer_2;
    QDialogButtonBox *buttonBox;
    QLineEdit *textColorBarmin;
    QComboBox *nameComboBox_2;
    QLabel *label_2;

    void setupUi(QDialog *IsoSurfaceDialog)
    {
        if (IsoSurfaceDialog->objectName().isEmpty())
            IsoSurfaceDialog->setObjectName(QString::fromUtf8("IsoSurfaceDialog"));
        IsoSurfaceDialog->resize(402, 434);
        gridLayout = new QGridLayout(IsoSurfaceDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(IsoSurfaceDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(180, 50));
        label->setMaximumSize(QSize(180, 50));
        QFont font;
        font.setPointSize(12);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 3);

        label_4 = new QLabel(IsoSurfaceDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(160, 50));
        label_4->setMaximumSize(QSize(160, 50));
        QFont font1;
        font1.setPointSize(11);
        label_4->setFont(font1);

        gridLayout->addWidget(label_4, 3, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        horizontalSlider = new QSlider(IsoSurfaceDialog);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setFont(font1);
        horizontalSlider->setMaximum(100);
        horizontalSlider->setSingleStep(10);
        horizontalSlider->setSliderPosition(50);
        horizontalSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider, 3, 4, 1, 1);

        labelParameterName = new QLabel(IsoSurfaceDialog);
        labelParameterName->setObjectName(QString::fromUtf8("labelParameterName"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelParameterName->sizePolicy().hasHeightForWidth());
        labelParameterName->setSizePolicy(sizePolicy);
        labelParameterName->setMinimumSize(QSize(160, 50));
        labelParameterName->setMaximumSize(QSize(160, 50));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Sans Serif"));
        font2.setPointSize(11);
        labelParameterName->setFont(font2);

        gridLayout->addWidget(labelParameterName, 1, 2, 1, 1);

        lableColorBarmin = new QLabel(IsoSurfaceDialog);
        lableColorBarmin->setObjectName(QString::fromUtf8("lableColorBarmin"));
        lableColorBarmin->setMinimumSize(QSize(160, 50));
        lableColorBarmin->setMaximumSize(QSize(160, 50));
        lableColorBarmin->setFont(font1);

        gridLayout->addWidget(lableColorBarmin, 2, 2, 1, 1);

        nameComboBox = new QComboBox(IsoSurfaceDialog);
        nameComboBox->setObjectName(QString::fromUtf8("nameComboBox"));
        nameComboBox->setMinimumSize(QSize(160, 30));
        nameComboBox->setMaximumSize(QSize(160, 30));
        nameComboBox->setFont(font1);

        gridLayout->addWidget(nameComboBox, 1, 4, 1, 1);

        labelParameterName_2 = new QLabel(IsoSurfaceDialog);
        labelParameterName_2->setObjectName(QString::fromUtf8("labelParameterName_2"));
        sizePolicy.setHeightForWidth(labelParameterName_2->sizePolicy().hasHeightForWidth());
        labelParameterName_2->setSizePolicy(sizePolicy);
        labelParameterName_2->setMinimumSize(QSize(160, 50));
        labelParameterName_2->setMaximumSize(QSize(160, 50));
        labelParameterName_2->setFont(font2);

        gridLayout->addWidget(labelParameterName_2, 5, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 3, 0, 1, 1);

        buttonBox = new QDialogButtonBox(IsoSurfaceDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setMinimumSize(QSize(0, 80));
        buttonBox->setMaximumSize(QSize(16777215, 80));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 6, 2, 1, 3);

        textColorBarmin = new QLineEdit(IsoSurfaceDialog);
        textColorBarmin->setObjectName(QString::fromUtf8("textColorBarmin"));
        textColorBarmin->setMinimumSize(QSize(160, 30));
        textColorBarmin->setMaximumSize(QSize(160, 30));
        textColorBarmin->setFont(font1);

        gridLayout->addWidget(textColorBarmin, 2, 4, 1, 1);

        nameComboBox_2 = new QComboBox(IsoSurfaceDialog);
        nameComboBox_2->setObjectName(QString::fromUtf8("nameComboBox_2"));
        nameComboBox_2->setMinimumSize(QSize(160, 30));
        nameComboBox_2->setMaximumSize(QSize(160, 30));
        nameComboBox_2->setFont(font1);

        gridLayout->addWidget(nameComboBox_2, 5, 4, 1, 1);

        label_2 = new QLabel(IsoSurfaceDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(180, 50));
        label_2->setMaximumSize(QSize(180, 50));
        label_2->setFont(font);

        gridLayout->addWidget(label_2, 4, 0, 1, 3);


        retranslateUi(IsoSurfaceDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), IsoSurfaceDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), IsoSurfaceDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(IsoSurfaceDialog);
    } // setupUi

    void retranslateUi(QDialog *IsoSurfaceDialog)
    {
        IsoSurfaceDialog->setWindowTitle(QCoreApplication::translate("IsoSurfaceDialog", "IsoSurfaceDialog", nullptr));
        label->setText(QCoreApplication::translate("IsoSurfaceDialog", "Make Iso-Surface by:", nullptr));
        label_4->setText(QCoreApplication::translate("IsoSurfaceDialog", "quick Set Value", nullptr));
        labelParameterName->setText(QCoreApplication::translate("IsoSurfaceDialog", "parameter Name: ", nullptr));
        lableColorBarmin->setText(QCoreApplication::translate("IsoSurfaceDialog", "set Value", nullptr));
        labelParameterName_2->setText(QCoreApplication::translate("IsoSurfaceDialog", "parameter Name: ", nullptr));
        textColorBarmin->setText(QString());
        label_2->setText(QCoreApplication::translate("IsoSurfaceDialog", "Flood Iso-Surface by:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class IsoSurfaceDialog: public Ui_IsoSurfaceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ISOSURFACEDIALOG_H
