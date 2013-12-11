#ifndef PIXMAPLOADDELEGATE_H
#define PIXMAPLOADDELEGATE_H

#include <QItemDelegate>

class PixmapLoadDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit PixmapLoadDelegate(QObject *parent = 0);
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;
};

#endif // PIXMAPLOADDELEGATE_H
