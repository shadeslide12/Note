/********************************************************************************
** Form generated from reading UI file 'CutplaneDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUTPLANEDIALOG_H
#define UI_CUTPLANEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CutplaneDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QTabWidget *cutplaneTabWidget;
    QWidget *sliceSettingsTab;
    QVBoxLayout *sliceSettingsLayout;
    QGroupBox *sliceControlGroup;
    QGridLayout *sliceControlLayout;
    QLabel *label;
    QComboBox *comboBox;
    QPushButton *Btn_AddNew;
    QCheckBox *checkBox;
    QLabel *label_SLoaction;
    QComboBox *Combo_SLiceLocation;
    QHBoxLayout *sliderLayout;
    QSlider *horizontalSlider;
    QLabel *label_ValueLocation;
    QPushButton *Btn_SetPosition;
    QWidget *MappingControl;
    QVBoxLayout *verticalLayout;
    QGroupBox *mappingGroup;
    QHBoxLayout *mappingLayout;
    QLabel *lab_MapVariable;
    QComboBox *Combo_SekectMapVariable;
    QSpacerItem *mappingSpacer;
    QGroupBox *colorMapGroup;
    QVBoxLayout *colorMapLayout;
    QHBoxLayout *colorSchemeLayout;
    QLabel *lab_ContourType;
    QComboBox *Combo_ContourType;
    QCheckBox *checkReverseColorMap;
    QHBoxLayout *distributionLayout;
    QLabel *label_3;
    QCheckBox *Check_Banded;
    QCheckBox *Check_Continuous;
    QSpacerItem *distributionSpacer;
    QGroupBox *contoursGroup;
    QGridLayout *contoursLayout;
    QLabel *label_5;
    QLineEdit *LnEdit_Min;
    QLabel *label_6;
    QLineEdit *LnEdit_Max;
    QLabel *label_7;
    QLineEdit *LnEdit_Numbers;
    QSpacerItem *contoursSpacer;
    QSpacerItem *sliceSettingsSpacer;
    QWidget *legendTab;
    QVBoxLayout *cp_legendTabLayout;
    QGroupBox *cp_showLegendGroupBox;
    QVBoxLayout *cp_showLegendLayout;
    QCheckBox *cp_checkShowLegend;
    QGroupBox *cp_legendSizeGroupBox;
    QFormLayout *cp_legendSizeFormLayout;
    QLabel *cp_labelScalarBarWidth;
    QLineEdit *cp_textScalarBarWidth;
    QLabel *cp_labelScalarBarHeight;
    QLineEdit *cp_textScalarBarHeight;
    QLabel *cp_labelLegendX;
    QLineEdit *cp_textLegendXPosition;
    QLabel *cp_labelLegendY;
    QLineEdit *cp_textLegendYPosition;
    QGroupBox *cp_legendOrientationGroupBox;
    QHBoxLayout *cp_orientationLayout;
    QRadioButton *cp_radioVertical;
    QRadioButton *cp_radioHorizontal;
    QGroupBox *cp_textAppearanceGroupBox;
    QFormLayout *cp_textAppearanceFormLayout;
    QLabel *cp_labelTextColor;
    QHBoxLayout *cp_textColorLayout;
    QPushButton *cp_btnTextColor;
    QSpacerItem *cp_textColorSpacer;
    QLabel *cp_labelFontSize;
    QSpinBox *cp_spinFontSize;
    QLabel *cp_labelFontFamily;
    QComboBox *cp_comboFontFamily;
    QLabel *cp_labelFontStyle;
    QHBoxLayout *cp_fontStyleLayout;
    QCheckBox *cp_checkBold;
    QCheckBox *cp_checkItalic;
    QGroupBox *cp_legendTitleGroupBox;
    QVBoxLayout *cp_legendTitleLayout;
    QCheckBox *cp_checkShowTitle;
    QHBoxLayout *cp_titleModeLayout;
    QLabel *cp_labelTitleMode;
    QComboBox *cp_comboTitleMode;
    QHBoxLayout *cp_titleTextLayout;
    QLabel *cp_labelTitleText;
    QLineEdit *cp_textTitleText;
    QSpacerItem *cp_legendVerticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CutplaneDialog)
    {
        if (CutplaneDialog->objectName().isEmpty())
            CutplaneDialog->setObjectName(QString::fromUtf8("CutplaneDialog"));
        CutplaneDialog->resize(550, 763);
        CutplaneDialog->setMinimumSize(QSize(550, 763));
        CutplaneDialog->setMaximumSize(QSize(800, 800));
        verticalLayout_2 = new QVBoxLayout(CutplaneDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        cutplaneTabWidget = new QTabWidget(CutplaneDialog);
        cutplaneTabWidget->setObjectName(QString::fromUtf8("cutplaneTabWidget"));
        sliceSettingsTab = new QWidget();
        sliceSettingsTab->setObjectName(QString::fromUtf8("sliceSettingsTab"));
        sliceSettingsLayout = new QVBoxLayout(sliceSettingsTab);
        sliceSettingsLayout->setObjectName(QString::fromUtf8("sliceSettingsLayout"));
        sliceControlGroup = new QGroupBox(sliceSettingsTab);
        sliceControlGroup->setObjectName(QString::fromUtf8("sliceControlGroup"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sliceControlGroup->sizePolicy().hasHeightForWidth());
        sliceControlGroup->setSizePolicy(sizePolicy);
        sliceControlLayout = new QGridLayout(sliceControlGroup);
        sliceControlLayout->setSpacing(10);
        sliceControlLayout->setObjectName(QString::fromUtf8("sliceControlLayout"));
        sliceControlLayout->setContentsMargins(20, -1, 20, -1);
        label = new QLabel(sliceControlGroup);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(10);
        label->setFont(font);

        sliceControlLayout->addWidget(label, 0, 0, 1, 1);

        comboBox = new QComboBox(sliceControlGroup);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setFont(font);

        sliceControlLayout->addWidget(comboBox, 0, 1, 1, 1);

        Btn_AddNew = new QPushButton(sliceControlGroup);
        Btn_AddNew->setObjectName(QString::fromUtf8("Btn_AddNew"));
        Btn_AddNew->setMinimumSize(QSize(120, 0));
        Btn_AddNew->setMaximumSize(QSize(60, 30));

        sliceControlLayout->addWidget(Btn_AddNew, 0, 2, 1, 1);

        checkBox = new QCheckBox(sliceControlGroup);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        sliceControlLayout->addWidget(checkBox, 0, 3, 1, 1);

        label_SLoaction = new QLabel(sliceControlGroup);
        label_SLoaction->setObjectName(QString::fromUtf8("label_SLoaction"));
        label_SLoaction->setFont(font);

        sliceControlLayout->addWidget(label_SLoaction, 1, 0, 1, 1);

        Combo_SLiceLocation = new QComboBox(sliceControlGroup);
        Combo_SLiceLocation->addItem(QString());
        Combo_SLiceLocation->addItem(QString());
        Combo_SLiceLocation->addItem(QString());
        Combo_SLiceLocation->setObjectName(QString::fromUtf8("Combo_SLiceLocation"));

        sliceControlLayout->addWidget(Combo_SLiceLocation, 1, 1, 1, 2);

        sliderLayout = new QHBoxLayout();
        sliderLayout->setObjectName(QString::fromUtf8("sliderLayout"));
        horizontalSlider = new QSlider(sliceControlGroup);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Orientation::Horizontal);

        sliderLayout->addWidget(horizontalSlider);

        label_ValueLocation = new QLabel(sliceControlGroup);
        label_ValueLocation->setObjectName(QString::fromUtf8("label_ValueLocation"));
        label_ValueLocation->setMinimumSize(QSize(100, 0));

        sliderLayout->addWidget(label_ValueLocation);

        Btn_SetPosition = new QPushButton(sliceControlGroup);
        Btn_SetPosition->setObjectName(QString::fromUtf8("Btn_SetPosition"));

        sliderLayout->addWidget(Btn_SetPosition);


        sliceControlLayout->addLayout(sliderLayout, 2, 0, 1, 4);


        sliceSettingsLayout->addWidget(sliceControlGroup);

        MappingControl = new QWidget(sliceSettingsTab);
        MappingControl->setObjectName(QString::fromUtf8("MappingControl"));
        verticalLayout = new QVBoxLayout(MappingControl);
        verticalLayout->setSpacing(25);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 20, -1, -1);
        mappingGroup = new QGroupBox(MappingControl);
        mappingGroup->setObjectName(QString::fromUtf8("mappingGroup"));
        sizePolicy.setHeightForWidth(mappingGroup->sizePolicy().hasHeightForWidth());
        mappingGroup->setSizePolicy(sizePolicy);
        mappingLayout = new QHBoxLayout(mappingGroup);
        mappingLayout->setObjectName(QString::fromUtf8("mappingLayout"));
        mappingLayout->setContentsMargins(20, -1, 20, -1);
        lab_MapVariable = new QLabel(mappingGroup);
        lab_MapVariable->setObjectName(QString::fromUtf8("lab_MapVariable"));
        lab_MapVariable->setFont(font);

        mappingLayout->addWidget(lab_MapVariable);

        Combo_SekectMapVariable = new QComboBox(mappingGroup);
        Combo_SekectMapVariable->setObjectName(QString::fromUtf8("Combo_SekectMapVariable"));
        Combo_SekectMapVariable->setMinimumSize(QSize(200, 0));

        mappingLayout->addWidget(Combo_SekectMapVariable);

        mappingSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        mappingLayout->addItem(mappingSpacer);


        verticalLayout->addWidget(mappingGroup);

        colorMapGroup = new QGroupBox(MappingControl);
        colorMapGroup->setObjectName(QString::fromUtf8("colorMapGroup"));
        sizePolicy.setHeightForWidth(colorMapGroup->sizePolicy().hasHeightForWidth());
        colorMapGroup->setSizePolicy(sizePolicy);
        colorMapLayout = new QVBoxLayout(colorMapGroup);
        colorMapLayout->setObjectName(QString::fromUtf8("colorMapLayout"));
        colorMapLayout->setContentsMargins(20, -1, 20, -1);
        colorSchemeLayout = new QHBoxLayout();
        colorSchemeLayout->setObjectName(QString::fromUtf8("colorSchemeLayout"));
        lab_ContourType = new QLabel(colorMapGroup);
        lab_ContourType->setObjectName(QString::fromUtf8("lab_ContourType"));
        lab_ContourType->setFont(font);

        colorSchemeLayout->addWidget(lab_ContourType);

        Combo_ContourType = new QComboBox(colorMapGroup);
        Combo_ContourType->setObjectName(QString::fromUtf8("Combo_ContourType"));
        Combo_ContourType->setMinimumSize(QSize(300, 32));
        Combo_ContourType->setIconSize(QSize(200, 20));

        colorSchemeLayout->addWidget(Combo_ContourType);


        colorMapLayout->addLayout(colorSchemeLayout);

        checkReverseColorMap = new QCheckBox(colorMapGroup);
        checkReverseColorMap->setObjectName(QString::fromUtf8("checkReverseColorMap"));

        colorMapLayout->addWidget(checkReverseColorMap);

        distributionLayout = new QHBoxLayout();
        distributionLayout->setObjectName(QString::fromUtf8("distributionLayout"));
        label_3 = new QLabel(colorMapGroup);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);

        distributionLayout->addWidget(label_3);

        Check_Banded = new QCheckBox(colorMapGroup);
        Check_Banded->setObjectName(QString::fromUtf8("Check_Banded"));
        Check_Banded->setChecked(true);

        distributionLayout->addWidget(Check_Banded);

        Check_Continuous = new QCheckBox(colorMapGroup);
        Check_Continuous->setObjectName(QString::fromUtf8("Check_Continuous"));
        Check_Continuous->setChecked(false);

        distributionLayout->addWidget(Check_Continuous);

        distributionSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        distributionLayout->addItem(distributionSpacer);


        colorMapLayout->addLayout(distributionLayout);


        verticalLayout->addWidget(colorMapGroup);

        contoursGroup = new QGroupBox(MappingControl);
        contoursGroup->setObjectName(QString::fromUtf8("contoursGroup"));
        sizePolicy.setHeightForWidth(contoursGroup->sizePolicy().hasHeightForWidth());
        contoursGroup->setSizePolicy(sizePolicy);
        contoursLayout = new QGridLayout(contoursGroup);
        contoursLayout->setSpacing(10);
        contoursLayout->setObjectName(QString::fromUtf8("contoursLayout"));
        contoursLayout->setContentsMargins(20, -1, 20, -1);
        label_5 = new QLabel(contoursGroup);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font);

        contoursLayout->addWidget(label_5, 0, 0, 1, 1);

        LnEdit_Min = new QLineEdit(contoursGroup);
        LnEdit_Min->setObjectName(QString::fromUtf8("LnEdit_Min"));
        LnEdit_Min->setMaximumSize(QSize(120, 16777215));

        contoursLayout->addWidget(LnEdit_Min, 0, 1, 1, 1);

        label_6 = new QLabel(contoursGroup);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font);

        contoursLayout->addWidget(label_6, 0, 2, 1, 1);

        LnEdit_Max = new QLineEdit(contoursGroup);
        LnEdit_Max->setObjectName(QString::fromUtf8("LnEdit_Max"));
        LnEdit_Max->setMaximumSize(QSize(120, 16777215));

        contoursLayout->addWidget(LnEdit_Max, 0, 3, 1, 1);

        label_7 = new QLabel(contoursGroup);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setFont(font);

        contoursLayout->addWidget(label_7, 1, 0, 1, 1);

        LnEdit_Numbers = new QLineEdit(contoursGroup);
        LnEdit_Numbers->setObjectName(QString::fromUtf8("LnEdit_Numbers"));
        LnEdit_Numbers->setMaximumSize(QSize(120, 16777215));

        contoursLayout->addWidget(LnEdit_Numbers, 1, 1, 1, 1);

        contoursSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        contoursLayout->addItem(contoursSpacer, 1, 2, 1, 2);


        verticalLayout->addWidget(contoursGroup);


        sliceSettingsLayout->addWidget(MappingControl);

        sliceSettingsSpacer = new QSpacerItem(20, 80, QSizePolicy::Expanding, QSizePolicy::Minimum);

        sliceSettingsLayout->addItem(sliceSettingsSpacer);

        cutplaneTabWidget->addTab(sliceSettingsTab, QString());
        legendTab = new QWidget();
        legendTab->setObjectName(QString::fromUtf8("legendTab"));
        cp_legendTabLayout = new QVBoxLayout(legendTab);
        cp_legendTabLayout->setSpacing(12);
        cp_legendTabLayout->setObjectName(QString::fromUtf8("cp_legendTabLayout"));
        cp_legendTabLayout->setContentsMargins(10, 10, 10, 10);
        cp_showLegendGroupBox = new QGroupBox(legendTab);
        cp_showLegendGroupBox->setObjectName(QString::fromUtf8("cp_showLegendGroupBox"));
        cp_showLegendLayout = new QVBoxLayout(cp_showLegendGroupBox);
        cp_showLegendLayout->setObjectName(QString::fromUtf8("cp_showLegendLayout"));
        cp_checkShowLegend = new QCheckBox(cp_showLegendGroupBox);
        cp_checkShowLegend->setObjectName(QString::fromUtf8("cp_checkShowLegend"));
        cp_checkShowLegend->setChecked(true);

        cp_showLegendLayout->addWidget(cp_checkShowLegend);


        cp_legendTabLayout->addWidget(cp_showLegendGroupBox);

        cp_legendSizeGroupBox = new QGroupBox(legendTab);
        cp_legendSizeGroupBox->setObjectName(QString::fromUtf8("cp_legendSizeGroupBox"));
        cp_legendSizeFormLayout = new QFormLayout(cp_legendSizeGroupBox);
        cp_legendSizeFormLayout->setObjectName(QString::fromUtf8("cp_legendSizeFormLayout"));
        cp_labelScalarBarWidth = new QLabel(cp_legendSizeGroupBox);
        cp_labelScalarBarWidth->setObjectName(QString::fromUtf8("cp_labelScalarBarWidth"));

        cp_legendSizeFormLayout->setWidget(0, QFormLayout::LabelRole, cp_labelScalarBarWidth);

        cp_textScalarBarWidth = new QLineEdit(cp_legendSizeGroupBox);
        cp_textScalarBarWidth->setObjectName(QString::fromUtf8("cp_textScalarBarWidth"));
        cp_textScalarBarWidth->setMinimumSize(QSize(150, 30));

        cp_legendSizeFormLayout->setWidget(0, QFormLayout::FieldRole, cp_textScalarBarWidth);

        cp_labelScalarBarHeight = new QLabel(cp_legendSizeGroupBox);
        cp_labelScalarBarHeight->setObjectName(QString::fromUtf8("cp_labelScalarBarHeight"));

        cp_legendSizeFormLayout->setWidget(1, QFormLayout::LabelRole, cp_labelScalarBarHeight);

        cp_textScalarBarHeight = new QLineEdit(cp_legendSizeGroupBox);
        cp_textScalarBarHeight->setObjectName(QString::fromUtf8("cp_textScalarBarHeight"));
        cp_textScalarBarHeight->setMinimumSize(QSize(150, 30));

        cp_legendSizeFormLayout->setWidget(1, QFormLayout::FieldRole, cp_textScalarBarHeight);

        cp_labelLegendX = new QLabel(cp_legendSizeGroupBox);
        cp_labelLegendX->setObjectName(QString::fromUtf8("cp_labelLegendX"));

        cp_legendSizeFormLayout->setWidget(2, QFormLayout::LabelRole, cp_labelLegendX);

        cp_textLegendXPosition = new QLineEdit(cp_legendSizeGroupBox);
        cp_textLegendXPosition->setObjectName(QString::fromUtf8("cp_textLegendXPosition"));
        cp_textLegendXPosition->setMinimumSize(QSize(150, 30));

        cp_legendSizeFormLayout->setWidget(2, QFormLayout::FieldRole, cp_textLegendXPosition);

        cp_labelLegendY = new QLabel(cp_legendSizeGroupBox);
        cp_labelLegendY->setObjectName(QString::fromUtf8("cp_labelLegendY"));

        cp_legendSizeFormLayout->setWidget(3, QFormLayout::LabelRole, cp_labelLegendY);

        cp_textLegendYPosition = new QLineEdit(cp_legendSizeGroupBox);
        cp_textLegendYPosition->setObjectName(QString::fromUtf8("cp_textLegendYPosition"));
        cp_textLegendYPosition->setMinimumSize(QSize(150, 30));

        cp_legendSizeFormLayout->setWidget(3, QFormLayout::FieldRole, cp_textLegendYPosition);


        cp_legendTabLayout->addWidget(cp_legendSizeGroupBox);

        cp_legendOrientationGroupBox = new QGroupBox(legendTab);
        cp_legendOrientationGroupBox->setObjectName(QString::fromUtf8("cp_legendOrientationGroupBox"));
        cp_orientationLayout = new QHBoxLayout(cp_legendOrientationGroupBox);
        cp_orientationLayout->setObjectName(QString::fromUtf8("cp_orientationLayout"));
        cp_radioVertical = new QRadioButton(cp_legendOrientationGroupBox);
        cp_radioVertical->setObjectName(QString::fromUtf8("cp_radioVertical"));
        cp_radioVertical->setChecked(true);

        cp_orientationLayout->addWidget(cp_radioVertical);

        cp_radioHorizontal = new QRadioButton(cp_legendOrientationGroupBox);
        cp_radioHorizontal->setObjectName(QString::fromUtf8("cp_radioHorizontal"));

        cp_orientationLayout->addWidget(cp_radioHorizontal);


        cp_legendTabLayout->addWidget(cp_legendOrientationGroupBox);

        cp_textAppearanceGroupBox = new QGroupBox(legendTab);
        cp_textAppearanceGroupBox->setObjectName(QString::fromUtf8("cp_textAppearanceGroupBox"));
        cp_textAppearanceFormLayout = new QFormLayout(cp_textAppearanceGroupBox);
        cp_textAppearanceFormLayout->setObjectName(QString::fromUtf8("cp_textAppearanceFormLayout"));
        cp_labelTextColor = new QLabel(cp_textAppearanceGroupBox);
        cp_labelTextColor->setObjectName(QString::fromUtf8("cp_labelTextColor"));

        cp_textAppearanceFormLayout->setWidget(0, QFormLayout::LabelRole, cp_labelTextColor);

        cp_textColorLayout = new QHBoxLayout();
        cp_textColorLayout->setObjectName(QString::fromUtf8("cp_textColorLayout"));
        cp_btnTextColor = new QPushButton(cp_textAppearanceGroupBox);
        cp_btnTextColor->setObjectName(QString::fromUtf8("cp_btnTextColor"));
        cp_btnTextColor->setMinimumSize(QSize(100, 30));

        cp_textColorLayout->addWidget(cp_btnTextColor);

        cp_textColorSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        cp_textColorLayout->addItem(cp_textColorSpacer);


        cp_textAppearanceFormLayout->setLayout(0, QFormLayout::FieldRole, cp_textColorLayout);

        cp_labelFontSize = new QLabel(cp_textAppearanceGroupBox);
        cp_labelFontSize->setObjectName(QString::fromUtf8("cp_labelFontSize"));

        cp_textAppearanceFormLayout->setWidget(1, QFormLayout::LabelRole, cp_labelFontSize);

        cp_spinFontSize = new QSpinBox(cp_textAppearanceGroupBox);
        cp_spinFontSize->setObjectName(QString::fromUtf8("cp_spinFontSize"));
        cp_spinFontSize->setMinimumSize(QSize(100, 30));
        cp_spinFontSize->setMinimum(8);
        cp_spinFontSize->setMaximum(48);
        cp_spinFontSize->setValue(16);

        cp_textAppearanceFormLayout->setWidget(1, QFormLayout::FieldRole, cp_spinFontSize);

        cp_labelFontFamily = new QLabel(cp_textAppearanceGroupBox);
        cp_labelFontFamily->setObjectName(QString::fromUtf8("cp_labelFontFamily"));

        cp_textAppearanceFormLayout->setWidget(2, QFormLayout::LabelRole, cp_labelFontFamily);

        cp_comboFontFamily = new QComboBox(cp_textAppearanceGroupBox);
        cp_comboFontFamily->addItem(QString());
        cp_comboFontFamily->addItem(QString());
        cp_comboFontFamily->addItem(QString());
        cp_comboFontFamily->addItem(QString());
        cp_comboFontFamily->setObjectName(QString::fromUtf8("cp_comboFontFamily"));
        cp_comboFontFamily->setMinimumSize(QSize(150, 30));

        cp_textAppearanceFormLayout->setWidget(2, QFormLayout::FieldRole, cp_comboFontFamily);

        cp_labelFontStyle = new QLabel(cp_textAppearanceGroupBox);
        cp_labelFontStyle->setObjectName(QString::fromUtf8("cp_labelFontStyle"));

        cp_textAppearanceFormLayout->setWidget(3, QFormLayout::LabelRole, cp_labelFontStyle);

        cp_fontStyleLayout = new QHBoxLayout();
        cp_fontStyleLayout->setObjectName(QString::fromUtf8("cp_fontStyleLayout"));
        cp_checkBold = new QCheckBox(cp_textAppearanceGroupBox);
        cp_checkBold->setObjectName(QString::fromUtf8("cp_checkBold"));
        cp_checkBold->setChecked(true);

        cp_fontStyleLayout->addWidget(cp_checkBold);

        cp_checkItalic = new QCheckBox(cp_textAppearanceGroupBox);
        cp_checkItalic->setObjectName(QString::fromUtf8("cp_checkItalic"));

        cp_fontStyleLayout->addWidget(cp_checkItalic);


        cp_textAppearanceFormLayout->setLayout(3, QFormLayout::FieldRole, cp_fontStyleLayout);


        cp_legendTabLayout->addWidget(cp_textAppearanceGroupBox);

        cp_legendTitleGroupBox = new QGroupBox(legendTab);
        cp_legendTitleGroupBox->setObjectName(QString::fromUtf8("cp_legendTitleGroupBox"));
        cp_legendTitleLayout = new QVBoxLayout(cp_legendTitleGroupBox);
        cp_legendTitleLayout->setObjectName(QString::fromUtf8("cp_legendTitleLayout"));
        cp_checkShowTitle = new QCheckBox(cp_legendTitleGroupBox);
        cp_checkShowTitle->setObjectName(QString::fromUtf8("cp_checkShowTitle"));
        cp_checkShowTitle->setChecked(true);

        cp_legendTitleLayout->addWidget(cp_checkShowTitle);

        cp_titleModeLayout = new QHBoxLayout();
        cp_titleModeLayout->setObjectName(QString::fromUtf8("cp_titleModeLayout"));
        cp_labelTitleMode = new QLabel(cp_legendTitleGroupBox);
        cp_labelTitleMode->setObjectName(QString::fromUtf8("cp_labelTitleMode"));

        cp_titleModeLayout->addWidget(cp_labelTitleMode);

        cp_comboTitleMode = new QComboBox(cp_legendTitleGroupBox);
        cp_comboTitleMode->addItem(QString());
        cp_comboTitleMode->addItem(QString());
        cp_comboTitleMode->setObjectName(QString::fromUtf8("cp_comboTitleMode"));
        cp_comboTitleMode->setMinimumSize(QSize(200, 30));

        cp_titleModeLayout->addWidget(cp_comboTitleMode);


        cp_legendTitleLayout->addLayout(cp_titleModeLayout);

        cp_titleTextLayout = new QHBoxLayout();
        cp_titleTextLayout->setObjectName(QString::fromUtf8("cp_titleTextLayout"));
        cp_labelTitleText = new QLabel(cp_legendTitleGroupBox);
        cp_labelTitleText->setObjectName(QString::fromUtf8("cp_labelTitleText"));

        cp_titleTextLayout->addWidget(cp_labelTitleText);

        cp_textTitleText = new QLineEdit(cp_legendTitleGroupBox);
        cp_textTitleText->setObjectName(QString::fromUtf8("cp_textTitleText"));
        cp_textTitleText->setEnabled(false);

        cp_titleTextLayout->addWidget(cp_textTitleText);


        cp_legendTitleLayout->addLayout(cp_titleTextLayout);


        cp_legendTabLayout->addWidget(cp_legendTitleGroupBox);

        cp_legendVerticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        cp_legendTabLayout->addItem(cp_legendVerticalSpacer);

        cutplaneTabWidget->addTab(legendTab, QString());

        verticalLayout_2->addWidget(cutplaneTabWidget);

        buttonBox = new QDialogButtonBox(CutplaneDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(CutplaneDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), CutplaneDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CutplaneDialog, SLOT(reject()));

        cutplaneTabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(CutplaneDialog);
    } // setupUi

    void retranslateUi(QDialog *CutplaneDialog)
    {
        CutplaneDialog->setWindowTitle(QCoreApplication::translate("CutplaneDialog", "Slice Details", nullptr));
        CutplaneDialog->setStyleSheet(QCoreApplication::translate("CutplaneDialog", "QDialog {\n"
"    background-color: #f5f5f5;\n"
"}\n"
"QGroupBox {\n"
"    font-weight: bold;\n"
"    border: 2px solid #cccccc;\n"
"    border-radius: 8px;\n"
"    margin-top: 1ex;\n"
"    padding-top: 10px;\n"
"}\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 5px 0 5px;\n"
"}\n"
"QLabel {\n"
"    color: #333333;\n"
"}\n"
"QPushButton {\n"
"    background-color: #5DADE2;\n"
"    border: none;\n"
"    color: white;\n"
"    padding: 8px 16px;\n"
"    border-radius: 4px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #3498db;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #2980b9;\n"
"}\n"
"QComboBox, QLineEdit {\n"
"    border: 1px solid #ddd;\n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"    background-color: white;\n"
"}\n"
"QComboBox:focus, QLineEdit:focus {\n"
"    border-color: #2980b9;\n"
"}\n"
"QSlider::groove:horizontal {\n"
"    border: 1px solid #999999;\n"
"    height: 8px;\n"
"    background: qli"
                        "neargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);\n"
"    margin: 2px 0;\n"
"    border-radius: 4px;\n"
"}\n"
"QSlider::handle:horizontal {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);\n"
"    border: 1px solid #5c5c5c;\n"
"    width: 18px;\n"
"    margin: -2px 0;\n"
"    border-radius: 9px;\n"
"}", nullptr));
        sliceControlGroup->setTitle(QCoreApplication::translate("CutplaneDialog", "Slice Control", nullptr));
        label->setText(QCoreApplication::translate("CutplaneDialog", "Slice No:", nullptr));
        Btn_AddNew->setText(QCoreApplication::translate("CutplaneDialog", "Extract Slices", nullptr));
        checkBox->setText(QCoreApplication::translate("CutplaneDialog", "Show Primary Slice", nullptr));
        label_SLoaction->setText(QCoreApplication::translate("CutplaneDialog", "Slice Locations:", nullptr));
        Combo_SLiceLocation->setItemText(0, QCoreApplication::translate("CutplaneDialog", "X-Planes", nullptr));
        Combo_SLiceLocation->setItemText(1, QCoreApplication::translate("CutplaneDialog", "Y-Planes", nullptr));
        Combo_SLiceLocation->setItemText(2, QCoreApplication::translate("CutplaneDialog", "Z-Planes", nullptr));

        label_ValueLocation->setText(QCoreApplication::translate("CutplaneDialog", "X: 0.0801", nullptr));
        Btn_SetPosition->setText(QCoreApplication::translate("CutplaneDialog", "Set Position", nullptr));
        mappingGroup->setTitle(QCoreApplication::translate("CutplaneDialog", "Mapping Variable", nullptr));
        lab_MapVariable->setText(QCoreApplication::translate("CutplaneDialog", "Variable:", nullptr));
        colorMapGroup->setTitle(QCoreApplication::translate("CutplaneDialog", "Color Map Options", nullptr));
        lab_ContourType->setText(QCoreApplication::translate("CutplaneDialog", "Color Scheme:", nullptr));
        checkReverseColorMap->setText(QCoreApplication::translate("CutplaneDialog", "Reverse Color Map", nullptr));
        label_3->setText(QCoreApplication::translate("CutplaneDialog", "Distribution Method:", nullptr));
        Check_Banded->setText(QCoreApplication::translate("CutplaneDialog", "Banded", nullptr));
        Check_Continuous->setText(QCoreApplication::translate("CutplaneDialog", "Continuous", nullptr));
        contoursGroup->setTitle(QCoreApplication::translate("CutplaneDialog", "Contours Level", nullptr));
        label_5->setText(QCoreApplication::translate("CutplaneDialog", "Minimum:", nullptr));
        LnEdit_Min->setInputMask(QString());
        LnEdit_Min->setText(QCoreApplication::translate("CutplaneDialog", "0.0", nullptr));
        label_6->setText(QCoreApplication::translate("CutplaneDialog", "Maximum:", nullptr));
        LnEdit_Max->setText(QCoreApplication::translate("CutplaneDialog", "2.0", nullptr));
        label_7->setText(QCoreApplication::translate("CutplaneDialog", "Numbers:", nullptr));
        LnEdit_Numbers->setText(QCoreApplication::translate("CutplaneDialog", "10", nullptr));
        cutplaneTabWidget->setTabText(cutplaneTabWidget->indexOf(sliceSettingsTab), QCoreApplication::translate("CutplaneDialog", "Slice Settings", nullptr));
        cp_showLegendGroupBox->setTitle(QCoreApplication::translate("CutplaneDialog", "Legend Display", nullptr));
        cp_checkShowLegend->setText(QCoreApplication::translate("CutplaneDialog", "Show Color Legend", nullptr));
        cp_legendSizeGroupBox->setTitle(QCoreApplication::translate("CutplaneDialog", "Size and Position", nullptr));
        cp_labelScalarBarWidth->setText(QCoreApplication::translate("CutplaneDialog", "Width:", nullptr));
        cp_textScalarBarWidth->setPlaceholderText(QCoreApplication::translate("CutplaneDialog", "0.1", nullptr));
        cp_labelScalarBarHeight->setText(QCoreApplication::translate("CutplaneDialog", "Height:", nullptr));
        cp_textScalarBarHeight->setPlaceholderText(QCoreApplication::translate("CutplaneDialog", "0.8", nullptr));
        cp_labelLegendX->setText(QCoreApplication::translate("CutplaneDialog", "X Position (%):", nullptr));
        cp_textLegendXPosition->setPlaceholderText(QCoreApplication::translate("CutplaneDialog", "90", nullptr));
        cp_labelLegendY->setText(QCoreApplication::translate("CutplaneDialog", "Y Position (%):", nullptr));
        cp_textLegendYPosition->setPlaceholderText(QCoreApplication::translate("CutplaneDialog", "10", nullptr));
        cp_legendOrientationGroupBox->setTitle(QCoreApplication::translate("CutplaneDialog", "Orientation", nullptr));
        cp_radioVertical->setText(QCoreApplication::translate("CutplaneDialog", "Vertical", nullptr));
        cp_radioHorizontal->setText(QCoreApplication::translate("CutplaneDialog", "Horizontal", nullptr));
        cp_textAppearanceGroupBox->setTitle(QCoreApplication::translate("CutplaneDialog", "Text Appearance", nullptr));
        cp_labelTextColor->setText(QCoreApplication::translate("CutplaneDialog", "Text Color:", nullptr));
        cp_btnTextColor->setStyleSheet(QCoreApplication::translate("CutplaneDialog", "QPushButton { background-color: black; color: white; }", nullptr));
        cp_btnTextColor->setText(QCoreApplication::translate("CutplaneDialog", "Black", nullptr));
        cp_labelFontSize->setText(QCoreApplication::translate("CutplaneDialog", "Font Size:", nullptr));
        cp_labelFontFamily->setText(QCoreApplication::translate("CutplaneDialog", "Font type:", nullptr));
        cp_comboFontFamily->setItemText(0, QCoreApplication::translate("CutplaneDialog", "Arial", nullptr));
        cp_comboFontFamily->setItemText(1, QCoreApplication::translate("CutplaneDialog", "Times New Roman", nullptr));
        cp_comboFontFamily->setItemText(2, QCoreApplication::translate("CutplaneDialog", "Courier New", nullptr));
        cp_comboFontFamily->setItemText(3, QCoreApplication::translate("CutplaneDialog", "Helvetica", nullptr));

        cp_labelFontStyle->setText(QCoreApplication::translate("CutplaneDialog", "Font Style:", nullptr));
        cp_checkBold->setText(QCoreApplication::translate("CutplaneDialog", "Bold", nullptr));
        cp_checkItalic->setText(QCoreApplication::translate("CutplaneDialog", "Italic", nullptr));
        cp_legendTitleGroupBox->setTitle(QCoreApplication::translate("CutplaneDialog", "Legend Title", nullptr));
        cp_checkShowTitle->setText(QCoreApplication::translate("CutplaneDialog", "Show Title", nullptr));
        cp_labelTitleMode->setText(QCoreApplication::translate("CutplaneDialog", "Title Mode:", nullptr));
        cp_comboTitleMode->setItemText(0, QCoreApplication::translate("CutplaneDialog", "Use Variable Name", nullptr));
        cp_comboTitleMode->setItemText(1, QCoreApplication::translate("CutplaneDialog", "Use Custom Text", nullptr));

        cp_labelTitleText->setText(QCoreApplication::translate("CutplaneDialog", "Custom Title:", nullptr));
        cp_textTitleText->setPlaceholderText(QCoreApplication::translate("CutplaneDialog", "Enter custom text", nullptr));
        cutplaneTabWidget->setTabText(cutplaneTabWidget->indexOf(legendTab), QCoreApplication::translate("CutplaneDialog", "Legend", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CutplaneDialog: public Ui_CutplaneDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUTPLANEDIALOG_H
