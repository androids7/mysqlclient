#ifndef EDITPARTS_H
#define EDITPARTS_H

#include <QDialog>

namespace Ui {
class EditParts;
}
class QSqlTableModel;
class QDataWidgetMapper;
class EditParts : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditParts(int id, QWidget *parent = 0);
    ~EditParts();
    void accept();
    void reject();

public slots:
    void inc();
    void dec();

private:
    Ui::EditParts *ui;
    int id;
    QSqlTableModel *model;
    QDataWidgetMapper *mapper;
};

#endif // EDITPARTS_H
