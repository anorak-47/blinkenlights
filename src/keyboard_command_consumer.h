#pragma once
#include <QtCore/QThread>

class KeyboardCommandConsumer : public QThread
{
    Q_OBJECT

public:
    KeyboardCommandConsumer(QObject *parent = NULL);

    void run() override;
};
