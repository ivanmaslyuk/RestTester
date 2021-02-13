QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Delegates/requestlistdelegate.cpp \
    Models/apimodel.cpp \
    Models/nodemodel.cpp \
    Models/parammodel.cpp \
    Models/requestmodel.cpp \
    TreeModel/treeitem.cpp \
    TreeModel/treemodel.cpp \
    Utils/markdownhtmlconverter.cpp \
    Widgets/documentationwidget.cpp \
    Widgets/endpointdetailwidget.cpp \
    Widgets/paramstable.cpp \
    Widgets/requestdatainputwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    serverapi.cpp

HEADERS += \
    Delegates/requestlistdelegate.h \
    Models/apimodel.h \
    Models/nodemodel.h \
    Models/parammodel.h \
    Models/requestmodel.h \
    TreeModel/treeitem.h \
    TreeModel/treemodel.h \
    Utils/markdownhtmlconverter.h \
    Widgets/documentationwidget.h \
    Widgets/endpointdetailwidget.h \
    Widgets/paramstable.h \
    Widgets/requestdatainputwidget.h \
    mainwindow.h \
    serverapi.h

FORMS += \
    Widgets/documentationwidget.ui \
    Widgets/endpointdetailwidget.ui \
    Widgets/paramstable.ui \
    Widgets/requestdatainputwidget.ui \
    mainwindow.ui

TRANSLATIONS += \
    RestTester_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    resources.qrc
