#pragma once
#include <QtGui/QColor>
#include <QtGui/QKeyEvent>
#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <vector>

using AnimationMatrix = std::vector<std::vector<QColor>>;

Q_DECLARE_METATYPE(AnimationMatrix)

class Animator : public QObject
{
    Q_OBJECT

public:
    explicit Animator(QObject *parent = 0);
    virtual ~Animator();

    virtual void startAnimation(unsigned int animation) = 0;
    virtual void stopAnimation() = 0;

    virtual void setColor(unsigned int row, unsigned int col, QColor const &color);
    virtual QColor getColor(unsigned int row, unsigned int col) const;

    virtual void setAnimationColor1(QColor const &color) const;
    virtual void setAnimationColor2(QColor const &color) const;

    virtual bool isKeyPressed(uint8_t row, uint8_t col) const;

    virtual void saveState();
    virtual void restoreState();

    virtual void update();

signals:
    void signalChanged(AnimationMatrix const &matrix);
    void signalStarted();
    void signalStopped();

public slots:
    void on_KeyPressEventInGraphicsView(QKeyEvent ev);
    void on_KeyReleaseEventInGraphicsView(QKeyEvent ev);

protected:
    virtual void keyPressEventInGraphicsView(QKeyEvent ev) = 0;
    virtual void keyReleaseEventInGraphicsView(QKeyEvent ev) = 0;

    AnimationMatrix _matrix;
    AnimationMatrix _stored_matrix;
    unsigned int _animation;
};
