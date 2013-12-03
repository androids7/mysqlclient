#ifndef EDITPROJECTS_H
#define EDITPROJECTS_H

#include <QDialog>

namespace Ui {
class EditProjects;
}
class QSqlRelationalTableModel;
class QDataWidgetMapper;
class EditProjects : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditProjects(int id, QWidget *parent = 0);
    ~EditProjects();
    void accept();
    void reject();

public slots:
    void inc();
    void dec();
    void add();
    void del();

private:
    Ui::EditProjects *ui;
    int id;
    QSqlRelationalTableModel *model;
    QDataWidgetMapper *mapper;
};

#endif // EDITPROJECTS_H
