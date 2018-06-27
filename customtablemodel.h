#ifndef CUSTOMTABLEMODEL_H
#define CUSTOMTABLEMODEL_H

#include <QModelIndex>
#include <QVariant>
#include <QAbstractTableModel>

class CustomTableModel : public QAbstractTableModel
{
private:
    QList<QList<QString> > list;
    int cols;
    int rows;
public:
    CustomTableModel(QList<QList<QString> > list, int cols, int rows);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
};

#endif // CUSTOMTABLEMODEL_H
