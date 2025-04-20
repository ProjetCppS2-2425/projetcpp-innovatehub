QT += core gui widgets sql charts
QT += charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += network sql
QT += network

CONFIG += c++17

INCLUDEPATH += $$PWD
# INCLUDEPATH += /chemin/vers/mes/includes  ← Décommente cette ligne si tu as des includes personnalisés

SOURCES += \
    connection.cpp \
    conseil.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    connection.h \
    conseil.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Deployment rules
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Compilation flags
QMAKE_CXXFLAGS += -DDEFINE_NAME
