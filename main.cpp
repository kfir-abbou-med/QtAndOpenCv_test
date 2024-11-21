#include <QApplication>
#include "CameraWindow.h"  // Include the header file for CameraWindow

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CameraWindow window;  // Create the CameraWindow object
    window.show();        // Show the window

    return app.exec();    // Start the Qt event loop
}
