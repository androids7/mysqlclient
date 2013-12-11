#ifndef MYSQLCLIENT_H
#define MYSQLCLIENT_H

#include <QMainWindow>
#include <QString>
#include <QSqlDatabase>


namespace Ui {
class MysqlClient;
}
class QDataWidgetMapper;
class QSqlRelationalDatabase;
class QSqlRelationalTableModel;
class QSqlTableModel;

class MysqlClient : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MysqlClient(QWidget *parent = 0);
    ~MysqlClient();
    bool createConnection(QString dbname);
    void showConnectionDialog();
    QSqlDatabase &getDb() { return db; }

public slots:
    void slotComboDatabaseChanged(int index);
    void slotSubmitEditing();
    void slotStartEditing();
    void slotCancelEditing();
    void slotFetchData();
    void slotAutoConnect();
    void showEditProjectsForm();

private:    
    void endEditing();
    void setupDelivery();

    bool isRelModel;
    void notImplemented();

    Ui::MysqlClient *ui;
    QSqlDatabase db;
    QSqlTableModel *model;
    QDataWidgetMapper *mapper;
    QSqlRelationalTableModel *relModel;

    QString hostname;
    QString username;
    QString password;
};

#endif // MYSQLCLIENT_H
