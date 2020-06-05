#ifndef KEYBOARDCONSOLE91TKL_H
#define KEYBOARDCONSOLE91TKL_H

#include <QWidget>

namespace Ui {
class KeyboardConsole91tkl;
}

class KeyboardConsole91tkl : public QWidget
{
    Q_OBJECT

public:
    explicit KeyboardConsole91tkl(QWidget *parent = 0);
    virtual ~KeyboardConsole91tkl();

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
    Ui::KeyboardConsole91tkl *ui;
    QString colorize(const QString &msg, QColor color);
    void append(const QString &msg, QColor color);
    QString finalize(const QString &msg);
};

#endif // KEYBOARDCONSOLE91TKL_H
