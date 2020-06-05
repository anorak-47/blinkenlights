#include <sectormapsavecommand.h>
#include "sectoreditor.h"
#include "ui_sectoreditor.h"
#include "sectoreditwidget.h"
#include "keyboard_command_queue.h"
#include "sectorcolorcommand.h"
#include "sectorsavecommand.h"
#include "sectormapcommand.h"
#include "sectormapgetcommand.h"
#include "sectormapsetcommand.h"
#include "sectormapsavecommand.h"
#include "keyboard_properties.h"
#include "common.h"
#include "keygraphicsitem.h"
#include <QtWidgets/QGraphicsItem>
#include <QtCore/QSettings>
#include <QtCore/QDebug>
#include <iostream>

SectorEditor::SectorEditor(QWidget *parent) : QWidget(parent), ui(new Ui::SectorEditor)
{
    qRegisterMetaType<sMapPtr>("sMapPtr");

    _oldSaturation = 0;
    _oldValue = 0;

    ui->setupUi(this);
    initSelectorsColorPaletteWidget();

    signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(slot_MapSectorButton_clicked(int)));

    auto properties = KeyboardPropertiesInstance().properties();

    if (properties->supportsColorMaps())
    {
        _colorMaps.resize(properties->colorMapCount(), ColorMap());
        for (unsigned int s = 0; s < properties->colorMapCount(); ++s)
            addColorMap(properties->colorMapName(s), s);

        if (properties->supportsSectors())
        {
            QLabel *label = new QLabel(_("Select/Unselect Sector"), this);
            label->setAlignment(Qt::AlignHCenter);
            ui->mapSectorButtonLayout->addWidget(label);

            for (unsigned int s = 0; s < properties->sectorCount(); ++s)
                addColorMapSectorSelector(properties->sectorName(s), s);
        }

        connect(this, SIGNAL(signalSyncedMap(sMapPtr)), this, SLOT(on_syncedMap(sMapPtr)));
    }

    if (properties->supportsSectors())
    {
        addColorMap(_("Sectors"), properties->colorMapCount());

        for (unsigned int s = 0; s < properties->sectorCount(); ++s)
            addSector(properties->sectorName(s), s);

        _inSectorMode = true;
        _oldMapIndex = properties->colorMapCount();

        ui->cbMap->setCurrentIndex(properties->colorMapCount());
    }

    if (!properties->supportsColorMaps())
        ui->cbMap->setDisabled(true);

    QSettings settings;
    ui->cbSync->setChecked(settings.value(properties->name() + "/sector/synctokeyboard", true).toBool());
    ui->pbSet->setEnabled(!ui->cbSync->isChecked());
}

SectorEditor::~SectorEditor()
{
    QSettings settings;
    auto properties = KeyboardPropertiesInstance().properties();
    settings.setValue(properties->name() + "/sector/synctokeyboard", ui->cbSync->isChecked());

    delete ui;
}

void SectorEditor::addSector(QString const &name, unsigned int index)
{
    SectorEditWidget *sew = new SectorEditWidget(name, index, ui->gbSectors);
    ui->glSectors->addWidget(sew, index, 1);

    connect(sew, SIGNAL(signalColorChanged(QColor)), this, SLOT(on_sectorColorChanged(QColor)));
    connect(sew, SIGNAL(signalUseColor(QColor)), this, SLOT(on_sectorUseColor(QColor)));
    connect(sew, SIGNAL(signalIlluminationChanged()), this, SIGNAL(signalGraphicsSceneChanged()));
    connect(this, SIGNAL(signalSectorColorChanged(QColor)), sew, SLOT(on_colorChanged(QColor)));
    connect(this, SIGNAL(signalSyncKeyboardStates()), sew, SLOT(on_syncSectorState()));
    connect(sew, SIGNAL(signalEditChanged(bool)), this, SLOT(slot_sectorWidget_editableChanged()));

    sew->setSyncOnChange(ui->cbSync->isChecked());
}

void SectorEditor::addColorMapSectorSelector(QString const &name, unsigned int index)
{
    // QPushButton *button = new QPushButton(_("Select") + " " + name, this);
    QPushButton *button = new QPushButton(name, this);
    button->setMaximumHeight(25);
    button->setObjectName("MapSectorCheckBox");
    connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(button, index);
    ui->mapSectorButtonLayout->addWidget(button);
}

void SectorEditor::addColorMap(QString const &name, unsigned int index)
{
    Q_UNUSED(index);
    ui->cbMap->addItem(name);
}

void SectorEditor::initSelectorsColorPaletteWidget()
{
    color_widgets::ColorPalette palette1;
    color_widgets::ColorPalette palette2;
    int columns = 12;
    palette1.setName("Palette 1");
    palette2.setName("Palette 2");
    palette1.setColumns(columns);
    palette2.setColumns(columns);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            float f = float(j) / columns;
            palette1.appendColor(QColor::fromHsvF(i / 8.0, 1 - f, 0.5 + f / 2));
            palette2.appendColor(QColor::fromHsvF(i / 8.0, 1 - f, 1 - f));
        }
    }
    color_widgets::ColorPaletteModel *model = new color_widgets::ColorPaletteModel;
    model->setParent(ui->pwSectors);
    model->addPalette(palette2, false);
    model->addPalette(palette1, false);

    ui->pwSectors->setModel(model);
}

void SectorEditor::colorChanged(const QColor &color)
{
    if (_inSectorMode)
    {
        emit signalSectorColorChanged(color);
    }
    else
    {
        if (ui->cbSync->isChecked())
            syncCurrentMapToKeyboard();
        emit signalMapColorChanged(color);
    }
}

void SectorEditor::on_sectorColorChanged(const QColor &color)
{
    QSignalBlocker b1(ui->pbColorDialogSectors);
    ui->pbColorDialogSectors->setColor(color);

    QSignalBlocker b2(ui->pwSectors);
    ui->pwSectors->setCurrentColor(color);

    QSignalBlocker b3(ui->cwSectors);
    ui->cwSectors->setColor(color);

    updateSaturationSlider(color);
    updateValueSlider(color);
}

void SectorEditor::on_sectorUseColor(const QColor &color)
{
    on_sectorColorChanged(color);
    colorChanged(color);
}

void SectorEditor::on_syncKeyboardStates()
{
    // qDebug() << __PRETTY_FUNCTION__;
    emit signalSyncKeyboardStates();
}

int SectorEditor::getAverageSectorSaturation()
{
    int saturation = 0;
    int editcount = 0;

    QList<SectorEditWidget *> widgets = findChildren<SectorEditWidget *>("SectorEditWidget");
    foreach (SectorEditWidget *widget, widgets)
    {
        if (widget->isEditEnabled())
        {
            QColor color = widget->getColor();
            saturation += color.hsvSaturation();
            editcount++;
        }
    }

    if (editcount > 1)
        saturation /= editcount;

    return saturation;
}

int SectorEditor::getAverageMapSaturation()
{
    int saturation = 0;
    int editcount = 0;

    auto klekeys = KeyboardPropertiesInstance().properties()->getKeys();
    for (auto &key : klekeys)
    {
        if (key->item->isSelected())
        {
            QColor color = key->color;
            saturation += color.hsvSaturation();
            editcount++;
        }
    }

    if (editcount > 1)
        saturation /= editcount;

    return saturation;
}

int SectorEditor::getAverageSectorValue()
{
    int value = 0;
    int editcount = 0;

    QList<SectorEditWidget *> widgets = findChildren<SectorEditWidget *>("SectorEditWidget");
    foreach (SectorEditWidget *widget, widgets)
    {
        if (widget->isEditEnabled())
        {
            QColor color = widget->getColor();
            value += color.value();
            editcount++;
        }
    }

    if (editcount > 1)
        value /= editcount;

    return value;
}

int SectorEditor::getAverageMapValue()
{
    int value = 0;
    int editcount = 0;

    auto klekeys = KeyboardPropertiesInstance().properties()->getKeys();
    for (auto &key : klekeys)
    {
        if (key->item->isSelected())
        {
            QColor color = key->color;
            value += color.value();
            editcount++;
        }
    }

    if (editcount > 1)
        value /= editcount;

    return value;
}

int SectorEditor::getAverageSaturation()
{
    if (inSectorMode())
        return getAverageSectorSaturation();
    return getAverageMapSaturation();
}

int SectorEditor::getAverageValue()
{
    if (inSectorMode())
        return getAverageSectorValue();
    return getAverageMapValue();
}

void SectorEditor::updateSaturationSlider(const QColor &color)
{
    int saturation = getAverageSaturation();
    qDebug() << "average saturation: " << saturation;

    QColor saturatedColor;
    QSignalBlocker gsSectorAllSaturationBL(ui->gsSectorAllSaturation);
    saturatedColor.setHsv(color.hsvHue(), 0, color.value());
    ui->gsSectorAllSaturation->setFirstColor(saturatedColor);
    saturatedColor.setHsv(color.hsvHue(), 255, color.value());
    ui->gsSectorAllSaturation->setLastColor(saturatedColor);
    ui->gsSectorAllSaturation->setValue(saturation);
}

void SectorEditor::updateValueSlider(const QColor &color)
{
    int value = getAverageValue();
    qDebug() << "average value: " << value;

    QColor enlightedColor;
    QSignalBlocker gsSectorAllValueBL(ui->gsSectorAllValue);
    enlightedColor.setHsv(color.hsvHue(), color.hsvSaturation(), 0);
    ui->gsSectorAllValue->setFirstColor(enlightedColor);
    enlightedColor.setHsv(color.hsvHue(), color.hsvSaturation(), 255);
    ui->gsSectorAllValue->setLastColor(enlightedColor);
    ui->gsSectorAllValue->setValue(value);
}

void SectorEditor::on_cwSectors_colorChanged(const QColor &color)
{
    QSignalBlocker b1(ui->pbColorDialogSectors);
    ui->pbColorDialogSectors->setColor(color);

    QSignalBlocker b2(ui->pwSectors);
    ui->pwSectors->setCurrentColor(color);

    updateSaturationSlider(color);
    updateValueSlider(color);

    colorChanged(color);
}

void SectorEditor::on_pwSectors_currentColorChanged(const QColor &color)
{
    ui->cwSectors->blockSignals(true);
    ui->cwSectors->setColor(color);
    ui->cwSectors->blockSignals(false);

    ui->pbColorDialogSectors->blockSignals(true);
    ui->pbColorDialogSectors->setColor(color);
    ui->pbColorDialogSectors->blockSignals(true);

    updateSaturationSlider(color);
    updateValueSlider(color);

    colorChanged(color);
}

void SectorEditor::on_pbColorDialogSectors_colorChanged(const QColor &color)
{
    ui->pwSectors->blockSignals(true);
    ui->pwSectors->setCurrentColor(color);
    ui->pwSectors->blockSignals(false);

    ui->cwSectors->blockSignals(true);
    ui->cwSectors->setColor(color);
    ui->cwSectors->blockSignals(false);

    updateSaturationSlider(color);
    updateValueSlider(color);

    colorChanged(color);
}

void SectorEditor::on_gsSectorAllSaturation_valueChanged(int value)
{
    int delta = value - _oldSaturation;
    _oldSaturation = value;
    qDebug() << "value: " << value;
    qDebug() << "delta: " << delta;

    QColor oldColor = ui->cwSectors->color();
    QColor color;
    color.setHsv(oldColor.hsvHue(), value, oldColor.value());

    QSignalBlocker b1(ui->pbColorDialogSectors);
    ui->pbColorDialogSectors->setColor(color);

    QSignalBlocker b2(ui->pwSectors);
    ui->pwSectors->setCurrentColor(color);

    QSignalBlocker b3(ui->cwSectors);
    ui->cwSectors->setColor(color);

    if (inSectorMode())
    {
        QList<SectorEditWidget *> widgets = findChildren<SectorEditWidget *>("SectorEditWidget");
        foreach (SectorEditWidget *widget, widgets)
        {
            QColor color = widget->getColor();
            int saturation = color.hsvSaturation() + delta;
            saturation = qMax(qMin(saturation, 255), 0);
            qDebug() << "saturation: " << saturation;
            QColor newColor;
            newColor.setHsv(color.hsvHue(), saturation, color.value());
            widget->setColor(newColor);
        }
    }
    else
    {
        auto klekeys = KeyboardPropertiesInstance().properties()->getKeys();
        for (auto &key : klekeys)
        {
            if (key->item->isSelected())
            {
                QColor color = key->color;
                int saturation = key->color.hsvSaturation() + delta;
                saturation = qMax(qMin(saturation, 255), 0);
                key->color.setHsv(color.hsvHue(), saturation, color.value());
            }
        }

        emit signalGraphicsSceneChanged();

        if (ui->cbSync->isChecked())
            syncCurrentMapToKeyboard();
    }

    updateValueSlider(color);
}

void SectorEditor::on_gsSectorAllValue_valueChanged(int value)
{
    int delta = value - _oldValue;
    _oldValue = value;
    qDebug() << "value: " << value;
    qDebug() << "delta: " << delta;

    QColor oldColor = ui->cwSectors->color();
    QColor color;
    color.setHsv(oldColor.hsvHue(), oldColor.hsvSaturation(), value);

    QSignalBlocker b1(ui->pbColorDialogSectors);
    ui->pbColorDialogSectors->setColor(color);

    QSignalBlocker b2(ui->pwSectors);
    ui->pwSectors->setCurrentColor(color);

    QSignalBlocker b3(ui->cwSectors);
    ui->cwSectors->setColor(color);

    if (inSectorMode())
    {
        QList<SectorEditWidget *> widgets = findChildren<SectorEditWidget *>("SectorEditWidget");
        foreach (SectorEditWidget *widget, widgets)
        {
            QColor color = widget->getColor();
            int value = color.value() + delta;
            value = qMax(qMin(value, 255), 0);
            qDebug() << "value: " << value;
            QColor newColor;
            newColor.setHsv(color.hsvHue(), color.hsvSaturation(), value);
            widget->setColor(newColor);
        }
    }
    else
    {
        auto klekeys = KeyboardPropertiesInstance().properties()->getKeys();
        for (auto &key : klekeys)
        {
            if (key->item->isSelected())
            {
                QColor color = key->color;
                int value = color.value() + delta;
                value = qMax(qMin(value, 255), 0);
                key->color.setHsv(color.hsvHue(), color.hsvSaturation(), value);
            }
        }

        emit signalGraphicsSceneChanged();

        if (ui->cbSync->isChecked())
            syncCurrentMapToKeyboard();
    }

    updateSaturationSlider(color);
}

void SectorEditor::slot_sectorWidget_editableChanged()
{
    QColor color = ui->cwSectors->color();
    updateValueSlider(color);
    updateSaturationSlider(color);
}

void SectorEditor::on_cbSync_toggled(bool checked)
{
    ui->pbSet->setEnabled(!checked);

    QList<SectorEditWidget *> widgets = findChildren<SectorEditWidget *>("SectorEditWidget");
    foreach (SectorEditWidget *widget, widgets)
        widget->setSyncOnChange(checked);
}

void SectorEditor::getMapRow(int row, bool getOnlyChangedColors, s_map_row &map_row)
{
    map_row.row = row;

    auto klekeys = KeyboardPropertiesInstance().properties()->getKeys();
    for (auto &key : klekeys)
    {
        if (key->row == row)
        {
            if (!getOnlyChangedColors || _diffColorMap.getColor(key->row, key->col) != key->color)
            {
                map_row.entries.emplace_back(s_map_entry(key->row, key->col, key->color));

                if (getOnlyChangedColors)
                    _diffColorMap.setColor(key->row, key->col, key->color);
            }
        }
    }
}

std::unique_ptr<s_map> SectorEditor::getMap(int row, bool getOnlyChangedColors, bool getAllRows)
{
    std::unique_ptr<s_map> map = std::unique_ptr<s_map>(new s_map);
    auto properties = KeyboardPropertiesInstance().properties();

    if (row < 0 || row > (int)properties->rows() || getAllRows)
    {
        for (int r = 0; r < (int)properties->rows(); ++r)
        {
            struct s_map_row map_row;
            getMapRow(r, getOnlyChangedColors, map_row);
            map->rows.push_back(map_row);
        }
    }
    else
    {
        struct s_map_row map_row;
        getMapRow(row, getOnlyChangedColors, map_row);
        map->rows.push_back(map_row);
    }

    return map;
}

std::unique_ptr<s_map> SectorEditor::getFullColorMap()
{
    return getMap(-1, false, true);
}

QString SectorEditor::getColorMapAsJSONString()
{
    std::unique_ptr<s_map> colormap = getFullColorMap();
    colormap->map = ui->cbMap->currentIndex();
    return KeyboardCommandTransferInterface().getProtocol()->getColorMapAsJSONString(std::move(colormap), false);
}

void SectorEditor::setColorMapFromJSONString(QString jsonstring)
{
    sMapPtr colormap = KeyboardCommandTransferInterface().getProtocol()->getColorMapFromJSONString(jsonstring);
    colormap->map = ui->cbMap->currentIndex();
    on_syncedMap(colormap);
}

int SectorEditor::getCurrentColorMapIndex()
{
    return ui->cbMap->currentIndex();
}

void SectorEditor::syncSectorsToKeyboard()
{
    QList<SectorEditWidget *> widgets = findChildren<SectorEditWidget *>("SectorEditWidget");
    foreach (SectorEditWidget *widget, widgets)
        widget->syncToKeyboard();
}

void SectorEditor::syncCurrentMapToKeyboard()
{
    qDebug() << __PRETTY_FUNCTION__ << ": set color map " << ui->cbMap->currentIndex();
    auto properties = KeyboardPropertiesInstance().properties();

    if (properties->supportsRowBasedMapUpdates())
    {
        for (unsigned int r = 0; r < properties->rows(); r++)
        {
            std::unique_ptr<s_map> colormap = getMap(r, properties->supportsDifferentialMapUpdates(), false);
            if (!colormap->rows.empty() && !colormap->rows.front().entries.empty())
            {
                colormap->map = ui->cbMap->currentIndex();
                KeyboardCommandQueueInstance().enqueue(
                    std::move(std::unique_ptr<KeyboardCommand>(new SectorMapCommand(std::move(colormap)))));
            }
        }
    }
    else
    {
        std::unique_ptr<s_map> colormap = getMap(-1, properties->supportsDifferentialMapUpdates(), true);
        if (!colormap->rows.empty())
        {
            colormap->map = ui->cbMap->currentIndex();
            KeyboardCommandQueueInstance().enqueue(
                std::move(std::unique_ptr<KeyboardCommand>(new SectorMapCommand(std::move(colormap)))));
        }
    }
}

void SectorEditor::syncCurrentMapFromKeyboard()
{
    qDebug() << __PRETTY_FUNCTION__ << ": get color map " << ui->cbMap->currentIndex();

    using namespace std::placeholders; // for _1, _2, _3...
    auto cmd = std::unique_ptr<KeyboardCommand>(
        new SectorMapGetCommand(ui->cbMap->currentIndex(), std::bind(&SectorEditor::syncedMap, this, _1)));
    KeyboardCommandQueueInstance().enqueue(std::move(cmd));
}

void SectorEditor::syncedMap(sMapPtr colormap)
{
    emit signalSyncedMap(colormap);
}

void SectorEditor::on_syncedMap(sMapPtr colormap)
{
    qDebug() << __PRETTY_FUNCTION__ << ": synced to map: " << colormap->map;
    qDebug() << __PRETTY_FUNCTION__ << ": synced map rows: " << colormap->rows.size();

    int mapindex = colormap->map;

    for (auto const &row : colormap->rows)
    {
        qDebug() << __PRETTY_FUNCTION__ << ": apply row " << row.row << ", columns: " << row.entries.size();
        for (auto const &column : row.entries)
        {
            _colorMaps[mapindex].setColor(row.row, column.col, column.color);
        }
    }

    if (!inSectorMode() && ui->cbMap->currentIndex() == mapindex)
    {
        qDebug() << __PRETTY_FUNCTION__ << ": update scene for map " << mapindex;
        auto klekeys = KeyboardPropertiesInstance().properties()->getKeys();
        for (auto &key : klekeys)
        {
            key->color = _colorMaps[mapindex].getColor(key->row, key->col);
            _diffColorMap.setColor(key->row, key->col, key->color);
        }

        emit signalGraphicsSceneChanged();
    }
}

void SectorEditor::on_pbSet_clicked()
{
    if (_inSectorMode)
    {
        syncSectorsToKeyboard();
    }
    else
    {
        syncCurrentMapToKeyboard();
    }
}

void SectorEditor::on_pbSave_clicked()
{
    if (_inSectorMode)
    {
        KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new SectorSaveCommand(255))));
    }
    else
    {
        KeyboardCommandQueueInstance().enqueue(
            std::move(std::unique_ptr<KeyboardCommand>(new SectorMapSaveCommand(ui->cbMap->currentIndex()))));
    }
}

void SectorEditor::on_pbReset_clicked()
{
    if (_inSectorMode)
    {
        on_syncKeyboardStates();
    }
    else
    {
        syncCurrentMapFromKeyboard();
    }
}

void SectorEditor::saveCurrentColorMap(int index)
{
    if (!_inSectorMode && _oldMapIndex != index)
    {
        // save
        qDebug() << __PRETTY_FUNCTION__ << ": new index: " << index << ", save color map to old index " << _oldMapIndex;
        auto klekeys = KeyboardPropertiesInstance().properties()->getKeys();
        for (auto &key : klekeys)
        {
            _colorMaps[_oldMapIndex].setColor(key->row, key->col, key->color);
        }
    }
}

void SectorEditor::restoreSelectedColorMap(bool inSectorMode, int index)
{
    if (!inSectorMode && _oldMapIndex != index)
    {
        if (_colorMaps[index].isSynced())
        {
            KeyboardCommandQueueInstance().enqueue(
                std::move(std::unique_ptr<KeyboardCommand>(new SectorMapSetCommand(ui->cbMap->currentIndex()))));

            qDebug() << __PRETTY_FUNCTION__ << ": restore color map from index " << index;
            auto klekeys = KeyboardPropertiesInstance().properties()->getKeys();
            for (auto &key : klekeys)
            {
                key->color = _colorMaps[index].getColor(key->row, key->col);
                _diffColorMap.setColor(key->row, key->col, key->color);
            }
        }
        else
        {
            qDebug() << __PRETTY_FUNCTION__ << ": sync map " << index << " to keyboard";
            syncCurrentMapFromKeyboard();
            _colorMaps[index].setSynced(true);
        }
    }
}

void SectorEditor::updateSectorEditorWidgets(bool inSectorMode)
{
    if (inSectorMode != _inSectorMode)
    {
        QList<SectorEditWidget *> widgets = findChildren<SectorEditWidget *>("SectorEditWidget");
        foreach (SectorEditWidget *widget, widgets)
            widget->setSectorColorsEnabled(inSectorMode);

        ui->stackedWidget->setCurrentIndex(inSectorMode ? 0 : 1);
    }
}

bool SectorEditor::inSectorMode() const
{
    return _inSectorMode;
}

void SectorEditor::on_cbMap_currentIndexChanged(int index)
{
    if (_oldMapIndex == index)
        return;

    bool inSectorMode = (index >= (int)KeyboardPropertiesInstance().properties()->colorMapCount());

    qDebug() << __PRETTY_FUNCTION__ << ": old " << _oldMapIndex;
    qDebug() << __PRETTY_FUNCTION__ << ": new " << index;
    qDebug() << __PRETTY_FUNCTION__ << ": _inSectorMode: " << _inSectorMode;
    qDebug() << __PRETTY_FUNCTION__ << ": inSectorMode: " << inSectorMode;

    saveCurrentColorMap(index);
    updateSectorEditorWidgets(inSectorMode);
    restoreSelectedColorMap(inSectorMode, index);
    emit signalSectorModeChanged(inSectorMode);

    _inSectorMode = inSectorMode;
    _oldMapIndex = index;

    updateSliders();

    emit signalGraphicsSceneChanged();
}

void SectorEditor::updateSliders()
{
    updateSaturationSlider(ui->cwSectors->color());
    updateValueSlider(ui->cwSectors->color());
}

void SectorEditor::on_pbMapSelectAll_clicked()
{
    emit signalMapSelectAll(true);
    updateSliders();
}

void SectorEditor::on_pbMapUnselectAll_clicked()
{
    emit signalMapSelectAll(false);
    updateSliders();
}

void SectorEditor::on_pbMapInvertSelection_clicked()
{
    emit signalMapSelectInvert();
    updateSliders();
}

void SectorEditor::slot_MapSectorButton_clicked(int sector)
{
    emit signalMapSectorSelected(sector);
    updateSliders();
}

void SectorEditor::on_pbMapUseColor_clicked()
{
    // TODO: get color from selected key
    colorChanged(ui->cwSectors->color());
    updateSliders();
}
