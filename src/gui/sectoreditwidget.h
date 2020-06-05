#ifndef SECTOREDITWIDGET_H
#define SECTOREDITWIDGET_H

#include <QWidget>

namespace Ui {
class SectorEditWidget;
}

class SectorEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SectorEditWidget(QString const &name, unsigned int id, QWidget *parent = 0);
    ~SectorEditWidget();

    bool syncOnChange() const;
    void setSyncOnChange(bool syncOnChange);

    void syncToKeyboard();

    void setSectorColorsEnabled(bool enabled);
    bool isSectorColorsEnabled() const;

    bool isEditEnabled() const;

    QColor getColor() const;
    void setColor(QColor const &color);

signals:
    void signalColorChanged(const QColor &color);
    void signalUseColor(const QColor &color);
    void signalIlluminationChanged();
    void signalEditChanged(bool editable);
    void signalSyncedSectorIllumination(unsigned int sector, bool illuminated);
    void signalSyncedSectorColor(unsigned int sector, QColor const &color);

public slots:
    void on_colorChanged(const QColor &color);
    void on_syncSectorState();

    void on_syncedSectorIllumination(unsigned int sector, bool illuminated);
    void on_syncedSectorColor(unsigned int sector, QColor const &color);
    
private slots:
    void on_cbSector_toggled(bool checked);
    void on_pbColorDialogSector_colorChanged(const QColor &color);

    void on_cbSectorEdit_toggled(bool checked);

    void on_pushButton_clicked();

private:
    void syncedSectorIllumination(unsigned int sector, bool illuminated);
    void syncedSectorColor(unsigned int sector, QColor const &color);

    void disableSectorColors();
    void updateSectorColors(bool forceUpdate = false);

    Ui::SectorEditWidget *ui;
    unsigned int _id;
    bool _syncOnChange = true;
    bool _showDisabledSectors = true;
};

#endif // SECTOREDITWIDGET_H
