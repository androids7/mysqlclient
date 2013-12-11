#include "pixmaploaddelegate.h"
#include <QLabel>
#include <QDebug>
#include <QVariant>
#include <QBuffer>
#include <QSqlQuery>
#include <QByteArray>
#include <QModelIndex>

PixmapLoadDelegate::PixmapLoadDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void PixmapLoadDelegate::setEditorData(QWidget *editor,
                                       const QModelIndex &index) const
{
    switch(index.column()) {
    case 4: {
        QLabel *label = qobject_cast<QLabel*>(editor);
        Q_ASSERT(label);
        if(label) {
            QByteArray bytes = index.data(Qt::EditRole).toByteArray();
            QPixmap pixmap;
            if(pixmap.loadFromData(bytes)) {
                label->setPixmap(pixmap);
            } else
                label->setText(tr("Нет изображения"));
        }
    } break;
    default:
        QItemDelegate::setEditorData(editor, index);
    }
}

void PixmapLoadDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                      const QModelIndex &index) const
{
    switch(index.column()) {
    case 4: {
        QLabel *label = qobject_cast<QLabel*>(editor);
        Q_ASSERT(label);
        if(label) {
            QByteArray bytes;
            QBuffer buffer(&bytes);
            buffer.open(QIODevice::WriteOnly);
            if(label->pixmap() == 0x0)
                model->setData(index, QVariant(), Qt::EditRole);
            else
                if(label->pixmap()->save(&buffer, "JPEG")) {
                    model->setData(index, QVariant(bytes), Qt::EditRole);
                }
        }
    } break;
    default:
        QItemDelegate::setModelData(editor, model, index);
    }
}
