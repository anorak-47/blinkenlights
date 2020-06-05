#include "ledbrightness.h"
#include "ui_ledbrightness.h"

LedBrightness::LedBrightness(const QString &name, QWidget *parent) : QWidget(parent), ui(new Ui::LedBrightness)
{
    ui->setupUi(this);
    ui->label->setText(name);
    ui->GradientSlider->setLastColor(Qt::red);
}

LedBrightness::~LedBrightness()
{
    delete ui;
}

bool LedBrightness::isChecked() const
{
    return ui->checkBox->isChecked();
}

void LedBrightness::setBrightness(unsigned int brightness)
{
    QSignalBlocker(ui->spinBox);
    ui->spinBox->setValue(brightness);

    QSignalBlocker(ui->GradientSlider);
    ui->GradientSlider->setValue(brightness);
}

unsigned int LedBrightness::brightness() const
{
    return ui->spinBox->value();
}

void LedBrightness::on_checkBox_toggled(bool checked)
{
    emit signalIlluminationChanged(checked);
}

void LedBrightness::on_GradientSlider_valueChanged(int value)
{
    QSignalBlocker(ui->spinBox);
    ui->spinBox->setValue(value);

    emit signalBrightnessChanged(value);
}

void LedBrightness::on_spinBox_valueChanged(int arg1)
{
    QSignalBlocker(ui->GradientSlider);
    ui->GradientSlider->setValue(arg1);

    emit signalBrightnessChanged(arg1);
}
