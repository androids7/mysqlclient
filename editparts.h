#ifndef EDITPARTS_H
#define EDITPARTS_H

#include <QDialog>

namespace Ui {
class EditParts;
}
class QLabel;
class QSqlTableModel;
class QSqlRelationalTableModel;
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
    void loadImage();
    void inc();
    void dec();
    void add();
    void del();    
    void first() { id = 0; }
    void last();

private:
    Ui::EditParts *ui;
    int id;
    QLabel *lbl;
    QPixmap *uploadedImage;
    QSqlTableModel *model;
    QDataWidgetMapper *mapper;
};

#endif // EDITPARTS_H
