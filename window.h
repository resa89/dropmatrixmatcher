#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QString>
#include <QtGui>
#include <iostream>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class QComboBox;
class QDataWidgetMapper;
class QItemSelectionModel;
class QLabel;
class QMessageBox;
class QLineEdit;
class QPushButton;
class QRadioButton;
class QSqlRelationalTableModel;
class QStandardItemModel;
class QStringListModel;
class QTextEdit;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0, QString imagepath = "/default");

public slots:
    void updateButtons(int row);
    void toSend();
    void setModel(QString printer);
    void setDocument(QString document);
    void setCountry(QString documentType);
     void toUpload();

private:
    void setupModel();
    void setPrinter();
    void setPrinterModel();
    void setDocumentType();
    void setCountry();
    void setDocumentName();
    void setUpMapper();
    void setDocumentTypeName();
    void setPrinterManufacturer();
    void setDocumentCurrencyType();
//    QLineEdit *otherDocument;
    QLabel *printerLabel;
    QLabel *printerModelLabel;
    QLabel *documentTypeLabel;
    QLabel *countryLabel;
    QLabel *documentNameLabel;
//    QLabel *amountLabel;
//    QLabel *otherDocumentLabel;
    QComboBox *comboBoxPrinter;
    QComboBox *comboBoxPrinterModel;
    QComboBox *comboBoxCountry;
    QMessageBox *msgBox;
//    QComboBox *comboBoxAmount;
    QComboBox *comboBoxDocumentType;
    QComboBox *comboBoxDocumentName;
    QPushButton *uploadButton;
    QPushButton *downloadButton;
    QPushButton *sendButton;

    QSqlRelationalTableModel *model;
    //QStandardItemModel *model;
    //QItemSelectionModel *selectionModel;
    QDataWidgetMapper *mapper;
    QStringListModel *typePrinterModel;
    QStringListModel *typeModelPrinterModel;
    QStringListModel *typeDocumentType;
    QStringListModel *countryType;
    QStringListModel *amountType;
    QStringListModel *documentName;
    int indexPrinter;
    int indexPrinterModel;
    int indexDocumentCountry;
    int indexDocumentType;
    int indexDocumentName;
    QString externalpath;
    QString localpath;
};

#endif
