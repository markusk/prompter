QT += widgets uitools

FORMS       = prompter.ui

HEADERS     = prompter.h

RESOURCES   = prompter.qrc

SOURCES     = prompter.cpp \
              main.cpp

INSTALLS += target

RC_ICONS += prompter.png

DISTFILES += \
    Info.plist
