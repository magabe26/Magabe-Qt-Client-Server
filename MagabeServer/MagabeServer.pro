
QT += qml quick network sql core xml concurrent

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

android: {

QT += androidextras

}

#COMMEN TO BUILD RELEASE PRODUCTION
DEFINES += MAGABE_TESTS

CONFIG += c++11

TARGET = MagabeServer

TEMPLATE = app
include(../SharedLibrary/Shared.pri)

SOURCES += main.cpp \
    Server/MagabeServer.cpp \
    Server/sslserver.cpp \
    Server/connectedusers.cpp \
    ControllerIdentifier/controlleridentifier.cpp \
    Controllers/controller.cpp \
    Controllers/usercontroller.cpp \
    Models/usermodel.cpp \
    Helpers/datawritter.cpp \
    Helpers/user.cpp \
    Models/serverfilesmodel.cpp \
    magabeserverapp.cpp \
    magabeservermanager.cpp \
    magabeserverqml.cpp \
    Helpers/connectedclientsmodel.cpp \
    Models/messagesmodel.cpp \
    Controllers/conversationcontroller.cpp \
    AndroidJNI/activityjniutils.cpp \
    AndroidJNI/nativeregister.cpp \

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS MGB_SERVER

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    Server/MagabeServer.h \
    Server/sslserver.h \
    security.h \
    Server/connectedusers.h \
    ControllerIdentifier/controlleridentifier.h \
    Controllers/controller.h \
    Controllers/usercontroller.h \
    Models/usermodel.h \
    Helpers/datawritter.h \
    Helpers/user.h \
    Models/serverfilesmodel.h \
    magabeserverapp.h \
    magabeservermanager.h \
    magabeserverqml.h \
    registeredobjects.h \
    Helpers/connectedclientsmodel.h \
    Models/messagesmodel.h \
    Controllers/conversationcontroller.h \
    AndroidJNI/activityjniobject.h \
    AndroidJNI/activityjniutils.h \
    AndroidJNI/nativeregister.h \
    AndroidJNI/pointerutil.h \

RESOURCES += \
    Server/Resources/certificatesandkey.qrc

android:DISTFILES += \
    android/src/com/magabelab/magabeserver/MagabeServerActivity.java \
    android/src/com/magabelab/magabeserver/Size.java \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/build.gradle \
    android/gradle.properties \
    android/local.properties

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += Qml/qml.qrc \
