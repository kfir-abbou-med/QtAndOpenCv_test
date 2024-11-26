#include "CameraWorker.h"
#include <iostream>

using namespace std;

CameraWorker::CameraWorker(int cameraIndex, QObject *parent)
    : QObject(parent), cameraIndex(cameraIndex), isRunning(false), brightnessFactor(1.0), zoomFactor(1.0) {}

CameraWorker::~CameraWorker() {
    stop();
}

void CameraWorker::start() {
    if (isRunning) return;
    cout << "Trying to capture video from index: " << cameraIndex << endl;
    capture.open(cameraIndex, cv::CAP_V4L2);
    if (!capture.isOpened()) {
        emit errorOccurred(QString("Failed to open camera %1").arg(cameraIndex));
        return;
    }
    cout << "Capture is open for index: " << cameraIndex << endl;
    isRunning = true;

    while (isRunning) {
        cv::Mat frame;
        if (!capture.read(frame) || frame.empty()) {
            emit errorOccurred(QString("Failed to read frame from camera %1").arg(cameraIndex));
            continue;
        }
        
        // Apply brightness adjustment
        frame.convertTo(frame, -1, brightnessFactor, 0);
        
        // Apply zoom by cropping
        int centerX = frame.cols / 2;
        int centerY = frame.rows / 2;
        int width = frame.cols / zoomFactor;
        int height = frame.rows / zoomFactor;
        cv::Rect zoomRect(centerX - width / 2, centerY - height / 2, width, height);
        cv::Mat zoomedFrame = frame(zoomRect);

        // Resize to the original size to fit the window
        cv::Mat resizedFrame;
        cv::resize(zoomedFrame, resizedFrame, cv::Size(frame.cols, frame.rows));

        // Convert OpenCV frame to QImage
        cv::Mat rgbFrame;
        cv::cvtColor(resizedFrame, rgbFrame, cv::COLOR_BGR2RGB);
        QImage image(reinterpret_cast<const uchar *>(rgbFrame.data), rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);

        emit frameReady(image.copy());
    }

    capture.release();
}

void CameraWorker::stop() {
    isRunning = false;
}

void CameraWorker::changeBrightness(double factor) {
    brightnessFactor = factor; // Update the brightness factor
    cout << "BRIGHTNESS -> factor: " << brightnessFactor << endl;
}

void CameraWorker::changeZoom(double factor) {
    zoomFactor = factor; // Update the zoom factor
    cout << "ZOOM -> factor: " << zoomFactor << endl;
}