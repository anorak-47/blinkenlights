#ifndef ANIMATOR91TKL_H
#define ANIMATOR91TKL_H

#include "animator.h"
#include <QtCore/QThread>

class Animator91tkl : public QThread, public Animator
{
public:
    Animator91tkl(QObject *parent = 0);

    virtual void startAnimation(unsigned int animation) override;
    virtual void stopAnimation() override;

    virtual void setAnimationColor1(QColor const &color) const;
    virtual void setAnimationColor2(QColor const &color) const;

    bool isKeyPressed(uint8_t row, uint8_t col) const override;

protected:
    virtual void run() override;
    virtual void keyPressEventInGraphicsView(QKeyEvent ev) override;
    virtual void keyReleaseEventInGraphicsView(QKeyEvent ev) override;

    std::vector<uint32_t> _pressed_keys;
};

#endif // ANIMATOR91TKL_H
