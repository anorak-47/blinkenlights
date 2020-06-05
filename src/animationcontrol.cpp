#include "animationcontrol.h"
#include "ui_animationcontrol.h"
#include "keyboard_properties.h"
#include "keyboard_command_queue.h"
#include "animationruncommand.h"
#include "animationcolorcommand.h"
#include "animationstatecommand.h"
#include "animationnamelistcommand.h"
#include "common.h"
#include <QtCore/QDebug>
#include <QtCore/QSettings>

AnimationControl::AnimationControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnimationControl)
{
    ui->setupUi(this);
    connect(this, SIGNAL(signalSyncedAnimationState(uint,bool,uint,QColor,QColor)), this, SLOT(on_syncedAnimationState(uint,bool,uint,QColor,QColor)), Qt::QueuedConnection);

    auto properties = KeyboardPropertiesInstance().properties();

    if (properties->supportsNativeAnimationNames())
    {
        connect(this, SIGNAL(signalSyncedAnimationNameList(QStringList)), this, SLOT(on_syncedAnimationNameList(QStringList)));
    }
    else
    {
        for (unsigned int s = 0; s < properties->animationCount(); ++s)
            addAnimation(properties->animationName(s), s);
    }    

    QSettings settings;
    _set_animation_on_change = settings.value(properties->name() + "/animation/setonchange", _set_animation_on_change).toBool();
    ui->cbSync->setChecked(settings.value(properties->name() + "/animation/synctokeyboard", true).toBool());
}

AnimationControl::~AnimationControl()
{
    QSettings settings;
    auto properties = KeyboardPropertiesInstance().properties();
    settings.setValue(properties->name() + "/animation/setonchange", _set_animation_on_change);
    settings.setValue(properties->name() + "/animation/synctokeyboard", ui->cbSync->isChecked());

    delete ui;
}

void AnimationControl::addAnimation(const QString &name, unsigned int index)
{
    Q_UNUSED(index);
    ui->cbAnimation->addItem(name);
}

void AnimationControl::on_syncAnimationState()
{
    using namespace std::placeholders;  // for _1, _2, _3...

    if (KeyboardPropertiesInstance().properties()->supportsNativeAnimationNames())
    {
        auto cmdState = std::unique_ptr<KeyboardCommand>(new AnimationNameListCommand(std::bind(&AnimationControl::syncedAnimationNameList, this, _1)));
        KeyboardCommandQueueInstance().enqueue(std::move(cmdState));
    }

    updateAnimationState();
}

void AnimationControl::on_syncedAnimationState(unsigned int animation, bool running, unsigned int fps, const QColor &color1 , const QColor &color2)
{
    _syncing = true;

    _animation_is_running = running;

    QSignalBlocker ab(ui->cbAnimation);
    ui->cbAnimation->setCurrentIndex(animation);
    _id = animation;

    QSignalBlocker c1b(ui->csColor1);
    ui->csColor1->setColor(color1);

    QSignalBlocker c2b(ui->csColor2);
    ui->csColor2->setColor(color2);

    QSignalBlocker sbb(ui->spinBox);
    ui->spinBox->setValue(fps);

    ui->lAnimationCurrentName->setText(ui->cbAnimation->itemText(animation));
    ui->lAnimationState->setText(running ? _("running") : _("stopped"));
    ui->lAniamtionFPS->setText(QString::number(fps) + _("fps"));

    _syncing = false;

    emit signalAnimationIsRunning(running);
}

void AnimationControl::on_syncedAnimationNameList(QStringList animation_names)
{
    //qDebug() << __PRETTY_FUNCTION__;

    ui->cbAnimation->clear();
    for (int i = 0; i < animation_names.size(); ++i)
        addAnimation(animation_names[i], i);
}

void AnimationControl::syncedAnimationState(unsigned int animation, bool running, unsigned int fps, const QColor &color1 , const QColor &color2)
{
    //qDebug() << __PRETTY_FUNCTION__;
    emit signalSyncedAnimationState(animation, running, fps, color1, color2);
}

void AnimationControl::syncedAnimationNameList(QStringList animation_names)
{
    //qDebug() << __PRETTY_FUNCTION__ << " " << animation_names.join("|");
    emit signalSyncedAnimationNameList(animation_names);
}

bool AnimationControl::changeAnimationOnSelection() const
{
    return _set_animation_on_change;
}

void AnimationControl::setChangeAnimationOnSelection(bool set_animation_on_change)
{
    _set_animation_on_change = set_animation_on_change;
}

bool AnimationControl::syncOnChange() const
{
    return _syncOnChange;
}

void AnimationControl::setSyncOnChange(bool syncOnChange)
{
    _syncOnChange = syncOnChange;
}

void AnimationControl::on_csColor1_colorChanged(const QColor &arg1)
{
    //qDebug() << __PRETTY_FUNCTION__;
    if (!_syncing && _syncOnChange)
        KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new AnimationColorCommand(_id, 0, arg1))));
}

void AnimationControl::on_csColor2_colorChanged(const QColor &arg1)
{
    //qDebug() << __PRETTY_FUNCTION__;
    if (!_syncing && _syncOnChange)
        KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new AnimationColorCommand(_id, 1, arg1))));
}

void AnimationControl::updateAnimationState()
{
    using namespace std::placeholders;  // for _1, _2, _3...

    auto cmdState = std::unique_ptr<KeyboardCommand>(new AnimationStateCommand(_id, std::bind(&AnimationControl::syncedAnimationState, this, _1, _2, _3, _4, _5)));
    KeyboardCommandQueueInstance().enqueue(std::move(cmdState));
}

void AnimationControl::on_pbAnimate_clicked()
{
    qDebug() << __PRETTY_FUNCTION__ << " id: " << _id;
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new AnimationRunCommand(_id, true))));
    updateAnimationState();
}

void AnimationControl::on_pbStop_clicked()
{
    qDebug() << __PRETTY_FUNCTION__ << " id: " << _id;
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new AnimationRunCommand(_id, false))));
    updateAnimationState();
}

void AnimationControl::on_cbAnimation_currentIndexChanged(int index)
{
    _id = index;

    qDebug() << __PRETTY_FUNCTION__ << " id: " << _id;
    qDebug() << __PRETTY_FUNCTION__ << " running: " << _animation_is_running;
    qDebug() << __PRETTY_FUNCTION__ << " autochange: " << _set_animation_on_change;

    if (_animation_is_running && _set_animation_on_change && ui->cbSync->isChecked())
    {
        KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new AnimationRunCommand(_id, true))));
        updateAnimationState();
    }
}

void AnimationControl::on_cbSync_toggled(bool checked)
{
    ui->pbSet->setEnabled(!checked);
    _syncOnChange = checked;
}

void AnimationControl::on_pbSet_clicked()
{
    if (_syncing)
        return;

    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new AnimationColorCommand(_id, 0, ui->csColor1->color()))));
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new AnimationColorCommand(_id, 1, ui->csColor2->color()))));

    //TODO: set FPS
    //KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new AnimationSetFPSCommand(_id, arg1))));

    if (_animation_is_running)
    {
        KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new AnimationRunCommand(_id, true))));
        updateAnimationState();
    }
}

void AnimationControl::on_pbSave_clicked()
{
    //KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new AnimationSaveCommand(_id))));
}

void AnimationControl::on_spinBox_valueChanged(int value)
{
    if (_syncing || !_syncOnChange)
        return;

    //TODO: set FPS
    //KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new AnimationSetFPSCommand(_id, arg1))));
}

void AnimationControl::on_pbFPS1_clicked()
{
    //KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new AnimationSetFPSCommand(_id, 10))));
}

void AnimationControl::on_pbFPS2_clicked()
{

}

void AnimationControl::on_pbFPS3_clicked()
{

}

void AnimationControl::on_pbFPS4_clicked()
{

}
