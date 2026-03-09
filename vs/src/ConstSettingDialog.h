#pragma  once

#include <QDialog>
#include <vtkAesReader.h>

QT_BEGIN_NAMESPACE
namespace Ui { class ConstSettingDialog; }
QT_END_NAMESPACE

class ConstSettingDialog : public QDialog {
Q_OBJECT

public:
    explicit ConstSettingDialog(QWidget *parent = nullptr);
    void setConstSettingDialog(std::vector<vtkAesReader::FlowData> flows, int flowNumber);
    ~ConstSettingDialog() override;
private slots:
    void setHeight();
    void changeFlow(int flow);
signals:
    void finishSetHeight(double);
    void finishSetFlow(int);
private:
    Ui::ConstSettingDialog *ui;
    double height;
};

