#include <QtGui>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QStringList>
#include <QFile>
#include <QDir>

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
    image_2 = new QImage;
    pattern = new QImage;

    ui->setupUi(this);
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    ui->imageLabel->setPixmap(QPixmap::fromImage(*image));
    ui->imageLabel_2->setPixmap(QPixmap::fromImage(*image_2));
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
    int tabnumber;
    tabnumber = ui->tabWidget->currentIndex();

    QString fileName;
    QFileDialog dialog;
    fileName = dialog.getOpenFileName(this,
        tr("Open Image"), "/Users/resa/Studium/WiSe2013/Thesis", tr("Image Files (*.png *.jpg *.bmp)"));

    ///check active tab
    if(tabnumber == 0 )
    {
        image->load(fileName);
        imagePath = fileName;

        int w = ui->imageLabel->width();
        int h = ui->imageLabel->height();
        ui->imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(w,h,Qt::KeepAspectRatio));
    }
    else
    {
        image_2->load(fileName);
        imagePath_2 = fileName;
        int w = ui->imageLabel_2->width();
        int h = ui->imageLabel_2->height();
        ui->imageLabel_2->setPixmap(QPixmap::fromImage(*image_2).scaled(w,h,Qt::KeepAspectRatio));
    }
}

void MainWindow::displayImageInImageLabel(Mat mat)
{
    int tabnumber;
    tabnumber = ui->tabWidget->currentIndex();

    QPixmap pixmap;

    ///check active tab
    if(tabnumber == 0)
    {
        int w = ui->imageLabel->width();
        int h = ui->imageLabel->height();

        pixmap = QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888));

        ui->imageLabel->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio));
    }
    else
    {
        int w = ui->imageLabel_2->width();
        int h = ui->imageLabel_2->height();

        pixmap = QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888));

        ui->imageLabel_2->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio));
    }
}


void MainWindow::on_findButton0_clicked()
{
    matchingWithMethod(0);}

void MainWindow::on_findButton1_clicked()
{
    matchingWithMethod(1);
}

void MainWindow::on_findButton2_clicked()
{
    matchingWithMethod(2);
}

void MainWindow::on_findButton3_clicked()
{
    matchingWithMethod(3);}

void MainWindow::on_findButton4_clicked()
{
    matchingWithMethod(4);
}

void MainWindow::on_findButton5_clicked()
{
    matchingWithMethod(5);
}

void MainWindow::matchingWithMethod(int method){

    match_method = method;
    int tabnumber;
    tabnumber = ui->tabWidget->currentIndex();

    /// Load image and template
    if (tabnumber == 0)
    {
        img = imread(this->imagePath.toStdString());
    }
    else
    {
        img = imread(this->imagePath_2.toStdString());
    }

    templ = imread(this->patternPath.toStdString());

    /// Create windows
    //namedWindow( image_window, CV_WINDOW_AUTOSIZE );
    namedWindow( result_window, CV_WINDOW_AUTOSIZE );

    /// Create Trackbar
    //char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
    //createTrackbar( trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod );

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
    for( i; i<50; i++ ){

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

        if(method<=1)
        {
            result.at<int>(minLoc.y, minLoc.x)=1;
            if(minVal>=0.6)
            {
                i=50;
            }
        }
        else
        {
            result.at<int>(maxLoc.y, maxLoc.x)=0;
            if(maxVal<=0.4)
            {
                i=50;
            }
        }
    }
    this->displayImageInImageLabel(img_display);

    waitKey(0);                                 //nötig?
}
