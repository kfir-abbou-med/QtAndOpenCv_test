#include "MainWindow.h"
#include "CameraWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Camera Manager");
    setFixedSize(300, 200);

    QPushButton *detectButton = new QPushButton("Detect Cameras", this);
    detectButton->setFixedSize(200, 50);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(detectButton, 0, Qt::AlignCenter);

    connect(detectButton, &QPushButton::clicked, this, &MainWindow::detectCameras);
}

void MainWindow::detectCameras()
{
    std::vector<int> availableCameras;
    for (int i = 0; i <= 6; ++i) {
        cv::VideoCapture testCapture(i);
        if (testCapture.isOpened()) {
            availableCameras.push_back(i);
            testCapture.release();
        }
    }

    if (availableCameras.empty()) {
        QMessageBox::warning(this, "No Cameras Found", "No available cameras were detected!");
        return;
    }

    for (int cameraIndex : availableCameras) {
        CameraWindow *cameraWindow = new CameraWindow(cameraIndex);
        cameraWindow->show();
    }
}
