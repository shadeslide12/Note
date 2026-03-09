/********************************************************************************
** Form generated from reading UI file 'ProbePanel.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROBEPANEL_H
#define UI_PROBEPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProbePanel
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *headerLayout;
    QLabel *titleLabel;
    QSpacerItem *horizontalSpacer;
    QPushButton *closeButton;
    QFrame *line;
    QTableWidget *probeTable;
    QHBoxLayout *bottomLayout;
    QLabel *hintLabel;
    QPushButton *clearButton;

    void setupUi(QWidget *ProbePanel)
    {
        if (ProbePanel->objectName().isEmpty())
            ProbePanel->setObjectName(QString::fromUtf8("ProbePanel"));
        ProbePanel->resize(350, 600);
        ProbePanel->setMinimumSize(QSize(300, 0));
        ProbePanel->setMaximumSize(QSize(500, 16777215));
        verticalLayout = new QVBoxLayout(ProbePanel);
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        headerLayout = new QHBoxLayout();
        headerLayout->setObjectName(QString::fromUtf8("headerLayout"));
        titleLabel = new QLabel(ProbePanel);
        titleLabel->setObjectName(QString::fromUtf8("titleLabel"));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        titleLabel->setFont(font);

        headerLayout->addWidget(titleLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        headerLayout->addItem(horizontalSpacer);

        closeButton = new QPushButton(ProbePanel);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setMaximumSize(QSize(25, 25));
        closeButton->setFlat(true);

        headerLayout->addWidget(closeButton);


        verticalLayout->addLayout(headerLayout);

        line = new QFrame(ProbePanel);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        probeTable = new QTableWidget(ProbePanel);
        if (probeTable->columnCount() < 2)
            probeTable->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        probeTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        probeTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        probeTable->setObjectName(QString::fromUtf8("probeTable"));
        probeTable->setAlternatingRowColors(true);
        probeTable->setSelectionMode(QAbstractItemView::SingleSelection);
        probeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        probeTable->horizontalHeader()->setVisible(true);
        probeTable->horizontalHeader()->setStretchLastSection(true);
        probeTable->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(probeTable);

        bottomLayout = new QHBoxLayout();
        bottomLayout->setObjectName(QString::fromUtf8("bottomLayout"));
        hintLabel = new QLabel(ProbePanel);
        hintLabel->setObjectName(QString::fromUtf8("hintLabel"));
        QFont font1;
        font1.setPointSize(8);
        font1.setItalic(true);
        hintLabel->setFont(font1);
        hintLabel->setWordWrap(true);

        bottomLayout->addWidget(hintLabel);

        clearButton = new QPushButton(ProbePanel);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));

        bottomLayout->addWidget(clearButton);


        verticalLayout->addLayout(bottomLayout);


        retranslateUi(ProbePanel);

        QMetaObject::connectSlotsByName(ProbePanel);
    } // setupUi

    void retranslateUi(QWidget *ProbePanel)
    {
        ProbePanel->setWindowTitle(QCoreApplication::translate("ProbePanel", "Probe", nullptr));
        titleLabel->setText(QCoreApplication::translate("ProbePanel", "Probe Data", nullptr));
#if QT_CONFIG(tooltip)
        closeButton->setToolTip(QCoreApplication::translate("ProbePanel", "Close panel", nullptr));
#endif // QT_CONFIG(tooltip)
        closeButton->setText(QCoreApplication::translate("ProbePanel", "\342\234\225", nullptr));
        QTableWidgetItem *___qtablewidgetitem = probeTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("ProbePanel", "Variable", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = probeTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("ProbePanel", "Value", nullptr));
        hintLabel->setText(QCoreApplication::translate("ProbePanel", "Click on geometry to probe", nullptr));
#if QT_CONFIG(tooltip)
        clearButton->setToolTip(QCoreApplication::translate("ProbePanel", "Clear probe data", nullptr));
#endif // QT_CONFIG(tooltip)
        clearButton->setText(QCoreApplication::translate("ProbePanel", "Clear", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProbePanel: public Ui_ProbePanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROBEPANEL_H
