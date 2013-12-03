#include "connectiondialog.h"
#include "ui_connectiondialog.h"
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

ConnectionDialog::ConnectionDialog(QSqlDatabase parent_db, QWidget *parent) :
    db(parent_db),
    QDialog(parent),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);
    connect(ui->btnCheck, SIGNAL(clicked()), this, SLOT(checkConnection()));
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

bool ConnectionDialog::checkConnection(bool silent)
{
    db.setUserName(ui->lineEditUsername->text());
    db.setHostName(ui->lineEditHostname->text());
    db.setPassword(ui->lineEditPassword->text());
    if(!db.open()) {
        QMessageBox::warning(this, "Database connection error",
                             "Cannot connect to database: " + db.lastError().text());
        return false;
    }
    if(!silent)
        QMessageBox::information(this, "Connecton checking",
                                 "Connection to database was successful!");
    return true;
}

void ConnectionDialog::accept()
{
    if(checkConnection(true))
        QDialog::accept();
}

QStringList ConnectionDialog::getValues()
{
    QStringList list;
    list.append(ui->lineEditUsername->text());
    list.append(ui->lineEditHostname->text());
    list.append(ui->lineEditPassword->text());
    return list;
}
