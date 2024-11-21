#ifndef CAMERA_WORKER_H
#define CAMERA_WORKER_H

#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>

class CameraWorker : public QObject
{
    Q_OBJECT

public:
    explicit CameraWorker(int cameraIndex, QObject *parent = nullptr);
    ~CameraWorker();

signals:
    void frameReady(const QImage &frame);
    void errorOccurred(const QString &error);

public slots:
    void start();
    void stop();

private:
    int cameraIndex;
    cv::VideoCapture capture;
    bool isRunning;
};

#endif // CAMERA_WORKER_H
