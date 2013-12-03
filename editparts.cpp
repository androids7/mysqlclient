#include "editparts.h"
#include "ui_editparts.h"

#include <QIcon>
#include <QDebug>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlRecord>
#include <QSqlRelation>
#include <QSqlDatabase>
#include <QDataWidgetMapper>
#include <QSqlRelationalDelegate>

EditParts::EditParts(int id, QWidget *parent) :
    id(id),
    QDialog(parent),
    ui(new Ui::EditParts)
{
    ui->setupUi(this);

    model = new QSqlTableModel(this);
    model->setTable("parts");
    model->setSort(1, Qt::AscendingOrder);
    model->select();

    //ui->comboBox->setModel(tableModel);
    //ui->comboBox->setModelColumn(tableModel->fieldIndex("name"));

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setModel(model);
    mapper->addMapping(ui->editName, 1);
    mapper->addMapping(ui->editMaterial, 2);
    mapper->addMapping(ui->spinboxWeight, 3);

    if(id == -1) {
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
}

EditParts::~EditParts()
{
    delete ui;
}


void EditParts::inc()
{
    id = qMin(model->rowCount(), id+1);
    mapper->setCurrentIndex(id);
}

void EditParts::dec()
{
    id = qMax(0, id-1);
    mapper->setCurrentIndex(id);
}

void EditParts::accept()
{
    mapper->submit();
}

void EditParts::reject()
{
    mapper->revert();
    QDialog::reject();
}
