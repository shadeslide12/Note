#include "script_editor_widget.h"
#include "script_engine.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFont>

ScriptEditorWidget::ScriptEditorWidget(ScriptEngine* scriptEngine, QWidget* parent)
    : QWidget(parent)
    , m_scriptEngine(scriptEngine)
    , m_scriptEditor(nullptr)
    , m_outputConsole(nullptr)
    , m_runButton(nullptr)
    , m_stopButton(nullptr)
    , m_loadButton(nullptr)
    , m_saveButton(nullptr)
    , m_clearButton(nullptr)
{
    setupUI();
    connectSignals();
}

ScriptEditorWidget::~ScriptEditorWidget()
{
    // Qt parent-child relationship handles cleanup
}

void ScriptEditorWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Script editor section
    QLabel* editorLabel = new QLabel("Lua Script:", this);
    mainLayout->addWidget(editorLabel);
    
    m_scriptEditor = new QTextEdit(this);
    m_scriptEditor->setMinimumHeight(200);
    QFont editorFont("Courier");
    editorFont.setStyleHint(QFont::Monospace);
    editorFont.setPointSize(10);
    m_scriptEditor->setFont(editorFont);
    m_scriptEditor->setPlaceholderText("-- Enter Lua script here\n-- Example:\n-- solver.start(100, 101325, 300, 293)\n-- print(\"Computation started\")");
    mainLayout->addWidget(m_scriptEditor);
    
    // Button row
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_runButton = new QPushButton("Run", this);
    m_stopButton = new QPushButton("Stop", this);
    m_loadButton = new QPushButton("Load", this);
    m_saveButton = new QPushButton("Save", this);
    m_clearButton = new QPushButton("Clear Output", this);
    
    m_runButton->setMinimumWidth(80);
    m_stopButton->setMinimumWidth(80);
    m_loadButton->setMinimumWidth(80);
    m_saveButton->setMinimumWidth(80);
    m_clearButton->setMinimumWidth(100);
    
    m_stopButton->setEnabled(false);
    
    buttonLayout->addWidget(m_runButton);
    buttonLayout->addWidget(m_stopButton);
    buttonLayout->addWidget(m_loadButton);
    buttonLayout->addWidget(m_saveButton);
    buttonLayout->addWidget(m_clearButton);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    
    // Output console section
    QLabel* consoleLabel = new QLabel("Output Console:", this);
    mainLayout->addWidget(consoleLabel);
    
    m_outputConsole = new QTextEdit(this);
    m_outputConsole->setMinimumHeight(150);
    m_outputConsole->setReadOnly(true);
    QFont consoleFont("Courier");
    consoleFont.setStyleHint(QFont::Monospace);
    consoleFont.setPointSize(9);
    m_outputConsole->setFont(consoleFont);
    m_outputConsole->setStyleSheet("background-color: #f5f5f5;");
    mainLayout->addWidget(m_outputConsole);
    
    setLayout(mainLayout);
}

void ScriptEditorWidget::connectSignals()
{
    // Connect button clicks
    connect(m_runButton, &QPushButton::clicked,
            this, &ScriptEditorWidget::onRunClicked);
    connect(m_stopButton, &QPushButton::clicked,
            this, &ScriptEditorWidget::onStopClicked);
    connect(m_loadButton, &QPushButton::clicked,
            this, &ScriptEditorWidget::onLoadClicked);
    connect(m_saveButton, &QPushButton::clicked,
            this, &ScriptEditorWidget::onSaveClicked);
    connect(m_clearButton, &QPushButton::clicked,
            this, &ScriptEditorWidget::onClearClicked);
    
    // Connect script engine signals
    if (m_scriptEngine) {
        connect(m_scriptEngine, &ScriptEngine::scriptOutput,
                this, &ScriptEditorWidget::onScriptOutput);
        connect(m_scriptEngine, &ScriptEngine::scriptError,
                this, &ScriptEditorWidget::onScriptError);
        connect(m_scriptEngine, &ScriptEngine::scriptStarted,
                this, &ScriptEditorWidget::onScriptStarted);
        connect(m_scriptEngine, &ScriptEngine::scriptFinished,
                this, &ScriptEditorWidget::onScriptFinished);
    }
}

QString ScriptEditorWidget::getScriptText() const
{
    return m_scriptEditor->toPlainText();
}

void ScriptEditorWidget::setScriptText(const QString& text)
{
    m_scriptEditor->setPlainText(text);
}

void ScriptEditorWidget::clearOutput()
{
    m_outputConsole->clear();
}

void ScriptEditorWidget::onRunClicked()
{
    QString script = getScriptText();
    
    if (script.trimmed().isEmpty()) {
        appendOutput("Error: Script is empty", "red");
        return;
    }
    
    clearOutput();
    appendOutput("=== Script Execution Started ===", "blue");
    
    if (m_scriptEngine) {
        m_scriptEngine->executeScript(script);
    }
}

void ScriptEditorWidget::onStopClicked()
{
    if (m_scriptEngine) {
        m_scriptEngine->stopScript();
        appendOutput("=== Stop Requested ===", "orange");
    }
}

void ScriptEditorWidget::onLoadClicked()
{
    QString filename = QFileDialog::getOpenFileName(
        this,
        "Load Lua Script",
        "",
        "Lua Scripts (*.lua);;All Files (*)"
    );
    
    if (filename.isEmpty()) {
        return;
    }
    
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Load Failed",
                             QString("Failed to open file:\n%1").arg(filename));
        return;
    }
    
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString content = in.readAll();
    file.close();
    
    setScriptText(content);
    appendOutput(QString("Loaded script from: %1").arg(filename), "green");
}

void ScriptEditorWidget::onSaveClicked()
{
    QString filename = QFileDialog::getSaveFileName(
        this,
        "Save Lua Script",
        "",
        "Lua Scripts (*.lua);;All Files (*)"
    );
    
    if (filename.isEmpty()) {
        return;
    }
    
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Save Failed",
                             QString("Failed to save file:\n%1").arg(filename));
        return;
    }
    
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << getScriptText();
    file.close();
    
    appendOutput(QString("Saved script to: %1").arg(filename), "green");
    QMessageBox::information(this, "Save Successful",
                            QString("Script saved to:\n%1").arg(filename));
}

void ScriptEditorWidget::onClearClicked()
{
    clearOutput();
}

void ScriptEditorWidget::onScriptOutput(const QString& text)
{
    appendOutput(text, "black");
}

void ScriptEditorWidget::onScriptError(const QString& error)
{
    appendOutput(QString("ERROR: %1").arg(error), "red");
}

void ScriptEditorWidget::onScriptStarted()
{
    m_runButton->setEnabled(false);
    m_stopButton->setEnabled(true);
    m_loadButton->setEnabled(false);
    m_scriptEditor->setReadOnly(true);
}

void ScriptEditorWidget::onScriptFinished()
{
    m_runButton->setEnabled(true);
    m_stopButton->setEnabled(false);
    m_loadButton->setEnabled(true);
    m_scriptEditor->setReadOnly(false);
    appendOutput("=== Script Execution Finished ===", "blue");
}

void ScriptEditorWidget::appendOutput(const QString& text, const QString& color)
{
    m_outputConsole->setTextColor(QColor(color));
    m_outputConsole->append(text);
    
    // Scroll to bottom
    QTextCursor cursor = m_outputConsole->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_outputConsole->setTextCursor(cursor);
}
