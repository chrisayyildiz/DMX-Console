QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    libola_ola.cpp \
    lightbinding.cpp \
    main.cpp \
    mainwindow.cpp \
    xmlparser.cpp

HEADERS += \
    libola_ola.h \
    lightbinding.h \
    mainwindow.h \
    xmlparser.h

FORMS += \
    mainwindow.ui

LIBS += -L/usr/local/lib -lolaserver -lola -lolacommon

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
