TEMPLATE = lib
TARGET = opmapwidget
DEFINES += OPMAPWIDGET_LIBRARY

include (../core/core.pri)
include (../internals/internals.pri)


#目标生成路径
DESTDIR = ../../../build

SOURCES += $$PWD/mapgraphicitem.cpp \
    $$PWD/opmapwidget.cpp \
    $$PWD/configuration.cpp \
    $$PWD/waypointitem.cpp \
    $$PWD/uavitem.cpp \
    $$PWD/gpsitem.cpp \
    $$PWD/trailitem.cpp \
    $$PWD/homeitem.cpp \
    $$PWD/mapripform.cpp \
    $$PWD/mapripper.cpp \
    $$PWD/traillineitem.cpp \
    $$PWD/waypointline.cpp \
    $$PWD/waypointcircle.cpp

LIBS += -L../../../build \
    -lcore \
    -linternals

POST_TARGETDEPS  += ../../../build/libcore.a
POST_TARGETDEPS  += ../../../build/libinternals.a

HEADERS += $$PWD/mapgraphicitem.h \
    $$PWD/opmapwidget.h \
    $$PWD/configuration.h \
    $$PWD/waypointitem.h \
    $$PWD/uavitem.h \
    $$PWD/gpsitem.h \
    $$PWD/uavmapfollowtype.h \
    $$PWD/uavtrailtype.h \
    $$PWD/trailitem.h \
    $$PWD/homeitem.h \
    $$PWD/mapripform.h \
    $$PWD/mapripper.h \
    $$PWD/traillineitem.h \
    $$PWD/waypointline.h \
    $$PWD/waypointcircle.h
QT += opengl
QT += network
QT += sql
QT += svg
RESOURCES += mapresources.qrc

FORMS += \
    mapripform.ui
