#ifndef SECTORCOLORCOMMAND_H
#define SECTORCOLORCOMMAND_H

#include "sectorchangecommand.h"
#include <QtGui/QColor>

class SectorColorCommand : public SectorChangeCommand
{
public:
    SectorColorCommand(unsigned int sector, QColor const &color);

    QColor color() const;
    void setColor(const QColor &color);

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

protected:
    QColor _color;
};

#endif // SECTORCOLORCOMMAND_H
