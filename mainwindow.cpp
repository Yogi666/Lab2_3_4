#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "file.h"
#include "constants.h"
#include "customtablemodel.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->posComboBox->addItem("Значение");
    ui->posComboBox->addItem("Стажер");
    ui->posComboBox->addItem("Программист");
    ui->posComboBox->addItem("Ведущий программист");
    ui->ageComboBox->addItem("Значение");
    for(int i = 1; i < 100; i++){
        ui->ageComboBox->addItem(QString::number(i));
    }

    this->tableCols = COL_SIZE;
    this->currIndex = 0;

    this->createDefaultTableView();
}

void MainWindow::createTableView(){
    CustomTableModel *model = new CustomTableModel(this->Matrix, COL_SIZE, this->Matrix.size());
    ui->mainTable->setModel(model);
}

void MainWindow::setDefaultMatrix(){
    this->tableRows = 1;
    QList<QString>buff;

    for(int j = 0; j < MAX_COL_SIZE; j++){
        buff.append("Значение");
    }

    this->Matrix.append(buff);
    buff.clear();
}

void MainWindow::errorMessage(){
    QMessageBox *msg = new QMessageBox;
    msg->setText("Внимание");
    msg->setInformativeText("В таблице нет записей!");
    msg->setStandardButtons(QMessageBox::Cancel);
    msg->exec();
}

void MainWindow::createDefaultTableView(){
    this->setDefaultMatrix();
    CustomTableModel *model = new CustomTableModel(this->Matrix, COL_SIZE, this->Matrix.size());
    ui->mainTable->setModel(model);
}

void MainWindow::on_removePushButton_clicked(){
    if(this->tableRows <= 0) this->errorMessage();
    else{
        int currRow = ui->mainTable->currentIndex().row();
        this->Matrix.removeAt(currRow);
        this->tableRows--;
        this->createTableView();
    }
}

void MainWindow::on_addPushButton_clicked(){
    QList<QString>row;
    row.append(ui->nameLineEdit->text());
    row.append(ui->lastnameLineEdit->text());
    row.append(ui->mfnumbeLineEdit->text());
    row.append(ui->posComboBox->currentText());
    row.append(ui->ageComboBox->currentText());
    row.append(ui->graduateLineEdit->text());

    this->tableRows++;
    this->Matrix.append(row);
    this->createTableView();
}

void MainWindow::on_savePushButton_clicked(){
    this->Matrix[this->currIndex][0] = ui->nameLineEdit->text();
    this->Matrix[this->currIndex][1] = ui->lastnameLineEdit->text();
    this->Matrix[this->currIndex][2] = ui->mfnumbeLineEdit->text();
    this->Matrix[this->currIndex][3] = ui->posComboBox->currentText();
    this->Matrix[this->currIndex][4] = ui->ageComboBox->currentText();
    this->Matrix[this->currIndex][5] = ui->graduateLineEdit->text();

    this->createTableView();
}

void MainWindow::on_mainTable_clicked(const QModelIndex &index){
    ui->nameLineEdit->clear();
    ui->lastnameLineEdit->clear();
    ui->mfnumbeLineEdit->clear();
    ui->graduateLineEdit->clear();
    this->currIndex = index.row();

    QList<QString>row;
    for(int i = 0; i < MAX_COL_SIZE - 1; i++){
        row.append(this->Matrix[index.row()][i]);
    }

    ui->nameLineEdit->insert(row[0]);
    ui->lastnameLineEdit->insert(row[1]);
    ui->mfnumbeLineEdit->insert(row[2]);
    ui->graduateLineEdit->insert(row[5]);

    int buff = 1;
    while(buff != ui->posComboBox->count()){
        ui->posComboBox->setCurrentIndex(buff);
        if(ui->posComboBox->currentText() == row[3]) break;
        else if(ui->posComboBox->count() - buff == 1) ui->posComboBox->setCurrentIndex(0);
        buff++;
    }

    buff = 1;
    while(buff != ui->ageComboBox->count()){
        ui->ageComboBox->setCurrentIndex(buff);
        if(ui->ageComboBox->currentText() == row[4]) break;
        else if(ui->ageComboBox->count() - buff == 1) ui->ageComboBox->setCurrentIndex(0);
        buff++;
    }
}

void MainWindow::on_uploadAction_triggered(){
    File *fl = new File("person.csv");
    bool trigg = fl->processInfo(fl->loadInfo());

    if(trigg == false) this->createDefaultTableView();
    else{
        this->Matrix = fl->getDataArray();
        this->tableRows = fl->getArraySize();
        this->createTableView();
    }
}

void MainWindow::on_SaveAsAction_triggered(){
    QFileDialog *fd = new QFileDialog;
    fd->setFileMode(QFileDialog::Directory);
    fd->exec();
    QFile saveFile(fd->directory().absolutePath()+"/output.csv");
    delete fd;

    if(saveFile.open(QIODevice::WriteOnly)){
         QList<QList<QString> >info;
         File *fl = new File();
         info = fl->packInfo(this->Matrix);

         QTextStream in(&saveFile);
         for(int i = 0; i < info.size(); i++){
             for(int j = 0; j < MAX_COL_SIZE - 1; j++){
                 in << info.at(i).at(j);
             }
             in << "\r\n";
         }

         saveFile.close();
    }
}

MainWindow::~MainWindow(){
    delete ui;
}
