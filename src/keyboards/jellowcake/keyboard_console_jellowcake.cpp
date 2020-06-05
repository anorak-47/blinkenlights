#include "keyboard_console_jellowcake.h"
#include "ui_keyboardconsole_jellowcake.h"
#include <QtCore/QSettings>
#include <QtWidgets/QCompleter>
#include <QtWidgets/QLineEdit>

KeyboardConsoleJellowcake::KeyboardConsoleJellowcake(QWidget *parent)
    : QWidget(parent), ui(new Ui::KeyboardConsoleJellowcake)
{
    ui->setupUi(this);

    QSettings settings;
    QSignalBlocker bl(ui->comboBox);
    ui->comboBox->addItems(settings.value("Jellowcake/console/inputs").toStringList());

    QStringList words;
    words << "info"
          << "ram"
          << "bl"
          << "animation"
          << "map"
          << "issi"
          << "debug"
          << "keymap"
          << "layer"
          << "boot"
          << "stabri"
          << "see"
          << "sleepled";
    ui->comboBox->setCompleter(new QCompleter(words));

    connect(ui->comboBox->lineEdit(), SIGNAL(returnPressed()), this, SLOT(combobox_returnPressed()));
}

KeyboardConsoleJellowcake::~KeyboardConsoleJellowcake()
{
    if (ui->comboBox->count() > 0)
    {
        QStringList items;
        for (int i = 0; i < ui->comboBox->count(); i++)
            items += ui->comboBox->itemText(i);

        QSettings settings;
        settings.setValue("Jellowcake/console/inputs", items);
    }

    delete ui;
}

QString KeyboardConsoleJellowcake::colorize(const QString &msg, QColor color)
{
    return QString("<font color=\"%1\">%2</font>").arg(color.name()).arg(msg);
}

QString KeyboardConsoleJellowcake::finalize(const QString &msg)
{
    QString out = msg;
    if (out.endsWith("\n"))
        out.remove(out.length() - 1, 1);
    out.replace("\n", "<br>");
    return out;
}

void KeyboardConsoleJellowcake::append(const QString &msg, QColor color)
{
    ui->plainTextEdit->appendHtml(colorize(finalize(msg), color));
}

void KeyboardConsoleJellowcake::on_request(const QString &request)
{
    append(request, "#857eb1");
}

void KeyboardConsoleJellowcake::on_response(const QString &response)
{
    append(response, "#318F8D");
}

void KeyboardConsoleJellowcake::on_warning(const QString &warnmsg)
{
    append(warnmsg, "#C18709");
}

void KeyboardConsoleJellowcake::on_error(const QString &errormsg)
{
    append(errormsg, "#950013");
}

void KeyboardConsoleJellowcake::on_message(const QString &msg)
{
    append(msg, Qt::black);
}

void KeyboardConsoleJellowcake::on_pushButton_clicked()
{
    emit signalRequest(ui->comboBox->currentText());
}

void KeyboardConsoleJellowcake::on_comboBox_currentIndexChanged(const QString &arg1)
{
    // emit signalRequest(arg1);
}

void KeyboardConsoleJellowcake::on_comboBox_editTextChanged(const QString &arg1)
{
    // emit signalRequest(arg1);
}

void KeyboardConsoleJellowcake::combobox_returnPressed()
{
    emit signalRequest(ui->comboBox->currentText());
}
