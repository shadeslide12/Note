#ifndef ControlPanel_h
#define ControlPanel_h

#include <QWidget>
#include <QDialog>
#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QMenu>
#include <QInputDialog>
#include <QEvent>
#include <QContextMenuEvent>
#include <vtkAesReader.h>

namespace Ui { class ControlPanel; }

class ControlPanel : public QDialog
{
    Q_OBJECT

public:
    ControlPanel(QWidget* parent = nullptr);
    ~ControlPanel();

    // ── Table Setup ──
    void setupTable(std::vector<std::vector<vtkAesReader::Boundary>> boundarys);
    void addCutplaneToTable(int cutplaneIndex, double* origin, double* normal);

    // ── Transparency Accessors ──
    void setTransparencyControlsEnabled(bool enabled);
    void getAllBoundaryTransparencies(std::vector<std::vector<double>>& transparencies);
    void setAllBoundaryTransparencies(const std::vector<std::vector<double>>& transparencies);

signals:
    // ── Visibility ──
    void setBoundarys(int meshNumber, int boundaryNumber, bool flag);
    void setCutplaneVisiable(int cutplaneNumber, bool flag);

    // ── Transparency ──
    void boundaryTransparencyChanged(int meshNumber, int boundaryNumber, double value);
    void sliceTransparencyChanged(int sliceNumber, double value);

    // ── Slice ──
    void sliceDeleteRequested(int cutplaneIndex);
    void sliceContourModeChanged(const QString &mode);

private slots:
    // ── Visibility Slots ──
    void onShowZoneCheckBoxToggled(bool checked);

    // ── Transparency Slots ──
    void onBoundaryTransparencyChanged(double value);
    void onSliceTransparencyChanged(double value);

    // ── Slice Slots ──
    void onSliceDeleteClicked();
    void onSliceContourModeChanged(const QString &text);

    // ── Context Menu Slots ──
    void showZoneContextMenu(const QPoint& pos);
    void showTransparencyContextMenu(const QPoint& pos);

private:
    Ui::ControlPanel *ui;

    // ── Row Data ──
    enum RowType { BOUNDARY, CUTPLANE };
    std::vector<RowType> rowTypes;
    std::vector<std::pair<int, int>> boundaryIndices;   // (meshIndex, boundaryIndex)
    struct CutplaneInfo {
        int index;
        double origin[3];
        double normal[3];
    };
    std::vector<CutplaneInfo> cutplaneInfos;

    // ── Cell Widget Factories ──
    QWidget* createCheckBoxWidget(bool checked);
    QWidget* createComboBoxWidget();
    QWidget* createTransparencyWidget(double value = 1.0);
    QWidget* createDeleteButtonWidget();

    // ── Table Helpers ──
    void removeCutplaneFromTable(int cutplaneIndex);
    void syncSliceContourMode(const QString &text, int excludeCutplaneIndex);

    // ── Context Menu Helpers ──
    QList<int> getSelectedRows();
    void setSelectedZonesVisibility(bool visible);
    void applyTransparencyToSelected(double value);
    void showCustomTransparencyDialog();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
};

#endif