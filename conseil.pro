QT += core gui widgets sql charts serialport network

# Utiliser C++17 pour compatibilité moderne
CONFIG += c++17

# Dossiers d'includes personnalisés (facultatif)
INCLUDEPATH += $$PWD

# Fichiers sources .cpp
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    arduino.cpp \
    connection.cpp \
    conseil.cpp

# Fichiers d'en-têtes .h
HEADERS += \
    mainwindow.h \
    arduino.h \
    connection.h \
    conseil.h

# Interfaces graphiques .ui
FORMS += \
    mainwindow.ui

# Déploiement (optionnel)
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
