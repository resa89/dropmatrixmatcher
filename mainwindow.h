#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class QPixmap;
class QLabel;
class QPushButton;
class QLayout;

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void displayImageInImageLabel(cv::Mat image);

private slots:
    void on_loadPattern_clicked();
    void on_loadImage_clicked();
    void enableFindButtons();

    void on_findButton0_clicked();
    void on_findButton1_clicked();
    void on_findButton2_clicked();
    void on_findButton3_clicked();
    void on_findButton4_clicked();
    void on_findButton5_clicked();


private:
    Ui::MainWindow *ui;
    QImage *pattern;
    QImage *image;
    QString patternPath;
    QString imagePath;
};

#endif // MAINWINDOW_H
