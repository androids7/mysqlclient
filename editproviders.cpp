#include "editproviders.h"
#include "ui_editproviders.h"

#include <QDebug>
#include <QVariant>
#include <QMessageBox>
#include <QtSql>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QDataWidgetMapper>

#include "connectiondialog.h"

EditProviders::EditProviders(int id, QWidget *parent) :
    id(id),
    QDialog(parent),
    ui(new Ui::EditProviders)
{
    ui->setupUi(this);

    model = new QSqlRelationalTableModel(this);
    model->setTable("providers");
    model->setRelation(2, QSqlRelation("cities", "city_ID", "name"));
    model->setSort(0, Qt::AscendingOrder);
    model->select();

    QSqlTableModel *tableModel = model->relationModel(2);
    ui->comboBox->setModel(tableModel);
    ui->comboBox->setModelColumn(tableModel->fieldIndex("name"));

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setModel(model);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(ui->editProviderName, 1);
    mapper->addMapping(ui->comboBox, 2);

    if(id == 0) {
        mapper->toFirst();
    } else {
        for(int i = 0; i < model->rowCount(); ++i) {
            QSqlRecord rec = model->record(i);
            if(rec.value(0).toInt() == id) {
                mapper->setCurrentIndex(i+1);
                break;
            }
        }
    }
    connect(ui->btnNext, SIGNAL(clicked()), mapper, SLOT(toNext()));
    connect(ui->btnNext, SIGNAL(clicked()), this, SLOT(inc()));
    connect(ui->btnPrev, SIGNAL(clicked()), mapper, SLOT(toPrevious()));
    connect(ui->btnPrev, SIGNAL(clicked()), this, SLOT(dec()));
    connect(ui->btnAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui->btnDel, SIGNAL(clicked()), this, SLOT(del()));
    connect(ui->btnFirst, SIGNAL(clicked()), mapper, SLOT(toFirst()));
    connect(ui->btnLast, SIGNAL(clicked()), mapper, SLOT(toLast()));
    connect(ui->btnFirst, SIGNAL(clicked()), this, SLOT(first()));
    connect(ui->btnLast, SIGNAL(clicked()), this, SLOT(last()));
}

EditProviders::~EditProviders()
{
    delete ui;
}

void EditProviders::add()
{
    int row = mapper->currentIndex();
    mapper->submit();
    model->insertRow(row);
    mapper->setCurrentIndex(row);
    ui->editProviderName->clear();
    ui->comboBox->setCurrentIndex(0);
    ui->editProviderName->setFocus();
    id = mapper->currentIndex();
}

void EditProviders::del()
{
    int row = mapper->currentIndex();
    model->removeRow(row);
    mapper->submit();
    mapper->setCurrentIndex(qMin(row, model->rowCount()-1));
}

void EditProviders::inc()
{
    qDebug() << id;
    id = qMin(model->rowCount()-1, id+1);
    mapper->setCurrentIndex(id);
}

void EditProviders::dec()
{
    id = qMax(0, id-1);
    mapper->setCurrentIndex(id);
}

void EditProviders::last()
{
    id = model->rowCount() - 1;
}

void EditProviders::accept()
{
    mapper->submit();
}

void EditProviders::reject()
{
    mapper->revert();
    QDialog::reject();
}
