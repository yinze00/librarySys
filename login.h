#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "QSqlDatabase"
#include "QSqlQuery"
#include "QSqlError"
#include "QString"

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_toolButton_clicked();
    void on_lineEdit_2_textChanged(const QString &arg1);

signals:
    void sendData(QSqlDatabase db);
private:
    Ui::login *ui;
    QSqlDatabase db;
};

#endif // LOGIN_H
