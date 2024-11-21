QT += core gui widgets

CONFIG += c++11
HEADERS += CameraWindow.h
SOURCES += main.cpp \
  CameraWindow.cpp  

INCLUDEPATH += /usr/include/opencv4
LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio
