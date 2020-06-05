#pragma once
#include <QWidget>

namespace Ui
{
class LedBrightness;
}

class LedBrightness : public QWidget
{
    Q_OBJECT

public:
    explicit LedBrightness(QString const &name, QWidget *parent = 0);
    ~LedBrightness();

    bool isChecked() const;

    void setBrightness(unsigned int brightness);
    unsigned int brightness() const;

signals:
    void signalBrightnessChanged(unsigned int brightness);
    void signalIlluminationChanged(bool on);

private slots:
    void on_checkBox_toggled(bool checked);
    void on_GradientSlider_valueChanged(int value);
    void on_spinBox_valueChanged(int arg1);

private:
    Ui::LedBrightness *ui;
};
