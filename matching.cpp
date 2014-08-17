#include <QtGui>
#include <fstream>

#include "mainwindow.h"
#include "ui_mainwindow.h"


using namespace std;
using namespace cv;

/// Global Variables
Mat img; Mat templ; Mat result; Mat img_display;

char* image_window = "Source Image";
char* result_window = "Result window";

int match_method;
int max_Trackbar = 5;

void MainWindow::matchingWithMethod(int method, float sensitivity)
{
    this->setCursor(Qt::WaitCursor);
    match_method = method;
    float exitValue;
    int tabnumber;
    tabnumber = ui->tabWidget->currentIndex();
    img = *coloredImage;

    templ = imread(this->patternPath.toStdString());

    /// Source image to display
    img.copyTo( img_display );

    /// Create the result matrix
    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;

    result.create( result_cols, result_rows, CV_32FC1 );

    /// Do the Matching and Normalize
    result = match(method);

   //------ export result data to view data --------

    ofstream myfile;
    myfile.open ("./DropMatrixMatcherData/resultData.txt");
    for (int r=0; r<result_rows; r++)
    {
        for (int c=0; c<result_cols; c++)
        {
            myfile << result.at<float>(r,c) << ",";
        }
        myfile << endl;
    }
    myfile.close();

    //------ export result data to view data --------

    /// Localizing the best match with minMaxLoc
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    Point matchLoc;

    for( int i=0; i<50; i++ ){

        minMax(result, &minLoc, &maxLoc, &minVal, &maxVal);
        if (i==0)
        {
            exitValue = (float)(maxVal-(maxVal-minVal)*sensitivity);
        }
        if((float)minVal>=(exitValue*1.00001))
        {
            i=50;
        }
        else
        {
           /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
           matchLoc = minLoc;

           /// Show me what you got
           rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );

           float test = result.at<float>(minLoc.y, minLoc.x);
            result.at<float>(minLoc.y, minLoc.x)=maxVal;
           test = result.at<float>(minLoc.y, minLoc.x);
        }
    }
    this->unsetCursor();
    this->displayImageInImageLabel(img_display);
}

Mat MainWindow::match(int method)
{
    int result_cols =  greyImage->cols - greyPattern->cols + 1;
    int result_rows = greyImage->rows - greyPattern->rows + 1;

    Mat localResult;

    localResult.create( result_cols, result_rows, CV_32FC1 );

    for( int y=0; y<localResult.rows; y++ )
    {
        for( int x=0; x<localResult.cols; x++ )
        {
            localResult.at<float>(y,x) = matchingAlgorithm(x,y, method) ;
        }
    }
    return localResult;
}

float MainWindow::matchingAlgorithm(int x, int y, int method)
{
    float pixelResult = 0;
    double greyImagePixelSum = 0;
    double greyImagePixelSumPow = 0;
    for( int y2=0; y2<greyPattern->rows; y2++ )
    {
        for( int x2=0; x2<greyPattern->cols; x2++ )
        {
            float summand = greyImage->at<float>(y+y2,x+x2);
            greyImagePixelSum += summand;
            greyImagePixelSumPow += summand*summand;
        }
    }
    for( int y2=0; y2<greyPattern->rows; y2++ )
    {
        for( int x2=0; x2<greyPattern->cols; x2++ )
        {
                float tmp = tmpFunction(x2,y2, method) - imgFunction(x+x2,y+y2, greyImagePixelSum, method);
                pixelResult += tmp*tmp;
        }
    }
    if (method == 8)
    {
        pixelResult /= sqrt( greyPatternPixelSumPow * greyImagePixelSumPow );
    }

    return pixelResult;
}

float MainWindow::tmpFunction( int x, int y, int method )
{
    float result;

    result = greyPattern->at<float>(y,x);
    if (method > 6)
    {
        result -= greyPatternPixelSum/(greyPattern->cols * greyPattern->rows);
    }
    return result;
}

float MainWindow::imgFunction( int x, int y, double greyImagePixelSum, int method )
{
    float result;

    result = greyImage->at<float>(y,x);
    if(method > 6)
    {
        result -= greyImagePixelSum/(greyPattern->cols * greyPattern->rows);
    }
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

void MainWindow::matchingWithCvMethod(int method, float sensitivity)
{
    match_method = method;
    int tabnumber;
    this->setCursor(Qt::WaitCursor);
    tabnumber = ui->tabWidget->currentIndex();

    img = *coloredImage;

    templ = *coloredPattern;

    /// Source image to display
    img.copyTo( img_display );

    /// Create the result matrix
    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;

    result.create( result_cols, result_rows, CV_32FC1 );

    /// Do the Matching and Normalize
    matchTemplate( img, templ, result, match_method );
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

    for( int i=0; i<50; i++ ){

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
             result.at<float>(minLoc.y, minLoc.x)=1;
           if(minVal>=1-sensitivity)
            {
                i=50;
            }
        }
        else
        {
            result.at<float>(maxLoc.y, maxLoc.x)=0;
            if(maxVal<=sensitivity)
            {
                i=50;
            }
        }
    }
    this->unsetCursor();
    this->displayImageInImageLabel(img_display);
}
