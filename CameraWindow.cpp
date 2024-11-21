#include <QThread>
#include <QVBoxLayout>
#include <QImage>
#include <QPixmap>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "CameraWindow.h"

using namespace std;

CameraWindow::CameraWindow(int cameraIndex, QWidget *parent)
    : QWidget(parent), worker(nullptr), workerThread(nullptr), brightnessFactor(1.0), zoomFactor(1.0), cameraIndex(cameraIndex){
    setWindowTitle(QString("Camera %1").arg(cameraIndex));
    setFixedSize(640, 480);

    // UI setup
    label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);

    startButton = new QPushButton("Start", this);
    stopButton = new QPushButton("Stop", this);
    stopButton->setEnabled(false);

    // Create sliders for brightness and zoom
    brightnessSlider = new QSlider(Qt::Horizontal, this);
    brightnessSlider->setRange(0, 200);  // 0 to 200 for brightness factor adjustment
    brightnessSlider->setValue(100); // Default value (normal brightness)

    zoomSlider = new QSlider(Qt::Horizontal, this);
    zoomSlider->setRange(50, 150);  // 50% to 150% zoom
    zoomSlider->setValue(100); // Default value (no zoom)

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(startButton);
    layout->addWidget(stopButton);
    // Adding video setting buttons
    layout->addWidget(brightnessSlider);
    layout->addWidget(zoomSlider);
    setLayout(layout);

    // Worker and thread setup
    worker = new CameraWorker(cameraIndex);
    workerThread = new QThread(this);
    worker->moveToThread(workerThread);

    connect(startButton, &QPushButton::clicked, worker, &CameraWorker::start);
    connect(stopButton, &QPushButton::clicked, worker, &CameraWorker::stop);
    connect(worker, &CameraWorker::frameReady, this, &CameraWindow::updateFrame);
    connect(worker, &CameraWorker::errorOccurred, this, &CameraWindow::handleWorkerError);
    // connect(brightnessSlider, &QSlider::valueChanged, this, &CameraWindow::changeBrightness);
    // connect(zoomSlider, &QSlider::valueChanged, this, &CameraWindow::changeZoom);
    // Connect slider changes to the worker's change methods
    connect(brightnessSlider, &QSlider::valueChanged, worker, &CameraWorker::changeBrightness);
    connect(zoomSlider, &QSlider::valueChanged, worker, &CameraWorker::changeZoom);


    connect(workerThread, &QThread::finished, worker, &CameraWorker::deleteLater);
    workerThread->start();
}

// void CameraWindow::updateFrame()
// {
//     if (isRunning) {
//         cv::Mat frame;
//         capture.read(frame);  // Capture a frame from the webcam

//         if (frame.empty()) {
//             return;
//         }

//         // Apply brightness adjustment
//         frame.convertTo(frame, -1, brightnessFactor, 0);

//         // Apply zoom by cropping
//         int centerX = frame.cols / 2;
//         int centerY = frame.rows / 2;
//         int width = frame.cols / zoomFactor;
//         int height = frame.rows / zoomFactor;
//         cv::Rect zoomRect(centerX - width / 2, centerY - height / 2, width, height);
//         cv::Mat zoomedFrame = frame(zoomRect);

//         // Resize to the original size to fit the window
//         cv::Mat resizedFrame;
//         cv::resize(zoomedFrame, resizedFrame, cv::Size(frame.cols, frame.rows));

//         // Convert the OpenCV image (cv::Mat) to a Qt image (QImage)
//         cv::Mat rgbFrame;
//         cv::cvtColor(resizedFrame, rgbFrame, cv::COLOR_BGR2RGB);
//         QImage img(reinterpret_cast<const uchar*>(rgbFrame.data), rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);

//         // Display the image in the label
//         label->setPixmap(QPixmap::fromImage(img));
//     }
// }

void CameraWindow::updateFrame(const QImage &image) {
    // Update the label with the new frame
    label->setPixmap(QPixmap::fromImage(image));
}

void CameraWindow::changeBrightness(int value)
{
    brightnessFactor = value / 100.0;  // Map the slider value (0-200) to a factor (0.0-2.0)
    cout << "BRIGHTNESS -> val: "<< value << ", factor: " << zoomFactor << endl;
}

void CameraWindow::changeZoom(int value)
{
    zoomFactor = value / 100.0;  // Map the slider value (50-150) to a zoom factor (0.5-1.5)
    cout << "ZOOM -> val: "<< value << ", factor: " << zoomFactor << endl;
}

void CameraWindow::handleWorkerError(const QString &error) {
    label->setText(error);

}
CameraWindow::~CameraWindow()
{
    // Stop the worker thread and clean up
    workerThread->quit();
    workerThread->wait();
    delete worker;  // Clean up the worker object
    delete workerThread;  // Clean up the worker thread
}
