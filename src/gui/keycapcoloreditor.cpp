#include "keycapcoloreditor.h"
#include "ui_keycapcoloreditor.h"

KeycapColorEditor::KeycapColorEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeycapColorEditor)
{
    ui->setupUi(this);
    initColorsColorPaletteWidget();
}

KeycapColorEditor::~KeycapColorEditor()
{
    delete ui;
}

void KeycapColorEditor::initColorsColorPaletteWidget()
{
    color_widgets::ColorPalette palette1;
    color_widgets::ColorPalette palette2;
    int columns = 12;
    palette1.setName("Palette 1");
    palette2.setName("Palette 2");
    palette1.setColumns(columns);
    palette2.setColumns(columns);
    for ( int i = 0; i < 8; i++ )
    {
        for ( int j = 0; j < columns; j++ )
        {
            float f = float(j)/columns;
            palette1.appendColor(QColor::fromHsvF(i/8.0,1-f,0.5+f/2));
            palette2.appendColor(QColor::fromHsvF(i/8.0,1-f,1-f));
        }
    }
    color_widgets::ColorPaletteModel *model = new color_widgets::ColorPaletteModel;
    model->setParent(ui->pwColors);
    model->addPalette(palette1, false);
    model->addPalette(palette2, false);

    ui->pwColors->setModel(model);
}
