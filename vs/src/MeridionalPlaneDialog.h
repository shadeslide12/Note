#pragma once

#include <QDialog>
#include "vtkAesReader.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MeridionalPlaneDialog; }
QT_END_NAMESPACE

class MeridionalPlaneDialog : public QDialog {
Q_OBJECT

public:
    explicit MeridionalPlaneDialog(QWidget *parent = nullptr);
    void setDialog(std::vector<vtkAesReader::FlowData> flows, int flowNumber);
    ~MeridionalPlaneDialog() override;
private slots:
    void changeFlow(int number);

signals:
    void finishSetZone(int);
    void finishSetMeiridonalFlow(int);

private:
    int zoneNumber;
    Ui::MeridionalPlaneDialog *ui;
};
