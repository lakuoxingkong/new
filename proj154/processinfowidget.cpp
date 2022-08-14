#include "processinfowidget.h"
#include "ui_processinfowidget.h"
#include<QFile>
#include<QDebug>
#include<QMessageBox>
#include<fstream>
#include<sstream>
#include<iostream>

ProcessInfoWidget::ProcessInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProcessInfoWidget)
{
    ui->setupUi(this);

//    std::ifstream ifile;
//    ifile.open("/proc/realtime_probe_tool/process_info",std::ios::in);
//    std::ifstream ifs("/home/ye/sample.txt");
//        std::stringstream buffer;
//        buffer << ifs.rdbuf();
//        std::string str(buffer.str());
//        std::cout << str;
//        ifs.close();


    currPage=1;
    maxPage=20;
    QFile file;
    file.setFileName("/proc/realtime_probe_tool/process_info");
    //file.setFileName("/home/ye/sample.txt");
    file.open(QIODevice::ReadOnly);
//    QString info=file.readAll();
//    qDebug()<<file.readAll();
//    info.append(QString(file.readAll()));

    QByteArray infoArray;
    QByteArray tmp;
    do {
        tmp=file.readLine();
        infoArray.append(tmp);
    } while(!tmp.isEmpty());
    QString info=infoArray;

    QStringList infoList=info.split("-- End item --");
    maxPage=infoList.size();
    if(!info.isEmpty()) {
        ui->lineEdit->setText(QString::number(currPage));
        ui->textBrowser->setText(info);
    }

    connect(ui->pushButton_1,&QPushButton::clicked,[=](){
        if(currPage > 1) {
            --currPage;
            ui->lineEdit->setText(QString::number(currPage));
            ui->textBrowser->setText(infoList.at(currPage-1));
        }
    });
    connect(ui->pushButton_2,&QPushButton::clicked,[=](){
        if(currPage < maxPage) {
            ++currPage;
            ui->lineEdit->setText(QString::number(currPage));
            ui->textBrowser->setText(infoList.at(currPage-1));
        }
    });
    connect(ui->pushButton_3,&QPushButton::clicked,[=](){
        int targetPage=ui->lineEdit->text().toInt();
        if(targetPage <= maxPage && targetPage >= 1) {
            currPage=targetPage;
            ui->textBrowser->setText(infoList.at(currPage-1));
        } else {
            QMessageBox::critical(this,"错误","输入的记录序号超出范围，请重新输入");
        }
    });
}

ProcessInfoWidget::~ProcessInfoWidget()
{
    delete ui;
}
