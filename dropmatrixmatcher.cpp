#include <QtGui>

#include "dropmatrixmatcher.h"
#include "ui_dropmatrixmatcher.h"
#include "helpwindow.h"

#include <QFileDialog>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;


float sensitivityRange = 0;

DropMatrixMatcher::DropMatrixMatcher(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DropMatrixMatcher)
{
    image = new QImage;
    image_1 = new QImage;
    image_2 = new QImage;
    greyImage = new Mat;
    coloredPattern = new Mat;
    coloredImage = new Mat;
    pattern = new QImage;
    greyPattern = new Mat;
    greyToScreen = new Mat;
    colored = true;
    imagePath = new QString;
    myLabel = new ProLabel;
    //hwindow = new HelpWindow;

//    QToolBar *toolBar = new QToolBar("toolBar");
//    QMenu *methodMenu = new QMenu( ui->toolButton );
//    ui->toolButton->setMenu( methodMenu );
//    ui->toolButton->setPopupMode( QToolButton::MenuButtonPopup );

    QIcon icon;
    icon.addFile("DMM.icns");
    this->setWindowIcon(icon);

    ui->setupUi(this);
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    ui->imageLabel_1->setPixmap(QPixmap::fromImage(*image));
    ui->imageLabel_2->setPixmap(QPixmap::fromImage(*image_2));
    ui->patternLabel->setPixmap(QPixmap::fromImage(*pattern));
    ui->dial->setRange(0,100);
    ui->tabWidget->setCurrentIndex(0);

    this->image = this->image_1;
    this->imagePath = &this->imagePath_1;
    this->myLabel = this->ui->imageLabel_1;
    this->setWindowTitle("Drop Matrix Matcher");
    this->setWindowIcon(QIcon("matcher.icns"));

//    ui->findButton0->setDefault(true);
//    ui->findButton0->setEnabled(false);
//    ui->findButton1->setDefault(true);
//    ui->findButton1->setEnabled(false);
//    ui->findButton2->setDefault(true);
//    ui->findButton2->setEnabled(false);
//    ui->findButton3->setDefault(true);
//    ui->findButton3->setEnabled(false);
//    ui->findButton4->setDefault(true);
//    ui->findButton4->setEnabled(false);
//    ui->findButton5->setDefault(true);
//    ui->findButton5->setEnabled(false);

    ui->brightnessSlider->setMinimum(-100);
    ui->brightnessSlider->setMaximum(100);
    ui->brightnessSlider->setSliderPosition(0);
    ui->contrastSlider->setMinimum(0);
    ui->contrastSlider->setMaximum(200);
    ui->contrastSlider->setSliderPosition(100);

    ui->brightnessSlider_2->setMinimum(-100);
    ui->brightnessSlider_2->setMaximum(100);
    ui->brightnessSlider_2->setSliderPosition(0);
    ui->contrastSlider_2->setMinimum(0);
    ui->contrastSlider_2->setMaximum(200);
    ui->contrastSlider_2->setSliderPosition(100);

    ui->sensitivityValueText->setMinimum(0);
    ui->sensitivityValueText->setMaximum(100);
    ui->sensitivityValueText->setValue(0);


    // Only if images are uploaded, find button is enabled
    connect(ui->loadImage, SIGNAL(clicked()),
            this, SLOT(enableFindButtons()));
    connect(ui->loadPattern, SIGNAL(clicked()),
            this, SLOT(enableFindButtons()));
    connect(ui->LoadSelectedPattern, SIGNAL(clicked()),
            this, SLOT(enableFindButtons()));

    connect(ui->brightnessSlider, SIGNAL(valueChanged(int)),
    this, SLOT(setBrightness(int)));
    connect(ui->contrastSlider, SIGNAL(valueChanged(int)),
    this, SLOT(setContrast(int)));
    connect(ui->brightnessSlider_2, SIGNAL(valueChanged(int)),
    this, SLOT(setImageBrightness(int)));
    connect(ui->contrastSlider_2, SIGNAL(valueChanged(int)),
    this, SLOT(setImageContrast(int)));

    connect(ui->dial, SIGNAL(valueChanged(int)),
            this, SLOT(sensitivity(int)));
    connect(ui->dial, SIGNAL(valueChanged(int)),
            ui->sensitivityValueText, SLOT(setValue(int)));
    connect(ui->sensitivityValueText, SIGNAL(valueChanged(int)),
            this, SLOT(sensitivity(int)));
    connect(ui->sensitivityValueText, SIGNAL(valueChanged(int)),
            ui->dial, SLOT(setValue(int)));

    connect(ui->filterButton, SIGNAL(clicked(bool)),
            this, SLOT(filterImage()));
    connect(ui->greyView, SIGNAL(clicked(bool)),
            this, SLOT(useGreyView()));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(tabChanged(int)));
//    connect(ui->pushButton, SIGNAL(clicked()),
//            this->hwindow, SLOT(show()));

    bool folderAlreadyExists = QDir("DropMatrixMatcherData").exists();
    if(!folderAlreadyExists)
    {
        QDir().mkdir("DropMatrixMatcherData");
    }
}


void DropMatrixMatcher::enableFindButtons()
{
    if (!image->isNull() && !pattern->isNull())
    {
//        ui->findButton0->setEnabled(true);
//        ui->findButton1->setEnabled(true);
//        ui->findButton2->setEnabled(true);
//        ui->findButton3->setEnabled(true);
//        ui->findButton4->setEnabled(true);
//        ui->findButton5->setEnabled(true);

    }
}

DropMatrixMatcher::~DropMatrixMatcher()
{
    delete ui;
}

void DropMatrixMatcher::tabChanged(int tab)
{
//    ui->brightnessSlider_2->setSliderPosition(0);
//    ui->contrastSlider_2->setSliderPosition(100);

    if(tab == 0)
    {
        this->image = this->image_1;
        this->imagePath = &(this->imagePath_1);
        this->myLabel = this->ui->imageLabel_1;
        //use correct image
        createAllImages(true);
//        this->image->save("./DropMatrixMatcherData/editedImage.png", 0, 100);
//        *coloredImage = imread("./DropMatrixMatcherData/editedImage.png");
//        createGreyImage(*coloredImage);
        // it's only possible to select a pattern out of the image in tab 1
//        ui->LoadSelectedPattern->setEnabled(true);
    }
    else{
        this->image = this->image_2;
        this->imagePath = &(this->imagePath_2);
        this->myLabel = this->ui->imageLabel_2;
        //use correct image
        createAllImages(true);
//        this->image->save("./DropMatrixMatcherData/editedImage.png", 0, 100);
//        *coloredImage = imread("./DropMatrixMatcherData/editedImage.png");
//        createGreyImage(*coloredImage);
        // it's only possible to select a pattern out of the image in tab 1
//        ui->LoadSelectedPattern->setEnabled(false);
    }


}

void DropMatrixMatcher::on_loadPattern_clicked()
{
    QString fileName;
    QFileDialog dialog;
    Mat matPattern;

    fileName = dialog.getOpenFileName(this,
        tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.bmp *.tif)"));
    int w = ui->patternLabel->width();
    int h = ui->patternLabel->height();

    if(!fileName.isEmpty()){
        pattern->load(fileName);
        patternPath = fileName;
        createAllImages(false);
    }
//    pattern->save("./DropMatrixMatcherData/editedPattern.png", 0, 100);
//    *coloredPattern = imread("./DropMatrixMatcherData/editedPattern.png");
//    createGreyPattern(*coloredPattern);

    //matPattern = imread(this->patternPath.toStdString());
    //createGreyPattern(matPattern);

    ui->patternLabel->setPixmap(QPixmap::fromImage(*pattern).scaled(w,h,Qt::KeepAspectRatio));  //evtl weglassen, wenn standardmäßig image/pattern in label angezeigt wird
    ui->brightnessSlider->setSliderPosition(0);
    ui->contrastSlider->setSliderPosition(100);
}

void DropMatrixMatcher::on_loadImage_clicked()
{
    QString fileName;
    QFileDialog dialog;
    fileName = dialog.getOpenFileName(this,
        tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.bmp *.tif)"));

    if(!fileName.isEmpty()){
        Mat matImage;
        image->load(fileName);
        *imagePath = fileName;

        createAllImages(true);
    }

//    image->save("./DropMatrixMatcherData/editedImage.png", 0, 100);
//    *coloredImage = imread("./DropMatrixMatcherData/editedImage.png");
//    createGreyImage(*coloredImage);

    int w = myLabel->width();
    int h = myLabel->height();

    myLabel->setPixmap(QPixmap::fromImage(*image).scaled(w,h,Qt::KeepAspectRatio));

    ui->brightnessSlider_2->setSliderPosition(0);
    ui->contrastSlider_2->setSliderPosition(100);
}

void DropMatrixMatcher::displayImageInImageLabel(Mat mat)
{
    QPixmap pixmap;

    int w = myLabel->width();
    int h = myLabel->height();
    //QImage::Format imageFormat = this->image->format();

    //mat.depthMat(image->height(),image->width(),CV_8UC4,(uchar*)image->bits(),image->bytesPerLine());

    QImage imageToShow = QImage((unsigned char*) mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    imageToShow = imageToShow.rgbSwapped();
    imageToShow.save("./DropMatrixMatcherData/imageToShow.png", 0, 100);
    pixmap = QPixmap::fromImage(imageToShow);
    myLabel->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio));
}


void DropMatrixMatcher::on_compareButton_clicked()
{
    if(this->pattern->isNull()){
        QMessageBox noDropMatrixSelected;
        noDropMatrixSelected.setText("You have to select a Drop Matrix first.");
        noDropMatrixSelected.exec();
        }else{
        int method = this->ui->comboBox->currentIndex();
        if(method == 0){
            //SQDiff
            matchingWithCvMethod(0, sensitivityRange);
        }else if(method == 1){
            //SQDiff Normed
            matchingWithCvMethod(1, sensitivityRange);
        }else if(method == 2){
            //TMCCorr
            matchingWithCvMethod(2, sensitivityRange);
        }else if(method == 3){
            //TMCCorr Normed
            matchingWithCvMethod(3, sensitivityRange);
        }else if(method == 4){
            //TMCCoeff
            matchingWithCvMethod(4, sensitivityRange);
        }else if(method == 5){
            //TMCCoeff Normed
            matchingWithCvMethod(5, sensitivityRange);
        }else if(method == 6){
            //my Simple Diff
            matchingWithCvMethod(6, sensitivityRange);
        }else if(method == 7){
            //my Diff
            matchingWithCvMethod(7, sensitivityRange);
        }else if(method == 8){
            //my Diff Normed
            matchingWithCvMethod(8, sensitivityRange);
        }
    }
}

//void DropMatrixMatcher::on_findButton0_clicked()
//{
//    matchingWithCvMethod(0, sensitivityRange);
//}

//void DropMatrixMatcher::on_findButton1_clicked()
//{
//    matchingWithCvMethod(1, sensitivityRange);
//}

//void DropMatrixMatcher::on_findButton2_clicked()
//{
//    matchingWithCvMethod(2, sensitivityRange);
//}

//void DropMatrixMatcher::on_findButton3_clicked()
//{
//    matchingWithCvMethod(3, sensitivityRange);
//}

//void DropMatrixMatcher::on_findButton4_clicked()
//{
//    matchingWithCvMethod(4, sensitivityRange);
//}

//void DropMatrixMatcher::on_findButton5_clicked()
//{
//    matchingWithCvMethod(5, sensitivityRange);
//}

//void DropMatrixMatcher::on_findButton6_clicked()
//{
//    matchingWithMethod(6, sensitivityRange);
//}

//void DropMatrixMatcher::on_findButton7_clicked()
//{
//    matchingWithMethod(7, sensitivityRange);
//}

//void DropMatrixMatcher::on_findButton8_clicked()
//{
//    matchingWithMethod(8, sensitivityRange);
//}

void DropMatrixMatcher::on_LoadSelectedPattern_clicked()
{
    int w = ui->patternLabel->width();
    int h = ui->patternLabel->height();
    QImage currentImage;
    currentImage.load("./DropMatrixMatcherData/editedImage.png");
    int imagewidth = currentImage.width();
    int imageheight = currentImage.height();
    int pixmapwidth = myLabel->pixmap()->width();
    int pixmapheight = myLabel->pixmap()->height();

    float widthFactor = (float)imagewidth/(float)pixmapwidth;
    float heightFactor = (float)imageheight/(float)pixmapheight;

    QString fileName = "./DropMatrixMatcherData/pattern01.png";         //Ort muss einstellbar sein, evt. globale Variable

    selectRect = myLabel->getQImageRect(widthFactor, heightFactor);

    *pattern = currentImage.copy(selectRect);

    pattern->save(fileName, 0, 100);        //muss aufgehellt werden
    patternPath = fileName;

    createAllImages(false);
//    pattern->save("./DropMatrixMatcherData/editedPattern.png", 0, 100);
//    *coloredPattern = imread("./DropMatrixMatcherData/editedPattern.png");
//    createGreyPattern(*coloredPattern);

    //Mat matPattern = imread(this->patternPath.toStdString());
    //createGreyPattern(matPattern);
    ui->patternLabel->setPixmap(QPixmap::fromImage(*pattern).scaled(w,h,Qt::KeepAspectRatio));
    ui->brightnessSlider->setSliderPosition(0);
    ui->contrastSlider->setSliderPosition(100);
}

void DropMatrixMatcher::setBrightness(int value)
{
    int w = ui->patternLabel->width();
    int h = ui->patternLabel->height();

    Mat matImage = imread(this->patternPath.toStdString());
    Mat new_image = Mat::zeros( matImage.size(), matImage.type() );
    float contrast = (float)(this->ui->contrastSlider->value())/100;

    for( int y = 0; y < matImage.rows; y++ )
    {
        for( int x = 0; x < matImage.cols; x++ )
        {
            for( int c = 0; c < 3; c++ )
            {
               new_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( contrast *(matImage.at<Vec3b>(y,x)[c]) + value );
            }
        }
    }    

   *pattern = QImage((unsigned char*) new_image.data, new_image.cols, new_image.rows, new_image.step, QImage::Format_RGB888);
    *pattern = pattern->rgbSwapped();
    createAllImages(false);
//    pattern->save("./DropMatrixMatcherData/editedPattern.png", 0, 100);
//    *coloredPattern = imread("./DropMatrixMatcherData/editedPattern.png");
//    createGreyPattern(*coloredPattern);
    ui->patternLabel->setPixmap(QPixmap::fromImage(*pattern).scaled(w,h,Qt::KeepAspectRatio));

}

void DropMatrixMatcher::setContrast(int value)
{
    int w = ui->patternLabel->width();
    int h = ui->patternLabel->height();
    /// Read pattern
    Mat matImage = imread(this->patternPath.toStdString());
    Mat new_image = Mat::zeros( matImage.size(), matImage.type() );

    float contrast = (float)value/100;
    int brightness = this->ui->brightnessSlider->value();

    for( int y = 0; y < matImage.rows; y++ )
    {
        for( int x = 0; x < matImage.cols; x++ )
        {
            for( int c = 0; c < 3; c++ )
            {
                new_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( contrast*( matImage.at<Vec3b>(y,x)[c] ) + brightness );
            }
        }
    }

    *pattern = QImage((unsigned char*) new_image.data, new_image.cols, new_image.rows, new_image.step, QImage::Format_RGB888);
    *pattern = pattern->rgbSwapped();
    createAllImages(false);
//    pattern->save("./DropMatrixMatcherData/editedPattern.png", 0, 100);
//    *coloredPattern = imread("./DropMatrixMatcherData/editedPattern.png");
//    createGreyPattern(*coloredPattern);

    ui->patternLabel->setPixmap(QPixmap::fromImage(*pattern).scaled(w,h,Qt::KeepAspectRatio));
}
void DropMatrixMatcher::setImageBrightness(int value)
{
    int w = myLabel->width();
    int h = myLabel->height();
    /// Read pattern
    Mat matImage = imread(this->imagePath->toStdString());
    Mat new_image = Mat::zeros( matImage.size(), matImage.type() );
    float contrast = (float)(this->ui->contrastSlider_2->value())/100;

    for( int y = 0; y < matImage.rows; y++ )
    {
        for( int x = 0; x < matImage.cols; x++ )
        {
            for( int c = 0; c < 3; c++ )
            {
               new_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( contrast *(matImage.at<Vec3b>(y,x)[c]) + value );
            }
        }
    }

   *image = QImage((unsigned char*) new_image.data, new_image.cols, new_image.rows, new_image.step, QImage::Format_RGB888);
    *image = image->rgbSwapped();

    createAllImages(true);
//    image->save("./DropMatrixMatcherData/editedImage.png", 0, 100);
//    *coloredImage = imread("./DropMatrixMatcherData/editedImage.png");
//    createGreyImage(*coloredImage);

    myLabel->setPixmap(QPixmap::fromImage(*image).scaled(w,h,Qt::KeepAspectRatio));

}

void DropMatrixMatcher::setImageContrast(int value)
{
    int w = myLabel->width();
    int h = myLabel->height();
    /// Read pattern
    Mat matImage = imread(this->imagePath->toStdString());
    Mat new_image = Mat::zeros( matImage.size(), matImage.type() );

    float contrast = (float)value/100;
    int brightness = this->ui->brightnessSlider_2->value();

    for( int y = 0; y < matImage.rows; y++ )
    {
        for( int x = 0; x < matImage.cols; x++ )
        {
            for( int c = 0; c < 3; c++ )
            {
                new_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( contrast*( matImage.at<Vec3b>(y,x)[c] ) + brightness );
            }
        }
    }

    *image = QImage((unsigned char*) new_image.data, new_image.cols, new_image.rows, new_image.step, QImage::Format_RGB888);
    *image = image->rgbSwapped();

    createAllImages(true);
//    image->save("./DropMatrixMatcherData/editedImage.png", 0, 100);
//    *coloredImage = imread("./DropMatrixMatcherData/editedImage.png");
//    createGreyImage(*coloredImage);

    myLabel->setPixmap(QPixmap::fromImage(*image).scaled(w,h,Qt::KeepAspectRatio));
}

void DropMatrixMatcher::sensitivity(int value)
{
    sensitivityRange = (float)value/100;
}

void DropMatrixMatcher::filterImage()
{
    if(this->ui->radioC->isChecked())
    {
        this->filter(0);
    }
    if(this->ui->radioM->isChecked())
    {
        this->filter(1);
    }
    if(this->ui->radioY->isChecked())
    {
        this->filter(2);
    }

    if (this->ui->filterButton->isFlat()){
//        if(!colored){
//            QImage originalColoredImg;
//            originalColoredImg.load(this->imagePath->toStdString());
//            createGreyImage(originalColoredImg);
//            QImage greyImg = QImage((unsigned char*) greyToScreen->data, greyToScreen->cols, greyToScreen->rows, greyToScreen->step, QImage::Format_RGB888);
//            //greyImg.save("./DropMatrixMatcherData/editedGreyImage.png", 0, 100);
//            //*greyImage = imread("./DropMatrixMatcherData/editedImage.png");
//            *image = greyImg;
//        }
        int w = myLabel->width();
        int h = myLabel->height();

        createAllImages(true);
//        image->save("./DropMatrixMatcherData/editedImage.png", 0, 100);
//        *coloredImage = imread("./DropMatrixMatcherData/editedImage.png");
//        createGreyImage(*coloredImage);

        myLabel->setPixmap(QPixmap::fromImage(*image).scaled(w,h,Qt::KeepAspectRatio));
        // und sag dass colored benutzt werden soll
        this->ui->filterButton->setFlat(false);
        this->ui->contrastSlider_2->setDisabled(false);
        this->ui->brightnessSlider_2->setDisabled(false);


    }else{
         this->ui->filterButton->setFlat(true);
        this->ui->contrastSlider_2->setDisabled(true);
        this->ui->brightnessSlider_2->setDisabled(true);
    }
}

void DropMatrixMatcher::useGreyView()
{
    if (!colored){
        int w = myLabel->width();
        int h = myLabel->height();

        QImage editedImg;
        editedImg.load("./DropMatrixMatcherData/editedImage.png");
        myLabel->setPixmap(QPixmap::fromImage(editedImg).scaled(w,h,Qt::KeepAspectRatio));
        colored = true;
        this->ui->greyView->setFlat(false);
    }else{
        this->filter(4);
        this->ui->greyView->setFlat(true);
        colored = false;
    }
}

void DropMatrixMatcher::filter(int cmyk)
{
    int w = myLabel->width();
    int h = myLabel->height();

    createGreyImage(*coloredImage, cmyk);

    if(cmyk<4)
    {
        //greyToScreen is not grey, it is C,M or Y   !!!!!!
        QImage filteredImg = QImage((unsigned char*) greyToScreen->data, greyToScreen->cols, greyToScreen->rows, greyToScreen->step, QImage::Format_RGB888);
        filteredImg.save("./DropMatrixMatcherData/editedImage.png", 0, 100);
        *coloredImage = imread("./DropMatrixMatcherData/editedImage.png");
        if(colored)
        {
            myLabel->setPixmap(QPixmap::fromImage(filteredImg).scaled(w,h,Qt::KeepAspectRatio));
        }else{
            createGreyImage(*coloredImage);
//            filter(4);
            QImage greyImg = QImage((unsigned char*) greyToScreen->data, greyToScreen->cols, greyToScreen->rows, greyToScreen->step, QImage::Format_RGB888);
            //greyImg.save("./DropMatrixMatcherData/editedGreyImage.png", 0, 100);
            //*greyImage = imread("./DropMatrixMatcherData/editedImage.png");
            myLabel->setPixmap(QPixmap::fromImage(greyImg).scaled(w,h,Qt::KeepAspectRatio));
        }
    }else{     // only used for greyView
        QImage greyQImage = QImage((unsigned char*) greyToScreen->data, greyToScreen->cols, greyToScreen->rows, greyToScreen->step, QImage::Format_RGB888);
        myLabel->setPixmap(QPixmap::fromImage(greyQImage).scaled(w,h,Qt::KeepAspectRatio));
    }
}
//void DropMatrixMatcher::filter(int cmyk)
//{
//    int w = myLabel->width();
//    int h = myLabel->height();

//    createGreyImage(*coloredImage, cmyk);

//    if(cmyk<4)
//    {
//        QImage filteredImg = QImage((unsigned char*) greyToScreen->data, greyToScreen->cols, greyToScreen->rows, greyToScreen->step, QImage::Format_RGB888);
//        filteredImg.save("./DropMatrixMatcherData/editedImage.png", 0, 100);
//        *coloredImage = imread("./DropMatrixMatcherData/editedImage.png");
//        myLabel->setPixmap(QPixmap::fromImage(filteredImg).scaled(w,h,Qt::KeepAspectRatio));
//        if(!colored){
//            filter(4);
//        }
//    }
//    // only used for greyView
//    else{
//        if(!colored){
//            createGreyImage(*coloredImage); //cmyk = 4
//            QImage greyImg = QImage((unsigned char*) greyToScreen->data, greyToScreen->cols, greyToScreen->rows, greyToScreen->step, QImage::Format_RGB888);
//            greyImg.save("./DropMatrixMatcherData/editedGreyImage.png", 0, 100);
//            *greyImage = imread("./DropMatrixMatcherData/editedGreyImage.png");
//            myLabel->setPixmap(QPixmap::fromImage(greyImg).scaled(w,h,Qt::KeepAspectRatio));
//        }

//        QImage greyQImage = QImage((unsigned char*) greyToScreen->data, greyToScreen->cols, greyToScreen->rows, greyToScreen->step, QImage::Format_RGB888);
//        myLabel->setPixmap(QPixmap::fromImage(greyQImage).scaled(w,h,Qt::KeepAspectRatio));
//    }
//}

Mat DropMatrixMatcher::qimage_to_mat_cpy(QImage const &img, int format)
{
    Mat mat(img.height(), img.width(), format);
    for (int i=0;i<img.height();i++) {
            memcpy(mat.ptr(i),img.scanLine(i),img.bytesPerLine());
        }
    return mat;
    //return Mat(img.height(), img.width(), format, img.bits(), img.bytesPerLine()).clone();
    //return Mat(img.height(), img.width(), format, const_cast<uchar*>(img.bits()), img.bytesPerLine()).clone();
    //return Mat(img.height(),img.width(),CV_8UC3,img.byteCount());
}

void DropMatrixMatcher::createAllImages(bool image)                        //image: true / pattern: false
{
    if (image){
        this->image->save("./DropMatrixMatcherData/editedImage.png", 0, 100);
        *coloredImage = imread("./DropMatrixMatcherData/editedImage.png");
        createGreyImage(*coloredImage);
    }else{
        this->pattern->save("./DropMatrixMatcherData/editedPattern.png", 0, 100);
        *coloredPattern = imread("./DropMatrixMatcherData/editedPattern.png");
        createGreyPattern(*coloredPattern);
    }
}

void DropMatrixMatcher::on_pushButton_clicked(){
    HelpWindow *hWindow = new HelpWindow(0);
    hWindow->setVisible(true);
}
