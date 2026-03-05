#ifndef CFD_SOLVER_H
#define CFD_SOLVER_H

#include <QObject>
#include <QAtomicInt>

/**
 * @brief CFDSolver执行迭代计算的核心组件
 * 
 * CFDSolver在独立的工作线程中运行，执行指定次数的迭代计算。
 * 每次迭代模拟CFD计算工作，并通过信号报告进度。
 * 支持通过原子标志中断计算。
 */
class CFDSolver : public QObject {
    Q_OBJECT
    
public:
    /**
     * @brief 构造函数
     * @param parent 父对象指针
     */
    explicit CFDSolver(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     */
    ~CFDSolver();
    
    /**
     * @brief 执行迭代计算
     * @param totalIterations 总迭代次数
     * @param startFrom 起始迭代位置（用于继续计算）
     * @param stopFlag 停止标志的引用，用于中断计算
     * @param inletPressure 入口压力 (Pa)
     * @param inletTemperature 入口温度 (K)
     * @param ambientTemperature 环境温度 (K)
     */
    void compute(int totalIterations, int startFrom, const QAtomicInt& stopFlag,
                 double inletPressure = 101325.0, 
                 double inletTemperature = 300.0, 
                 double ambientTemperature = 293.0);
    
signals:
    /**
     * @brief 进度更新信号
     * @param current 当前完成的迭代次数
     * @param total 总迭代次数
     */
    void progressUpdated(int current, int total);
    
    /**
     * @brief 温度更新信号
     * @param iteration 当前迭代次数
     * @param temperature 当前温度 (K)
     */
    void temperatureUpdated(int iteration, double temperature);
    
    /**
     * @brief 计算完成信号
     */
    void computationCompleted();
    
    /**
     * @brief 计算错误信号
     * @param error 错误描述信息
     */
    void computationError(const QString& error);
    
private:
    /**
     * @brief 执行单次迭代
     * @param iterationNumber 当前迭代编号
     */
    void performIteration(int iterationNumber);
    
    /**
     * @brief 模拟计算工作（添加延迟）
     */
    void simulateComputationWork();
    
    /**
     * @brief 计算温度场
     * @param iteration 当前迭代次数
     * @return 计算得到的温度值 (K)
     */
    double calculateTemperature(int iteration);
    
    /// 每次迭代的延迟时间（毫秒）
    static constexpr int ITERATION_DELAY_MS = 100;
    
    // Temperature calculation parameters
    double m_inletPressure;
    double m_inletTemperature;
    double m_ambientTemperature;
    double m_currentTemperature;
};

#endif // CFD_SOLVER_H
