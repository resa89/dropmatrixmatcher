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
class QRect;

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_loadPattern_clicked();
    void on_loadImage_clicked();
    void enableFindButtons();
    void setBrightness(int value);
    void setContrast(int value);
    void sensitivity(int value);
    void filterImage();

    void on_findButton0_clicked();
    void on_findButton1_clicked();
    void on_findButton2_clicked();
    void on_findButton3_clicked();
    void on_findButton4_clicked();
    void on_findButton5_clicked();
    void on_findButton6_clicked();
    void on_findButton7_clicked();
    void on_findButton8_clicked();

    void on_LoadSelectedPattern_clicked();

private:
    Ui::MainWindow *ui;
    QImage *pattern;
    cv::Mat *greyPattern;
    QImage *image;
    QImage *image_2;
    cv::Mat *greyImage;
    cv::Mat *greyToScreen;
    QString patternPath;
    QString imagePath;
    QString imagePath_2;
    QRect selectRect;
    double greyPatternPixelSum;
    double greyPatternPixelSumPow;

    void displayImageInImageLabel(cv::Mat image);
    void matchingWithMethod(int method, float sensitivity);
    void createGreyImage(cv::Mat colorImage, int cmyk=4);
    void createGreyPattern(cv::Mat colorPattern, int cmyk=4);
    cv::Mat match(int method);
    float matchingAlgorithm( int x, int y, int method );
    float tmpFunction( int x, int y, int method );
    float imgFunction( int x, int y, double greyImagePixelSum, int method );
    void minMax(cv::Mat matResult, cv::Point* min, cv::Point* max, double* minVal, double* maxVal);
    void filter(int cmyk);

};

#endif // MAINWINDOW_H
