#include "animator.h"
#include "keyboard_properties.h"
#include <QtCore/QDebug>

Animator::Animator(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<AnimationMatrix>("AnimationMatrix");

    for (unsigned int i = 0; i < KeyboardPropertiesInstance().properties()->rows(); ++i)
    {
        std::vector<QColor> row;
        row.resize(KeyboardPropertiesInstance().properties()->columns(), QColor());
        _matrix.push_back(row);
    }

    for (unsigned int i = 0; i < KeyboardPropertiesInstance().properties()->rows(); ++i)
    {
        std::vector<QColor> row;
        row.resize(KeyboardPropertiesInstance().properties()->columns(), QColor());
        _stored_matrix.push_back(row);
    }
}

Animator::~Animator()
{
}

void Animator::setColor(unsigned int row, unsigned int col, const QColor &color)
{
    _matrix[row][col] = color;
}

QColor Animator::getColor(unsigned int row, unsigned int col) const
{
    return _matrix[row][col];
}

void Animator::setAnimationColor1(const QColor &color) const
{

}

void Animator::setAnimationColor2(const QColor &color) const
{

}

bool Animator::isKeyPressed(uint8_t row, uint8_t col) const
{
    return false;
}

void Animator::saveState()
{
    auto klekeys = KeyboardPropertiesInstance().properties()->getKeys();
    for (auto &key : klekeys)
        _stored_matrix[key->row][key->col] = key->color;
}

void Animator::restoreState()
{
    auto klekeys = KeyboardPropertiesInstance().properties()->getKeys();
    for (auto &key : klekeys)
        key->color = _stored_matrix[key->row][key->col];

    emit signalChanged(_stored_matrix);
}

void Animator::update()
{
    emit signalChanged(_matrix);
}

void Animator::on_KeyPressEventInGraphicsView(QKeyEvent ev)
{
    keyPressEventInGraphicsView(ev);
}

void Animator::on_KeyReleaseEventInGraphicsView(QKeyEvent ev)
{
    keyReleaseEventInGraphicsView(ev);
}
