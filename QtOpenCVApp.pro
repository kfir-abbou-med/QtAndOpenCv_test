<<<<<<< HEAD
QT += core gui widgets

CONFIG += c++11

# Remove architecture-specific paths
INCLUDEPATH += \
    /usr/include/opencv4 \
    /usr/include/x86_64-linux-gnu/qt5 \
    /usr/include/x86_64-linux-gnu/qt5/QtWidgets \
    /usr/include/x86_64-linux-gnu/qt5/QtGui \
    /usr/include/x86_64-linux-gnu/qt5/QtCore \
    /usr/include/c++/11 \
    /usr/include/x86_64-linux-gnu/c++/11 \
    /usr/include/c++/11/backward

HEADERS += CameraWindow.h \
           CameraWorker.h \
           MainWindow.h

SOURCES += MainWindow.cpp \
           CameraWindow.cpp \
           CameraWorker.cpp \
           main.cpp
           

LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lv4l2 

DEFINES += HAVE_VIDEOINPUT
=======
QT += core gui widgets

CONFIG += c++11
HEADERS += CameraWindow.h
SOURCES += main.cpp \
  CameraWindow.cpp  

INCLUDEPATH += /usr/include/opencv4
LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio
>>>>>>> main
