#ifndef EDITPROVIDERS_H
#define EDITPROVIDERS_H

#include <QDialog>

namespace Ui {
class EditProviders;
}
class QSqlRelationalTableModel;
class QDataWidgetMapper;
class EditProviders : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditProviders(int id, QWidget *parent = 0);
    ~EditProviders();
    void accept();
    void reject();

public slots:
    void inc();
    void dec();
    void add();
    void del();
    void first() { id = 0; }
    void last();

private:
    Ui::EditProviders *ui;
    int id;
    QSqlRelationalTableModel *model;
    QDataWidgetMapper *mapper;
};

#endif // EDITPROVIDERS_H
