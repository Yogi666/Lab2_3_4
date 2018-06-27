#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QMenu>
#include <QMenuBar>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QList<QList<QString> > Matrix;
    short int tableCols;
    short int tableRows;
    short int currIndex;

public:
    explicit MainWindow(QWidget *parent = 0);
    void createDefaultTableView();
    void createTableView();
    void setDefaultMatrix();
    void errorMessage();
    ~MainWindow();

private slots:
    void on_removePushButton_clicked();
    void on_addPushButton_clicked();
    void on_savePushButton_clicked();
    void on_mainTable_clicked(const QModelIndex &index);

    void on_uploadAction_triggered();
    void on_SaveAsAction_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
