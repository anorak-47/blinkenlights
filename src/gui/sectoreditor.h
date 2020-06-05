#ifndef SECTOREDITOR_H
#define SECTOREDITOR_H

#include "colormap.h"
#include <QWidget>
#include <QtCore/QSignalMapper>
#include <keyboard_interface.h>

namespace Ui {
class SectorEditor;
}

class SectorEditor : public QWidget
{
    Q_OBJECT

public:
    explicit SectorEditor(QWidget *parent = 0);
    ~SectorEditor();

    void addSector(QString const &name, unsigned int index);
    void addColorMapSectorSelector(const QString &name, unsigned int index);
    void addColorMap(const QString &name, unsigned int index);
    void updateGraphicsItemsFlags(unsigned int inSectorMode);
    
    bool inSectorMode() const;

    QString getColorMapAsJSONString();
    void setColorMapFromJSONString(QString jsonstring);

    int getCurrentColorMapIndex();

signals:
    void signalSectorColorChanged(const QColor &color);
    void signalMapColorChanged(const QColor &color);
    void signalGraphicsSceneChanged();
    void signalSyncKeyboardStates();
    void signalSectorModeChanged(bool inSectorMode);
    void signalSyncedMap(sMapPtr map);

    void signalMapSectorSelected(int sector);
    void signalMapSelectAll(bool select);
    void signalMapSelectInvert();

public slots:
    void on_sectorColorChanged(const QColor &color);
    void on_sectorUseColor(const QColor &color);
    void on_syncKeyboardStates();

private slots:
    void on_cwSectors_colorChanged(const QColor &color);
    void on_pwSectors_currentColorChanged(const QColor &color);
    void on_pbColorDialogSectors_colorChanged(const QColor &color);
    //void on_cleSectors_colorChanged(const QColor &color);
    void on_cbSync_toggled(bool checked);
    void on_pbSet_clicked();
    void on_pbSave_clicked();
    void on_cbMap_currentIndexChanged(int index);
    void on_pbReset_clicked();
    void on_syncedMap(sMapPtr colormap);
    void on_gsSectorAllSaturation_valueChanged(int value);
    void on_gsSectorAllValue_valueChanged(int value);

    void on_pbMapSelectAll_clicked();
    void on_pbMapUnselectAll_clicked();
    void on_pbMapInvertSelection_clicked();

    void slot_sectorWidget_editableChanged();
    void slot_MapSectorButton_clicked(int sector);

    void on_pbMapUseColor_clicked();

private:
    void initSelectorsColorPaletteWidget();
    void colorChanged(const QColor &color);

    void saveCurrentColorMap(int index);
    void updateSectorEditorWidgets(bool inSectorMode);
    void restoreSelectedColorMap(bool inSectorMode, int index);

    void getMapRow(int row, bool getOnlyChangedColors, struct s_map_row &map_row);
    std::unique_ptr<s_map> getMap(int row, bool getOnlyChangedColors, bool getAllRows);
    std::unique_ptr<s_map> getFullColorMap();
    void syncSectorsToKeyboard();
    void syncCurrentMapFromKeyboard();

    void syncCurrentMapToKeyboard();
    void syncedMap(sMapPtr colormap);

    void updateValueSlider(const QColor &color);
    void updateSaturationSlider(const QColor &color);
    void updateSliders();

    int getAverageSectorSaturation();
    int getAverageSectorValue();

    int getAverageMapSaturation();
    int getAverageMapValue();

    int getAverageSaturation();
    int getAverageValue();

    Ui::SectorEditor *ui;
    bool _inSectorMode = false;
    int _oldMapIndex = 0;
    std::vector<ColorMap> _colorMaps;
    ColorMap _diffColorMap;
    int _oldSaturation;
    int _oldValue;
    QSignalMapper *signalMapper;
};

#endif // SECTOREDITOR_H
