#include "customtablemodel.h"
#include "constants.h"
#include <QDebug>

int CustomTableModel::rowCount(const QModelIndex &parent) const{
    return this->rows;
}

int CustomTableModel::columnCount(const QModelIndex &parent) const{
    return this->cols;
}

QVariant CustomTableModel::data(const QModelIndex &index, int role) const{
    if (role == Qt::DisplayRole) {
        QString answer = QString(this->list[index.row()][index.column()]);
        return(QVariant(answer));
    }
    return(QVariant());
}

CustomTableModel::CustomTableModel(QList<QList<QString> > list, int cols, int rows){
    this->list = list;
    this->cols = cols;
    this->rows = rows;
}

QVariant CustomTableModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(role != Qt::DisplayRole) {
        return(QVariant());
    }
    if(orientation == Qt::Horizontal) {
        switch(section) {
        case 0:
            return(QVariant(QString::fromUtf8("Имя")));

        case 1:
            return(QVariant(QString::fromUtf8("Фамилия")));

        case 2:
            return(QVariant(QString::fromUtf8("Зарплата")));

        case 3:
            return(QVariant(QString::fromUtf8("Должность")));

        case 4:
            return(QVariant(QString::fromUtf8("Возраст")));

        case 5:
            return(QVariant(QString::fromUtf8("Образование")));

        default:
            return(QVariant());
        }
    }
    return(QVariant());
}
