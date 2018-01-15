#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QWidget>
#include <QString>
#include <string.h>
#include "ServerConnection.h"



namespace Ui {
class Notepad;
}

class Notepad : public QWidget
{
    Q_OBJECT

public:
    explicit Notepad(QWidget *parent = 0);
    ~Notepad();

private:
    static const int PORT = 6969;
    ServerConnection *server;
    QString previousText;

private slots:

    void on_pushButton_clicked();

    void on_textEdit_textChanged();

private:
    Ui::Notepad *ui;
};

#endif // NOTEPAD_H
