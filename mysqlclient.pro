CONFIG += qt c++11
SOURCES += \
    main.cpp \
    mysqlclient.cpp \
    connectiondialog.cpp \
    editprojects.cpp \
    editparts.cpp

QT += widgets gui sql

QMAKE_CXXFLAGS += -std=c++11

HEADERS += \
    mysqlclient.h \
    connectiondialog.h \
    editprojects.h \
    editparts.h

FORMS += \
    mysqlclient.ui \
    connectiondialog.ui \
    editprojects.ui \
    editparts.ui

RESOURCES += \
    img.qrc
