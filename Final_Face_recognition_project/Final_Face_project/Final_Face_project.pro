TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        face_recognition.cpp \
        main.cpp \
        mysqlite.cpp \


HEADERS += \
    face_recognition.h \
    mysqlite.h \


#sqlite3库的配置
INCLUDEPATH += C:/tool/sqlite-dll-win-x86-3440000/
LIBS += -LC:/tool/sqlite-dll-win-x86-3440000/  -lsqlite3

#opencv库的配置
INCLUDEPATH += C:\opencv455-win32\include
LIBS += -LC:\opencv455-win32\x64\mingw\bin -lopencv_core455 -lopencv_imgcodecs455 -lopencv_imgproc455 -lopencv_highgui455 -lopencv_objdetect455



