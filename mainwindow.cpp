#include <QtGui>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QMessageBox>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/// Global Variables
Mat img; Mat templ; Mat result; Mat img_display;

char* image_window = "Source Image";
char* result_window = "Result window";

int match_method;
int max_Trackbar = 5;

/// Function Headers
void MatchingMethod( int, void* );


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    image = new QImage;
    pattern = new QImage;

    ui->setupUi(this);
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    ui->imageLabel->setPixmap(QPixmap::fromImage(*image));
    ui->patternLabel->setPixmap(QPixmap::fromImage(*pattern));

    ui->findButton0->setDefault(true);
    ui->findButton0->setEnabled(false);
    ui->findButton1->setDefault(true);
    ui->findButton1->setEnabled(false);
    ui->findButton2->setDefault(true);
    ui->findButton2->setEnabled(false);
    ui->findButton3->setDefault(true);
    ui->findButton3->setEnabled(false);
    ui->findButton4->setDefault(true);
    ui->findButton4->setEnabled(false);
    ui->findButton5->setDefault(true);
    ui->findButton5->setEnabled(false);

    // Only if images are uploaded, find button is enabled
    connect(ui->loadImage, SIGNAL(clicked()),
            this, SLOT(enableFindButtons()));
    connect(ui->loadPattern, SIGNAL(clicked()),
            this, SLOT(enableFindButtons()));
}


void MainWindow::enableFindButtons()
{
    if (!image->isNull() && !pattern->isNull())
    {
        ui->findButton0->setEnabled(true);
        ui->findButton1->setEnabled(true);
        ui->findButton2->setEnabled(true);
        ui->findButton3->setEnabled(true);
        ui->findButton4->setEnabled(true);
        ui->findButton5->setEnabled(true);

    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadPattern_clicked()
{
    QString fileName;
    QFileDialog dialog;
    fileName = dialog.getOpenFileName(this,
        tr("Open Image"), "/Users/resa/Studium/WiSe2013/Thesis", tr("Image Files (*.png *.jpg *.bmp)"));
    int w = ui->patternLabel->width();
    int h = ui->patternLabel->height();

    pattern->load(fileName);
    ui->patternLabel->setPixmap(QPixmap::fromImage(*pattern).scaled(w,h,Qt::KeepAspectRatio));  //evtl weglassen, wenn standardmäßig image/pattern in label angezeigt wird

    patternPath = fileName;                          //ACHTUNG! Pointer auf Speicher

    /// Copy T-eil von Image
    //QImage image;
    //image.fromData(fileName);
    //QImage copy;
    //copy = image.copy( 0, 0, 128, 128);
    //copy.save("cropped_image.jpg");}
}

void MainWindow::on_loadImage_clicked()
{
    QString fileName;
    QFileDialog dialog;
    fileName = dialog.getOpenFileName(this,
        tr("Open Image"), "/Users/resa/Studium/WiSe2013/Thesis", tr("Image Files (*.png *.jpg *.bmp)"));
    int w = ui->imageLabel->width();
    int h = ui->imageLabel->height();

    image->load(fileName);
    ui->imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(w,h,Qt::KeepAspectRatio));      //s.oben
    imagePath = fileName;                                                    //ACHTUNG! Pointer auf Speicher
}

void MainWindow::displayImageInImageLabel(Mat mat)
{
    QPixmap pixmap;
    int w = ui->imageLabel->width();
    int h = ui->imageLabel->height();

    pixmap = QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888));

    ui->imageLabel->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio));
}

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
/*
 *void MatchingMethod( int, void* )
{

  //imshow( image_window, img_display );
  //imshow( result_window, result );

  return;
}
*/

void MainWindow::on_findButton0_clicked()
{
    /// Load image and template
    img = imread(this->imagePath.toStdString());
    templ = imread(this->patternPath.toStdString());

    /// Create windows
    namedWindow( image_window, CV_WINDOW_AUTOSIZE );
    namedWindow( result_window, CV_WINDOW_AUTOSIZE );

    /// Create Trackbar
    //char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
    //createTrackbar( trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod );

    match_method = 0;

    /// Source image to display
    img.copyTo( img_display );

    /// Create the result matrix
    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;

    result.create( result_cols, result_rows, CV_32FC1 );

    /// Do the Matching and Normalize
    matchTemplate( img, templ, result, match_method );
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

    int i = 0;
    for( i; i<10; i++ ){

        /// Localizing the best match with minMaxLoc
        double minVal; double maxVal; Point minLoc; Point maxLoc;
        Point matchLoc;

        minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

        /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
        if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
          { matchLoc = minLoc; }
        else
          { matchLoc = maxLoc; }

        /// Show me what you got
        rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );

        result.at<int>(maxLoc.y, maxLoc.x)=0;
        result.at<int>(maxLoc.y, maxLoc.x-1)=0;
    }

    this->displayImageInImageLabel(img_display);

    waitKey(0);
}

void MainWindow::on_findButton1_clicked()
{
    /// Load image and template
    img = imread(this->imagePath.toStdString());
    templ = imread(this->patternPath.toStdString());

    /// Create windows
    namedWindow( image_window, CV_WINDOW_AUTOSIZE );
    namedWindow( result_window, CV_WINDOW_AUTOSIZE );

    /// Create Trackbar
    //char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
    //createTrackbar( trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod );

    match_method = 1;

    /// Source image to display
    img.copyTo( img_display );

    /// Create the result matrix
    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;

    result.create( result_cols, result_rows, CV_32FC1 );

    /// Do the Matching and Normalize
    matchTemplate( img, templ, result, match_method );
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

    int i = 0;
    for( i; i<10; i++ ){

        /// Localizing the best match with minMaxLoc
        double minVal; double maxVal; Point minLoc; Point maxLoc;
        Point matchLoc;

        minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

        /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
        if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
          { matchLoc = minLoc; }
        else
          { matchLoc = maxLoc; }

        /// Show me what you got
        rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );

        result.at<int>(maxLoc.y, maxLoc.x)=0;
        result.at<int>(maxLoc.y, maxLoc.x-1)=0;
    }

    this->displayImageInImageLabel(img_display);

    waitKey(0);
}

void MainWindow::on_findButton2_clicked()
{
    /// Load image and template
    img = imread(this->imagePath.toStdString());
    templ = imread(this->patternPath.toStdString());

    /// Create windows
    namedWindow( image_window, CV_WINDOW_AUTOSIZE );
    namedWindow( result_window, CV_WINDOW_AUTOSIZE );

    /// Create Trackbar
    //char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
    //createTrackbar( trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod );

    match_method = 2;

    /// Source image to display
    img.copyTo( img_display );

    /// Create the result matrix
    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;

    result.create( result_cols, result_rows, CV_32FC1 );

    /// Do the Matching and Normalize
    matchTemplate( img, templ, result, match_method );
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

    int i = 0;
    for( i; i<10; i++ ){

        /// Localizing the best match with minMaxLoc
        double minVal; double maxVal; Point minLoc; Point maxLoc;
        Point matchLoc;

        minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

        /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
        if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
          { matchLoc = minLoc; }
        else
          { matchLoc = maxLoc; }

        /// Show me what you got
        rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );

        result.at<int>(maxLoc.y, maxLoc.x)=0;
        result.at<int>(maxLoc.y, maxLoc.x-1)=0;
    }

    this->displayImageInImageLabel(img_display);

    waitKey(0);
}

void MainWindow::on_findButton3_clicked()
{
    /// Load image and template
    img = imread(this->imagePath.toStdString());
    templ = imread(this->patternPath.toStdString());

    /// Create windows
    namedWindow( image_window, CV_WINDOW_AUTOSIZE );
    namedWindow( result_window, CV_WINDOW_AUTOSIZE );

    /// Create Trackbar
    //char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
    //createTrackbar( trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod );

    match_method = 3;

    /// Source image to display
    img.copyTo( img_display );

    /// Create the result matrix
    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;

    result.create( result_cols, result_rows, CV_32FC1 );

    /// Do the Matching and Normalize
    matchTemplate( img, templ, result, match_method );
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

    int i = 0;
    for( i; i<10; i++ ){

        /// Localizing the best match with minMaxLoc
        double minVal; double maxVal; Point minLoc; Point maxLoc;
        Point matchLoc;

        minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

        /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
        if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
          { matchLoc = minLoc; }
        else
          { matchLoc = maxLoc; }

        /// Show me what you got
        rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );

        result.at<int>(maxLoc.y, maxLoc.x)=0;
        result.at<int>(maxLoc.y, maxLoc.x-1)=0;
    }

    this->displayImageInImageLabel(img_display);

    waitKey(0);
}

void MainWindow::on_findButton4_clicked()
{
    /// Load image and template
    img = imread(this->imagePath.toStdString());
    templ = imread(this->patternPath.toStdString());

    /// Create windows
    namedWindow( image_window, CV_WINDOW_AUTOSIZE );
    namedWindow( result_window, CV_WINDOW_AUTOSIZE );

    /// Create Trackbar
    //char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
    //createTrackbar( trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod );

    match_method = 4;

    /// Source image to display
    img.copyTo( img_display );

    /// Create the result matrix
    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;

    result.create( result_cols, result_rows, CV_32FC1 );

    /// Do the Matching and Normalize
    matchTemplate( img, templ, result, match_method );
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

    int i = 0;
    for( i; i<10; i++ ){

        /// Localizing the best match with minMaxLoc
        double minVal; double maxVal; Point minLoc; Point maxLoc;
        Point matchLoc;

        minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

        /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
        if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
          { matchLoc = minLoc; }
        else
          { matchLoc = maxLoc; }

        /// Show me what you got
        rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );

        result.at<int>(maxLoc.y, maxLoc.x)=0;
        result.at<int>(maxLoc.y, maxLoc.x-1)=0;
    }

    this->displayImageInImageLabel(img_display);

    waitKey(0);
}

void MainWindow::on_findButton5_clicked()
{
    /// Load image and template
    img = imread(this->imagePath.toStdString());
    templ = imread(this->patternPath.toStdString());

    /// Create windows
    namedWindow( image_window, CV_WINDOW_AUTOSIZE );
    namedWindow( result_window, CV_WINDOW_AUTOSIZE );

    /// Create Trackbar
    //char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
    //createTrackbar( trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod );

    match_method = 5;

    /// Source image to display
    img.copyTo( img_display );

    /// Create the result matrix
    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;

    result.create( result_cols, result_rows, CV_32FC1 );

    /// Do the Matching and Normalize
    matchTemplate( img, templ, result, match_method );
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

    int i = 0;
    for( i; i<10; i++ ){

        /// Localizing the best match with minMaxLoc
        double minVal; double maxVal; Point minLoc; Point maxLoc;
        Point matchLoc;

        minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

        /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
        if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
          { matchLoc = minLoc; }
        else
          { matchLoc = maxLoc; }

        /// Show me what you got
        rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );

        result.at<int>(maxLoc.y, maxLoc.x)=0;
        result.at<int>(maxLoc.y, maxLoc.x-1)=0;
    }
    this->displayImageInImageLabel(img_display);

    waitKey(0);
}

