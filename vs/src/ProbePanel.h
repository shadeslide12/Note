#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <vector>
#include <string>
#include <utility>

namespace Ui {
    class ProbePanel;
}

class ProbePanel : public QWidget
{
    Q_OBJECT

public:
    explicit ProbePanel(QWidget *parent = nullptr);
    ~ProbePanel();

    // 更新探测数据
    void UpdateProbeData(const std::string& coordinates, 
                         const std::vector<std::pair<std::string, double>>& data);
    
    // 清空数据
    void ClearData();

signals:
    // 面板被关闭时发出信号
    void panelClosed();

protected:
    // 重写 hideEvent 以发出信号
    void hideEvent(QHideEvent* event) override;

private:
    Ui::ProbePanel *ui;
};
