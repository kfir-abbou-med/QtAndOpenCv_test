#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <vector>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void detectCameras();
};

#endif // MAINWINDOW_H
