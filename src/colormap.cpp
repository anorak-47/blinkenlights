#include "colormap.h"
#include <QtCore/QDebug>

QString ColorMap::_colorIndex = "%1x%2";

ColorMap::ColorMap()
{
}

void ColorMap::setColor(unsigned int row, unsigned int column, QColor const &color)
{
    QString index = _colorIndex.arg(row).arg(column);

    auto it = _colors.find(index);
    if (it == _colors.end())
    {
        _colors.insert(std::pair<QString, QColor>(index, color));
        return;
    }

    it->second = color;
}

QColor ColorMap::getColor(unsigned int row, unsigned int column)
{
    QString index = _colorIndex.arg(row).arg(column);
    auto it = _colors.find(index);
    if (it != _colors.end())
    {
        return it->second;
    }

    return QColor();
}

void ColorMap::setSynced(bool synced)
{
    isInSync = synced;
}

bool ColorMap::isSynced() const
{
    return isInSync;
}
