#include <QApplication>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>
#include <QVariant>
#include "mysqlclient.h"

#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>

#include <QTextCodec>

#include "editprojects.h"

void test() {
    //QStringList QSqlDatabase::tables(QSql::Tables);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("1291_ziju");
    db.setPassword("123");
    db.open();
    QSqlQuery query("show databases", db);
    while(query.next())
        qDebug() << query.value(0);
    exit(0);
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QTextCodec *c = QTextCodec::codecForLocale();
    QTextCodec::setCodecForTr(c);
    //test();
    //EditProjects dlg;
    //dlg.show();
    MysqlClient client;
    client.show();
    client.showConnectionDialog();
    return app.exec();
}
