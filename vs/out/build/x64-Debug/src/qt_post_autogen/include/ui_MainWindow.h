/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QVTKOpenGLNativeWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpenFile;
    QAction *actionLoadMesh;
    QAction *actionUpdateFlow;
    QAction *actionAddScalarBarActor;
    QAction *actionAddAxisActor;
    QAction *actionAddOutlineActor;
    QAction *actionAddPointInformation;
    QAction *actionCalculatorFunction;
    QAction *actionNewWindow;
    QAction *actionExport_Picture;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_3;
    QTabWidget *tabWidget;
    QWidget *tab_normal;
    QVBoxLayout *verticalLayout_tab1;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents_2;
    QVBoxLayout *verticalLayout;
    QLabel *label_5;
    QWidget *widget_4;
    QVBoxLayout *verticalLayout_2;
    QWidget *buttonWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *xoyView;
    QPushButton *xozView;
    QPushButton *yozView;
    QPushButton *xyzView;
    QLabel *label;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer_5;
    QCheckBox *contourCheckBox;
    QPushButton *contourSettingButton;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_6;
    QCheckBox *meshCheckBox;
    QSpacerItem *horizontalSpacer_7;
    QCheckBox *shadeCheckBox;
    QCheckBox *edgeCheckBox;
    QPushButton *selectBoundaryButton;
    QLabel *label_4;
    QWidget *widget_3;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer_2;
    QCheckBox *slicesCheckBox;
    QCheckBox *isoSurfaceCheckBox;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer;
    QCheckBox *streamlineCheckBox;
    QPushButton *IsoSurfaceSettingButton;
    QPushButton *addSliceButton;
    QPushButton *addStreamLineButton;
    QLabel *label_6;
    QWidget *widget_5;
    QGridLayout *gridLayout_4;
    QSpacerItem *horizontalSpacer_16;
    QCheckBox *lightingCheckBox;
    QSpacerItem *horizontalSpacer_14;
    QPushButton *lightingButton;
    QPushButton *transparantButton;
    QCheckBox *transparancyCheckBox;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_10;
    QCheckBox *ConstHeightCheckBox;
    QPushButton *ConstHeightpushButton;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_7;
    QComboBox *CBtn_BackGround;
    QSpacerItem *verticalSpacer;
    QWidget *tab_turbo;
    QVBoxLayout *turboTabLayout;
    QLabel *label_3;
    QPushButton *Btn_TurboInitial;
    QWidget *Wi_TurboSet;
    QVBoxLayout *turboSetLayout;
    QWidget *Wi_BasicInfo;
    QVBoxLayout *basicInfoLayout;
    QCheckBox *Check_3Dview;
    QCheckBox *Check_Meri;
    QCheckBox *Check_BladeToBlade;
    QCheckBox *Check_ThreeView;
    QWidget *Wi_Copy;
    QGridLayout *gridLayout_3;
    QLabel *label_8;
    QLabel *label_9;
    QComboBox *com_copy;
    QLabel *label_10;
    QLineEdit *Led_Copy;
    QScrollArea *periodicCopyScrollArea;
    QWidget *periodicCopyContainer;
    QGridLayout *periodicCopyGridLayout;
    QPushButton *Btn_ClearCopyCurrentZone;
    QPushButton *Btn_ClearAllZoneCopy;
    QWidget *Wi_B2B_2;
    QGridLayout *Wi_B2B;
    QLabel *label_BladeToBlade;
    QLabel *label_11;
    QComboBox *com_Variable;
    QLabel *label_12;
    QLineEdit *LEd_range_Min;
    QLabel *label_13;
    QLineEdit *LEd_range_Max;
    QLabel *label_14;
    QLineEdit *LEd_Level;
    QLabel *label_19;
    QSlider *Sli_Span;
    QWidget *Wi_B2B_3;
    QGridLayout *Wi_B2B_4;
    QLabel *label_BladeToBlade_2;
    QLabel *label_17;
    QComboBox *comboBox_3;
    QLabel *label_15;
    QLineEdit *LEd_range_Min_2;
    QLabel *label_16;
    QLineEdit *LEd_range_Max_2;
    QLabel *label_18;
    QLineEdit *LEd_Level_2;
    QSpacerItem *turboVerticalSpacer;
    QVBoxLayout *VTKLayout;
    QVTKOpenGLNativeWidget *vtkBox;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuTools;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1666, 1000);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        MainWindow->setFont(font);
        actionOpenFile = new QAction(MainWindow);
        actionOpenFile->setObjectName(QString::fromUtf8("actionOpenFile"));
        actionLoadMesh = new QAction(MainWindow);
        actionLoadMesh->setObjectName(QString::fromUtf8("actionLoadMesh"));
        actionUpdateFlow = new QAction(MainWindow);
        actionUpdateFlow->setObjectName(QString::fromUtf8("actionUpdateFlow"));
        actionAddScalarBarActor = new QAction(MainWindow);
        actionAddScalarBarActor->setObjectName(QString::fromUtf8("actionAddScalarBarActor"));
        actionAddAxisActor = new QAction(MainWindow);
        actionAddAxisActor->setObjectName(QString::fromUtf8("actionAddAxisActor"));
        actionAddOutlineActor = new QAction(MainWindow);
        actionAddOutlineActor->setObjectName(QString::fromUtf8("actionAddOutlineActor"));
        actionAddPointInformation = new QAction(MainWindow);
        actionAddPointInformation->setObjectName(QString::fromUtf8("actionAddPointInformation"));
        actionCalculatorFunction = new QAction(MainWindow);
        actionCalculatorFunction->setObjectName(QString::fromUtf8("actionCalculatorFunction"));
        actionNewWindow = new QAction(MainWindow);
        actionNewWindow->setObjectName(QString::fromUtf8("actionNewWindow"));
        actionExport_Picture = new QAction(MainWindow);
        actionExport_Picture->setObjectName(QString::fromUtf8("actionExport_Picture"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout_2 = new QHBoxLayout(centralwidget);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        widget_2 = new QWidget(centralwidget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout_3 = new QHBoxLayout(widget_2);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(widget_2);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy1);
        tabWidget->setMinimumSize(QSize(260, 0));
        tabWidget->setMaximumSize(QSize(240, 16777215));
        tab_normal = new QWidget();
        tab_normal->setObjectName(QString::fromUtf8("tab_normal"));
        verticalLayout_tab1 = new QVBoxLayout(tab_normal);
        verticalLayout_tab1->setObjectName(QString::fromUtf8("verticalLayout_tab1"));
        scrollArea = new QScrollArea(tab_normal);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setEnabled(false);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 238, 867));
        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents_2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_5 = new QLabel(scrollAreaWidgetContents_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy2);
        label_5->setFont(font);

        verticalLayout->addWidget(label_5);

        widget_4 = new QWidget(scrollAreaWidgetContents_2);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        sizePolicy2.setHeightForWidth(widget_4->sizePolicy().hasHeightForWidth());
        widget_4->setSizePolicy(sizePolicy2);
        verticalLayout_2 = new QVBoxLayout(widget_4);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        buttonWidget = new QWidget(widget_4);
        buttonWidget->setObjectName(QString::fromUtf8("buttonWidget"));
        sizePolicy2.setHeightForWidth(buttonWidget->sizePolicy().hasHeightForWidth());
        buttonWidget->setSizePolicy(sizePolicy2);
        horizontalLayout = new QHBoxLayout(buttonWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        xoyView = new QPushButton(buttonWidget);
        xoyView->setObjectName(QString::fromUtf8("xoyView"));
        sizePolicy2.setHeightForWidth(xoyView->sizePolicy().hasHeightForWidth());
        xoyView->setSizePolicy(sizePolicy2);
        xoyView->setMinimumSize(QSize(35, 35));
        xoyView->setMaximumSize(QSize(35, 35));

        horizontalLayout->addWidget(xoyView);

        xozView = new QPushButton(buttonWidget);
        xozView->setObjectName(QString::fromUtf8("xozView"));
        sizePolicy2.setHeightForWidth(xozView->sizePolicy().hasHeightForWidth());
        xozView->setSizePolicy(sizePolicy2);
        xozView->setMinimumSize(QSize(35, 35));
        xozView->setMaximumSize(QSize(35, 35));

        horizontalLayout->addWidget(xozView);

        yozView = new QPushButton(buttonWidget);
        yozView->setObjectName(QString::fromUtf8("yozView"));
        sizePolicy2.setHeightForWidth(yozView->sizePolicy().hasHeightForWidth());
        yozView->setSizePolicy(sizePolicy2);
        yozView->setMinimumSize(QSize(35, 35));
        yozView->setMaximumSize(QSize(35, 35));

        horizontalLayout->addWidget(yozView);

        xyzView = new QPushButton(buttonWidget);
        xyzView->setObjectName(QString::fromUtf8("xyzView"));
        sizePolicy2.setHeightForWidth(xyzView->sizePolicy().hasHeightForWidth());
        xyzView->setSizePolicy(sizePolicy2);
        xyzView->setMinimumSize(QSize(35, 35));
        xyzView->setMaximumSize(QSize(35, 35));

        horizontalLayout->addWidget(xyzView);


        verticalLayout_2->addWidget(buttonWidget);


        verticalLayout->addWidget(widget_4);

        label = new QLabel(scrollAreaWidgetContents_2);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);
        label->setFont(font);
        label->setContextMenuPolicy(Qt::ContextMenuPolicy::PreventContextMenu);

        verticalLayout->addWidget(label);

        widget = new QWidget(scrollAreaWidgetContents_2);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy3);
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalSpacer_5 = new QSpacerItem(10, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_5, 3, 0, 1, 1);

        contourCheckBox = new QCheckBox(widget);
        contourCheckBox->setObjectName(QString::fromUtf8("contourCheckBox"));
        contourCheckBox->setMinimumSize(QSize(150, 0));
        contourCheckBox->setFont(font);
        contourCheckBox->setChecked(true);

        gridLayout_2->addWidget(contourCheckBox, 3, 1, 1, 1);

        contourSettingButton = new QPushButton(widget);
        contourSettingButton->setObjectName(QString::fromUtf8("contourSettingButton"));
        contourSettingButton->setMinimumSize(QSize(20, 20));
        contourSettingButton->setMaximumSize(QSize(20, 20));

        gridLayout_2->addWidget(contourSettingButton, 3, 2, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(10, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 2, 0, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(10, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_6, 4, 0, 1, 1);

        meshCheckBox = new QCheckBox(widget);
        meshCheckBox->setObjectName(QString::fromUtf8("meshCheckBox"));
        meshCheckBox->setFont(font);

        gridLayout_2->addWidget(meshCheckBox, 2, 1, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(10, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_7, 5, 0, 1, 1);

        shadeCheckBox = new QCheckBox(widget);
        shadeCheckBox->setObjectName(QString::fromUtf8("shadeCheckBox"));
        shadeCheckBox->setFont(font);
        shadeCheckBox->setChecked(true);

        gridLayout_2->addWidget(shadeCheckBox, 4, 1, 1, 1);

        edgeCheckBox = new QCheckBox(widget);
        edgeCheckBox->setObjectName(QString::fromUtf8("edgeCheckBox"));
        edgeCheckBox->setFont(font);

        gridLayout_2->addWidget(edgeCheckBox, 5, 1, 1, 1);


        verticalLayout->addWidget(widget);

        selectBoundaryButton = new QPushButton(scrollAreaWidgetContents_2);
        selectBoundaryButton->setObjectName(QString::fromUtf8("selectBoundaryButton"));
        selectBoundaryButton->setFont(font);

        verticalLayout->addWidget(selectBoundaryButton);

        label_4 = new QLabel(scrollAreaWidgetContents_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy2.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy2);
        label_4->setFont(font);

        verticalLayout->addWidget(label_4);

        widget_3 = new QWidget(scrollAreaWidgetContents_2);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        sizePolicy3.setHeightForWidth(widget_3->sizePolicy().hasHeightForWidth());
        widget_3->setSizePolicy(sizePolicy3);
        widget_3->setMinimumSize(QSize(220, 0));
        widget_3->setMaximumSize(QSize(220, 16777215));
        gridLayout = new QGridLayout(widget_3);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalSpacer_2 = new QSpacerItem(10, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 2, 0, 1, 1);

        slicesCheckBox = new QCheckBox(widget_3);
        slicesCheckBox->setObjectName(QString::fromUtf8("slicesCheckBox"));
        slicesCheckBox->setMinimumSize(QSize(186, 0));
        slicesCheckBox->setFont(font);

        gridLayout->addWidget(slicesCheckBox, 2, 1, 1, 1);

        isoSurfaceCheckBox = new QCheckBox(widget_3);
        isoSurfaceCheckBox->setObjectName(QString::fromUtf8("isoSurfaceCheckBox"));
        sizePolicy.setHeightForWidth(isoSurfaceCheckBox->sizePolicy().hasHeightForWidth());
        isoSurfaceCheckBox->setSizePolicy(sizePolicy);
        isoSurfaceCheckBox->setMinimumSize(QSize(186, 1));
        isoSurfaceCheckBox->setMaximumSize(QSize(16777215, 16777215));
        isoSurfaceCheckBox->setFont(font);
        isoSurfaceCheckBox->setIconSize(QSize(16, 16));
        isoSurfaceCheckBox->setChecked(false);
        isoSurfaceCheckBox->setAutoExclusive(false);

        gridLayout->addWidget(isoSurfaceCheckBox, 1, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(10, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 3, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(10, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        streamlineCheckBox = new QCheckBox(widget_3);
        streamlineCheckBox->setObjectName(QString::fromUtf8("streamlineCheckBox"));
        streamlineCheckBox->setMinimumSize(QSize(186, 0));
        streamlineCheckBox->setFont(font);

        gridLayout->addWidget(streamlineCheckBox, 3, 1, 1, 1);

        IsoSurfaceSettingButton = new QPushButton(widget_3);
        IsoSurfaceSettingButton->setObjectName(QString::fromUtf8("IsoSurfaceSettingButton"));
        IsoSurfaceSettingButton->setMinimumSize(QSize(20, 20));
        IsoSurfaceSettingButton->setMaximumSize(QSize(20, 20));

        gridLayout->addWidget(IsoSurfaceSettingButton, 1, 2, 1, 1);

        addSliceButton = new QPushButton(widget_3);
        addSliceButton->setObjectName(QString::fromUtf8("addSliceButton"));
        addSliceButton->setMinimumSize(QSize(20, 20));
        addSliceButton->setMaximumSize(QSize(20, 20));

        gridLayout->addWidget(addSliceButton, 2, 2, 1, 1);

        addStreamLineButton = new QPushButton(widget_3);
        addStreamLineButton->setObjectName(QString::fromUtf8("addStreamLineButton"));
        addStreamLineButton->setMinimumSize(QSize(20, 20));
        addStreamLineButton->setMaximumSize(QSize(20, 20));

        gridLayout->addWidget(addStreamLineButton, 3, 2, 1, 1);


        verticalLayout->addWidget(widget_3);

        label_6 = new QLabel(scrollAreaWidgetContents_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        sizePolicy2.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy2);
        label_6->setFont(font);

        verticalLayout->addWidget(label_6);

        widget_5 = new QWidget(scrollAreaWidgetContents_2);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));
        sizePolicy3.setHeightForWidth(widget_5->sizePolicy().hasHeightForWidth());
        widget_5->setSizePolicy(sizePolicy3);
        widget_5->setMinimumSize(QSize(220, 0));
        widget_5->setMaximumSize(QSize(220, 16777215));
        gridLayout_4 = new QGridLayout(widget_5);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalSpacer_16 = new QSpacerItem(10, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_16, 1, 0, 1, 1);

        lightingCheckBox = new QCheckBox(widget_5);
        lightingCheckBox->setObjectName(QString::fromUtf8("lightingCheckBox"));
        lightingCheckBox->setMinimumSize(QSize(186, 0));
        lightingCheckBox->setFont(font);

        gridLayout_4->addWidget(lightingCheckBox, 2, 1, 1, 1);

        horizontalSpacer_14 = new QSpacerItem(10, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_14, 2, 0, 1, 1);

        lightingButton = new QPushButton(widget_5);
        lightingButton->setObjectName(QString::fromUtf8("lightingButton"));
        lightingButton->setMinimumSize(QSize(20, 20));
        lightingButton->setMaximumSize(QSize(20, 20));

        gridLayout_4->addWidget(lightingButton, 2, 2, 1, 1);

        transparantButton = new QPushButton(widget_5);
        transparantButton->setObjectName(QString::fromUtf8("transparantButton"));
        transparantButton->setMinimumSize(QSize(20, 20));
        transparantButton->setMaximumSize(QSize(20, 20));

        gridLayout_4->addWidget(transparantButton, 1, 2, 1, 1);

        transparancyCheckBox = new QCheckBox(widget_5);
        transparancyCheckBox->setObjectName(QString::fromUtf8("transparancyCheckBox"));
        sizePolicy.setHeightForWidth(transparancyCheckBox->sizePolicy().hasHeightForWidth());
        transparancyCheckBox->setSizePolicy(sizePolicy);
        transparancyCheckBox->setMinimumSize(QSize(186, 1));
        transparancyCheckBox->setMaximumSize(QSize(16777215, 16777215));
        transparancyCheckBox->setFont(font);
        transparancyCheckBox->setIconSize(QSize(16, 16));
        transparancyCheckBox->setChecked(false);
        transparancyCheckBox->setAutoExclusive(false);

        gridLayout_4->addWidget(transparancyCheckBox, 1, 1, 1, 1);


        verticalLayout->addWidget(widget_5);

        label_2 = new QLabel(scrollAreaWidgetContents_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_10 = new QSpacerItem(10, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_10);

        ConstHeightCheckBox = new QCheckBox(scrollAreaWidgetContents_2);
        ConstHeightCheckBox->setObjectName(QString::fromUtf8("ConstHeightCheckBox"));

        horizontalLayout_5->addWidget(ConstHeightCheckBox);

        ConstHeightpushButton = new QPushButton(scrollAreaWidgetContents_2);
        ConstHeightpushButton->setObjectName(QString::fromUtf8("ConstHeightpushButton"));
        ConstHeightpushButton->setMinimumSize(QSize(20, 20));
        ConstHeightpushButton->setMaximumSize(QSize(20, 20));

        horizontalLayout_5->addWidget(ConstHeightpushButton);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_7 = new QLabel(scrollAreaWidgetContents_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(true);
        label_7->setFont(font1);

        horizontalLayout_6->addWidget(label_7);

        CBtn_BackGround = new QComboBox(scrollAreaWidgetContents_2);
        CBtn_BackGround->addItem(QString());
        CBtn_BackGround->addItem(QString());
        CBtn_BackGround->setObjectName(QString::fromUtf8("CBtn_BackGround"));
        QFont font2;
        font2.setPointSize(9);
        font2.setBold(true);
        CBtn_BackGround->setFont(font2);

        horizontalLayout_6->addWidget(CBtn_BackGround);


        verticalLayout->addLayout(horizontalLayout_6);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(verticalSpacer);

        scrollArea->setWidget(scrollAreaWidgetContents_2);

        verticalLayout_tab1->addWidget(scrollArea);

        tabWidget->addTab(tab_normal, QString());
        tab_turbo = new QWidget();
        tab_turbo->setObjectName(QString::fromUtf8("tab_turbo"));
        turboTabLayout = new QVBoxLayout(tab_turbo);
        turboTabLayout->setObjectName(QString::fromUtf8("turboTabLayout"));
        turboTabLayout->setContentsMargins(4, 4, 4, 4);
        label_3 = new QLabel(tab_turbo);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);

        turboTabLayout->addWidget(label_3);

        Btn_TurboInitial = new QPushButton(tab_turbo);
        Btn_TurboInitial->setObjectName(QString::fromUtf8("Btn_TurboInitial"));
        Btn_TurboInitial->setFont(font);

        turboTabLayout->addWidget(Btn_TurboInitial);

        Wi_TurboSet = new QWidget(tab_turbo);
        Wi_TurboSet->setObjectName(QString::fromUtf8("Wi_TurboSet"));
        Wi_TurboSet->setEnabled(false);
        turboSetLayout = new QVBoxLayout(Wi_TurboSet);
        turboSetLayout->setSpacing(6);
        turboSetLayout->setObjectName(QString::fromUtf8("turboSetLayout"));
        turboSetLayout->setContentsMargins(0, 0, 0, 0);
        Wi_BasicInfo = new QWidget(Wi_TurboSet);
        Wi_BasicInfo->setObjectName(QString::fromUtf8("Wi_BasicInfo"));
        basicInfoLayout = new QVBoxLayout(Wi_BasicInfo);
        basicInfoLayout->setSpacing(2);
        basicInfoLayout->setObjectName(QString::fromUtf8("basicInfoLayout"));
        basicInfoLayout->setContentsMargins(0, 0, 0, 0);
        Check_3Dview = new QCheckBox(Wi_BasicInfo);
        Check_3Dview->setObjectName(QString::fromUtf8("Check_3Dview"));
        Check_3Dview->setFont(font1);
        Check_3Dview->setChecked(true);

        basicInfoLayout->addWidget(Check_3Dview);

        Check_Meri = new QCheckBox(Wi_BasicInfo);
        Check_Meri->setObjectName(QString::fromUtf8("Check_Meri"));
        Check_Meri->setFont(font1);

        basicInfoLayout->addWidget(Check_Meri);

        Check_BladeToBlade = new QCheckBox(Wi_BasicInfo);
        Check_BladeToBlade->setObjectName(QString::fromUtf8("Check_BladeToBlade"));
        Check_BladeToBlade->setFont(font1);

        basicInfoLayout->addWidget(Check_BladeToBlade);

        Check_ThreeView = new QCheckBox(Wi_BasicInfo);
        Check_ThreeView->setObjectName(QString::fromUtf8("Check_ThreeView"));
        Check_ThreeView->setFont(font1);

        basicInfoLayout->addWidget(Check_ThreeView);


        turboSetLayout->addWidget(Wi_BasicInfo);

        Wi_Copy = new QWidget(Wi_TurboSet);
        Wi_Copy->setObjectName(QString::fromUtf8("Wi_Copy"));
        gridLayout_3 = new QGridLayout(Wi_Copy);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_8 = new QLabel(Wi_Copy);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setFont(font);

        gridLayout_3->addWidget(label_8, 0, 0, 1, 2);

        label_9 = new QLabel(Wi_Copy);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        QFont font3;
        font3.setPointSize(10);
        font3.setBold(false);
        label_9->setFont(font3);

        gridLayout_3->addWidget(label_9, 1, 0, 1, 1);

        com_copy = new QComboBox(Wi_Copy);
        com_copy->setObjectName(QString::fromUtf8("com_copy"));

        gridLayout_3->addWidget(com_copy, 1, 1, 1, 1);

        label_10 = new QLabel(Wi_Copy);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setFont(font3);

        gridLayout_3->addWidget(label_10, 2, 0, 1, 1);

        Led_Copy = new QLineEdit(Wi_Copy);
        Led_Copy->setObjectName(QString::fromUtf8("Led_Copy"));

        gridLayout_3->addWidget(Led_Copy, 2, 1, 1, 1);

        periodicCopyScrollArea = new QScrollArea(Wi_Copy);
        periodicCopyScrollArea->setObjectName(QString::fromUtf8("periodicCopyScrollArea"));
        periodicCopyScrollArea->setMinimumSize(QSize(0, 180));
        periodicCopyScrollArea->setFrameShape(QFrame::Shape::NoFrame);
        periodicCopyScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        periodicCopyScrollArea->setWidgetResizable(true);
        periodicCopyContainer = new QWidget();
        periodicCopyContainer->setObjectName(QString::fromUtf8("periodicCopyContainer"));
        periodicCopyContainer->setGeometry(QRect(0, 0, 228, 180));
        periodicCopyGridLayout = new QGridLayout(periodicCopyContainer);
        periodicCopyGridLayout->setObjectName(QString::fromUtf8("periodicCopyGridLayout"));
        periodicCopyScrollArea->setWidget(periodicCopyContainer);

        gridLayout_3->addWidget(periodicCopyScrollArea, 3, 0, 1, 2);

        Btn_ClearCopyCurrentZone = new QPushButton(Wi_Copy);
        Btn_ClearCopyCurrentZone->setObjectName(QString::fromUtf8("Btn_ClearCopyCurrentZone"));

        gridLayout_3->addWidget(Btn_ClearCopyCurrentZone, 4, 0, 1, 1);

        Btn_ClearAllZoneCopy = new QPushButton(Wi_Copy);
        Btn_ClearAllZoneCopy->setObjectName(QString::fromUtf8("Btn_ClearAllZoneCopy"));

        gridLayout_3->addWidget(Btn_ClearAllZoneCopy, 4, 1, 1, 1);


        turboSetLayout->addWidget(Wi_Copy);

        Wi_B2B_2 = new QWidget(Wi_TurboSet);
        Wi_B2B_2->setObjectName(QString::fromUtf8("Wi_B2B_2"));
        Wi_B2B = new QGridLayout(Wi_B2B_2);
        Wi_B2B->setObjectName(QString::fromUtf8("Wi_B2B"));
        label_BladeToBlade = new QLabel(Wi_B2B_2);
        label_BladeToBlade->setObjectName(QString::fromUtf8("label_BladeToBlade"));
        label_BladeToBlade->setFont(font);

        Wi_B2B->addWidget(label_BladeToBlade, 0, 0, 1, 2);

        label_11 = new QLabel(Wi_B2B_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setFont(font3);

        Wi_B2B->addWidget(label_11, 1, 0, 1, 1);

        com_Variable = new QComboBox(Wi_B2B_2);
        com_Variable->setObjectName(QString::fromUtf8("com_Variable"));

        Wi_B2B->addWidget(com_Variable, 1, 1, 1, 1);

        label_12 = new QLabel(Wi_B2B_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setFont(font3);

        Wi_B2B->addWidget(label_12, 2, 0, 1, 1);

        LEd_range_Min = new QLineEdit(Wi_B2B_2);
        LEd_range_Min->setObjectName(QString::fromUtf8("LEd_range_Min"));

        Wi_B2B->addWidget(LEd_range_Min, 2, 1, 1, 1);

        label_13 = new QLabel(Wi_B2B_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setFont(font3);

        Wi_B2B->addWidget(label_13, 3, 0, 1, 1);

        LEd_range_Max = new QLineEdit(Wi_B2B_2);
        LEd_range_Max->setObjectName(QString::fromUtf8("LEd_range_Max"));

        Wi_B2B->addWidget(LEd_range_Max, 3, 1, 1, 1);

        label_14 = new QLabel(Wi_B2B_2);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setFont(font3);

        Wi_B2B->addWidget(label_14, 4, 0, 1, 1);

        LEd_Level = new QLineEdit(Wi_B2B_2);
        LEd_Level->setObjectName(QString::fromUtf8("LEd_Level"));

        Wi_B2B->addWidget(LEd_Level, 4, 1, 1, 1);

        label_19 = new QLabel(Wi_B2B_2);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setFont(font3);

        Wi_B2B->addWidget(label_19, 5, 0, 1, 1);

        Sli_Span = new QSlider(Wi_B2B_2);
        Sli_Span->setObjectName(QString::fromUtf8("Sli_Span"));
        Sli_Span->setOrientation(Qt::Orientation::Horizontal);

        Wi_B2B->addWidget(Sli_Span, 5, 1, 1, 1);


        turboSetLayout->addWidget(Wi_B2B_2);

        Wi_B2B_3 = new QWidget(Wi_TurboSet);
        Wi_B2B_3->setObjectName(QString::fromUtf8("Wi_B2B_3"));
        Wi_B2B_4 = new QGridLayout(Wi_B2B_3);
        Wi_B2B_4->setObjectName(QString::fromUtf8("Wi_B2B_4"));
        label_BladeToBlade_2 = new QLabel(Wi_B2B_3);
        label_BladeToBlade_2->setObjectName(QString::fromUtf8("label_BladeToBlade_2"));
        label_BladeToBlade_2->setFont(font);

        Wi_B2B_4->addWidget(label_BladeToBlade_2, 0, 0, 1, 2);

        label_17 = new QLabel(Wi_B2B_3);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setFont(font3);

        Wi_B2B_4->addWidget(label_17, 1, 0, 1, 1);

        comboBox_3 = new QComboBox(Wi_B2B_3);
        comboBox_3->setObjectName(QString::fromUtf8("comboBox_3"));

        Wi_B2B_4->addWidget(comboBox_3, 1, 1, 1, 1);

        label_15 = new QLabel(Wi_B2B_3);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setFont(font3);

        Wi_B2B_4->addWidget(label_15, 2, 0, 1, 1);

        LEd_range_Min_2 = new QLineEdit(Wi_B2B_3);
        LEd_range_Min_2->setObjectName(QString::fromUtf8("LEd_range_Min_2"));

        Wi_B2B_4->addWidget(LEd_range_Min_2, 2, 1, 1, 1);

        label_16 = new QLabel(Wi_B2B_3);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setFont(font3);

        Wi_B2B_4->addWidget(label_16, 3, 0, 1, 1);

        LEd_range_Max_2 = new QLineEdit(Wi_B2B_3);
        LEd_range_Max_2->setObjectName(QString::fromUtf8("LEd_range_Max_2"));

        Wi_B2B_4->addWidget(LEd_range_Max_2, 3, 1, 1, 1);

        label_18 = new QLabel(Wi_B2B_3);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setFont(font3);

        Wi_B2B_4->addWidget(label_18, 4, 0, 1, 1);

        LEd_Level_2 = new QLineEdit(Wi_B2B_3);
        LEd_Level_2->setObjectName(QString::fromUtf8("LEd_Level_2"));

        Wi_B2B_4->addWidget(LEd_Level_2, 4, 1, 1, 1);


        turboSetLayout->addWidget(Wi_B2B_3);

        turboVerticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        turboSetLayout->addItem(turboVerticalSpacer);


        turboTabLayout->addWidget(Wi_TurboSet);

        tabWidget->addTab(tab_turbo, QString());

        horizontalLayout_3->addWidget(tabWidget);

        VTKLayout = new QVBoxLayout();
        VTKLayout->setSpacing(0);
        VTKLayout->setObjectName(QString::fromUtf8("VTKLayout"));
        vtkBox = new QVTKOpenGLNativeWidget(widget_2);
        vtkBox->setObjectName(QString::fromUtf8("vtkBox"));

        VTKLayout->addWidget(vtkBox);


        horizontalLayout_3->addLayout(VTKLayout);


        horizontalLayout_2->addWidget(widget_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1666, 27));
        QFont font4;
        font4.setPointSize(12);
        font4.setBold(true);
        menubar->setFont(font4);
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuFile->setFont(font4);
        menuTools = new QMenu(menubar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuTools->setFont(font4);
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuTools->menuAction());
        menuFile->addAction(actionLoadMesh);
        menuFile->addAction(actionUpdateFlow);
        menuFile->addAction(actionExport_Picture);
        toolBar->addAction(actionNewWindow);
        toolBar->addAction(actionOpenFile);
        toolBar->addAction(actionAddScalarBarActor);
        toolBar->addAction(actionAddAxisActor);
        toolBar->addAction(actionAddOutlineActor);
        toolBar->addAction(actionAddPointInformation);
        toolBar->addAction(actionCalculatorFunction);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "CFD Post-processing Tool", nullptr));
        actionOpenFile->setText(QCoreApplication::translate("MainWindow", "OpenFile", nullptr));
#if QT_CONFIG(tooltip)
        actionOpenFile->setToolTip(QCoreApplication::translate("MainWindow", "open aes mesh file", nullptr));
#endif // QT_CONFIG(tooltip)
        actionLoadMesh->setText(QCoreApplication::translate("MainWindow", "&Load Mesh", nullptr));
        actionUpdateFlow->setText(QCoreApplication::translate("MainWindow", "&Update Flow", nullptr));
        actionAddScalarBarActor->setText(QCoreApplication::translate("MainWindow", "AddScalarBarActor", nullptr));
#if QT_CONFIG(tooltip)
        actionAddScalarBarActor->setToolTip(QCoreApplication::translate("MainWindow", "add scalarBar", nullptr));
#endif // QT_CONFIG(tooltip)
        actionAddAxisActor->setText(QCoreApplication::translate("MainWindow", "AddAxisActor", nullptr));
#if QT_CONFIG(tooltip)
        actionAddAxisActor->setToolTip(QCoreApplication::translate("MainWindow", "add coordinate axis", nullptr));
#endif // QT_CONFIG(tooltip)
        actionAddOutlineActor->setText(QCoreApplication::translate("MainWindow", "AddOutlineActor", nullptr));
#if QT_CONFIG(tooltip)
        actionAddOutlineActor->setToolTip(QCoreApplication::translate("MainWindow", "add outline", nullptr));
#endif // QT_CONFIG(tooltip)
        actionAddPointInformation->setText(QCoreApplication::translate("MainWindow", "Probe Data", nullptr));
#if QT_CONFIG(tooltip)
        actionAddPointInformation->setToolTip(QCoreApplication::translate("MainWindow", "add point attribute information", nullptr));
#endif // QT_CONFIG(tooltip)
        actionCalculatorFunction->setText(QCoreApplication::translate("MainWindow", "CalculatorFunction", nullptr));
#if QT_CONFIG(tooltip)
        actionCalculatorFunction->setToolTip(QCoreApplication::translate("MainWindow", "add new parameter", nullptr));
#endif // QT_CONFIG(tooltip)
        actionNewWindow->setText(QCoreApplication::translate("MainWindow", "NewWindow", nullptr));
#if QT_CONFIG(tooltip)
        actionNewWindow->setToolTip(QCoreApplication::translate("MainWindow", "create a new window", nullptr));
#endif // QT_CONFIG(tooltip)
        actionExport_Picture->setText(QCoreApplication::translate("MainWindow", "Export Picture", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Orientation view", nullptr));
        xoyView->setText(QString());
        xozView->setText(QString());
        yozView->setText(QString());
        xyzView->setText(QString());
        label->setText(QCoreApplication::translate("MainWindow", "Show basic object", nullptr));
        contourCheckBox->setText(QCoreApplication::translate("MainWindow", "Contour", nullptr));
        contourSettingButton->setText(QString());
        meshCheckBox->setText(QCoreApplication::translate("MainWindow", "Mesh", nullptr));
        shadeCheckBox->setText(QCoreApplication::translate("MainWindow", "Shade", nullptr));
        edgeCheckBox->setText(QCoreApplication::translate("MainWindow", "Edge", nullptr));
        selectBoundaryButton->setText(QCoreApplication::translate("MainWindow", "Control Panel", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Show derived object", nullptr));
        slicesCheckBox->setText(QCoreApplication::translate("MainWindow", "Slices", nullptr));
        isoSurfaceCheckBox->setText(QCoreApplication::translate("MainWindow", "Iso-Surfaces", nullptr));
        streamlineCheckBox->setText(QCoreApplication::translate("MainWindow", "Streamtraces", nullptr));
        IsoSurfaceSettingButton->setText(QString());
        addSliceButton->setText(QString());
        addStreamLineButton->setText(QString());
        label_6->setText(QCoreApplication::translate("MainWindow", "Adjust Show Effects", nullptr));
        lightingCheckBox->setText(QCoreApplication::translate("MainWindow", "Lighting", nullptr));
        lightingButton->setText(QString());
        transparantButton->setText(QString());
        transparancyCheckBox->setText(QCoreApplication::translate("MainWindow", "Transparancy", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Specialized", nullptr));
        ConstHeightCheckBox->setText(QCoreApplication::translate("MainWindow", "Const Height", nullptr));
        ConstHeightpushButton->setText(QString());
        label_7->setText(QCoreApplication::translate("MainWindow", "BackGround", nullptr));
        CBtn_BackGround->setItemText(0, QCoreApplication::translate("MainWindow", "Sky Blue", nullptr));
        CBtn_BackGround->setItemText(1, QCoreApplication::translate("MainWindow", "White", nullptr));

        tabWidget->setTabText(tabWidget->indexOf(tab_normal), QCoreApplication::translate("MainWindow", "General", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "View Control", nullptr));
        Btn_TurboInitial->setText(QCoreApplication::translate("MainWindow", "Initialize", nullptr));
        Check_3Dview->setText(QCoreApplication::translate("MainWindow", "3D View", nullptr));
        Check_Meri->setText(QCoreApplication::translate("MainWindow", "Meridional", nullptr));
        Check_BladeToBlade->setText(QCoreApplication::translate("MainWindow", "Blade-Blade", nullptr));
        Check_ThreeView->setText(QCoreApplication::translate("MainWindow", "Three View", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Graphical Instancing", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Domain", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Num of copies", nullptr));
        Btn_ClearCopyCurrentZone->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        Btn_ClearAllZoneCopy->setText(QCoreApplication::translate("MainWindow", "Clear All", nullptr));
        label_BladeToBlade->setText(QCoreApplication::translate("MainWindow", "Blade to Blade", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "Variable", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "Range Min", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "Range Max", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "Levels", nullptr));
        label_19->setText(QCoreApplication::translate("MainWindow", "span", nullptr));
        label_BladeToBlade_2->setText(QCoreApplication::translate("MainWindow", "Meridional Plane", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "Variable", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Range Min", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "Range Max", nullptr));
        label_18->setText(QCoreApplication::translate("MainWindow", "Levels", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_turbo), QCoreApplication::translate("MainWindow", "Turbo", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "&File", nullptr));
        menuTools->setTitle(QCoreApplication::translate("MainWindow", "&Tools", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
