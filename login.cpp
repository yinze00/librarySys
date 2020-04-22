#include "login.h"
#include "ui_login.h"
#include <string>
#include <iostream>
#include "QKeyEvent"
login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    setFixedSize(this->width(),this->height());
    ui->lineEdit->setPlaceholderText("请输入账号");
    ui->lineEdit_2->setPlaceholderText("请输入密码");
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->label_3->hide();

}

login::~login()
{
    delete ui;
}

void login::on_toolButton_clicked()
{
    QString account = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();
    if(!account.isEmpty() && !password.isEmpty()){
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("localhost");
//        db.setUserName(account);
//        db.setPassword(password);
        db.setUserName("root");
        db.setPassword("123456");
        db.setDatabaseName("libirary");
        if(db.open()){
            //登录成功
//            std::cout<<"login success!"<<std::endl;
            emit sendData(db);
            this->hide();
        }else{
            ui->label_3->show();
            ui->lineEdit_2->clear();
            ui->label_3->show();
        }
    }else{
        ui->label_3->hide();
    }
}

void login::on_lineEdit_2_textChanged(const QString &arg1)
{
    ui->label_3->hide();
}\

void login::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Enter){
        on_toolButton_clicked();
    }
}
