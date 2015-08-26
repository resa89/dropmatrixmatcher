#include <QtWidgets>
#include <QtSql>
#include <iostream>

#include "window.h"

Window::Window(QWidget *parent, QString imagepath)
: QWidget(parent)
{
        localpath = imagepath;
        std::cout << localpath.toStdString() << std::endl;

        setupModel();
        setPrinter();
        setPrinterModel();
        setDocumentType();
        setCountry();
        setDocumentName();

        printerLabel = new QLabel(tr("Printer:"));
        comboBoxPrinter = new QComboBox();
        comboBoxPrinter->setModel(typePrinterModel);
        printerModelLabel = new QLabel(tr("Model"));
        comboBoxPrinterModel = new QComboBox();
        comboBoxPrinterModel->setModel(typeModelPrinterModel);
        documentTypeLabel = new QLabel(tr("Document type"));
        comboBoxDocumentType = new QComboBox();
        comboBoxDocumentType->setModel(typeDocumentType);
        countryLabel = new QLabel(tr("Country"));
        comboBoxCountry = new QComboBox();
        comboBoxCountry->setModel(countryType);
        documentNameLabel = new QLabel(tr("Document name"));
        comboBoxDocumentName = new QComboBox();
        comboBoxDocumentName->setModel(documentName);
        sendButton = new QPushButton(tr("&Send"));
        mapper = new QDataWidgetMapper(this);

        setPrinterManufacturer();
        setDocumentTypeName();
        setDocumentCurrencyType();
        setCountry(comboBoxDocumentType->currentText());
        setModel(comboBoxPrinter->currentText());
        setDocument(comboBoxDocumentType->currentText());

        printerLabel->setBuddy(comboBoxPrinter);
        printerModelLabel->setBuddy(comboBoxPrinterModel);
        documentTypeLabel->setBuddy(comboBoxDocumentType);
        countryLabel->setBuddy(comboBoxCountry);
        documentNameLabel->setBuddy(comboBoxDocumentName);

//        connect(sendButton, SIGNAL(clicked()),
//                    this, SLOT(toUpload()));
        connect(sendButton, SIGNAL(clicked()),
                    this, SLOT(toUpload()));
        connect(comboBoxPrinter, SIGNAL(currentIndexChanged(QString)),
                this, SLOT(setModel(QString)));
        connect(comboBoxDocumentType, SIGNAL(currentIndexChanged(QString)),
                this, SLOT(setDocument(QString)));
        connect(comboBoxDocumentType, SIGNAL(currentIndexChanged(QString)),
                this, SLOT(setCountry(QString)));

      QGridLayout *layout = new QGridLayout();
            layout->addWidget(printerLabel, 0, 0, 1, 1);
            layout->addWidget(comboBoxPrinter, 0, 1, 1, 1);
            layout->addWidget(printerModelLabel, 1, 0, 1, 1);
            layout->addWidget(comboBoxPrinterModel, 1, 1, 1, 1);
            layout->addWidget(documentTypeLabel, 2, 0, 1, 1);
            layout->addWidget(comboBoxDocumentType, 2, 1, 1, 1);
            layout->addWidget(countryLabel, 3, 0, 1, 1);
            layout->addWidget(comboBoxCountry, 3, 1, 1, 1);
            layout->addWidget(documentNameLabel,4, 0, 1, 1);
            layout->addWidget(comboBoxDocumentName, 4, 1, 1, 1);
            layout->addWidget(sendButton, 6, 1, 1, 1);
            setLayout(layout);

            setWindowTitle(tr("Insert"));
}

void Window::setupModel()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("141.37.176.19");
    db.setDatabaseName("dmm");
    db.setUserName("root");
    db.setPassword("dmm$ios");
    if (!db.open()) {
        QMessageBox::critical(0, tr("Cannot open database"),
            tr("Unable to establish a database connection."),
            QMessageBox::Cancel);
        return;
    }
}

void Window::updateButtons(int row)
{
     sendButton->setEnabled(row < model->rowCount() - 1);
     uploadButton->setEnabled(row < model->rowCount() - 1);
     downloadButton->setEnabled(row < model->rowCount() - 1);
}
 void Window::setPrinter()
{
    QStringList printer;
    typePrinterModel = new QStringListModel(printer, this);
}

void Window::setPrinterModel(){

    QStringList printerModel;
    typeModelPrinterModel = new QStringListModel(printerModel, this);
}

void Window::setDocumentType(){

    QStringList documentType;
    typeDocumentType = new QStringListModel(documentType, this);
}

void Window::setCountry(){

    QStringList country;
    countryType = new QStringListModel(country, this);
}

void Window::setDocumentName(){

    QStringList document;
    documentName = new QStringListModel(document, this);
}

/*void Window::setUpMapper(){
    model = new QSqlRelationalTableModel(this);
    model->setTable("DOCUMENT_PRINTER_SUMMERY");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    indexDocumentName = model->fieldIndex("ID_DOCUMENT");

    model->setRelation(indexDocumentName,
    QSqlRelation("DOCUMENT_TYPE", "ID", "DOCUMENT_NAME"));
    QSqlTableModel *relModel5 = model->relationModel(indexDocumentName);
    comboBoxDocumentName->setModel(relModel5);
    comboBoxDocumentName->setModelColumn(relModel5->fieldIndex("DOCUMENT_NAME"));

    mapper->setModel(model);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(comboBoxDocumentName, indexDocumentName);

}*/

void Window::setPrinterManufacturer(){
    QSqlQuery query;
    query.prepare("SELECT DISTINCT manufacturer FROM PRINTER");
    query.exec();
    QString printer;
    while (query.next()) {
            printer = query.value(0).toString();
            comboBoxPrinter->addItem(printer);
            //qDebug() << type;
     }
}

void Window::setDocumentCurrencyType(){
    QSqlQuery query;
    query.prepare("SELECT DISTINCT document_name FROM DOCUMENT_TYPE");
    query.exec();
    QString document;
    while (query.next()) {
            document = query.value(0).toString();
            comboBoxDocumentName->addItem(document);
            //qDebug() << type;
     }
}


void Window::setModel(QString printer){

    if(printer =="HP"){
        comboBoxPrinterModel->clear();
        QSqlQueryModel *model = new QSqlQueryModel (comboBoxPrinterModel);
        model->setQuery ("SELECT model FROM PRINTER WHERE manufacturer = 'HP'");
        comboBoxPrinterModel->setModel(model);
    }
     else if(printer =="Canon"){
        comboBoxPrinterModel->clear();
        QSqlQueryModel *model = new QSqlQueryModel (comboBoxPrinterModel);
        model->setQuery ("SELECT model FROM PRINTER WHERE manufacturer = 'Canon'");
        comboBoxPrinterModel->setModel(model);
    }
      else if(printer == "Epson"){
          comboBoxPrinterModel->clear();
          QSqlQueryModel *model = new QSqlQueryModel (comboBoxPrinterModel);
          model->setQuery ("SELECT model FROM PRINTER WHERE manufacturer = 'Epson'");
          comboBoxPrinterModel->setModel(model);
      }
    else if(printer == "Brother"){
        comboBoxPrinterModel->clear();
        QSqlQueryModel *model = new QSqlQueryModel (comboBoxPrinterModel);
        model->setQuery ("SELECT model FROM PRINTER WHERE manufacturer = 'Brother'");
        comboBoxPrinterModel->setModel(model);
    }
}

void Window::setDocument(QString document){
    if(document=="currency"){
        comboBoxDocumentName->setDisabled(false);
        QSqlQueryModel *model = new QSqlQueryModel (comboBoxCountry);
        model->setQuery ("SELECT country FROM DOCUMENT_TYPE WHERE type = 'currency'");
        comboBoxCountry->setModel(model);
    }
    else{
        comboBoxDocumentName->setDisabled(true);
    }
}

void Window::setCountry(QString documentType){
    if(documentType =="id-card"){
        comboBoxCountry->clear();
        QSqlQueryModel *model = new QSqlQueryModel (comboBoxCountry);
        model->setQuery ("SELECT country FROM DOCUMENT_TYPE WHERE TYPE = 'id-card'");
        comboBoxCountry->setModel(model);
       }
    else if(documentType =="contract"){
        comboBoxCountry->clear();
        QSqlQueryModel *model = new QSqlQueryModel (comboBoxCountry);
        model->setQuery ("SELECT country FROM DOCUMENT_TYPE WHERE TYPE = 'contract'");
        comboBoxCountry->setModel(model);
       }
}

void Window::setDocumentTypeName(){
    QSqlQuery query;
    query.prepare("SELECT DISTINCT type FROM DOCUMENT_TYPE");
    query.exec();
    QString type;
    while (query.next()) {
            type = query.value(0).toString();
            comboBoxDocumentType->addItem(type);
            //qDebug() << type;
     }
}

void Window::toSend(){

    QSqlQuery query;
    query.prepare( "INSERT INTO DOCUMENT_PRINTER_SUMMERY (picture_path, id_document, id_printer) VALUES (:picture_path, :id_document, :id_printer)" );
    query.bindValue(":picture_path", externalpath);

    //QString selectedPrinter = getCurrentPrinter(comboBoxPrinter->currentText());
    QSqlQuery query2;
    int printer;
    query2.prepare("SELECT id FROM PRINTER WHERE manufacturer = :manufacturer");
    query2.bindValue(":manufacturer", comboBoxPrinter->currentText());
    query2.exec();
    while (query2.next()) {
            printer = query2.value(0).toInt();
            //qDebug() << printer;
     }
     std::cout << printer << std::endl;

     QSqlQuery query3;
     int document;
     query3.prepare("SELECT id FROM DOCUMENT_TYPE WHERE type = :type");
     query3.bindValue(":type", comboBoxDocumentType->currentText());
     query3.exec();
     while (query3.next()) {
             document = query3.value(0).toInt();
             //qDebug() << document;
      }
     std::cout << document << std::endl;

    query.bindValue(":id_document", document);
    query.bindValue(":id_printer", printer);
      if( !query.exec() ){
           qDebug() << query.lastError();
      }
           else{
             qDebug( "Inserted!" );
             QMessageBox msgBox;
             msgBox.setText("Your data was inserted");
             msgBox.exec();
      }
}

void Window::toUpload(){
   // std::cout << localpath.toStdString() << std::endl;
    QFile* file = new QFile(localpath);
    //QFileInfo fileInfo(file.fileName());
    //QString filename(fileInfo.fileName());
    //QString filename = file->fileName();
    QStringList parts = localpath.split("/");
    QString filename = parts.at(parts.size()-1);

    //std::cout << filename.toStdString() << std::endl;
    //std::cout << localpath.toStdString() << std::endl;
    QString printer = comboBoxPrinter->currentText();
    QString printerModel = comboBoxPrinterModel->currentText();
    QString folder = printer.append(printerModel);
    std::cout << folder.toStdString() << std::endl;
    externalpath = "ftp://141.37.176.19/printer/"+folder+"/"+filename;
    std::cout << externalpath.toStdString() << std::endl;
    QUrl uploadurl(externalpath);
        uploadurl.setUserName("dmm");
        uploadurl.setPassword("dmm$ios");
        uploadurl.setPort(21);
        QNetworkRequest upload(uploadurl);
        QNetworkAccessManager *uploadman = new QNetworkAccessManager(this);


        //QFile* file = new QFile("C:\Users\sabrina\Desktop\Sonstiges\AgileVorgehensmodelleAufgabe.pdf");
        if(!file->exists()){
            qDebug() << "file not exists";
        }
        if(file->open(QIODevice::ReadOnly)) {
             QNetworkReply *rep =
             uploadman->put(upload, file);

              qDebug() << "Uploaded";
              QEventLoop event;
              connect(uploadman,SIGNAL(finished(QNetworkReply *)),&event, SLOT(quit()));
              event.exec();

              std::cout << rep->error() << std::endl;

        }
        else
            qDebug() << "Could not open file";

    if (!uploadurl.isValid()) {
        QMessageBox::critical(0, tr("Cannot open connection to ftp server"),
            tr("Unable to establish a ftp connection."),
            QMessageBox::Cancel);
        return;
    }

    toSend();
}

