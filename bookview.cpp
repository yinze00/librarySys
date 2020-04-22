#include "bookview.h"

QString BookView::settedtext[8]={"作  者:","出版社:","出版年:","类  别:","价  格:","书  号:","已借出:","在  册:"};

//构造函数参数表传递SQL读取的一系列值然后然后接到对应的
BookView::BookView(/*QWidget *p*/)/*QStringList *data = new QStringList()*/
{
    // 设置各个文字label

    // 将固定的文本label添加到widget对象中，方便之后用布局管理器排版在scrollarea.
//    for(int i = 0;i<8;i++){
//        QLabel *t = new QLabel(bookview);
//        t->setText(settedtext[i]);
//        t->setGeometry()
//        Text<<t;
//    }
//    this->bookview->setParent(p);
//    bookview->setParent(p);
}

QFrame* BookView::Genbookview(QSqlQuery *currquery){
    QFrame* ans = new QFrame();
    QFont font("Microsoft YaHei");
    font.setPointSize(13);
    ans->setFont(font);
    QGridLayout *glo = new QGridLayout(ans);
    QLabel* pic = new QLabel(ans);



//    QLabel *pic = new QLabel("<img src = "C:\Users\Yinze\Desktop\jzm.jpg">");
    pic->setParent(ans);
    QLabel* name = new QLabel(ans);
    QLabel* auth = new QLabel(ans);
    QLabel* pname = new QLabel(ans);
    QLabel* pyear = new QLabel(ans);
    QLabel* ptype = new QLabel(ans);
    QLabel* price = new QLabel(ans);
    QLabel* bno = new QLabel(ans);
    QLabel* borr = new QLabel(ans);
    QLabel* stoc = new QLabel(ans);

    //绘制bookview
//    QImage *img = new QImage;
//    img->load("C:/Users/Yinze/Desktop/545.JPG");
//    pic->setScaledContents(true);
//    pic->setPixmap(QPixmap::fromImage(*img));
    QUrl url(currquery->value("img_url").toString());
//    QUrl url("https://img.alicdn.com/bao/uploaded/i2/1016552675/TB1EyfpdyCYBuNkHFCcXXcHtVXa_!!0-item_pic.jpg");
    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QByteArray jpegData = reply->readAll();
    QPixmap pixmap;
    pixmap.loadFromData(jpegData);
    pixmap.scaled(100, 70, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    pic->setPixmap(pixmap);
    pic->setScaledContents(true);
    pic->setFixedSize(QSize(200,500));

//    pic->setPixmap(QPixmap("C:/Users/Yinze/Desktop/jzm.jpg").scaled(190,380,Qt::KeepAspectRatio, Qt::SmoothTransformation));
    name->setText(currquery->value("name").toString());
    QFont titlefont( "Microsoft YaHei", 14, 50);
    name->setFont(titlefont);
    pic->setStyleSheet("border:1px solid blue");
    auth->setText(settedtext[0]+currquery->value("author").toString());
    pname->setText(settedtext[1]+currquery->value("publish").toString());
    pyear->setText(settedtext[2]+currquery->value("publish_date").toString());
    ptype->setText(settedtext[3]+"机器学习");
    price->setText(settedtext[4]+currquery->value("price").toString());
    bno->setText(settedtext[5]+currquery->value("price").toString());
    borr->setText(settedtext[6]+"817");
    stoc->setText(settedtext[7]+"-1");


    glo->addWidget(pic,0,0,7,1);
    glo->addWidget(name,0,1,2,2);
    glo->addWidget(auth,2,1,1,2);
    glo->addWidget(pname,3,1,1,2);
    glo->addWidget(pyear,4,1);
    glo->addWidget(ptype,4,2);
    glo->addWidget(price,5,1);
    glo->addWidget(bno,5,2);
    glo->addWidget(borr,6,1);
    glo->addWidget(stoc,6,2);

    glo->setSpacing(10);
    glo->setMargin(10);

    return ans;
}

bool BookView::downIURL_to_picture(const QString &url, const QString &fileName)
{
    //构造请求
    QNetworkAccessManager manager;
    QNetworkRequest request;
    request.setUrl(url);
    // 发送请求
    QNetworkReply *reply = manager.get(request);
    //开启一个局部的事件循环，等待响应结束，退出
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    //判断是否出错,出错则结束
    if (reply->error() != QNetworkReply::NoError)
    {
        return false;
    }
    //保存文件
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
        return false;
    file.write(reply->readAll());
    file.close();
    delete reply;
    return true;
 }
