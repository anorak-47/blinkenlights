#include "keyboard_console_91tkl.h"
#include "ui_keyboardconsole91tkl.h"
#include <QtCore/QSettings>
#include <QtWidgets/QCompleter>
#include <QtWidgets/QLineEdit>

KeyboardConsole91tkl::KeyboardConsole91tkl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeyboardConsole91tkl)
{
    ui->setupUi(this);

    QSettings settings;
    QSignalBlocker bl(ui->comboBox);
    ui->comboBox->addItems(settings.value("91tkl/console/inputs").toStringList());

    QStringList words;
    words << "sector" << "info" << "ram" << "ee" << "fee" << "bee" << "bl" << "animation" << "map" << "issi" << "debug" << "keymap" << "layer" << "boot" << "stabri" << "see" << "sleepled";
    ui->comboBox->setCompleter(new QCompleter(words));

    connect(ui->comboBox->lineEdit(), SIGNAL(returnPressed()), this, SLOT(combobox_returnPressed()));
}

KeyboardConsole91tkl::~KeyboardConsole91tkl()
{
    if (ui->comboBox->count() > 0)
    {
        QStringList items;
        for (int i = 0; i < ui->comboBox->count(); i++)
            items += ui->comboBox->itemText(i);

        QSettings settings;
        settings.setValue("91tkl/console/inputs", items);
    }

    delete ui;
}

QString KeyboardConsole91tkl::colorize(const QString &msg, QColor color)
{
    return QString("<font color=\"%1\">%2</font>").arg(color.name()).arg(msg);
}

QString KeyboardConsole91tkl::finalize(const QString &msg)
{
    QString out = msg;
    if (out.endsWith("\n"))
        out.remove(out.length()-1, 1);
    out.replace("\n", "<br>");
    return out;
}

void KeyboardConsole91tkl::append(const QString &msg, QColor color)
{
    ui->plainTextEdit->appendHtml(colorize(finalize(msg), color));
}

void KeyboardConsole91tkl::on_request(const QString &request)
{
    append(request, "#857eb1");
}

void KeyboardConsole91tkl::on_response(const QString &response)
{
    append(response, "#318F8D");
}

void KeyboardConsole91tkl::on_warning(const QString &warnmsg)
{
    append(warnmsg, "#C18709");
}

void KeyboardConsole91tkl::on_error(const QString &errormsg)
{
    append(errormsg, "#950013");
}

void KeyboardConsole91tkl::on_message(const QString &msg)
{
    append(msg, Qt::black);
}

void KeyboardConsole91tkl::on_pushButton_clicked()
{
    emit signalRequest(ui->comboBox->currentText());
}

void KeyboardConsole91tkl::on_comboBox_currentIndexChanged(const QString &arg1)
{
    //emit signalRequest(arg1);
}

void KeyboardConsole91tkl::on_comboBox_editTextChanged(const QString &arg1)
{
    //emit signalRequest(arg1);
}

void KeyboardConsole91tkl::combobox_returnPressed()
{
    emit signalRequest(ui->comboBox->currentText());
}
