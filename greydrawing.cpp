#include <QtGui>

#include "mainwindow.h"
#include "ui_mainwindow.h"

//using namespace std;
using namespace cv;


void MainWindow::createGreyImage(Mat colorImage, int cmyk)
{
    int w = colorImage.cols;
    int h = colorImage.rows;

    float red = 0.299;
    float green = 0.587;
    float blue = 0.114;

    int cmyArray[3];

    greyImage->create(h, w, CV_32FC1);
    greyToScreen->create(h, w, CV_32FC3);

    for( int y=0; y<h; y++ )
    {
        for( int x=0; x<w; x++ )
        {
            bool draw = true;
            int first;
            int second;

            ///only for filtering
            if( cmyk != 4 )
            {
                ///converting to CMY values
                cmyArray[0] = 255-colorImage.at<Vec3b>(y,x)[2];
                cmyArray[1] = 255-colorImage.at<Vec3b>(y,x)[1];
                cmyArray[2] = 255-colorImage.at<Vec3b>(y,x)[0];

                greyImage->at<float>(y,x) = 255-cmyArray[cmyk];
                             //greyToScreen only for Testing
                greyToScreen->at<Vec3b>(y,x)[cmyk] = (unsigned char)greyImage->at<float>(y,x);             //
                greyToScreen->at<Vec3b>(y,x)[(cmyk+1)%3] = 255;
                greyToScreen->at<Vec3b>(y,x)[(cmyk+2)%3] = 255;             //
            }
            else{

                greyImage->at<float>(y,x) = blue*colorImage.at<Vec3b>(y,x)[0]+green*colorImage.at<Vec3b>(y,x)[1]+red*colorImage.at<Vec3b>(y,x)[2];
                greyToScreen->at<Vec3b>(y,x)[0] = (unsigned char)greyImage->at<float>(y,x);             //greyToScreen only for Testing
                greyToScreen->at<Vec3b>(y,x)[1] = (unsigned char)greyImage->at<float>(y,x);             //
                greyToScreen->at<Vec3b>(y,x)[2] = (unsigned char)greyImage->at<float>(y,x);             //
            }
            /*
            greyToScreen->at<Vec3b>(y,x)[0] = (unsigned char)greyImage->at<float>(y,x);             //greyToScreen only for Testing
            greyToScreen->at<Vec3b>(y,x)[1] = (unsigned char)greyImage->at<float>(y,x);             //
            greyToScreen->at<Vec3b>(y,x)[2] = (unsigned char)greyImage->at<float>(y,x);             //
            */
/*
                ///finding largest cmy value
                if (cmyArray[0] > cmyArray[1])
                {
                    first = 0;
                    second = 1;
                }
                else{
                    first = 1;
                    second = 0;
                }
                if(cmyArray[2] > cmyArray[first])
                {
                    second = first;
                    first = 2;
                }
                else{
                    if(cmyArray[2] > cmyArray[second])
                    {
                        second = 2;
                    }
                }
                ///do not draw when its not the filtered value OR the color is not definitly identified
                if(first != cmyk || cmyArray[first] < 1.5*cmyArray[second] || cmyArray[0]+cmyArray[1]+cmyArray[2]<50 )
                {
                    draw = false;
                }
            }

            if (draw)
            {
                greyImage->at<float>(y,x) = red*colorImage.at<Vec3b>(y,x)[0]+green*colorImage.at<Vec3b>(y,x)[1]+blue*colorImage.at<Vec3b>(y,x)[2];
                greyToScreen->at<Vec3b>(y,x)[0] = (unsigned char)greyImage->at<float>(y,x);             //greyToScreen only for Testing
                greyToScreen->at<Vec3b>(y,x)[1] = (unsigned char)greyImage->at<float>(y,x);             //
                greyToScreen->at<Vec3b>(y,x)[2] = (unsigned char)greyImage->at<float>(y,x);             //
            }
            ///draw white pixel
            else{
                c 255;
                greyToScreen->at<Vec3b>(y,x)[0] = (unsigned char)greyImage->at<float>(y,x);             //greyToScreen only for Testing
                greyToScreen->at<Vec3b>(y,x)[1] = (unsigned char)greyImage->at<float>(y,x);             //
                greyToScreen->at<Vec3b>(y,x)[2] = (unsigned char)greyImage->at<float>(y,x);             //
            }
            */

        }
    }
}

void MainWindow::createGreyPattern(Mat colorPattern, int cmyk)          //filter is not implemented yet
{
    int w = colorPattern.cols;
    int h = colorPattern.rows;
    greyPatternPixelSum = 0;
    greyPatternPixelSumPow = 0;
    float red = 0.299;
    float green = 0.587;
    float blue = 0.114;

    greyPattern->create(h, w, CV_32FC1);
    greyToScreen->create(h, w, CV_32FC3);

    for( int y=0; y<h; y++ )
    {
        for( int x=0; x<w; x++ )
        {
            greyPattern->at<float>(y,x) = red*colorPattern.at<Vec3b>(y,x)[0]+green*colorPattern.at<Vec3b>(y,x)[1]+blue*colorPattern.at<Vec3b>(y,x)[2];
            double summand = greyPattern->at<float>(y,x);
            greyPatternPixelSum += summand;
            greyPatternPixelSumPow += summand*summand;
            greyToScreen->at<Vec3b>(y,x)[0] = (unsigned char)greyPattern->at<float>(y,x);             //greyToScreen only for Testing
            greyToScreen->at<Vec3b>(y,x)[1] = (unsigned char)greyPattern->at<float>(y,x);             //
            greyToScreen->at<Vec3b>(y,x)[2] = (unsigned char)greyPattern->at<float>(y,x);             //
        }
    }
}
