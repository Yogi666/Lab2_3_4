#ifndef FILE_H
#define FILE_H

#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

class File{
private:
    QString file;
    QList<QList<QString>>informMatrix;

    void set_info();
    void errorDialogWindow(int num);
public:
    File(QString file);
    File();

    QString loadInfo();
    QList<QList<QString> > packInfo(QList<QList<QString> > Matrix);

    QList<QList<QString> > getDataArray();

    bool processInfo(QString raw_str);
    int getArraySize();
};

#endif // FILE_H
