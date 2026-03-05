#ifndef SCRIPT_EDITOR_WIDGET_H
#define SCRIPT_EDITOR_WIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>

// Forward declaration
class ScriptEngine;

/**
 * @brief ScriptEditorWidget provides UI for Lua script editing and execution
 * 
 * This widget includes:
 * - Script editor (QTextEdit)
 * - Output console (QTextEdit)
 * - Control buttons (Run, Stop, Load, Save, Clear)
 */
class ScriptEditorWidget : public QWidget {
    Q_OBJECT
    
public:
    /**
     * @brief Constructor
     * @param scriptEngine Script engine for execution
     * @param parent Parent widget
     */
    explicit ScriptEditorWidget(ScriptEngine* scriptEngine, QWidget* parent = nullptr);
    
    /**
     * @brief Destructor
     */
    ~ScriptEditorWidget();
    
    /**
     * @brief Get current script text
     * @return Script text
     */
    QString getScriptText() const;
    
    /**
     * @brief Set script text
     * @param text Script text
     */
    void setScriptText(const QString& text);
    
    /**
     * @brief Clear output console
     */
    void clearOutput();
    
public slots:
    /**
     * @brief Handle script output
     * @param text Output text
     */
    void onScriptOutput(const QString& text);
    
    /**
     * @brief Handle script error
     * @param error Error message
     */
    void onScriptError(const QString& error);
    
    /**
     * @brief Handle script started
     */
    void onScriptStarted();
    
    /**
     * @brief Handle script finished
     */
    void onScriptFinished();
    
private slots:
    /**
     * @brief Handle Run button click
     */
    void onRunClicked();
    
    /**
     * @brief Handle Stop button click
     */
    void onStopClicked();
    
    /**
     * @brief Handle Load button click
     */
    void onLoadClicked();
    
    /**
     * @brief Handle Save button click
     */
    void onSaveClicked();
    
    /**
     * @brief Handle Clear button click
     */
    void onClearClicked();
    
private:
    /**
     * @brief Setup UI components
     */
    void setupUI();
    
    /**
     * @brief Connect signals and slots
     */
    void connectSignals();
    
    /**
     * @brief Append text to output console
     * @param text Text to append
     * @param color Text color
     */
    void appendOutput(const QString& text, const QString& color = "black");
    
    ScriptEngine* m_scriptEngine;       ///< Script engine
    QTextEdit* m_scriptEditor;          ///< Script editor
    QTextEdit* m_outputConsole;         ///< Output console
    QPushButton* m_runButton;           ///< Run button
    QPushButton* m_stopButton;          ///< Stop button
    QPushButton* m_loadButton;          ///< Load button
    QPushButton* m_saveButton;          ///< Save button
    QPushButton* m_clearButton;         ///< Clear button
};

#endif // SCRIPT_EDITOR_WIDGET_H
