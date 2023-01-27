QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 app_bundle
CONFIG += sdk_no_version_check
//CONFIG += debug_and_release debug_and_release_target

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cmd/cmd_get_information.cpp \
    cmd/cmd_read.cpp \
    cmd/cmd_subgraph.cpp \
    cmd/color.cpp \
    global.cpp \
    gui/GraphicsViewer.cpp \
    gui/gui_arrow.cpp \
    gui/gui_cnode.cpp \
    gui/gui_cscene.cpp \
    gui/gui_node.cpp \
    gui/gui_rnode.cpp \
    gui/mainwindow.cpp \
    main.cpp \

HEADERS += \
    cmd/cmd_Course.h \
    cmd/cmd_LogicP.h \
    cmd/cmd_Point.h \
    global.h \
    gui/GraphicsViewer.h \
    gui/gui_arrow.h \
    gui/gui_cnode.h \
    gui/gui_cscene.h \
    gui/gui_node.h \
    gui/gui_rnode.h \
    gui/mainwindow.h \

FORMS += \
    gui/mainwindow.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    res/style.css
