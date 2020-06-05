#include "keygraphicsitem.h"
#include <QtWidgets/QStyleOptionGraphicsItem>
#include <QtCore/QDebug>

SimpleKeyGraphicsItem::SimpleKeyGraphicsItem(std::shared_ptr<KLEKey> klekey, QGraphicsItem *parent)
    : QGraphicsItem(parent), _klekey(klekey)
{
    setFlag(ItemIsSelectable, false);
    setAcceptHoverEvents(false);

    klekey->item = this; // WAAAAAA! RUN FAST!
}

void SimpleKeyGraphicsItem::setKeyColor(const QColor &color)
{
    _klekey->color = color;
}

void SimpleKeyGraphicsItem::setSelectable(bool on)
{
    setFlag(ItemIsSelectable, on);
}

void SimpleKeyGraphicsItem::drawLegends(QPainter *painter, int penWidth)
{
    if (_adaptForegroundColor)
    {
        if (_klekey->color.lightness() > 100)
            painter->setPen(QPen(Qt::black, 1));
        else
            painter->setPen(QPen(Qt::white, 1));
    }
    else
    {
        painter->setPen(QPen(Qt::white, 1));
    }

    painter->save();

    QFont font = painter->font();
    font.setPixelSize(_klekey->f * 5);
    painter->setFont(font);

    QFontMetrics fm = painter->fontMetrics();
    int y = penWidth + 3;
    int x = penWidth + 3;
    int xpos;
    int ypos;
    unsigned int position = 0;

    for (QStringList::const_iterator it = _klekey->codes.constBegin(); it != _klekey->codes.constEnd(); ++it, ++position)
    {
        if (_klekey->codes[position].isEmpty())
           continue;

        /*
       0 top left\n
       1 bottom left\n
       2 top right\n
       3 bottom right\n
       4 front left\n
       5 front right\n
       6 center left\n
       7 center right\n
       8 top center\n
       9 center\n
       0 bottom center\n
       1 front center"
       */

        switch (position)
        {
        case 0:
            xpos = x;
            ypos = y + fm.lineSpacing();
            break;
        case 1:
            xpos = x;
            ypos = qRound(_klekey->height * 100.0) - y*2;
            break;
        case 3:
            xpos = qRound(_klekey->width * 100.0) - fm.width(_klekey->codes[position]) - x*2;
            ypos = qRound(_klekey->height * 100.0) - y*2;
            break;
        case 6:
            xpos = x;
            ypos = (qRound(_klekey->height * 100.0) - y*2 - fm.lineSpacing()) / 2 + fm.lineSpacing();
            break;
        default:
            xpos = x;
            ypos = y + fm.height();
            break;
        }

        //qDebug() << "pos: " << position << ", [" << _klekey->codes[position] << "], xpos: " << xpos << ", ypos: " << ypos;

        painter->drawText(xpos, ypos, QString("%1").arg(_klekey->codes[position]));
    }

    if (_showRowColumn)
    {
        font.setPixelSize(15);
        painter->setFont(font);
        QFontMetrics fm = painter->fontMetrics();

        QString str = QString("(%1|%2)").arg(_klekey->row).arg(_klekey->col);
        xpos = qRound(_klekey->width * 100.0) - fm.width(str) - x*2;
        ypos = qRound(_klekey->height * 100.0) - y*2;

        painter->drawText(xpos, ypos, str);
    }

    painter->restore();
}

bool SimpleKeyGraphicsItem::adaptForegroundColor() const
{
    return _adaptForegroundColor;
}

void SimpleKeyGraphicsItem::setAdaptForegroundColor(bool adaptForegroundColor)
{
    _adaptForegroundColor = adaptForegroundColor;
}

bool SimpleKeyGraphicsItem::showRowColumn() const
{
    return _showRowColumn;
}

void SimpleKeyGraphicsItem::setShowRowColumn(bool showRowColumn)
{
    _showRowColumn = showRowColumn;
}

void SimpleKeyGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                  QWidget *widget)
{
    Q_UNUSED(widget);

    QBrush b = painter->brush();

    if (option->state & QStyle::State_Selected || option->state & QStyle::State_MouseOver)
    {
        QColor frameColor = (option->state & QStyle::State_Selected) ? Qt::white : Qt::black;
        if (option->state & QStyle::State_MouseOver)
            frameColor = Qt::yellow;

        painter->setPen(QPen(frameColor, 1));
        painter->setBrush(QBrush(frameColor));

        painter->drawRect(0, 0, 100.0 * _klekey->width, 100.0 * _klekey->height);
        //painter->drawRoundedRect(0, 0, 100.0 * _klekey->width, 100.0 * _klekey->height, 15, 15);
    }

    painter->setPen(QPen(_klekey->color, 1));
    painter->setBrush(QBrush(_klekey->color));

    int penWidth = painter->pen().width();
    penWidth += 2;

    painter->drawRoundedRect(penWidth, penWidth, 100.0 * _klekey->width - penWidth*2, 100.0 * _klekey->height - penWidth*2, 10, 10);
    //painter->drawRect(penWidth, penWidth, 100.0 * _klekey->width - penWidth*2, 100.0 * _klekey->height - penWidth*2);

    painter->setBrush(b);

    drawLegends(painter, penWidth);

    /*
    if (_klekey->selected)
    {
        _klekey->selected = false;
        //setSelected(true);
        setSelected(!isSelected());
    }
    */
}

QRectF SimpleKeyGraphicsItem::boundingRect() const
{
    return QRectF(0.0, 0.0, 100.0 * _klekey->width, 100.0 * _klekey->height);
}

QPainterPath SimpleKeyGraphicsItem::shape() const
{
    QPainterPath path;
    path.addRect(20.0, 20.0, 100.0 * _klekey->width - 40, 100.0 * _klekey->height - 40);
    return path;
}
