#pragma once
#include <QtWidgets/QWidget>

namespace Ui
{
class KeyboardConsoleJellowcake;
}

class KeyboardConsoleJellowcake : public QWidget
{
    Q_OBJECT

public:
    explicit KeyboardConsoleJellowcake(QWidget *parent = 0);
    virtual ~KeyboardConsoleJellowcake();

signals:
    void signalRequest(QString const &request);

public slots:
    void on_request(QString const &request);
    void on_response(QString const &response);
    void on_warning(QString const &warnmsg);
    void on_error(QString const &errormsg);
    void on_message(QString const &msg);

private slots:
    void on_pushButton_clicked();
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_comboBox_editTextChanged(const QString &arg1);
    void combobox_returnPressed();

private:
    Ui::KeyboardConsoleJellowcake *ui;
    QString colorize(const QString &msg, QColor color);
    void append(const QString &msg, QColor color);
    QString finalize(const QString &msg);
};
