#ifndef CAMERA_WINDOW_H
#define CAMERA_WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <opencv2/opencv.hpp>

class CameraWindow : public QWidget
{
    Q_OBJECT // This macro is necessary for Qt's meta-object system

public:
    explicit CameraWindow(QWidget *parent = nullptr);
    ~CameraWindow();

private slots:
    void startFeed();
    void stopFeed();
    void updateFrame();

private:
    cv::VideoCapture capture;
    bool isRunning;
    QTimer *timer;
    QLabel *label;
    QPushButton *startButton;
    QPushButton *stopButton;
};

#endif // CAMERA_WINDOW_H
