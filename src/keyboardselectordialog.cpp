
#include "keyboardselectordialog.h"
#include "keyboard_factory.h"
#include "ui_keyboardselector.h"
#include <QtCore/QDebug>

KeyboardSelectorDialog::KeyboardSelectorDialog(present_keyboard_list &keyboardList, QWidget *parent)
    : QDialog(parent), ui(new Ui::KeyboardSelectorDialog), _keyboardList(keyboardList)
{
    ui->setupUi(this);

    for (auto const &keyboard : _keyboardList)
    {
        ui->cbKeyboard->addItem(getKeyboardName(keyboard.keyboard));
    }

    if (!_keyboardList.empty())
    {
        auto const &first_kb = _keyboardList.front();

        for (auto const &kb_found : first_kb.keyboards_found_list)
        {
            ui->cbPort->addItem(kb_found.portName);
            setLabelTexts(kb_found);
        }

        ui->cbPort->setEnabled(first_kb.keyboards_found_list.size() > 1);
    }

    ui->cbKeyboard->setEnabled(_keyboardList.size() > 1);
}

KeyboardSelectorDialog::~KeyboardSelectorDialog()
{
}

s_device_info KeyboardSelectorDialog::getSelectedKeyboardDeviceInfo() const
{
    auto const &keyboard = _keyboardList[ui->cbKeyboard->currentIndex()];
    return keyboard.keyboards_found_list[ui->cbPort->currentIndex()];
}

SupportedKeyboards KeyboardSelectorDialog::getSelectedKeyboardType() const
{
    auto const &keyboard = _keyboardList[ui->cbKeyboard->currentIndex()];
    return keyboard.keyboard;
}

void KeyboardSelectorDialog::setLabelTexts(const s_device_info &kb)
{
    ui->lDescription->setText(kb.description);
    ui->lProduct->setText(kb.product);
    ui->lManufacturer->setText(kb.manufacturer);
    ui->lBuild->setText(kb.build);
    ui->lVersion->setText(kb.version);
    ui->lPID->setText(kb.pid);
    ui->lVID->setText(kb.vid);
    ui->lSerial->setText(kb.serialnumber);
}

void KeyboardSelectorDialog::on_cbKeyboard_activated(int index)
{
    if (!(_keyboardList.size() > static_cast<std::size_t>(index)))
        return;

    auto const &keyboard = _keyboardList[index];

    ui->cbPort->clear();

    if (keyboard.keyboards_found_list.empty())
        return;

    for (auto const &kb_found : keyboard.keyboards_found_list)
    {
        ui->cbPort->addItem(kb_found.portName);
    }

    auto const &kb = keyboard.keyboards_found_list.front();
    setLabelTexts(kb);

    ui->cbPort->setEnabled(keyboard.keyboards_found_list.size() > 1);
}

void KeyboardSelectorDialog::on_cbPort_activated(int index)
{
    auto const &keyboard = _keyboardList[ui->cbKeyboard->currentIndex()];

    if (!(keyboard.keyboards_found_list.size() > static_cast<std::size_t>(index)))
        return;

    auto const &kb = keyboard.keyboards_found_list[index];
    setLabelTexts(kb);
}
