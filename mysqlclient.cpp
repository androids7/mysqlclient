#include "mysqlclient.h"
#include "ui_mysqlclient.h"
#include <QDebug>
#include <QVariant>
#include <QMessageBox>
//#include <QSql>
#include <QtSql>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QDataWidgetMapper>
#include <QApplication>

#include "connectiondialog.h"
#include "editparts.h"
#include "editprojects.h"
#include "editproviders.h"

MysqlClient::MysqlClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MysqlClient)
{
    db = QSqlDatabase::addDatabase("QMYSQL");

    ui->setupUi(this);
    ui->btnConnect->setEnabled(false);
    connect(ui->comboDatabase, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotComboDatabaseChanged(int)));
    connect(ui->btnConnect, SIGNAL(clicked()),
            this, SLOT(slotFetchData()));
    connect(ui->btnStartEditing, SIGNAL(clicked()),
            this, SLOT(slotStartEditing()));
    connect(ui->btnSubmitEditing, SIGNAL(clicked()),
            this, SLOT(slotSubmitEditing()));
    connect(ui->btnCancelEditing, SIGNAL(clicked()),
            this, SLOT(slotCancelEditing()));
    connect(ui->comboTable, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotAutoConnect()));

    ui->tableView->setSortingEnabled(true);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);    
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(ui->menuOptions->actions()[0], SIGNAL(triggered()),
            this, SLOT(showEditProjectsForm()));
}

void MysqlClient::notImplemented()
{
    QMessageBox::information(this, "Attention!", "This feature has not been implemented yet.");
}

void MysqlClient::showEditProjectsForm()
{
    int id = 1;
    QModelIndex index = ui->tableView->currentIndex();
    if(index.isValid()) {
        QSqlRecord record;
        if(isRelModel)
            record = relModel->record(index.row());
        else {
            record = model->record(index.row());
        }
        id = record.value(0).toInt();
    }    
    //qDebug() << id;
    id--;
    switch(ui->comboTable->currentIndex()) {
    case 2: { /* item "parts" was selected */
        EditParts dlg(id, this);
        dlg.exec();
        }
        break;
    case 3: { /* item "projects" was selected */
        EditProjects dlg(id, this);
        dlg.exec();
        }
        break;
    case 4: { /* item "providers" was selected */
        EditProviders dlg(id, this);
        dlg.exec();
        }
        break;
    default:
        notImplemented();
        return;
    }
    slotFetchData();
    /*
    dlg2.exec();*/
}

void MysqlClient::slotAutoConnect()
{
    if(ui->checkboxAutoConn->isChecked()) {
        slotFetchData();
    }
}

MysqlClient::~MysqlClient()
{
    delete ui;
}

void MysqlClient::slotStartEditing()
{
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->btnStartEditing->setEnabled(false);
    ui->btnSubmitEditing->setEnabled(true);
    ui->btnCancelEditing->setEnabled(true);    
    ui->btnConnect->setEnabled(false);
}
void MysqlClient::slotSubmitEditing()
{
    if(isRelModel)
        relModel->submitAll();
    else
        model->submitAll();
    endEditing();
}
void MysqlClient::slotCancelEditing()
{
    if(isRelModel)
        relModel->revertAll();
    else
        model->revertAll();
    endEditing();
}

void MysqlClient::endEditing()
{
    ui->btnStartEditing->setEnabled(true);
    ui->btnSubmitEditing->setEnabled(false);
    ui->btnCancelEditing->setEnabled(false);
    ui->btnConnect->setEnabled(true);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

bool MysqlClient::createConnection(QString dbname)
{
    db.setDatabaseName(dbname);
    if(!db.open()) {
        QMessageBox::warning(this,
                    tr("Ошибка подключения"), db.lastError().text());
        return false;
    }
    return true;
}

void MysqlClient::slotFetchData()
{
    /*  ERROR! Creation model on stack and passing it to view:
     *
     *  void someFunc( ... ) {
     *      QSqlTableModel model;
     *      ...
     *      view->setModel(&model);
     *  }
     */
    QString databaseName = ui->comboDatabase->currentText();
    QString tableName = ui->comboTable->currentText();
    if(!createConnection(databaseName)) {
        return;
    }
    QStringList fkTables;
    fkTables << "projects" << "providers" << "delivery";
    if(fkTables.contains(tableName)) {
        isRelModel = true;
        relModel = new QSqlRelationalTableModel(this);
        relModel->setTable(tableName);
        if(tableName == "delivery")
            setupDelivery();
        else {
            relModel->setRelation(2, QSqlRelation("cities", "city_ID", "name"));
            relModel->setHeaderData(1, Qt::Horizontal, tr("name"));
            relModel->setHeaderData(2, Qt::Horizontal, tr("city"));
        }
        relModel->setSort(0, Qt::AscendingOrder);
        relModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        relModel->select();
        ui->tableView->setModel(relModel);
        ui->tableView->setItemDelegate(new QSqlRelationalDelegate(this));
    } else {
        isRelModel = false;
        model = new QSqlTableModel;
        model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        model->setTable("`"+tableName+"`");
        model->setSort(0, Qt::AscendingOrder);
        if(!model->select()) {
            QMessageBox::warning(this, "Ошибка извлечения", model->lastError().text());
            return;
        }
        ui->tableView->setModel(model);
    }
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->hideColumn(0);
    ui->tableView->setColumnHidden(4, (tableName == "parts"));
}

void MysqlClient::setupDelivery()
{
    relModel->setRelation(1, QSqlRelation("projects", "project_ID", "name"));
    relModel->setRelation(2, QSqlRelation("providers", "provider_ID", "name"));
    relModel->setRelation(3, QSqlRelation("parts", "part_ID", "name"));
    relModel->setHeaderData(1, Qt::Horizontal, tr("project"));
    relModel->setHeaderData(2, Qt::Horizontal, tr("provider"));
    relModel->setHeaderData(3, Qt::Horizontal, tr("part"));
}

void MysqlClient::showConnectionDialog()
{
    ConnectionDialog dlg(db);
    if(dlg.exec()) {        
        QStringList values { dlg.getValues() };
        username = values.at(0);
        hostname = values.at(1);
        password = values.at(2);
        db.setHostName(hostname);
        db.setUserName(username);
        db.setPassword(password);        
        if(!db.open()) {
            return;
        }
        QSqlQuery query("SHOW DATABASES");
        while(query.next()) {
            ui->comboDatabase->addItem(query.value(0).toString());
        }
        ui->comboDatabase->setCurrentIndex(1);
        db.close();
    } else {        
        exit(0);
    }
}

void MysqlClient::slotComboDatabaseChanged(int index)
{
    QSqlQuery query;
    QString dbname = ui->comboDatabase->itemText(index);
    ui->comboTable->clear();
    createConnection(dbname);
    query.exec("SHOW TABLES");
    QStringList allowed;
    allowed << "cities" << "parts" << "projects"
            << "providers" << "delivery";
    while(query.next()) {
        QString tableName = query.value(0).toString();
        if(dbname == "1291_ziju") {
            if(allowed.contains(tableName))
                ui->comboTable->addItem(tableName);
        } else {
            ui->comboTable->addItem(tableName);
        }
    }
    ui->comboTable->setCurrentIndex(0);    
    ui->btnConnect->setEnabled(true);
    db.close();
}
