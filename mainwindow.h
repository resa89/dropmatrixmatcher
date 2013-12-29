#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private slots:
    void on_loadPattern_clicked();
    void on_loadImage_clicked();
    void on_findButton_clicked();
    void enableFindButton();

private:
    Ui::MainWindow *ui;
    QImage *pattern;
    QImage *image;
    QString *patternPath;
    QString *imagePath;
};

#endif // MAINWINDOW_H
