#include "sectoreditwidget.h"
#include "ui_sectoreditwidget.h"
#include "keyboard_command_queue.h"
#include "sectorilluminatecommand.h"
#include "sectorcolorcommand.h"
#include "sectorilluminationstatecommand.h"
#include "sectorcolorstatecommand.h"
#include "keyboard_properties.h"
#include <QtCore/QDebug>

SectorEditWidget::SectorEditWidget(const QString &name, unsigned int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SectorEditWidget),
    _id(id)
{
    ui->setupUi(this);
    ui->label->setText(name);

    connect(this, SIGNAL(signalSyncedSectorIllumination(uint,bool)), this, SLOT(on_syncedSectorIllumination(uint,bool)), Qt::QueuedConnection);
    connect(this, SIGNAL(signalSyncedSectorColor(uint,QColor)), this, SLOT(on_syncedSectorColor(uint,QColor)), Qt::QueuedConnection);

    disableSectorColors();
}

SectorEditWidget::~SectorEditWidget()
{
    delete ui;
}

void SectorEditWidget::on_colorChanged(const QColor &color)
{
    setColor(color);
}

void SectorEditWidget::on_syncSectorState()
{
    //qDebug() << __PRETTY_FUNCTION__ << ": " << _id;

    using namespace std::placeholders;  // for _1, _2, _3...
    auto cmdState = std::unique_ptr<KeyboardCommand>(new SectorIlluminationStateCommand(_id, std::bind(&SectorEditWidget::syncedSectorIllumination, this, _1, _2)));
    KeyboardCommandQueueInstance().enqueue(std::move(cmdState));

    auto cmdColor = std::unique_ptr<KeyboardCommand>(new SectorColorStateCommand(_id, std::bind(&SectorEditWidget::syncedSectorColor, this, _1, _2)));
    KeyboardCommandQueueInstance().enqueue(std::move(cmdColor));
}

void SectorEditWidget::on_syncedSectorIllumination(unsigned int sector, bool illuminated)
{
    //qDebug() << __PRETTY_FUNCTION__ << ": " << sector << ", " << illuminated;

    if (sector != _id)
        return;

    QSignalBlocker b(ui->cbSector);
    ui->cbSector->setChecked(illuminated);
}

void SectorEditWidget::on_syncedSectorColor(unsigned int sector, const QColor &color)
{
    //qDebug() << __PRETTY_FUNCTION__ << ": " << sector << ", " << color;

    if (sector != _id)
        return;

    QSignalBlocker b(ui->pbColorDialogSector);
    ui->pbColorDialogSector->setColor(color);

    updateSectorColors(true);

    emit signalIlluminationChanged();
}

void SectorEditWidget::on_cbSector_toggled(bool checked)
{
    qDebug() << __PRETTY_FUNCTION__ << ": checked: " << checked;

    if (checked)
        updateSectorColors(true);
    else
        disableSectorColors();

    if (ui->cbSectorEdit->isChecked())
        emit signalColorChanged(ui->pbColorDialogSector->color());
    else
        emit signalIlluminationChanged();

    if (!_syncOnChange)
        return;

    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new SectorIlluminateCommand(_id, checked))));
}

void SectorEditWidget::updateSectorColors(bool forceUpdate)
{
    if (forceUpdate || ui->cbSectorEdit->isChecked())
    {
        auto keys = KeyboardPropertiesInstance().properties()->getSectorKeys(_id);
        for (auto &key : keys)
            key->color = ui->pbColorDialogSector->color();
    }
}

void SectorEditWidget::disableSectorColors()
{
    if (!_showDisabledSectors)
        return;
    auto keys = KeyboardPropertiesInstance().properties()->getSectorKeys(_id);
    for (auto &key : keys)
        key->color = Qt::black;
}

void SectorEditWidget::setSectorColorsEnabled(bool enabled)
{
    setEnabled(enabled);

    if (enabled && ui->cbSector->isChecked())
        updateSectorColors(true);
    else
        disableSectorColors();
}

bool SectorEditWidget::isSectorColorsEnabled() const
{
    return ui->cbSector->isChecked();
}

bool SectorEditWidget::isEditEnabled() const
{
    return ui->cbSectorEdit->isChecked();
}

QColor SectorEditWidget::getColor() const
{
    return ui->pbColorDialogSector->color();
}

void SectorEditWidget::setColor(const QColor &color)
{
    if (!isEnabled() || !ui->cbSectorEdit->isChecked() || !ui->cbSector->isChecked())
        return;

    QSignalBlocker b(ui->pbColorDialogSector);
    ui->pbColorDialogSector->setColor(color);

    updateSectorColors();

    if (!_syncOnChange)
        return;

    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new SectorColorCommand(_id, color))));
}

void SectorEditWidget::on_pbColorDialogSector_colorChanged(const QColor &color)
{
    updateSectorColors();
    emit signalColorChanged(color);
}

void SectorEditWidget::syncedSectorIllumination(unsigned int sector, bool illuminated)
{
    emit signalSyncedSectorIllumination(sector, illuminated);
}

void SectorEditWidget::syncedSectorColor(unsigned int sector, const QColor &color)
{
    emit signalSyncedSectorColor(sector, color);
}

bool SectorEditWidget::syncOnChange() const
{
    return _syncOnChange;
}

void SectorEditWidget::setSyncOnChange(bool syncOnChange)
{
    //qDebug() << "syncOnChange: " << syncOnChange;
    _syncOnChange = syncOnChange;
}

void SectorEditWidget::syncToKeyboard()
{
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new SectorIlluminateCommand(_id, ui->cbSector->isChecked()))));
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new SectorColorCommand(_id, ui->pbColorDialogSector->color()))));
}

void SectorEditWidget::on_cbSectorEdit_toggled(bool checked)
{
    ui->pbColorDialogSector->setEnabled(checked);
    updateSectorColors();
    emit signalEditChanged(checked);
}

void SectorEditWidget::on_pushButton_clicked()
{
    emit signalUseColor(ui->pbColorDialogSector->color());
}
