#include "editprojects.h"
#include "ui_editprojects.h"

#include <QIcon>
#include <QDebug>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlRecord>
#include <QSqlRelation>
#include <QSqlDatabase>
#include <QDataWidgetMapper>
#include <QSqlRelationalDelegate>

EditProjects::EditProjects(int id, QWidget *parent) :
    id(id),
    QDialog(parent),
    ui(new Ui::EditProjects)
{
    ui->setupUi(this);

    model = new QSqlRelationalTableModel(this);
    model->setTable("projects");
    model->setRelation(2, QSqlRelation("cities", "city_ID", "name"));
    model->setSort(1, Qt::AscendingOrder);
    model->select();

    QSqlTableModel *tableModel = model->relationModel(2);
    ui->comboBox->setModel(tableModel);
    ui->comboBox->setModelColumn(tableModel->fieldIndex("name"));

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setModel(model);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(ui->editProjectName, 1);
    mapper->addMapping(ui->comboBox, 2);

    if(id == 0) {
        mapper->toFirst();
    } else {
        for(int i = 0; i < model->rowCount(); ++i) {
            QSqlRecord rec = model->record(i);
            if(rec.value(0).toInt() == id) {
                mapper->setCurrentIndex(i);
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
}

EditProjects::~EditProjects()
{
    delete ui;
}

void EditProjects::add()
{
    mapper->submit();
    id++;
    model->insertRow(id);
    mapper->setCurrentIndex(id);
    ui->editProjectName->clear();
    ui->comboBox->setCurrentIndex(0);
    ui->editProjectName->setFocus();
}

void EditProjects::del()
{

}

void EditProjects::inc()
{
    //qDebug() << "old inc: " << id;
    id = qMin(model->rowCount()-1, id+1);
    //qDebug() << "new inc: " << id;
    mapper->setCurrentIndex(id);
}

void EditProjects::dec()
{
    //qDebug() << "old dec: " << id;
    id = qMax(0, id-1);
    //qDebug() << "new dec: " << id;
    mapper->setCurrentIndex(id);
}

void EditProjects::accept()
{
    mapper->submit();
}

void EditProjects::reject()
{
    mapper->revert();
    QDialog::reject();
}
