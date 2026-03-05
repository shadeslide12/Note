#ifndef CFD_SOLVER_THREAD_H
#define CFD_SOLVER_THREAD_H

#include <QThread>
#include <QAtomicInt>

// Forward declaration
class CFDSolver;

/**
 * @brief CFDSolverThread管理求解器工作线程
 * 
 * CFDSolverThread继承自QThread，负责在独立线程中运行CFDSolver。
 * 提供线程安全的启动和停止控制，并将求解器的信号转发到主线程。
 */
class CFDSolverThread : public QThread {
    Q_OBJECT
    
public:
    /**
     * @brief 构造函数
     * @param parent 父对象指针
     */
    explicit CFDSolverThread(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     * 
     * 确保线程正确停止并清理资源
     */
    ~CFDSolverThread();
    
public slots:
    /**
     * @brief 启动计算
     * @param totalIterations 总迭代次数
     * @param startFrom 起始迭代位置（用于继续计算）
     * @param inletPressure 入口压力 (Pa)
     * @param inletTemperature 入口温度 (K)
     * @param ambientTemperature 环境温度 (K)
     */
    void startComputation(int totalIterations, int startFrom,
                         double inletPressure = 101325.0,
                         double inletTemperature = 300.0,
                         double ambientTemperature = 293.0);
    
    /**
     * @brief 停止计算
     * 
     * 设置停止标志，请求工作线程中断计算
     */
    void stopComputation();
    
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
    
protected:
    /**
     * @brief 线程执行函数
     * 
     * 在独立线程中创建CFDSolver并执行计算
     */
    void run() override;
    
private:
    CFDSolver* m_solver;           ///< 求解器实例（在工作线程中创建）
    int m_totalIterations;         ///< 总迭代次数
    int m_startFrom;               ///< 起始迭代位置
    QAtomicInt m_stopRequested;    ///< 线程安全的停止标志
    double m_inletPressure;        ///< 入口压力 (Pa)
    double m_inletTemperature;     ///< 入口温度 (K)
    double m_ambientTemperature;   ///< 环境温度 (K)
};

#endif // CFD_SOLVER_THREAD_H
