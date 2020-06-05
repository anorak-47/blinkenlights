#ifndef KEYCAPCOLOREDITOR_H
#define KEYCAPCOLOREDITOR_H

#include <QWidget>

namespace Ui {
class KeycapColorEditor;
}

class KeycapColorEditor : public QWidget
{
    Q_OBJECT

public:
    explicit KeycapColorEditor(QWidget *parent = 0);
    ~KeycapColorEditor();

private:
    void initColorsColorPaletteWidget();
    Ui::KeycapColorEditor *ui;
};

#endif // KEYCAPCOLOREDITOR_H
