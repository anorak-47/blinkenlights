#ifndef KEYGRAPHICSITEM_H
#define KEYGRAPHICSITEM_H

#include "klekey.h"
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsScene>
#include <QtGui/QPainter>
#include <memory>

class SimpleKeyGraphicsItem : public QGraphicsItem
{
public:
    SimpleKeyGraphicsItem(std::shared_ptr<KLEKey> klekey, QGraphicsItem *parent = Q_NULLPTR);

    void setKeyColor(QColor const &color);
    void setSelectable(bool on);

    bool showRowColumn() const;
    void setShowRowColumn(bool showRowColumn);

    bool adaptForegroundColor() const;
    void setAdaptForegroundColor(bool adaptForegroundColor);

protected:
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void drawLegends(QPainter *painter, int penWidth);
    
private:
    std::shared_ptr<KLEKey> _klekey;
    bool _showRowColumn = true;
    bool _adaptForegroundColor = true;
};

#endif // KEYGRAPHICSITEM_H
