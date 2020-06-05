#ifndef ANIMATIONCONTROL_H
#define ANIMATIONCONTROL_H

#include <QtWidgets/QWidget>
#include <QtCore/QString>

namespace Ui
{
class AnimationControl;
}

class AnimationControl : public QWidget
{
    Q_OBJECT

public:
    explicit AnimationControl(QWidget *parent = 0);
    ~AnimationControl();

    void addAnimation(QString const &name, unsigned int index);

    bool syncOnChange() const;
    void setSyncOnChange(bool syncOnChange);

    bool changeAnimationOnSelection() const;
    void setChangeAnimationOnSelection(bool set_animation_on_change);

signals:
    void signalSyncedAnimationState(unsigned int animation, bool running, unsigned int fps, const QColor &color1,
                                    const QColor &color2);
    void signalSyncedAnimationNameList(QStringList);

    void signalAnimationIsRunning(bool running);

public slots:
    void on_syncAnimationState();

    void on_syncedAnimationState(unsigned int animation, bool running, unsigned int fps, const QColor &color1,
                                 const QColor &color2);
    void on_syncedAnimationNameList(QStringList animation_names);

protected:
    void updateAnimationState();

private slots:
    void on_csColor1_colorChanged(const QColor &arg1);
    void on_csColor2_colorChanged(const QColor &arg1);
    void on_pbAnimate_clicked();
    void on_pbStop_clicked();
    void on_cbAnimation_currentIndexChanged(int index);
    void on_cbSync_toggled(bool checked);
    void on_pbSet_clicked();
    void on_pbSave_clicked();
    void on_spinBox_valueChanged(int value);

    void on_pbFPS1_clicked();
    void on_pbFPS2_clicked();
    void on_pbFPS3_clicked();
    void on_pbFPS4_clicked();

private:
    void syncedAnimationState(unsigned int animation, bool running, unsigned int fps, const QColor &color1,
                              const QColor &color2);
    void syncedAnimationNameList(QStringList animation_names);

    unsigned int _id = 0;
    bool _syncing = false;
    bool _syncOnChange = true;
    bool _animation_is_running = false;
    bool _set_animation_on_change = true;
    Ui::AnimationControl *ui;
};

#endif // ANIMATIONCONTROL_H
