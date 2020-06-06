QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    hash.cpp \
    hashlibpp/src/hl_md5.cpp \
    hashlibpp/src/hl_md5wrapper.cpp \
    hashlibpp/src/hl_sha1.cpp \
    hashlibpp/src/hl_sha1wrapper.cpp \
    hashlibpp/src/hl_sha256.cpp \
    hashlibpp/src/hl_sha256wrapper.cpp \
    hashlibpp/src/hl_sha2ext.cpp \
    hashlibpp/src/hl_sha384wrapper.cpp \
    hashlibpp/src/hl_sha512wrapper.cpp \
    hashlibpp/src/hl_wrapperfactory.cpp \
    main.cpp \
    mainwindow.cpp \
    tools.cpp

HEADERS += \
    hash.hpp \
    hashlibpp/src/hashlibpp.h \
    hashlibpp/src/hl_exception.h \
    hashlibpp/src/hl_hashwrapper.h \
    hashlibpp/src/hl_md5.h \
    hashlibpp/src/hl_md5wrapper.h \
    hashlibpp/src/hl_sha1.h \
    hashlibpp/src/hl_sha1wrapper.h \
    hashlibpp/src/hl_sha256.h \
    hashlibpp/src/hl_sha256wrapper.h \
    hashlibpp/src/hl_sha2ext.h \
    hashlibpp/src/hl_sha2mac.h \
    hashlibpp/src/hl_sha384wrapper.h \
    hashlibpp/src/hl_sha512wrapper.h \
    hashlibpp/src/hl_types.h \
    hashlibpp/src/hl_wrapperfactory.h \
    mainwindow.h \
    tools.hpp

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
