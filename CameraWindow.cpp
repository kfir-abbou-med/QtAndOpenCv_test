#include "CameraWindow.h"
#include <QVBoxLayout>
#include <QImage>
#include <QPixmap>
#include <opencv2/opencv.hpp>

CameraWindow::CameraWindow(QWidget *parent) : QWidget(parent), capture(0), isRunning(false)
{
    // Set up the window layout
    setWindowTitle("Webcam Feed");
    setFixedSize(640, 480);

    // Create a label to display the webcam feed
    label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);

    // Create start and stop buttons
    startButton = new QPushButton("Start", this);
    stopButton = new QPushButton("Stop", this);
    stopButton->setEnabled(false);

    // Create layout
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(label);
    layout->addWidget(startButton);
    layout->addWidget(stopButton);
    setLayout(layout);

    // Connect buttons to respective slots
    connect(startButton, &QPushButton::clicked, this, &CameraWindow::startFeed);
    connect(stopButton, &QPushButton::clicked, this, &CameraWindow::stopFeed);

    // Set up a timer to capture frames from the camera
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CameraWindow::updateFrame);
}

CameraWindow::~CameraWindow() {
    if (capture.isOpened()) {
        capture.release();  // Release the camera when the window is destroyed
    }
}

void CameraWindow::startFeed()
{
    if (!capture.isOpened()) {
        capture.open(0);  // Open the default camera
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

        // Convert the OpenCV image (cv::Mat) to a Qt image (QImage)
        cv::Mat rgbFrame;
        cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);
        QImage img(reinterpret_cast<const uchar*>(rgbFrame.data), rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);

        // Display the image in the label
        label->setPixmap(QPixmap::fromImage(img));
    }
}
