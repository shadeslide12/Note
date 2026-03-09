#pragma once
#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class ConstHeightPlaneDialog; }
QT_END_NAMESPACE

class ConstHeightPlaneDialog : public QDialog {
Q_OBJECT

public:
    explicit ConstHeightPlaneDialog(QWidget *parent = nullptr);

    ~ConstHeightPlaneDialog() override;
private slots:
    void setParameters();

signals:
    void finishSetParameters(double);

private:
    Ui::ConstHeightPlaneDialog *ui;
    double height;
};
