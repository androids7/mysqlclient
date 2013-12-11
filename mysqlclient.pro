CONFIG += qt c++11
SOURCES += \
    main.cpp \
    mysqlclient.cpp \
    connectiondialog.cpp \
    editprojects.cpp \
    editparts.cpp \
    editproviders.cpp \
    pixmaploaddelegate.cpp

QT += widgets gui sql

QMAKE_CXXFLAGS += -std=c++11

HEADERS += \
    mysqlclient.h \
    connectiondialog.h \
    editprojects.h \
    editparts.h \
    editproviders.h \
    pixmaploaddelegate.h

FORMS += \
    mysqlclient.ui \
    connectiondialog.ui \
    editprojects.ui \
    editparts.ui \
    editproviders.ui

RESOURCES += \
    img.qrc
