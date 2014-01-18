#include <QtGui>

#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <QStringList>
//#include <QFile>
//#include <QDir>
//#include <iostream>
//#include <stdio.h>

using namespace std;
using namespace cv;

/// Global Variables
Mat img; Mat templ; Mat result; Mat img_display;

char* image_window = "Source Image";
char* result_window = "Result window";

int match_method;
int max_Trackbar = 5;

/// Function Headers
//void MatchingMethod( int, void* );



void MainWindow::matchingWithMethod(int method, float sensivity)
{

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

    /// Source image to display
    img.copyTo( img_display );

    /// Create the result matrix
    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;

    result.create( result_cols, result_rows, CV_32FC1 );

    /// Do the Matching and Normalize
    if (match_method < 6)
    {
        matchTemplate( img, templ, result, match_method );
    }
    else
    {
        result = match();
    }

    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

    for( int i=0; i<50; i++ ){

        /// Localizing the best match with minMaxLoc
        double minVal; double maxVal; Point minLoc; Point maxLoc;
        Point matchLoc;
        if (match_method < 6)
        {
            minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
        }
        else
        {
            minMax(result, &minLoc, &maxLoc, &minVal, &maxVal);
        }
        /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
        if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED || match_method >= 6 )
          { matchLoc = minLoc; }
        else
          { matchLoc = maxLoc; }

        /// Show me what you got
        rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );

        if(method<=1 || method >=6 )
        {
            float test = result.at<float>(minLoc.y, minLoc.x);
             result.at<float>(minLoc.y, minLoc.x)=1;
            test = result.at<float>(minLoc.y, minLoc.x);
           if(minVal>=1-sensivity)
            {
                i=50;
            }
        }
        else
        {
            result.at<float>(maxLoc.y, maxLoc.x)=0;
            if(maxVal<=sensivity)
            {
                i=50;
            }
        }
    }
    this->displayImageInImageLabel(img_display);
}

Mat MainWindow::match()
{
    int result_cols =  greyImage->cols - greyPattern->cols + 1;
    int result_rows = greyImage->rows - greyPattern->rows + 1;

    Mat localResult;

    localResult.create( result_cols, result_rows, CV_32FC1 );

    for( int y=0; y<localResult.rows; y++ )
    {
        for( int x=0; x<localResult.cols; x++ )
        {
            localResult.at<float>(y,x) = matchingAlgorithm(x,y);
        }
    }
    return localResult;
}

float MainWindow::matchingAlgorithm(int x, int y)
{
    float pixelResult = 0;
    double greyImagePixelSum = 0;
    for( int y2=0; y2<greyPattern->rows; y2++ )
    {
        for( int x2=0; x2<greyPattern->cols; x2++ )
        {
            greyImagePixelSum += greyImage->at<float>(y+y2,x+x2);

        }
    }
    for( int y2=0; y2<greyPattern->rows; y2++ )
    {
        for( int x2=0; x2<greyPattern->cols; x2++ )
        {
            pixelResult += pow( tmpFunction(x2,y2) - imgFunction(x+x2,y+y2, greyImagePixelSum), (float)(2.0) );
        }
    }
    return pixelResult;
}

float MainWindow::tmpFunction( int x, int y )
{
    float result;
    result = greyPattern->at<float>(y,x) - greyPatternPixelSum/(greyPattern->cols * greyPattern->rows);
    return result;
}

float MainWindow::imgFunction( int x, int y, double greyImagePixelSum )
{

    float result;
    result = greyImage->at<float>(y,x) - greyImagePixelSum/(greyPattern->cols * greyPattern->rows);
    return result;
}

void MainWindow::minMax(Mat matResult, Point* min, Point* max, double* minVal, double* maxVal)
{
    *minVal = matResult.at<float>(0,0);
    *maxVal = matResult.at<float>(0,0);

    for( int y=0; y<matResult.rows; y++ )
    {
        for( int x=0; x<matResult.cols; x++ )
        {
            if( matResult.at<float>(y,x) < (float)(*minVal) )
            {
                *minVal = matResult.at<float>(y,x);
                min->x = x;
                min->y = y;
            }
            if( matResult.at<float>(y,x) > (float)(*maxVal) )
            {
                *maxVal = matResult.at<float>(y,x);
                max->x = x;
                max->y = y;
            }
        }
    }
}
