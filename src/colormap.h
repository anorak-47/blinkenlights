#ifndef COLORMAP_H
#define COLORMAP_H

#include <QtGui/QColor>
#include <map>

class ColorMap
{
public:
    ColorMap();

    void setColor(unsigned int row, unsigned int column, const QColor &color);
    QColor getColor(unsigned int row, unsigned int column);

    void setSynced(bool synced);
    bool isSynced() const;

private:
    static QString _colorIndex;
    bool isInSync{};
    std::map<QString, QColor> _colors;
};

#endif // COLORMAP_H
