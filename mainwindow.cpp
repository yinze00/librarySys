#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QList"

//QList<QWidget *> Q;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(this->width(),this->height());
    login *lo = new login();
    connect(lo,SIGNAL(sendData(QSqlDatabase)),this,SLOT(receiveData(QSqlDatabase)));
    lo->show();
    this->setFont(QFont( "Microsoft YaHei", 10,25));
    alert = new QLabel(ui->tab_4);
    //UI设计
    ui->lineEdit->setPlaceholderText("书名:");
    ui->lineEdit_2->setPlaceholderText("￥:");
    ui->lineEdit_3->setPlaceholderText("￥:");
    ui->lineEdit_7->setPlaceholderText("类别");
    ui->lineEdit_8->setPlaceholderText("出版社");
    ui->lineEdit_6->setPlaceholderText("作者");
    ui->widget->hide();
    ui->widget_2->hide();
    ui->widget_3->hide();
    ui->widget_4->hide();
    ui->widget_5->hide();
    ui->widget_6->hide();
    ui->widget_7->hide();
    ui->widget_8->hide();
    ui->widget_9->hide();
    ui->widget_10->hide();
//    ui->scrollAreaWidgetContents->hide();
    ui->scrollArea->hide();
    ui->toolButton_2->hide();
    ui->toolButton_3->hide();
    ui->toolButton_4->hide();
    ui->lineEdit_9->hide();
    ui->lineEdit_10->setPlaceholderText("请输入书籍编号");


    //书籍管理
    ui->lineEdit_11->setPlaceholderText("书名例如:《他改变了中国》");
    ui->lineEdit_12->setPlaceholderText("作者例如:江泽民");
    ui->lineEdit_13->setPlaceholderText("书号,例如:19260817");
    ui->lineEdit_14->setPlaceholderText("出版社,例如：香港记者出版社");
    ui->lineEdit_15->setPlaceholderText("出版年,例如：1926");
    ui->lineEdit_16->setPlaceholderText("类别,例如:传记类");
    ui->lineEdit_19->setPlaceholderText("入库量,例如:20");
    ui->lineEdit_29->setPlaceholderText("价格,例如:59");

    //翻页显示
    connect(ui->toolButton_2,SIGNAL(clicked()),this,SLOT(flushview()));

    //入库部分
    insertstm = "insert into douban(isbm,category,name,publish,publish_date,author,price,onshelf)values";

    //DBA借书卡管理
    ui->lineEdit_30->setEnabled(false);     //
    ui->toolButton_9->setEnabled(false);
    ui->label_211->hide();
    this->flag = true;

    //借还书
    ui->lineEdit_10->setPlaceholderText("输入书号");
    ui->lineEdit_21->setPlaceholderText("输入借书卡号");
    ui->frame_5->hide();
    ui->frame_6->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::receiveData(QSqlDatabase ddb){
    this->show();
    this->db = ddb;
    if(this->db.open()){
        ui->lineEdit->setText("他改变了中国");
    }
}



/***************************************************************************************
 * the slot function is excuting when the 'search' button was clicked.
 * 在此函数中实现图书检索的ui
 *
 *
 **************************************************************************************/

void MainWindow::on_toolButton_clicked()
{
    // 开始检索,并将数据库检索结果用bookview类实现，然后再scrollarea上显示。
    ui->scrollArea->show();
    ui->toolButton_2->show();
    ui->toolButton_3->show();
    ui->toolButton_4->show();
    ui->lineEdit_9->show();
    //    ui->widget_2->setStyleSheet("border:1px solid #A0A0A0");
    glayout = new QGridLayout(ui->scrollAreaWidgetContents);


    //以下尝试连接db直接输出
    if(db.open()){
        std::cout<<"success"<<std::endl;
    }
    query = new QSqlQuery();
    query->exec("select img_url,name,author,publish,publish_date,price from douban");
    int totalnum = query->size();
    int totalpages = (totalnum+9)/10;
    ui->toolButton_2->setText(QString::number(totalpages));

    //通过函数show view 来显示每一个scrollAreaContent的内容.

    query->next();

    ui->lineEdit_9->setPlaceholderText(QString::number(1));

    this->currpage=1;
    this->op = next;
    this->totalviewnums = query->size();

//    flushview(&query,glayout,0,totalnum);

    flushview();

    glayout->setVerticalSpacing(10);
    glayout->setHorizontalSpacing(10);
    glayout->setContentsMargins(10,10,10,10);

}

void MainWindow::flushview(){

    // 生成预览视图
    ui->lineEdit_9->setText(QString::number(this->currpage));
    int currviewnum = (currpage*10<totalviewnums)?10:totalviewnums%10;


    if(op==last)
        for(int i = 0;i<currviewnum+10;i++)
            query->last();
    while (!viewstore.empty()) {
        viewstore.back()->hide();
        viewstore.pop_back();
    }

    for(int i = 0;i<10;i++){
        BookView *book = new BookView();
        QFrame *temp =(i<currviewnum)? book->Genbookview(query):new QFrame();
        viewstore<<temp;
        query->next();
        temp->setParent(ui->scrollAreaWidgetContents);
        temp->setStyleSheet("QFrame#temp {border:1px solid yellow}");
        glayout->addWidget(temp,i/2,i%2);
    }
}

void MainWindow::on_toolButton_5_clicked()
{
    // 开始借书或者还书
    BookView *queryresult = new BookView();
    QFrame *queryview = queryresult->Genbookview(new QSqlQuery());
    QFrame *background = new QFrame();
    queryview->setParent(ui->tab_4);
    background->setParent(ui->tab_4);
    background->setGeometry(379,199,612,262);
    background->setStyleSheet("border:2px solid blue");
    background->show();
    queryview->setGeometry(380,200,611,261);
    queryview->show();
    QLineEdit *input = new QLineEdit(ui->tab_4);
    input->setPlaceholderText("借书卡号:");
    input->setGeometry(512,500,380,30);
    input->show();
    QToolButton *enter = new QToolButton(ui->tab_4);
//    enter->insertItem(0,"确认借出");
//    enter->insertItem(1,"确认还回");
    enter->setGeometry(812,500,80,30);
    enter->show();

    if(ui->comboBox->currentIndex()==0){
        //借书
        if(false){
            //可以借出
        }else {
            //不能借出
            alert->setText("没有库存，其他借出预计归还:2020-2-29");
            alert->setFont(QFont("Microsoft YaHei",15,75));
            alert->setGeometry(input->geometry().x(),input->geometry().y()+100,input->geometry().width(),input->geometry().height());
            alert->show();
//            enter->move(enter->geometry().x(),enter->geometry().y()+100);
//            input->move(input->geometry().x(),input->geometry().y()+100);
        }
        enter->setText("确认借出");

    }else{
        //还书
        enter->setText("确认还回");
        alert->hide();
        
    }
}
/*************************************************************************
 *
 *  模式选择：
 *
 *  1. 添加模式
 *     输入姓名，部门，电话号码后，查询library card 表中最大的primary key生成NO
 *
 *
 *  2. 删除模式
 *     输入卡号后，点击“开始”查询并显示，点击确认删除后，检查时候有书未还
 *
 *
 *
 *
 * **********************************************************************/
void MainWindow::on_comboBox_2_currentIndexChanged()
{
    ui->label_211->hide();

    if(ui->comboBox_2->currentIndex()==1){
        ui->toolButton_11->setText("确 认 删 除");
        ui->toolButton_10->hide();
        ui->lineEdit_30->setEnabled(true);
        ui->toolButton_9->setEnabled(true);
        ui->label_206->setText("确认删除如下借书卡");

    }else if(ui->comboBox_2->currentIndex()==0){
        ui->toolButton_10->show();
        ui->toolButton_11->setText("确 认 添 加");
        ui->lineEdit_30->setEnabled(false);     //
        ui->toolButton_9->setEnabled(false);
    }
}



void MainWindow::on_toolButton_4_clicked()
{
    //下一页
    op = next;
    this->currpage+=1;

//    delete ui->scrollAreaWidgetContents->children();
    flushview();

}

void MainWindow::on_toolButton_3_clicked()
{
    //上一页
    op = last;
    this->currpage-=1;
    flushview();
}

/****************************************************
 * 点击上传图片，此时Qt打开文件选择对话框
 * 选择文件，返回QString 类型的文件路径
 * 此时this.filename也将改变
 ****************************************************/
void MainWindow::on_toolButton_6_clicked()
{
//    QString picpath;
//    QFileDialog *filesel = new QFileDialog(ui->tab_3);
//    filesel->setWindowTitle("选择图片");
//    filesel->setDirectory(".");
//    filesel->setNameFilter(tr("Images (*.png *.bmp *.jpg *.tif *.GIF)"));
//    filesel->setFileMode(QFileDialog::ExistingFiles);
//    filesel->setModal(QFileDialog::Detail);
//    picpath = filesel->getOpenFileName();
     filename = QFileDialog::getOpenFileName(
            this,
            tr("open a picture."),
            "D:/",
            tr("images(*.png *jpeg *bmp);;video files(*.avi *.mp4 *.wmv);;All files(*.*)"));

//    std::cout<<fileName.toStdString()<<std::endl;
}



void MainWindow::on_comboBox_5_currentIndexChanged()
{
    if(ui->comboBox_5->currentIndex()==1){
        ui->frame->hide();
        filename = QFileDialog::getOpenFileName(
               this,
               tr("open a picture."),
               "C:/Users/Yinze/Documents/librarySys",
               tr("*.txt"));
        if(!filename.isEmpty()){
            //打开并读取txt文件
//            std::cout<<filename.toStdString()<<std::endl;
            readshow();
        }

    }else{
        delete this->table;
        ui->frame->show();
    }
}

void MainWindow::readshow(){
    QFile file(filename);
//    std::cout<<filename.toStdString()<<std::endl;

    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        std::cout<<"file cannot open!"<<std::endl;
    }

    QTextStream stream(&file);
    stream.setCodec(QTextCodec::codecForName("UTF-8"));

    if(file.atEnd()){
        std::cout<<"end"<<std::endl;
    }

    QStandardItemModel * model = new QStandardItemModel();
    QStringList labels = tr("书号,类别,书名,出版社,年份,作者,价格,数量").simplified().split(",");
    model->setHorizontalHeaderLabels(labels);


    this->table = new QTableView(ui->tab_3);
    table->setGeometry(ui->frame->geometry());
    table->setGridStyle(Qt::DashDotLine);


    QStringList list = stream.readAll().remove("(").remove(")").split("\n");

    QListIterator<QString>li(list);

    int i = -1;
    while (li.hasNext()) {
            //每一条记录
        i++;
        QString temp = li.next();
        if(temp.isEmpty())
            break;
//        cout<<temp.toStdString()<<endl;
        insertstm.append("(");
        insertstm.append(temp);
        insertstm.append("),");
        cout<<insertstm.toStdString()<<endl;
        QStringList each =  temp.split(", ");
        int j = -1;
        QStandardItem * tableitem;
        foreach(QString item,each){
            j++;
//            std::cout<<item.toStdString()<<std::endl;
            tableitem = new QStandardItem(item);
            tableitem->setTextAlignment(Qt::AlignCenter);
            model->setItem(i,j,tableitem);
        }
        if(j==7)flag = false;
        else{
            insertstm = "insert into douban(isbm,category,name,publish,publish_date,author,price,onshelf)values";
            QMessageBox::information(NULL,"Warning","your *.txt file is not standard!",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            break;
        }
    }

    insertstm.replace(insertstm.length()-1,1,";");
    cout<<insertstm.toStdString()<<endl;
    file.close();
    table->setModel(model);
    table->show();

}

void MainWindow::on_toolButton_7_clicked()
{
    if(ui->lineEdit_13->text().isEmpty()|
            ui->lineEdit_14->text().isEmpty()|
            ui->lineEdit_11->text().isEmpty()|
            ui->lineEdit_12->text().isEmpty()|
            ui->lineEdit_13->text().isEmpty()|
            ui->lineEdit_15->text().isEmpty()|
            ui->lineEdit_16->text().isEmpty()|
            ui->lineEdit_17->text().isEmpty()|
            ui->lineEdit_18->text().isEmpty()|
            ui->lineEdit_19->text().isEmpty()|
            ui->lineEdit_29->text().isEmpty()
            ){
        QMessageBox::information(NULL,"提示","请填完表单",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }else{
        //单本入库

        insertstm.append(QString("(%1,%2,%3,%4,%5,%6,%7,%8);")
                   .arg(ui->lineEdit_13->text())
                   .arg(ui->lineEdit_16->text())
                   .arg(ui->lineEdit_11->text())
                   .arg(ui->lineEdit_14->text())
                   .arg(ui->lineEdit_15->text())
                   .arg(ui->lineEdit_12->text())
                   .arg(ui->lineEdit_29->text())
                   .arg(ui->lineEdit_19->text())
                   );
        cout<<insertstm.toStdString()<<endl;
        flag = true;
    }


    if(flag){
//        db.exec(insertstm);// 执行入库操作
    }

}

/**************************************************
 *  借书卡管理页面的 右上角“开始”按钮事件
 *
 *
 *
 *
 * ***********************************************/
void MainWindow::on_toolButton_9_clicked()
{
    ui->label_211->hide();
    query = new QSqlQuery();
    dbastm = QString("select cname,department,category from card where cno = %1;").arg(ui->lineEdit_30->text().toUInt());
    query->exec(dbastm);
    query->next();

    cout<<query->value(0).toString().toStdString()<<endl;
//    cout<<query->value(0).toString().toStdString()<<endl;
    ui->lineEdit_31->setText(query->value(0).toString());
    ui->lineEdit_32->setText(query->value(1).toString());
    ui->lineEdit_34->setText(ui->lineEdit_30->text());
    if(query->value(2).toString()=="Teacher"){
        ui->checkBox_5->setChecked(true);
        ui->checkBox_6->setChecked(false);
    }else{
        ui->checkBox_5->setChecked(false);
        ui->checkBox_6->setChecked(true);
    }
}

/***************************************************
 * 
 * DBA表管理，实现删除/添加
 * 删除时，从ui->lineEdit_30->text()读卡号，并查询
 *        在中间的借书卡中显示出来
 *
 *        dbaStm的
 *
 * 添加时，手动输入中间借书卡，自动生成一个primary key
 *        卡号
 * 
 * 
 * 
 * *************************************************/
void MainWindow::on_toolButton_11_clicked()
{
    if(ui->comboBox_2->currentIndex()==1){
        //确认删除
        dbastm = QString("delete from card where card.cno = %1;").arg(ui->lineEdit_30->text().toInt());
        query = new QSqlQuery();
        if(query->exec(dbastm)){
            //提示
            ui->label_211->setText("成功删除");
            ui->label_211->show();
            ui->lineEdit_31->clear();
            ui->lineEdit_32->clear();
            ui->lineEdit_34->clear();
            ui->checkBox_5->clearMask();
            ui->checkBox_6->clearMask();
        }

    }else{
        //确 认增加
        dbastm = QString("insert into card(cno,cname,category,department)values(%1,'%2','%3','%4');")
                .arg(ui->lineEdit_34->text().toUInt())
                .arg(ui->lineEdit_31->text())
                .arg(ui->checkBox_5->isChecked()?"Teacher":"Student")
                .arg(ui->lineEdit_32->text());
//        query = new QSqlQuery();
//        query->prepare("insert into card(cno,cname,category,department)values(3,yinze,'Student','CS');");
        cout<<dbastm.toStdString()<<endl;
        query = new QSqlQuery();
        if(query->exec(dbastm)){
            ui->label_211->setText("成功添加");
            ui->label_211->show();
        }
    }

}


void MainWindow::on_toolButton_10_clicked()
{
    // 添加一个借书卡之前，先自动生成一个cno
    query = new QSqlQuery();
    query->exec("select max(cno) from card");
    query->next();
    ui->lineEdit_34->setText(QString::number(query->value(0).toULongLong()+1));

}

void MainWindow::on_lineEdit_31_editingFinished()
{
    query = new QSqlQuery();
    query->exec("select max(cno) from card");
    query->next();
    if(ui->comboBox_2->currentIndex()==0)
        ui->lineEdit_34->setText(QString::number(query->value(0).toULongLong()+1));
    ui->label_211->hide();
}


void MainWindow::on_lineEdit_21_editingFinished()
{
    //输入借书卡号借书后
    query = new QSqlQuery();
    query->prepare("select name from douban where isbm = (select bno from borrow where cno = 1");
//    query->exec();

    //test
    ui->frame_5->show();
    QStandardItemModel * model = new QStandardItemModel();
    QStringList labels = tr("封面,书号,书名,出版社,年份,作者,价格").simplified().split(",");
    model->setHorizontalHeaderLabels(labels);

    QTableView *borrtable = new QTableView(ui->tab_4);
    borrtable->setGeometry(ui->frame_5->geometry());
    //查询borrow表并显示cno为当前LineEdit_21->text()的字段的




    //显示表
    borrtable->setModel(model);
    borrtable->show();
}

void MainWindow::on_lineEdit_10_editingFinished()
{
    //输完书号

    ui->frame_6->show();
    query = new QSqlQuery();
    query->prepare("select img_url,name,author,publish,publish_date,price from douban where isbm = 9781447155706");
//    int isbm = ui->lineEdit_10->text().toInt();
//    QString a = QString("select img_url,name,author,publish,publish_date,price from douban where isbm = %1").arg(isbm);
    if(query->exec()){
        query->next();
        //开始给frame_6的借还书预览赋值

        ui->bname->setText(query->value(1).toString());
        ui->bauthor->setText(query->value(2).toString());
        ui->bpress->setText(query->value(3).toString());
        ui->bpressyear->setText(query->value(4).toString());
        ui->bprice->setText(query->value(5).toString());
        ui->bcategory->setText("机器学习");

        QUrl U(query->value(0).toString());
        QNetworkAccessManager manager;
        QEventLoop loop;
        QNetworkReply *reply = manager.get(QNetworkRequest(U));
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        QByteArray jpegData = reply->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(jpegData);
        pixmap.scaled(100, 70, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        ui->bpic->setPixmap(pixmap);
        ui->bpic->setScaledContents(true);
//        ui->bpic->setFixedSize(QSize(200,500));

    }
}

void MainWindow::on_toolButton_8_clicked()
{
    //此段代码中执行一个插入/删除操作
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    //
    if(ui->comboBox->currentIndex()==0){
        ui->toolButton_8->setText("确 认 还 回");

    }else{
        ui->toolButton_8->setText("确 认 借 出");
    }
}
