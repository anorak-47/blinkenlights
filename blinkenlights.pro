#-------------------------------------------------
#
# Project created by QtCreator 2017-03-27T14:27:21
#
#-------------------------------------------------

QT += core gui
QT += widgets
QT += serialport
qtHaveModule(opengl): QT += opengl

TARGET = blinkenlights
TEMPLATE = app

include(/home/wenk/git/Qt-Color-Widgets/color_widgets.pri)

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/src/gui
INCLUDEPATH += $$PWD/src/commands
INCLUDEPATH += $$PWD/src/keyboards/91tkl
INCLUDEPATH += $$PWD/src/keyboards/91tkl/animations
INCLUDEPATH += /home/wenk/git/Qt-Color-Widgets/include/QtColorWidgets

SOURCES += src/main.cpp\
    src/utils.cpp \
    src/colormap.cpp \
    src/ledbrightness.cpp \
    src/serial_settings_dialog.cpp \
    src/keyboard_command.cpp \
    src/keyboard_command_queue.cpp \
    src/keyboard_command_consumer.cpp \
    src/keyboard_factory.cpp \
    src/keyboard_interface.cpp \
    src/keyboard_protocol_interface.cpp \
    src/keyboard_properties.cpp \
    src/threaded_serial_port.cpp \
    src/keyboardselectordialog.cpp \
    src/animator.cpp \
    src/animationcontrol.cpp \
    src/klekey.cpp \
    src/gui/keycapcoloreditor.cpp \
    src/gui/keygraphicsitem.cpp \
    src/gui/mainwindow.cpp \
    src/gui/sectoreditor.cpp \
    src/gui/sectoreditwidget.cpp \
    src/keyboards/91tkl/keyboard_interface_91tkl.cpp \
    src/keyboards/91tkl/keyboard_settings_91tkl.cpp \
    src/keyboards/91tkl/keyboard_properties_91tkl.cpp \
    src/keyboards/91tkl/keyboard_console_91tkl.cpp \
    src/keyboards/91tkl/threaded_serial_port_91tkl.cpp \
    src/keyboards/91tkl/animator_91tkl.cpp \
    src/keyboards/91tkl/animations/animation.cpp \
    src/keyboards/91tkl/animations/animation_utils.cpp \
    src/keyboards/91tkl/animations/breathing.cpp \
    src/keyboards/91tkl/animations/color.cpp \
    src/keyboards/91tkl/animations/color_cycle_all.cpp \
    src/keyboards/91tkl/animations/color_cycle_left_right.cpp \
    src/keyboards/91tkl/animations/color_cycle_up_down.cpp \
    src/keyboards/91tkl/animations/color_wave.cpp \
    src/keyboards/91tkl/animations/compat.cpp \
    src/keyboards/91tkl/animations/conway.cpp \
    src/keyboards/91tkl/animations/floating_plasma.cpp \
    src/keyboards/91tkl/animations/flying_ball.cpp \
    src/keyboards/91tkl/animations/gradient_full_flicker.cpp \
    src/keyboards/91tkl/animations/gradient_left_right.cpp \
    src/keyboards/91tkl/animations/gradient_up_down.cpp \
    src/keyboards/91tkl/animations/jellybean_raindrops.cpp \
    src/keyboards/91tkl/animations/key_led_map.cpp \
    src/keyboards/91tkl/animations/raindrops.cpp \
    src/keyboards/91tkl/animations/sweep.cpp \
    src/keyboards/91tkl/animations/type_o_circles.cpp \
    src/keyboards/91tkl/animations/type_o_matic.cpp \
    src/keyboards/91tkl/animations/type_o_raindrops.cpp \
    src/keyboards/jellowcake/keyboard_interface_jellowcake.cpp \
    src/keyboards/jellowcake/keyboard_settings_jellowcake.cpp \
    src/keyboards/jellowcake/keyboard_properties_jellowcake.cpp \
    src/keyboards/jellowcake/keyboard_console_jellowcake.cpp \
    src/keyboards/jellowcake/threaded_serial_port_jellowcake.cpp \    
    src/commands/animationchangecommand.cpp \
    src/commands/animationcolorcommand.cpp \
    src/commands/animationnamelistcommand.cpp \
    src/commands/animationruncommand.cpp \
    src/commands/animationstatecommand.cpp \
    src/commands/bootloaderstartcommand.cpp \
    src/commands/debugconfiggetcommand.cpp \
    src/commands/debugconfigsavecommand.cpp \
    src/commands/debugconfigsetcommand.cpp \
    src/commands/keyboardidcommand.cpp \
    src/commands/keymapconfiggetcommand.cpp \
    src/commands/keymapconfigsavecommand.cpp \
    src/commands/keymapconfigsetcommand.cpp \
    src/commands/ledbrightnesscommand.cpp \
    src/commands/ledchangecommand.cpp \
    src/commands/ledgetpwmcommand.cpp \
    src/commands/ledilluminatecommand.cpp \
    src/commands/ledsavecommand.cpp \
    src/commands/sectorchangecommand.cpp \
    src/commands/sectorcolorcommand.cpp \
    src/commands/sectorcolorstatecommand.cpp \
    src/commands/sectorilluminatecommand.cpp \
    src/commands/sectorilluminationstatecommand.cpp \
    src/commands/sectormapcommand.cpp \
    src/commands/sectormapgetcommand.cpp \
    src/commands/sectormapsetcommand.cpp \
    src/commands/sectorsavecommand.cpp \
    src/commands/sectormapsavecommand.cpp \
    src/commands/sectorstatecommand.cpp \
    src/commands/sleepledcommand.cpp \
    src/commands/blgetcommand.cpp \
    src/commands/blsetcommand.cpp \
    src/commands/blgetbrightnesscommand.cpp \
    src/commands/blsetbrightnesscommand.cpp \





HEADERS  += \
    src/common.h \
    src/utils.h \
    src/colormap.h \
    src/ledbrightness.h \
    src/serial_settings_dialog.h \
    src/keyboard_command.h \
    src/keyboard_command_queue.h \
    src/keyboard_command_consumer.h \
    src/keyboard_factory.h \
    src/keyboard_interface.h \
    src/keyboard_protocol_interface.h \
    src/keyboard_properties.h \
    src/threaded_serial_port.h \
    src/keyboardselectordialog.h \
    src/animator.h \
    src/animationcontrol.h \
    src/klekey.h \
    src/gui/keycapcoloreditor.h \
    src/gui/keygraphicsitem.h \
    src/gui/mainwindow.h \
    src/gui/sectoreditor.h \
    src/gui/sectoreditwidget.h \
    src/keyboards/91tkl/keyboard_interface_91tkl.h \
    src/keyboards/91tkl/keyboard_settings_91tkl.h \
    src/keyboards/91tkl/keyboard_properties_91tkl.h \
    src/keyboards/91tkl/keyboard_console_91tkl.h \
    src/keyboards/91tkl/threaded_serial_port_91tkl.h \
    src/keyboards/91tkl/animator_91tkl.h \
    src/keyboards/91tkl/animations/animation.h \
    src/keyboards/91tkl/animations/animation_utils.h \
    src/keyboards/91tkl/animations/breathing.h \
    src/keyboards/91tkl/animations/color.h \
    src/keyboards/91tkl/animations/color_cycle_all.h \
    src/keyboards/91tkl/animations/color_cycle_left_right.h \
    src/keyboards/91tkl/animations/color_cycle_up_down.h \
    src/keyboards/91tkl/animations/color_wave.h \
    src/keyboards/91tkl/animations/compat.h \
    src/keyboards/91tkl/animations/conway.h \
    src/keyboards/91tkl/animations/floating_plasma.h \
    src/keyboards/91tkl/animations/flying_ball.h \
    src/keyboards/91tkl/animations/gradient_full_flicker.h \
    src/keyboards/91tkl/animations/gradient_left_right.h \
    src/keyboards/91tkl/animations/gradient_up_down.h \
    src/keyboards/91tkl/animations/jellybean_raindrops.h \
    src/keyboards/91tkl/animations/key_led_map.h \
    src/keyboards/91tkl/animations/raindrops.h \
    src/keyboards/91tkl/animations/sweep.h \
    src/keyboards/91tkl/animations/type_o_circles.h \
    src/keyboards/91tkl/animations/type_o_matic.h \
    src/keyboards/91tkl/animations/type_o_raindrops.h \
    src/keyboards/jellowcake/keyboard_interface_jellowcake.h \
    src/keyboards/jellowcake/keyboard_settings_jellowcake.h \
    src/keyboards/jellowcake/keyboard_properties_jellowcake.h \
    src/keyboards/jellowcake/keyboard_console_jellowcake.h \
    src/keyboards/jellowcake/threaded_serial_port_jellowcake.h \    
    src/commands/animationchangecommand.h \
    src/commands/animationcolorcommand.h \
    src/commands/animationnamelistcommand.h \
    src/commands/animationruncommand.h \
    src/commands/animationstatecommand.h \
    src/commands/bootloaderstartcommand.h \
    src/commands/debugconfiggetcommand.h \
    src/commands/debugconfigsavecommand.h \
    src/commands/debugconfigsetcommand.h \
    src/commands/keyboardidcommand.h \
    src/commands/keymapconfiggetcommand.h \
    src/commands/keymapconfigsavecommand.h \
    src/commands/keymapconfigsetcommand.h \
    src/commands/ledbrightnesscommand.h \
    src/commands/ledchangecommand.h \
    src/commands/ledgetpwmcommand.h \
    src/commands/ledilluminatecommand.h \
    src/commands/ledsavecommand.h \
    src/commands/sectorchangecommand.h \
    src/commands/sectorcolorcommand.h \
    src/commands/sectorcolorstatecommand.h \
    src/commands/sectorilluminatecommand.h \
    src/commands/sectorilluminationstatecommand.h \
    src/commands/sectormapcommand.h \
    src/commands/sectormapsavecommand.h \
    src/commands/sectormapgetcommand.h \
    src/commands/sectormapsetcommand.h \
    src/commands/sectorsavecommand.h \
    src/commands/sectorstatecommand.h \
    src/commands/sleepledcommand.h \
    src/commands/blgetcommand.h \
    src/commands/blsetcommand.h \
    src/commands/blgetbrightnesscommand.h \
    src/commands/blsetbrightnesscommand.h \




FORMS    += form/keyboardselector.ui \
    form/ledbrightness.ui \
    form/serial_settings_dialog.ui \
    form/animationcontrol.ui \
    form/keycapcoloreditor.ui \
    form/mainwindow.ui \
    form/sectoreditor.ui \
    form/sectoreditwidget.ui \
    src/keyboards/91tkl/form/keyboardconsole91tkl.ui \
    src/keyboards/91tkl/form/keyboardsettings91tkl.ui \
    src/keyboards/jellowcake/form/keyboardconsole_jellowcake.ui \
    src/keyboards/jellowcake/form/keyboardsettings_jellowcake.ui
    

