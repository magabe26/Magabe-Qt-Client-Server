INCLUDEPATH += $$PWD
QT += qml quick  widgets #used by Qawesame
include(../SharedLibrary/Extensions/QtAwesome/QtAwesome.pri)


HEADERS +=    \
    $$PWD/Helpers/extras.h \
    $$PWD/Helpers/magabedata.h \
    $$PWD/Helpers/magabetime.h \
    $$PWD/Helpers/mydevice.h \
    $$PWD/Helpers/simplecrypt.h \
    $$PWD/Db/createtablestatement.h \
    $$PWD/Db/insertstatement.h \
    $$PWD/Helpers/Icons/icons.h \
    $$PWD/Helpers/Icons/magabeiconprovider.h \
    $$PWD/Helpers/magabesingleton.h \
    $$PWD/Helpers/constants.h \
    $$PWD/Db/magabedb.h \
    $$PWD/Models/model.h \
    $$PWD/Models/statistticsmodel.h \
    $$PWD/Helpers/mfile.h \
    $$PWD/debug.h \
    $$PWD/Helpers/connectionpipe.h \
    $$PWD/Helpers/datafilter.h \
    $$PWD/Helpers/fileutils.h \
    $$PWD/Helpers/conversation.h \
    $$PWD/Helpers/messages.h \
    $$PWD/Helpers/mopaque.h \
    $$PWD/Helpers/mqmlcallback.h \
    $$PWD/Helpers/errors.h \
    $$PWD/Helpers/sort.h \
    $$PWD/Helpers/mcppcallback.h \
    $$PWD/Helpers/messagetype.h

SOURCES += \
    $$PWD/Helpers/extras.cpp \
    $$PWD/Helpers/magabedata.cpp \
    $$PWD/Helpers/magabetime.cpp \
    $$PWD/Helpers/mydevice.cpp \
    $$PWD/Helpers/simplecrypt.cpp \
    $$PWD/Db/createtablestatement.cpp \
    $$PWD/Db/insertstatement.cpp \
    $$PWD/Helpers/Icons/magabeiconprovider.cpp \
    $$PWD/Db/magabedb.cpp \
    $$PWD/Models/model.cpp \
    $$PWD/Models/statistticsmodel.cpp \
    $$PWD/Helpers/mfile.cpp \
    $$PWD/Helpers/connectionpipe.cpp \
    $$PWD/Helpers/datafilter.cpp \
    $$PWD/Helpers/fileutils.cpp \
    $$PWD/Helpers/conversation.cpp \
    $$PWD/Helpers/messages.cpp \
    $$PWD/Helpers/mqmlcallback.cpp \
    $$PWD/Helpers/errors.cpp \
    $$PWD/Helpers/sort.cpp

win32:HEADERS +=  \
  
win32:SOURCES += \

RESOURCES += $$PWD/Qml/sharedqml.qrc \
