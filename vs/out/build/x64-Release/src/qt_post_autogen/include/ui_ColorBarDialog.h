/********************************************************************************
** Form generated from reading UI file 'ColorBarDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLORBARDIALOG_H
#define UI_COLORBARDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ColorBarDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *contourTab;
    QVBoxLayout *contourTabLayout;
    QGroupBox *variableGroupBox;
    QFormLayout *variableFormLayout;
    QLabel *labelParameterName;
    QComboBox *nameComboBox;
    QLabel *labelParameterRange;
    QLabel *textParameterRange;
    QGroupBox *colorRangeGroupBox;
    QFormLayout *colorRangeFormLayout;
    QLabel *lableColorBarmin;
    QLineEdit *textColorBarmin;
    QLabel *labelColorBarmax;
    QLineEdit *textColorBarMax;
    QLabel *labelColorNumber;
    QLineEdit *textColorNumber;
    QGroupBox *colorMapGroupBox;
    QVBoxLayout *colorMapLayout;
    QLabel *labelColorMapPreset;
    QComboBox *comboColorMapPreset;
    QCheckBox *checkReverseColorMap;
    QSpacerItem *contourVerticalSpacer;
    QWidget *legendTab;
    QVBoxLayout *legendTabLayout;
    QGroupBox *showLegendGroupBox;
    QVBoxLayout *showLegendLayout;
    QCheckBox *checkShowLegend;
    QGroupBox *legendSizeGroupBox;
    QFormLayout *legendSizeFormLayout;
    QLabel *labelScalarBarWidth;
    QLineEdit *textScalarBarWidth;
    QLabel *labelScalarBarHeight;
    QLineEdit *textScalarBarHeight;
    QLabel *labelLegendX;
    QLineEdit *textLegendXPosition;
    QLabel *labelLegendY;
    QLineEdit *textLegendYPosition;
    QGroupBox *legendOrientationGroupBox;
    QHBoxLayout *orientationLayout;
    QRadioButton *radioVertical;
    QRadioButton *radioHorizontal;
    QGroupBox *textAppearanceGroupBox;
    QFormLayout *textAppearanceFormLayout;
    QLabel *labelTextColor;
    QHBoxLayout *textColorLayout;
    QPushButton *btnTextColor;
    QSpacerItem *textColorSpacer;
    QLabel *labelFontSize;
    QSpinBox *spinFontSize;
    QLabel *labelFontFamily;
    QComboBox *comboFontFamily;
    QLabel *labelFontStyle;
    QHBoxLayout *fontStyleLayout;
    QCheckBox *checkBold;
    QCheckBox *checkItalic;
    QGroupBox *legendTitleGroupBox;
    QVBoxLayout *legendTitleLayout;
    QCheckBox *checkShowTitle;
    QHBoxLayout *titleModeLayout;
    QLabel *labelTitleMode;
    QComboBox *comboTitleMode;
    QHBoxLayout *titleTextLayout;
    QLabel *labelTitleText;
    QLineEdit *textTitleText;
    QSpacerItem *legendVerticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ColorBarDialog)
    {
        if (ColorBarDialog->objectName().isEmpty())
            ColorBarDialog->setObjectName(QString::fromUtf8("ColorBarDialog"));
        ColorBarDialog->resize(550, 763);
        QFont font;
        font.setPointSize(10);
        ColorBarDialog->setFont(font);
        verticalLayout = new QVBoxLayout(ColorBarDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(ColorBarDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        contourTab = new QWidget();
        contourTab->setObjectName(QString::fromUtf8("contourTab"));
        contourTabLayout = new QVBoxLayout(contourTab);
        contourTabLayout->setObjectName(QString::fromUtf8("contourTabLayout"));
        variableGroupBox = new QGroupBox(contourTab);
        variableGroupBox->setObjectName(QString::fromUtf8("variableGroupBox"));
        variableFormLayout = new QFormLayout(variableGroupBox);
        variableFormLayout->setObjectName(QString::fromUtf8("variableFormLayout"));
        labelParameterName = new QLabel(variableGroupBox);
        labelParameterName->setObjectName(QString::fromUtf8("labelParameterName"));

        variableFormLayout->setWidget(0, QFormLayout::LabelRole, labelParameterName);

        nameComboBox = new QComboBox(variableGroupBox);
        nameComboBox->setObjectName(QString::fromUtf8("nameComboBox"));
        nameComboBox->setMinimumSize(QSize(200, 30));

        variableFormLayout->setWidget(0, QFormLayout::FieldRole, nameComboBox);

        labelParameterRange = new QLabel(variableGroupBox);
        labelParameterRange->setObjectName(QString::fromUtf8("labelParameterRange"));

        variableFormLayout->setWidget(1, QFormLayout::LabelRole, labelParameterRange);

        textParameterRange = new QLabel(variableGroupBox);
        textParameterRange->setObjectName(QString::fromUtf8("textParameterRange"));

        variableFormLayout->setWidget(1, QFormLayout::FieldRole, textParameterRange);


        contourTabLayout->addWidget(variableGroupBox);

        colorRangeGroupBox = new QGroupBox(contourTab);
        colorRangeGroupBox->setObjectName(QString::fromUtf8("colorRangeGroupBox"));
        colorRangeFormLayout = new QFormLayout(colorRangeGroupBox);
        colorRangeFormLayout->setObjectName(QString::fromUtf8("colorRangeFormLayout"));
        lableColorBarmin = new QLabel(colorRangeGroupBox);
        lableColorBarmin->setObjectName(QString::fromUtf8("lableColorBarmin"));

        colorRangeFormLayout->setWidget(0, QFormLayout::LabelRole, lableColorBarmin);

        textColorBarmin = new QLineEdit(colorRangeGroupBox);
        textColorBarmin->setObjectName(QString::fromUtf8("textColorBarmin"));
        textColorBarmin->setMinimumSize(QSize(200, 30));

        colorRangeFormLayout->setWidget(0, QFormLayout::FieldRole, textColorBarmin);

        labelColorBarmax = new QLabel(colorRangeGroupBox);
        labelColorBarmax->setObjectName(QString::fromUtf8("labelColorBarmax"));

        colorRangeFormLayout->setWidget(1, QFormLayout::LabelRole, labelColorBarmax);

        textColorBarMax = new QLineEdit(colorRangeGroupBox);
        textColorBarMax->setObjectName(QString::fromUtf8("textColorBarMax"));
        textColorBarMax->setMinimumSize(QSize(200, 30));

        colorRangeFormLayout->setWidget(1, QFormLayout::FieldRole, textColorBarMax);

        labelColorNumber = new QLabel(colorRangeGroupBox);
        labelColorNumber->setObjectName(QString::fromUtf8("labelColorNumber"));

        colorRangeFormLayout->setWidget(2, QFormLayout::LabelRole, labelColorNumber);

        textColorNumber = new QLineEdit(colorRangeGroupBox);
        textColorNumber->setObjectName(QString::fromUtf8("textColorNumber"));
        textColorNumber->setMinimumSize(QSize(200, 30));

        colorRangeFormLayout->setWidget(2, QFormLayout::FieldRole, textColorNumber);


        contourTabLayout->addWidget(colorRangeGroupBox);

        colorMapGroupBox = new QGroupBox(contourTab);
        colorMapGroupBox->setObjectName(QString::fromUtf8("colorMapGroupBox"));
        colorMapLayout = new QVBoxLayout(colorMapGroupBox);
        colorMapLayout->setObjectName(QString::fromUtf8("colorMapLayout"));
        labelColorMapPreset = new QLabel(colorMapGroupBox);
        labelColorMapPreset->setObjectName(QString::fromUtf8("labelColorMapPreset"));

        colorMapLayout->addWidget(labelColorMapPreset);

        comboColorMapPreset = new QComboBox(colorMapGroupBox);
        comboColorMapPreset->setObjectName(QString::fromUtf8("comboColorMapPreset"));
        comboColorMapPreset->setMinimumSize(QSize(0, 32));
        comboColorMapPreset->setIconSize(QSize(200, 20));

        colorMapLayout->addWidget(comboColorMapPreset);

        checkReverseColorMap = new QCheckBox(colorMapGroupBox);
        checkReverseColorMap->setObjectName(QString::fromUtf8("checkReverseColorMap"));

        colorMapLayout->addWidget(checkReverseColorMap);


        contourTabLayout->addWidget(colorMapGroupBox);

        contourVerticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        contourTabLayout->addItem(contourVerticalSpacer);

        tabWidget->addTab(contourTab, QString());
        legendTab = new QWidget();
        legendTab->setObjectName(QString::fromUtf8("legendTab"));
        legendTabLayout = new QVBoxLayout(legendTab);
        legendTabLayout->setObjectName(QString::fromUtf8("legendTabLayout"));
        showLegendGroupBox = new QGroupBox(legendTab);
        showLegendGroupBox->setObjectName(QString::fromUtf8("showLegendGroupBox"));
        showLegendLayout = new QVBoxLayout(showLegendGroupBox);
        showLegendLayout->setObjectName(QString::fromUtf8("showLegendLayout"));
        checkShowLegend = new QCheckBox(showLegendGroupBox);
        checkShowLegend->setObjectName(QString::fromUtf8("checkShowLegend"));
        checkShowLegend->setChecked(true);

        showLegendLayout->addWidget(checkShowLegend);


        legendTabLayout->addWidget(showLegendGroupBox);

        legendSizeGroupBox = new QGroupBox(legendTab);
        legendSizeGroupBox->setObjectName(QString::fromUtf8("legendSizeGroupBox"));
        legendSizeFormLayout = new QFormLayout(legendSizeGroupBox);
        legendSizeFormLayout->setObjectName(QString::fromUtf8("legendSizeFormLayout"));
        labelScalarBarWidth = new QLabel(legendSizeGroupBox);
        labelScalarBarWidth->setObjectName(QString::fromUtf8("labelScalarBarWidth"));

        legendSizeFormLayout->setWidget(0, QFormLayout::LabelRole, labelScalarBarWidth);

        textScalarBarWidth = new QLineEdit(legendSizeGroupBox);
        textScalarBarWidth->setObjectName(QString::fromUtf8("textScalarBarWidth"));
        textScalarBarWidth->setMinimumSize(QSize(150, 30));

        legendSizeFormLayout->setWidget(0, QFormLayout::FieldRole, textScalarBarWidth);

        labelScalarBarHeight = new QLabel(legendSizeGroupBox);
        labelScalarBarHeight->setObjectName(QString::fromUtf8("labelScalarBarHeight"));

        legendSizeFormLayout->setWidget(1, QFormLayout::LabelRole, labelScalarBarHeight);

        textScalarBarHeight = new QLineEdit(legendSizeGroupBox);
        textScalarBarHeight->setObjectName(QString::fromUtf8("textScalarBarHeight"));
        textScalarBarHeight->setMinimumSize(QSize(150, 30));

        legendSizeFormLayout->setWidget(1, QFormLayout::FieldRole, textScalarBarHeight);

        labelLegendX = new QLabel(legendSizeGroupBox);
        labelLegendX->setObjectName(QString::fromUtf8("labelLegendX"));

        legendSizeFormLayout->setWidget(2, QFormLayout::LabelRole, labelLegendX);

        textLegendXPosition = new QLineEdit(legendSizeGroupBox);
        textLegendXPosition->setObjectName(QString::fromUtf8("textLegendXPosition"));
        textLegendXPosition->setMinimumSize(QSize(150, 30));

        legendSizeFormLayout->setWidget(2, QFormLayout::FieldRole, textLegendXPosition);

        labelLegendY = new QLabel(legendSizeGroupBox);
        labelLegendY->setObjectName(QString::fromUtf8("labelLegendY"));

        legendSizeFormLayout->setWidget(3, QFormLayout::LabelRole, labelLegendY);

        textLegendYPosition = new QLineEdit(legendSizeGroupBox);
        textLegendYPosition->setObjectName(QString::fromUtf8("textLegendYPosition"));
        textLegendYPosition->setMinimumSize(QSize(150, 30));

        legendSizeFormLayout->setWidget(3, QFormLayout::FieldRole, textLegendYPosition);


        legendTabLayout->addWidget(legendSizeGroupBox);

        legendOrientationGroupBox = new QGroupBox(legendTab);
        legendOrientationGroupBox->setObjectName(QString::fromUtf8("legendOrientationGroupBox"));
        orientationLayout = new QHBoxLayout(legendOrientationGroupBox);
        orientationLayout->setObjectName(QString::fromUtf8("orientationLayout"));
        radioVertical = new QRadioButton(legendOrientationGroupBox);
        radioVertical->setObjectName(QString::fromUtf8("radioVertical"));
        radioVertical->setChecked(true);

        orientationLayout->addWidget(radioVertical);

        radioHorizontal = new QRadioButton(legendOrientationGroupBox);
        radioHorizontal->setObjectName(QString::fromUtf8("radioHorizontal"));

        orientationLayout->addWidget(radioHorizontal);


        legendTabLayout->addWidget(legendOrientationGroupBox);

        textAppearanceGroupBox = new QGroupBox(legendTab);
        textAppearanceGroupBox->setObjectName(QString::fromUtf8("textAppearanceGroupBox"));
        textAppearanceFormLayout = new QFormLayout(textAppearanceGroupBox);
        textAppearanceFormLayout->setObjectName(QString::fromUtf8("textAppearanceFormLayout"));
        labelTextColor = new QLabel(textAppearanceGroupBox);
        labelTextColor->setObjectName(QString::fromUtf8("labelTextColor"));

        textAppearanceFormLayout->setWidget(0, QFormLayout::LabelRole, labelTextColor);

        textColorLayout = new QHBoxLayout();
        textColorLayout->setObjectName(QString::fromUtf8("textColorLayout"));
        btnTextColor = new QPushButton(textAppearanceGroupBox);
        btnTextColor->setObjectName(QString::fromUtf8("btnTextColor"));
        btnTextColor->setMinimumSize(QSize(100, 30));

        textColorLayout->addWidget(btnTextColor);

        textColorSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        textColorLayout->addItem(textColorSpacer);


        textAppearanceFormLayout->setLayout(0, QFormLayout::FieldRole, textColorLayout);

        labelFontSize = new QLabel(textAppearanceGroupBox);
        labelFontSize->setObjectName(QString::fromUtf8("labelFontSize"));

        textAppearanceFormLayout->setWidget(1, QFormLayout::LabelRole, labelFontSize);

        spinFontSize = new QSpinBox(textAppearanceGroupBox);
        spinFontSize->setObjectName(QString::fromUtf8("spinFontSize"));
        spinFontSize->setMinimumSize(QSize(100, 30));
        spinFontSize->setMinimum(8);
        spinFontSize->setMaximum(48);
        spinFontSize->setValue(16);

        textAppearanceFormLayout->setWidget(1, QFormLayout::FieldRole, spinFontSize);

        labelFontFamily = new QLabel(textAppearanceGroupBox);
        labelFontFamily->setObjectName(QString::fromUtf8("labelFontFamily"));

        textAppearanceFormLayout->setWidget(2, QFormLayout::LabelRole, labelFontFamily);

        comboFontFamily = new QComboBox(textAppearanceGroupBox);
        comboFontFamily->addItem(QString());
        comboFontFamily->addItem(QString());
        comboFontFamily->addItem(QString());
        comboFontFamily->addItem(QString());
        comboFontFamily->setObjectName(QString::fromUtf8("comboFontFamily"));
        comboFontFamily->setMinimumSize(QSize(150, 30));

        textAppearanceFormLayout->setWidget(2, QFormLayout::FieldRole, comboFontFamily);

        labelFontStyle = new QLabel(textAppearanceGroupBox);
        labelFontStyle->setObjectName(QString::fromUtf8("labelFontStyle"));

        textAppearanceFormLayout->setWidget(3, QFormLayout::LabelRole, labelFontStyle);

        fontStyleLayout = new QHBoxLayout();
        fontStyleLayout->setObjectName(QString::fromUtf8("fontStyleLayout"));
        checkBold = new QCheckBox(textAppearanceGroupBox);
        checkBold->setObjectName(QString::fromUtf8("checkBold"));
        checkBold->setChecked(true);

        fontStyleLayout->addWidget(checkBold);

        checkItalic = new QCheckBox(textAppearanceGroupBox);
        checkItalic->setObjectName(QString::fromUtf8("checkItalic"));

        fontStyleLayout->addWidget(checkItalic);


        textAppearanceFormLayout->setLayout(3, QFormLayout::FieldRole, fontStyleLayout);


        legendTabLayout->addWidget(textAppearanceGroupBox);

        legendTitleGroupBox = new QGroupBox(legendTab);
        legendTitleGroupBox->setObjectName(QString::fromUtf8("legendTitleGroupBox"));
        legendTitleLayout = new QVBoxLayout(legendTitleGroupBox);
        legendTitleLayout->setObjectName(QString::fromUtf8("legendTitleLayout"));
        checkShowTitle = new QCheckBox(legendTitleGroupBox);
        checkShowTitle->setObjectName(QString::fromUtf8("checkShowTitle"));
        checkShowTitle->setChecked(true);

        legendTitleLayout->addWidget(checkShowTitle);

        titleModeLayout = new QHBoxLayout();
        titleModeLayout->setObjectName(QString::fromUtf8("titleModeLayout"));
        labelTitleMode = new QLabel(legendTitleGroupBox);
        labelTitleMode->setObjectName(QString::fromUtf8("labelTitleMode"));

        titleModeLayout->addWidget(labelTitleMode);

        comboTitleMode = new QComboBox(legendTitleGroupBox);
        comboTitleMode->addItem(QString());
        comboTitleMode->addItem(QString());
        comboTitleMode->setObjectName(QString::fromUtf8("comboTitleMode"));
        comboTitleMode->setMinimumSize(QSize(200, 30));

        titleModeLayout->addWidget(comboTitleMode);


        legendTitleLayout->addLayout(titleModeLayout);

        titleTextLayout = new QHBoxLayout();
        titleTextLayout->setObjectName(QString::fromUtf8("titleTextLayout"));
        labelTitleText = new QLabel(legendTitleGroupBox);
        labelTitleText->setObjectName(QString::fromUtf8("labelTitleText"));

        titleTextLayout->addWidget(labelTitleText);

        textTitleText = new QLineEdit(legendTitleGroupBox);
        textTitleText->setObjectName(QString::fromUtf8("textTitleText"));
        textTitleText->setEnabled(false);

        titleTextLayout->addWidget(textTitleText);


        legendTitleLayout->addLayout(titleTextLayout);


        legendTabLayout->addWidget(legendTitleGroupBox);

        legendVerticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        legendTabLayout->addItem(legendVerticalSpacer);

        tabWidget->addTab(legendTab, QString());

        verticalLayout->addWidget(tabWidget);

        buttonBox = new QDialogButtonBox(ColorBarDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ColorBarDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ColorBarDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ColorBarDialog, SLOT(reject()));

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(ColorBarDialog);
    } // setupUi

    void retranslateUi(QDialog *ColorBarDialog)
    {
        ColorBarDialog->setWindowTitle(QCoreApplication::translate("ColorBarDialog", "Contour & Legend Settings", nullptr));
        variableGroupBox->setTitle(QCoreApplication::translate("ColorBarDialog", "Variable Selection", nullptr));
        labelParameterName->setText(QCoreApplication::translate("ColorBarDialog", "Parameter Name:", nullptr));
        labelParameterRange->setText(QCoreApplication::translate("ColorBarDialog", "Data Range:", nullptr));
        textParameterRange->setStyleSheet(QCoreApplication::translate("ColorBarDialog", "QLabel { color: #0066cc; font-weight: bold; }", nullptr));
        textParameterRange->setText(QCoreApplication::translate("ColorBarDialog", "[Min, Max]", nullptr));
        colorRangeGroupBox->setTitle(QCoreApplication::translate("ColorBarDialog", "Color Range", nullptr));
        lableColorBarmin->setText(QCoreApplication::translate("ColorBarDialog", "Minimum:", nullptr));
        textColorBarmin->setPlaceholderText(QCoreApplication::translate("ColorBarDialog", "Enter minimum value", nullptr));
        labelColorBarmax->setText(QCoreApplication::translate("ColorBarDialog", "Maximum:", nullptr));
        textColorBarMax->setPlaceholderText(QCoreApplication::translate("ColorBarDialog", "Enter maximum value", nullptr));
        labelColorNumber->setText(QCoreApplication::translate("ColorBarDialog", "Number of Colors:", nullptr));
        textColorNumber->setPlaceholderText(QCoreApplication::translate("ColorBarDialog", "10", nullptr));
        colorMapGroupBox->setTitle(QCoreApplication::translate("ColorBarDialog", "Color Map Options", nullptr));
        labelColorMapPreset->setText(QCoreApplication::translate("ColorBarDialog", "Select a color map from presets:", nullptr));
        checkReverseColorMap->setText(QCoreApplication::translate("ColorBarDialog", "Reverse Color Map", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(contourTab), QCoreApplication::translate("ColorBarDialog", "Contour", nullptr));
        showLegendGroupBox->setTitle(QCoreApplication::translate("ColorBarDialog", "Legend Display", nullptr));
        checkShowLegend->setText(QCoreApplication::translate("ColorBarDialog", "Show Color Legend", nullptr));
        legendSizeGroupBox->setTitle(QCoreApplication::translate("ColorBarDialog", "Size and Position", nullptr));
        labelScalarBarWidth->setText(QCoreApplication::translate("ColorBarDialog", "Width:", nullptr));
        textScalarBarWidth->setPlaceholderText(QCoreApplication::translate("ColorBarDialog", "0.1 ", nullptr));
        labelScalarBarHeight->setText(QCoreApplication::translate("ColorBarDialog", "Height:", nullptr));
        textScalarBarHeight->setPlaceholderText(QCoreApplication::translate("ColorBarDialog", "0.8 ", nullptr));
        labelLegendX->setText(QCoreApplication::translate("ColorBarDialog", "X Position (%):", nullptr));
        textLegendXPosition->setPlaceholderText(QCoreApplication::translate("ColorBarDialog", "90", nullptr));
        labelLegendY->setText(QCoreApplication::translate("ColorBarDialog", "Y Position (%):", nullptr));
        textLegendYPosition->setPlaceholderText(QCoreApplication::translate("ColorBarDialog", "10", nullptr));
        legendOrientationGroupBox->setTitle(QCoreApplication::translate("ColorBarDialog", "Orientation", nullptr));
        radioVertical->setText(QCoreApplication::translate("ColorBarDialog", "Vertical", nullptr));
        radioHorizontal->setText(QCoreApplication::translate("ColorBarDialog", "Horizontal", nullptr));
        textAppearanceGroupBox->setTitle(QCoreApplication::translate("ColorBarDialog", "Text Appearance", nullptr));
        labelTextColor->setText(QCoreApplication::translate("ColorBarDialog", "Text Color:", nullptr));
        btnTextColor->setStyleSheet(QCoreApplication::translate("ColorBarDialog", "QPushButton { background-color: black; color: white; }", nullptr));
        btnTextColor->setText(QCoreApplication::translate("ColorBarDialog", "Black", nullptr));
        labelFontSize->setText(QCoreApplication::translate("ColorBarDialog", "Font Size:", nullptr));
        labelFontFamily->setText(QCoreApplication::translate("ColorBarDialog", "Font type:", nullptr));
        comboFontFamily->setItemText(0, QCoreApplication::translate("ColorBarDialog", "Arial", nullptr));
        comboFontFamily->setItemText(1, QCoreApplication::translate("ColorBarDialog", "Times New Roman", nullptr));
        comboFontFamily->setItemText(2, QCoreApplication::translate("ColorBarDialog", "Courier New", nullptr));
        comboFontFamily->setItemText(3, QCoreApplication::translate("ColorBarDialog", "Helvetica", nullptr));

        labelFontStyle->setText(QCoreApplication::translate("ColorBarDialog", "Font Style:", nullptr));
        checkBold->setText(QCoreApplication::translate("ColorBarDialog", "Bold", nullptr));
        checkItalic->setText(QCoreApplication::translate("ColorBarDialog", "Italic", nullptr));
        legendTitleGroupBox->setTitle(QCoreApplication::translate("ColorBarDialog", "Legend Title", nullptr));
        checkShowTitle->setText(QCoreApplication::translate("ColorBarDialog", "Show Title", nullptr));
        labelTitleMode->setText(QCoreApplication::translate("ColorBarDialog", "Title Mode:", nullptr));
        comboTitleMode->setItemText(0, QCoreApplication::translate("ColorBarDialog", "Use Variable Name", nullptr));
        comboTitleMode->setItemText(1, QCoreApplication::translate("ColorBarDialog", "Use Custom Text", nullptr));

        labelTitleText->setText(QCoreApplication::translate("ColorBarDialog", "Custom Title:", nullptr));
        textTitleText->setPlaceholderText(QCoreApplication::translate("ColorBarDialog", "Enter custom text", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(legendTab), QCoreApplication::translate("ColorBarDialog", "Legend", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ColorBarDialog: public Ui_ColorBarDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLORBARDIALOG_H
