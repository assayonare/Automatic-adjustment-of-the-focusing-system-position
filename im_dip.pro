QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Histogram/add_lines.cpp \
    Histogram/add_lines_2.cpp \
    Histogram/histogram.cpp \
    Histogram/histogramitem.cpp \
    Histogram/plot.cpp \
    MotionSystem/motionsystem.cpp \
    Tiff/service_functions.cpp \
    main.cpp \
    imageproc.cpp \
    mainwindow.cpp

HEADERS += \
    Histogram/add_lines.h \
    Histogram/add_lines_2.h \
    Histogram/histogram.h \
    Histogram/histogramitem.h \
    Histogram/plot.h \
    MotionSystem/motionsystem.h \
    MotionSystem/motionsystemstructures.h \
    Tiff/definitions.h \
    Tiff/service_functions.h \
    Tiff/tiff.h \
    Tiff/tiffconf.h \
    Tiff/tiffio2.h \
    Tiff/tiffvers.h \
    imageproc.h \
    mainwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../opencv/build/x64/vc16/lib/ -lopencv_world490
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../opencv/build/x64/vc16/lib/ -lopencv_world490d

INCLUDEPATH += $$PWD/../../../opencv/build/include
DEPENDPATH += $$PWD/../../../opencv/build/include

FORMS += \
    mainwindow.ui

DISTFILES +=
