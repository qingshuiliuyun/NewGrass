include (../common.pri)
HEADERS += \
    $$PWD/projections/lks94projection.h \
    $$PWD/projections/mercatorprojection.h \
    $$PWD/projections/mercatorprojectionyandex.h \
    $$PWD/projections/platecarreeprojection.h \
    $$PWD/projections/platecarreeprojectionpergo.h \
    $$PWD/copyrightstrings.h \
    $$PWD/core.h \
    $$PWD/debugheader.h \
    $$PWD/loadtask.h \
    $$PWD/mousewheelzoomtype.h \
    $$PWD/pointlatlng.h \
    $$PWD/pureprojection.h \
    $$PWD/rectangle.h \
    $$PWD/rectlatlng.h \
    $$PWD/sizelatlng.h \
    $$PWD/tile.h \
    $$PWD/tilematrix.h
SOURCES += \
    $$PWD/projections/lks94projection.cpp \
    $$PWD/projections/mercatorprojection.cpp \
    $$PWD/projections/mercatorprojectionyandex.cpp \
    $$PWD/projections/platecarreeprojection.cpp \
    $$PWD/projections/platecarreeprojectionpergo.cpp \
    $$PWD/core.cpp \
    $$PWD/loadtask.cpp \
    $$PWD/MouseWheelZoomType.cpp \
    $$PWD/pointlatlng.cpp \
    $$PWD/pureprojection.cpp \
    $$PWD/rectangle.cpp \
    $$PWD/rectlatlng.cpp \
    $$PWD/sizelatlng.cpp \
    $$PWD/tile.cpp \
    $$PWD/tilematrix.cpp

