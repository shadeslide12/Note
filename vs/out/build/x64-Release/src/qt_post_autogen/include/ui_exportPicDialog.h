/********************************************************************************
** Form generated from reading UI file 'exportPicDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPORTPICDIALOG_H
#define UI_EXPORTPICDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ExportPicDialog
{
public:
    QVBoxLayout *mainLayout;
    QGroupBox *fileGroupBox;
    QVBoxLayout *fileLayout;
    QHBoxLayout *pathLayout;
    QLabel *pathLabel;
    QLineEdit *pathLineEdit;
    QPushButton *browseButton;
    QHBoxLayout *fileNameLayout;
    QLabel *fileNameLabel;
    QLineEdit *fileNameLineEdit;
    QHBoxLayout *formatLayout;
    QLabel *formatLabel;
    QComboBox *formatComboBox;
    QSpacerItem *formatSpacer;
    QGroupBox *viewGroupBox;
    QVBoxLayout *viewLayout;
    QRadioButton *mainViewRadio;
    QRadioButton *meridionalViewRadio;
    QRadioButton *bladeToBladeViewRadio;
    QGroupBox *imageGroupBox;
    QGridLayout *imageLayout;
    QLabel *resolutionLabel;
    QComboBox *resolutionComboBox;
    QLabel *heightLabel;
    QSpinBox *widthSpinBox;
    QLabel *widthLabel;
    QSpinBox *heightSpinBox;
    QHBoxLayout *buttonLayout;
    QSpacerItem *buttonSpacer;
    QPushButton *exportButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *ExportPicDialog)
    {
        if (ExportPicDialog->objectName().isEmpty())
            ExportPicDialog->setObjectName(QString::fromUtf8("ExportPicDialog"));
        ExportPicDialog->resize(500, 526);
        QFont font;
        font.setPointSize(10);
        ExportPicDialog->setFont(font);
        mainLayout = new QVBoxLayout(ExportPicDialog);
        mainLayout->setSpacing(10);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        mainLayout->setContentsMargins(15, 15, 15, 15);
        fileGroupBox = new QGroupBox(ExportPicDialog);
        fileGroupBox->setObjectName(QString::fromUtf8("fileGroupBox"));
        fileLayout = new QVBoxLayout(fileGroupBox);
        fileLayout->setObjectName(QString::fromUtf8("fileLayout"));
        pathLayout = new QHBoxLayout();
        pathLayout->setObjectName(QString::fromUtf8("pathLayout"));
        pathLabel = new QLabel(fileGroupBox);
        pathLabel->setObjectName(QString::fromUtf8("pathLabel"));

        pathLayout->addWidget(pathLabel);

        pathLineEdit = new QLineEdit(fileGroupBox);
        pathLineEdit->setObjectName(QString::fromUtf8("pathLineEdit"));
        pathLineEdit->setReadOnly(true);

        pathLayout->addWidget(pathLineEdit);

        browseButton = new QPushButton(fileGroupBox);
        browseButton->setObjectName(QString::fromUtf8("browseButton"));

        pathLayout->addWidget(browseButton);


        fileLayout->addLayout(pathLayout);

        fileNameLayout = new QHBoxLayout();
        fileNameLayout->setObjectName(QString::fromUtf8("fileNameLayout"));
        fileNameLabel = new QLabel(fileGroupBox);
        fileNameLabel->setObjectName(QString::fromUtf8("fileNameLabel"));

        fileNameLayout->addWidget(fileNameLabel);

        fileNameLineEdit = new QLineEdit(fileGroupBox);
        fileNameLineEdit->setObjectName(QString::fromUtf8("fileNameLineEdit"));

        fileNameLayout->addWidget(fileNameLineEdit);


        fileLayout->addLayout(fileNameLayout);

        formatLayout = new QHBoxLayout();
        formatLayout->setObjectName(QString::fromUtf8("formatLayout"));
        formatLabel = new QLabel(fileGroupBox);
        formatLabel->setObjectName(QString::fromUtf8("formatLabel"));

        formatLayout->addWidget(formatLabel);

        formatComboBox = new QComboBox(fileGroupBox);
        formatComboBox->addItem(QString());
        formatComboBox->addItem(QString());
        formatComboBox->setObjectName(QString::fromUtf8("formatComboBox"));

        formatLayout->addWidget(formatComboBox);

        formatSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formatLayout->addItem(formatSpacer);


        fileLayout->addLayout(formatLayout);


        mainLayout->addWidget(fileGroupBox);

        viewGroupBox = new QGroupBox(ExportPicDialog);
        viewGroupBox->setObjectName(QString::fromUtf8("viewGroupBox"));
        viewLayout = new QVBoxLayout(viewGroupBox);
        viewLayout->setObjectName(QString::fromUtf8("viewLayout"));
        mainViewRadio = new QRadioButton(viewGroupBox);
        mainViewRadio->setObjectName(QString::fromUtf8("mainViewRadio"));
        mainViewRadio->setChecked(true);

        viewLayout->addWidget(mainViewRadio);

        meridionalViewRadio = new QRadioButton(viewGroupBox);
        meridionalViewRadio->setObjectName(QString::fromUtf8("meridionalViewRadio"));

        viewLayout->addWidget(meridionalViewRadio);

        bladeToBladeViewRadio = new QRadioButton(viewGroupBox);
        bladeToBladeViewRadio->setObjectName(QString::fromUtf8("bladeToBladeViewRadio"));

        viewLayout->addWidget(bladeToBladeViewRadio);


        mainLayout->addWidget(viewGroupBox);

        imageGroupBox = new QGroupBox(ExportPicDialog);
        imageGroupBox->setObjectName(QString::fromUtf8("imageGroupBox"));
        imageLayout = new QGridLayout(imageGroupBox);
        imageLayout->setObjectName(QString::fromUtf8("imageLayout"));
        resolutionLabel = new QLabel(imageGroupBox);
        resolutionLabel->setObjectName(QString::fromUtf8("resolutionLabel"));

        imageLayout->addWidget(resolutionLabel, 0, 0, 1, 1);

        resolutionComboBox = new QComboBox(imageGroupBox);
        resolutionComboBox->addItem(QString());
        resolutionComboBox->addItem(QString());
        resolutionComboBox->setObjectName(QString::fromUtf8("resolutionComboBox"));

        imageLayout->addWidget(resolutionComboBox, 0, 1, 1, 1);

        heightLabel = new QLabel(imageGroupBox);
        heightLabel->setObjectName(QString::fromUtf8("heightLabel"));
        heightLabel->setEnabled(false);

        imageLayout->addWidget(heightLabel, 2, 0, 1, 1);

        widthSpinBox = new QSpinBox(imageGroupBox);
        widthSpinBox->setObjectName(QString::fromUtf8("widthSpinBox"));
        widthSpinBox->setEnabled(false);
        widthSpinBox->setMinimum(100);
        widthSpinBox->setMaximum(10000);
        widthSpinBox->setValue(1920);

        imageLayout->addWidget(widthSpinBox, 1, 1, 1, 1);

        widthLabel = new QLabel(imageGroupBox);
        widthLabel->setObjectName(QString::fromUtf8("widthLabel"));
        widthLabel->setEnabled(false);

        imageLayout->addWidget(widthLabel, 1, 0, 1, 1);

        heightSpinBox = new QSpinBox(imageGroupBox);
        heightSpinBox->setObjectName(QString::fromUtf8("heightSpinBox"));
        heightSpinBox->setEnabled(false);
        heightSpinBox->setMinimum(100);
        heightSpinBox->setMaximum(10000);
        heightSpinBox->setValue(1080);

        imageLayout->addWidget(heightSpinBox, 2, 1, 1, 1);


        mainLayout->addWidget(imageGroupBox);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName(QString::fromUtf8("buttonLayout"));
        buttonSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttonLayout->addItem(buttonSpacer);

        exportButton = new QPushButton(ExportPicDialog);
        exportButton->setObjectName(QString::fromUtf8("exportButton"));

        buttonLayout->addWidget(exportButton);

        cancelButton = new QPushButton(ExportPicDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        buttonLayout->addWidget(cancelButton);


        mainLayout->addLayout(buttonLayout);


        retranslateUi(ExportPicDialog);

        exportButton->setDefault(true);


        QMetaObject::connectSlotsByName(ExportPicDialog);
    } // setupUi

    void retranslateUi(QDialog *ExportPicDialog)
    {
        ExportPicDialog->setWindowTitle(QCoreApplication::translate("ExportPicDialog", "Export Picture", nullptr));
        fileGroupBox->setTitle(QCoreApplication::translate("ExportPicDialog", "File Settings", nullptr));
        pathLabel->setText(QCoreApplication::translate("ExportPicDialog", "Save Path:", nullptr));
        pathLineEdit->setPlaceholderText(QCoreApplication::translate("ExportPicDialog", "Select save path...", nullptr));
        browseButton->setText(QCoreApplication::translate("ExportPicDialog", "Browse...", nullptr));
        fileNameLabel->setText(QCoreApplication::translate("ExportPicDialog", "File Name:", nullptr));
        fileNameLineEdit->setPlaceholderText(QCoreApplication::translate("ExportPicDialog", "screenshot", nullptr));
        formatLabel->setText(QCoreApplication::translate("ExportPicDialog", "Format:", nullptr));
        formatComboBox->setItemText(0, QCoreApplication::translate("ExportPicDialog", "PNG", nullptr));
        formatComboBox->setItemText(1, QCoreApplication::translate("ExportPicDialog", "JPEG", nullptr));

        viewGroupBox->setTitle(QCoreApplication::translate("ExportPicDialog", "View Selection", nullptr));
        mainViewRadio->setText(QCoreApplication::translate("ExportPicDialog", "3D View (Main)", nullptr));
        meridionalViewRadio->setText(QCoreApplication::translate("ExportPicDialog", "Meridional View", nullptr));
        bladeToBladeViewRadio->setText(QCoreApplication::translate("ExportPicDialog", "Blade-to-Blade View", nullptr));
        imageGroupBox->setTitle(QCoreApplication::translate("ExportPicDialog", "Output Settings", nullptr));
        resolutionLabel->setText(QCoreApplication::translate("ExportPicDialog", "Output Size:", nullptr));
        resolutionComboBox->setItemText(0, QCoreApplication::translate("ExportPicDialog", "Default", nullptr));
        resolutionComboBox->setItemText(1, QCoreApplication::translate("ExportPicDialog", "Custom", nullptr));

        heightLabel->setText(QCoreApplication::translate("ExportPicDialog", "Height:", nullptr));
        widthSpinBox->setSuffix(QCoreApplication::translate("ExportPicDialog", " px", nullptr));
        widthLabel->setText(QCoreApplication::translate("ExportPicDialog", "Width:", nullptr));
        heightSpinBox->setSuffix(QCoreApplication::translate("ExportPicDialog", " px", nullptr));
        exportButton->setText(QCoreApplication::translate("ExportPicDialog", "Export", nullptr));
        cancelButton->setText(QCoreApplication::translate("ExportPicDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ExportPicDialog: public Ui_ExportPicDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORTPICDIALOG_H
