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
    model->setSort(0, Qt::AscendingOrder);
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

EditProjects::~EditProjects()
{
    delete ui;
}

void EditProjects::add()
{
    int row = mapper->currentIndex();
    mapper->submit();    
    model->insertRow(row);
    mapper->setCurrentIndex(row);
    ui->editProjectName->clear();
    ui->comboBox->setCurrentIndex(0);
    ui->editProjectName->setFocus();
    id = mapper->currentIndex();
}

void EditProjects::del()
{
    int row = mapper->currentIndex();
    model->removeRow(row);
    mapper->submit();
    mapper->setCurrentIndex(qMin(row, model->rowCount()-1));
}

void EditProjects::inc()
{
    //qDebug() << "old inc: " << id;
    id = qMin(model->rowCount()-1, id+1);
    mapper->setCurrentIndex(id);
    //qDebug() << "new inc: " << id;

}

void EditProjects::dec()
{
    //qDebug() << "old dec: " << id;
    id = qMax(0, id-1);
    mapper->setCurrentIndex(id);
    //qDebug() << "new dec: " << id;
}

void EditProjects::last()
{
    id = model->rowCount() - 1;
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
