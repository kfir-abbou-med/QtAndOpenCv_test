#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}



// #include <QApplication>
// #include <QWidget>
// #include <QVBoxLayout>
// #include <QPushButton>
// #include <QMessageBox>
// #include <opencv2/opencv.hpp>
// #include "CameraWindow.h"

// // A simple QWidget to manage and launch CameraWindow instances
// class MainWindow : public QWidget
// {
//     Q_OBJECT

// public:
//     MainWindow(QWidget *parent = nullptr) : QWidget(parent)
//     {
//         setWindowTitle("Camera Manager");
//         setFixedSize(300, 200);

//         // Create "Detect Cameras" button
//         QPushButton *detectButton = new QPushButton("Detect Cameras", this);
//         detectButton->setFixedSize(200, 50);

//         QVBoxLayout *layout = new QVBoxLayout(this);
//         layout->addWidget(detectButton, 0, Qt::AlignCenter);

//         connect(detectButton, &QPushButton::clicked, this, &MainWindow::detectCameras);
//     }

// private slots:
//     void detectCameras()
//     {
//         // Search for available cameras in the range 0 to 6
//         std::vector<int> availableCameras;
//         for (int i = 0; i <= 6; ++i) {
//             cv::VideoCapture testCapture(i);
//             if (testCapture.isOpened()) {
//                 availableCameras.push_back(i);
//                 testCapture.release();
//             }
//         }

//         if (availableCameras.empty()) {
//             QMessageBox::warning(this, "No Cameras Found", "No available cameras were detected!");
//             return;
//         }

//         // Launch a new CameraWindow for each available camera
//         for (int cameraIndex : availableCameras) {
//             CameraWindow *cameraWindow = new CameraWindow(cameraIndex);
//             cameraWindow->show();
//         }
//     }
// };

// int main(int argc, char *argv[])
// {
//     QApplication app(argc, argv);

//     MainWindow mainWindow;
//     mainWindow.show();

//     return app.exec();
// }
