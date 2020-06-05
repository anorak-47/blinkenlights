#ifndef KLEKEY_H
#define KLEKEY_H

#include <QtCore/QStringList>
#include <QtGui/QColor>

class SimpleKeyGraphicsItem;

class KLEKey
{
public:
    KLEKey();

    double width = 1.0;
    double height = 1.0;

    double x = 0.0;
    double y = 0.0;

    int row = 0;
    int col = 0;

    int f = 5;
    int f2 = 5;

    QStringList codes;
    QColor color;
    //bool selected = false;

    SimpleKeyGraphicsItem *item = 0;
};

#endif // KLEKEY_H
