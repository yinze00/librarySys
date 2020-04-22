#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "login.h"
#include <QGridLayout>
#include <QSizePolicy>
#include "bookview.h"
#include <iostream>
#include <string>
#include <QVector>
#include <QFileDialog>
#include <QTextCodec>
#include <QTableView>
#include <QStandardItemModel>
#include <QMessageBox>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
using std::cout;
using std::endl;
QT_END_NAMESPACE
typedef enum pageop{next,last}NOP;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    void receiveData(QSqlDatabase);
    void flushview();
    void readshow();

signals:
    void reflush(NOP op);

private slots:
    void on_toolButton_clicked();

    void on_toolButton_5_clicked();

    void on_comboBox_2_currentIndexChanged();

    void on_toolButton_9_clicked();

    void on_toolButton_4_clicked();

    void on_toolButton_3_clicked();

    void on_toolButton_6_clicked();

    void on_toolButton_8_clicked();

    void on_comboBox_5_currentIndexChanged();

    void on_toolButton_7_clicked();

    void on_toolButton_11_clicked();
    
    void on_toolButton_10_clicked();


    void on_lineEdit_31_editingFinished();


    void on_lineEdit_21_editingFinished();

    void on_lineEdit_10_editingFinished();

    void on_comboBox_currentIndexChanged(int index);

private:
    enum pageop{nextpage,laspage}pageop = nextpage;

    Ui::MainWindow *ui;
    QSqlDatabase db;
    QLabel *alert;
    QSqlQuery *query;
    QGridLayout *glayout;
    QVector<QFrame*> viewstore;
    int currpage;
    int totalviewnums;
    NOP op;
    QString filename;
    QTableView *table;
    QString updatestm,querystm,insertstm,dbastm;//分别对应借还书,图书检索,图书入库,借书卡
    bool flag;

};
#endif // MAINWINDOW_H
