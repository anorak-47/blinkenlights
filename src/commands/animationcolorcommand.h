#ifndef ANIMATIONCOLORCOMMAND_H
#define ANIMATIONCOLORCOMMAND_H

#include "animationchangecommand.h"
#include <QtGui/QColor>

class AnimationColorCommand : public AnimationChangeCommand
{
public:
    AnimationColorCommand(unsigned int animation, unsigned int colorid, QColor const &color);

    QColor color() const;
    void setColor(const QColor &color);

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

    unsigned int colorid() const;
    void setColorid(unsigned int colorid);

protected:
    unsigned int _colorid;
    QColor _color;
};

#endif // ANIMATIONCOLORCOMMAND_H
