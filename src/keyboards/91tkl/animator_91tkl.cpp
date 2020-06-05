
#include "keyboard_properties.h"
#include "animation.h"
#include <QtCore/QDebug>
#include "animator_91tkl.h"

Animator91tkl::Animator91tkl(QObject *parent)
    : QThread(parent)
{
    _pressed_keys.resize(KeyboardPropertiesInstance().properties()->rows(), 0);
    initialize_animation();
}

void Animator91tkl::startAnimation(unsigned int animation)
{
    qDebug() << __PRETTY_FUNCTION__ << ": start " << animation;
    _animation = animation;

    set_and_start_animation((animation_names) animation);

    saveState();
    emit signalStarted();

    if (!isRunning())
        start();
}

void Animator91tkl::stopAnimation()
{
    qDebug() << __PRETTY_FUNCTION__ << ": stop";

    stop_animation();

    if (isRunning())
    {
        requestInterruption();
        wait(3000);
    }

    emit signalStopped();

    restoreState();
}

void Animator91tkl::setAnimationColor1(const QColor &color) const
{

}

void Animator91tkl::setAnimationColor2(const QColor &color) const
{

}

bool Animator91tkl::isKeyPressed(uint8_t row, uint8_t col) const
{
    return _pressed_keys[row] & (1 << col);
}

void Animator91tkl::run()
{
    while (!isInterruptionRequested())
    {
        system_tick();
        animate();
        usleep(1000);
    }
}

void Animator91tkl::keyPressEventInGraphicsView(QKeyEvent ev)
{
    QString evkey = ev.text().toUpper();
    qDebug() << "key: " << evkey;

    auto klekeys = KeyboardPropertiesInstance().properties()->getKeys();
    for (auto &key : klekeys)
    {
        if (key->codes.contains(evkey))
        {
            _pressed_keys[key->row] |= (1 << key->col);
            animation_typematrix_row(key->row, (1 << key->col));
            break;
        }
    }
}

void Animator91tkl::keyReleaseEventInGraphicsView(QKeyEvent ev)
{
    QString evkey = ev.text().toUpper();
    auto klekeys = KeyboardPropertiesInstance().properties()->getKeys();
    for (auto &key : klekeys)
    {
        if (key->codes.contains(evkey))
        {
            _pressed_keys[key->row] &= ~(1 << key->col);
            break;
        }
    }
}
