#include "file.h"
#include "constants.h"
#include <QDebug>

void File::errorDialogWindow(int num){
    switch(num){
        case 1:{
            QMessageBox *msg = new QMessageBox;
            msg->setText("Внимание");
            msg->setInformativeText("В файле присутсвует запрещенная комбинация символов!!!");
            msg->setStandardButtons(QMessageBox::Cancel);
            msg->exec();

            delete msg;
            break;
        }
        case 2: {
            QMessageBox *msg = new QMessageBox;
            msg->setText("Внимание");
            msg->setInformativeText("Неверный формат!!!!");
            msg->setStandardButtons(QMessageBox::Cancel);
            msg->exec();

            delete msg;
            break;
        }
        default:{
            qDebug() << "Неверное значение";
        }
    }
}

File::File(QString file){
    this->file = file;
}

File::File(){}

QString File::loadInfo(){
    QFileDialog *f_dial = new QFileDialog;
    QFile *fl = new QFile(f_dial->getOpenFileName(0,"","","*.csv"));

    QString r_str;
    r_str.clear();
    QTextStream out(fl);

    if(fl->open(QIODevice::ReadOnly | QIODevice::Text)){
        while(!out.atEnd()){
            r_str = r_str + out.readLine().append("\n");
        }
        fl->close();
    }

    //************************* Работа со спец. символами **************************

    int size = r_str.size();
    for(int k = 0; k < size; k++){
        if((k == 0) && (r_str[k] == "\"")) r_str.replace(k,1,"");
        else if((r_str[k] == "\"") && ((r_str[k+1] == "\n") || (r_str[k-1] == "\n"))) r_str.replace(k,1,"");

        if(r_str[k] == "?") return "NULL";

        //  Обработка символа сплошного печатания

        if(((r_str[k] == ",") && (r_str[k+1] == "\"") && (r_str[k+2] == "\""))){
            r_str.replace(k+1,2,"?");
            k++;
        }
        else if(((r_str[k] == "\"") && (r_str[k+1] == "\"") && (r_str[k+2] == ","))) r_str.replace(k,2,"?");

        if(((k == 0) && (r_str[k] == "\"") && (r_str[k+1] == "\""))) r_str.replace(k,2,"?"); // Крайне левое положение
        else if((r_str[k] == "\"") && (r_str[k+1] == "\"") && (r_str[k+3] == "\n")) r_str.replace(k,2,"?"); // Крайне правое положение

        //
    }

    //*****************************************************************************

    delete f_dial;
    delete fl;

    return r_str;
}

QList<QList<QString> >File::packInfo(QList<QList<QString> > Matrix){
    QList<QList<QString> >rows;
    QList<QString> cell;
    QString str;
    QString end, begin;
    bool isVisit;

    for(int i = 0; i < Matrix.size(); i++){
        for(int j = 0; j < MAX_COL_SIZE - 1; j++){
            int k = 0;
            isVisit = false;
            Matrix[i][j].append("\n");

            while(Matrix[i][j].at(k) != "\n"){
                qDebug() << Matrix[i][j].at(k).unicode();
                if(!((Matrix[i][j].at(k).unicode() >= 1040 && Matrix[i][j].at(k).unicode() <= 1103)
                   || (Matrix[i][j].at(k).unicode() >= 48 && Matrix[i][j].at(k).unicode() <= 57))
                   && isVisit == false){
                        qDebug() << "here";
                        str = str + Matrix[i][j].at(k);
                        begin = "\"";
                        end = "\",";
                        isVisit = true;
                }
                else if(Matrix[i][j].at(k) == "\"") str = str + "\"\"";
                else str = str + Matrix[i][j].at(k);
                k++;
            }

            qDebug() << str;
            str.prepend(begin);
            begin.clear();

            if(end.isEmpty()){
                end = ",";
                str.append(end);
            }
            else str.append(end);
            end.clear();

            cell.append(str);
            qDebug() << cell;
            str.clear();

            Matrix[i][j].replace(Matrix[i][j].size() - 1,1,"");
        }
        rows.append(cell);
        cell.clear();
    }

    return rows;
}

bool File::processInfo(QString raw_str){
    if(raw_str == "NULL"){
       this->errorDialogWindow(1);
       return false;
    }

    //qDebug() << raw_str;

    QList<QString>txt_row;
    QString word;

    for(int k = 0; k < raw_str.size(); k++){
        if(raw_str[k] != "\n"){
            switch (raw_str[k].unicode()) {
                case 34:{
                    while(raw_str[k] == "\""){
                        k++;
                    }
                    break;
                }
                case 63:{
                    k++;
                    while(raw_str[k] != "?"){
                        if(raw_str[k] == "\""){
                            int counter = 0;
                            while(raw_str[k] == "\""){
                                counter++;
                                k++;

                                if(counter == 4){
                                    word = word + "\"";
                                    counter = 0;
                                }
                            }
                        }
                        else{
                            word = word + raw_str[k];
                            k++;
                        }
                    }
                    break;
                }
                case 44:{
                    //qDebug() << word;
                    txt_row.append(word);
                    word.clear();
                    break;
                }
                default:{
                    word = word + raw_str[k];
                    break;
                }
            }
        }else{
            //qDebug() << word;
            txt_row.append(word);
            word.clear();

            if(txt_row.size() <  MAX_COL_SIZE){
                int buff = txt_row.size() + 1;
                for(int i = 0; i < MAX_COL_SIZE - buff; i++){
                    txt_row.append("Значение");
                }
            }

            if(txt_row.size() >= MAX_COL_SIZE){
                this->errorDialogWindow(2);
                return false;
            }

            this->informMatrix.append(txt_row);
            txt_row.clear();
       }
    }

    return true;
}

QList<QList<QString>> File::getDataArray(){
    return this->informMatrix;
}

int File::getArraySize(){
    return this->informMatrix.size();
}
