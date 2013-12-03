#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QSqlDatabase>

namespace Ui {
class ConnectionDialog;
}
class QSqlDatabase;
class ConnectionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConnectionDialog(QSqlDatabase parent_db, QWidget *parent = 0);
    ~ConnectionDialog();    
    QStringList getValues();
    void accept();

public slots:
    bool checkConnection(bool silent = false);
    
private:
    Ui::ConnectionDialog *ui;
    QSqlDatabase db;
};

#endif // CONNECTIONDIALOG_H
