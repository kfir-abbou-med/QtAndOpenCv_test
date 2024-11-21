#include "CameraWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QPixmap>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

CameraWindow::CameraWindow(int cameraIndex, QWidget *parent)
    : QWidget(parent), capture(cameraIndex), isRunning(false), brightnessFactor(1.0), zoomFactor(1.0), cameraIndex(cameraIndex)
{
    setWindowTitle("Webcam Feed - Camera " + QString::number(cameraIndex));
    setFixedSize(640, 520);  // Adjust height for additional controls

    // Create a label to display the webcam feed
    label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);

    // Create start and stop buttons
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

    // Create layout for buttons and sliders
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(label);
    layout->addWidget(startButton);
    layout->addWidget(stopButton);

    QHBoxLayout *controlsLayout = new QHBoxLayout();
    controlsLayout->addWidget(new QLabel("Brightness"));
    controlsLayout->addWidget(brightnessSlider);
    layout->addLayout(controlsLayout);

    QHBoxLayout *zoomLayout = new QHBoxLayout();
    zoomLayout->addWidget(new QLabel("Zoom"));
    zoomLayout->addWidget(zoomSlider);
    layout->addLayout(zoomLayout);

    setLayout(layout);

    // Connect buttons and sliders to respective slots
    connect(startButton, &QPushButton::clicked, this, &CameraWindow::startFeed);
    connect(stopButton, &QPushButton::clicked, this, &CameraWindow::stopFeed);
    connect(brightnessSlider, &QSlider::valueChanged, this, &CameraWindow::changeBrightness);
    connect(zoomSlider, &QSlider::valueChanged, this, &CameraWindow::changeZoom);

    // Set up a timer to capture frames from the camera
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CameraWindow::updateFrame);
}

CameraWindow::~CameraWindow()
{
    if (capture.isOpened()) {
        capture.release();
    }
}

void CameraWindow::startFeed()
{
    if (!capture.isOpened()) {
        capture.open(cameraIndex);  // Open the specified camera
    }

    if (capture.isOpened()) {
        isRunning = true;
        timer->start(33);  // Update the frame every 33 ms (30 FPS)
        startButton->setEnabled(false);
        stopButton->setEnabled(true);
    }
}

void CameraWindow::stopFeed()
{
    if (capture.isOpened()) {
        isRunning = false;
        capture.release();
        timer->stop();
        startButton->setEnabled(true);
        stopButton->setEnabled(false);
    }
}

void CameraWindow::updateFrame()
{
    if (isRunning) {
        cv::Mat frame;
        capture.read(frame);  // Capture a frame from the webcam

        if (frame.empty()) {
            return;
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

        // Convert the OpenCV image (cv::Mat) to a Qt image (QImage)
        cv::Mat rgbFrame;
        cv::cvtColor(resizedFrame, rgbFrame, cv::COLOR_BGR2RGB);
        QImage img(reinterpret_cast<const uchar*>(rgbFrame.data), rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);

        // Display the image in the label
        label->setPixmap(QPixmap::fromImage(img));
    }
}

void CameraWindow::changeBrightness(int value)
{
    brightnessFactor = value / 100.0;  // Map the slider value (0-200) to a factor (0.0-2.0)
}

void CameraWindow::changeZoom(int value)
{
    zoomFactor = value / 100.0;  // Map the slider value (50-150) to a zoom factor (0.5-1.5)
    cout << "val: "<< value << "factor: " << zoomFactor << endl;
}
