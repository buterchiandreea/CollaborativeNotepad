#include <string.h>
#include <QFileDialog>
#include <QString>
#include <QTextStream>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QMessageBox>

#include "ServerConnection.h"
#include "notepad.h"
#include "ui_notepad.h"
#include "hack_connect.h"

using std::string;


Notepad::Notepad(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Notepad) {
    ui->setupUi(this);
    this->setWindowTitle("New Document");
    this->server = new ServerConnection(Notepad::PORT);
    int code;
    if((code = this->server->Connect()) != 0) {

        QMessageBox messageBox;
        messageBox.critical(0,"Error",strerror(code));
    }


    this->server->WriteCommand("Hello, there!");

}

Notepad::~Notepad() {
    delete ui;
    int code;
    if((code = this->server->CloseServerConnection()) != 0) {

        QMessageBox messageBox;
        messageBox.critical(0,"Error", strerror(code));
    }
}


void Notepad::on_pushButton_clicked()
{
    QString filename;
    QString defaulFileName = windowTitle();
    defaulFileName.append(".txt");

    filename = QFileDialog::getSaveFileName(this, "Save Document", defaulFileName);

    QFile saveFile(filename);
    if(saveFile.open(QFile::WriteOnly | QFile::Text)) {

        QTextStream out(&saveFile);
        out << ui->textEdit->toPlainText();
        saveFile.flush();
        saveFile.close();
    }

}



void Notepad::on_textEdit_textChanged()
{
    QString currentText;
    currentText = ui->textEdit->toPlainText();

    this->previousText = currentText;
}




