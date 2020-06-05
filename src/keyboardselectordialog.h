#ifndef KEYBOARDSELECTORDIALOG_H
#define KEYBOARDSELECTORDIALOG_H

#include "keyboard_factory.h"
#include <QtWidgets/QDialog>

namespace Ui {
class KeyboardSelectorDialog;
}

class KeyboardSelectorDialog : public QDialog
{
    Q_OBJECT

public:
    KeyboardSelectorDialog(present_keyboard_list &keyboardList, QWidget *parent = 0);
    virtual ~KeyboardSelectorDialog();

    s_device_info getSelectedKeyboardDeviceInfo() const;
    SupportedKeyboards getSelectedKeyboardType() const;

private slots:
    void on_cbKeyboard_activated(int index);
    void on_cbPort_activated(int index);

private:
    Ui::KeyboardSelectorDialog *ui;
    present_keyboard_list &_keyboardList;
    void setLabelTexts(struct s_device_info const &kb);
};

#endif // KEYBOARDSELECTORDIALOG_H
