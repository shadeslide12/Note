#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QColor>
#include <QMap>
#include <QVariant>

/**
 * @brief 属性面板组件
 * 
 * 显示和编辑选中对象的属性，支持多种属性类型
 */
class PropertyPanel : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyPanel(QWidget *parent = nullptr);
    ~PropertyPanel();

    // 属性类型
    enum PropertyType {
        StringType,
        IntType,
        DoubleType,
        BoolType,
        ColorType,
        ComboType
    };

    // 清除所有属性
    void clearProperties();
    
    // 添加属性
    void addStringProperty(const QString &name, const QString &value);
    void addIntProperty(const QString &name, int value, int min = 0, int max = 100);
    void addDoubleProperty(const QString &name, double value, double min = 0.0, double max = 1.0, int decimals = 2);
    void addBoolProperty(const QString &name, bool value);
    void addColorProperty(const QString &name, const QColor &value);
    void addComboProperty(const QString &name, const QStringList &options, const QString &currentValue);
    
    // 获取属性值
    QVariant getPropertyValue(const QString &name) const;
    
    // 设置属性值
    void setPropertyValue(const QString &name, const QVariant &value);
    
    // 设置对象名称（显示在标题栏）
    void setObjectName(const QString &name);

signals:
    // 当属性值改变时发出
    void propertyValueChanged(const QString &propertyName, const QVariant &newValue);

private slots:
    void onCellChanged(int row, int column);

private:
    QTableWidget *propertyTable;
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    
    // 存储属性类型
    QMap<QString, PropertyType> propertyTypes;
    QMap<QString, int> propertyRows; // 属性名称到行号的映射
    
    void setupUI();
    void setupConnections();
    
    // 创建不同类型的编辑器
    QWidget* createEditorForType(PropertyType type, const QVariant &value, 
                                  const QVariant &param1 = QVariant(), 
                                  const QVariant &param2 = QVariant());
};
