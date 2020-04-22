#ifndef BOOKVIEW_H
#define BOOKVIEW_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QList>
#include <QString>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QtNetwork/QNetworkAccessManager>
#include <QEventLoop>
#include <QByteArray>
#include <QtNetwork/QNetworkReply>
#include <iostream>
#include <QSqlQuery>

class BookView
{
public:
    BookView(/*QWidget* p*/);
//    QList<QLbale*>
//    QString bname,QString author,QString pname,QString pyear,QString category,QString price,QString bno,QString borrowed,QString onshelf
    QList<QLabel*> Data;
//    QWidget *bookview;
    static QString settedtext[8];
    QFrame* Genbookview(QSqlQuery *currquery);
    bool downIURL_to_picture(const QString &url, const QString &fileName);

    QFormLayout ly;


private:
    QList<QLabel*> Text;
};

#endif // BOOKVIEW_H
