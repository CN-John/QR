#include "mainwindow.h"
#include "ui_mainwindow.h"
extern "C"{
#include "qrencode.h"
}
#include <QImage>
#include <QPainter>
#include <QColor>
#include <QPixmap>
#include <QDir>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QRcode *qrcode = QRcode_encodeString("Hello World!", 2, QR_ECLEVEL_Q, QR_MODE_8, 0);
//    GenerateQRcode("横坐标:108.52685\n纵坐标:30.7799\n古树名:古树\n调查号:14804\n树种中:黄葛树\n树种拉:Ficus virens Ait. var. sublanceolata (Miq.) Corner\n科:桑科\n属:榕属\n乡镇名:长岭镇\n村名称:响滩社区居委会\n小地名:岩口\n位置:乡村\n分布:散生\n权属:集体\n真实树:125\n估测树:125\n古树等:三级\n树高:20\n胸围:375\n平均冠:13\n东西冠:14\n南北冠:12\n海拔:310\n坡向:西北\n坡度:10\n坡位:山谷\n土壤名:紫色土\n生长势:正常株\n生长环:良好\n影响生:无\n现存状:无\n古树历:无\n管护人:长岭镇响滩社区4组\n管护_1:长岭镇响滩社区4组\n树木特:无\n树种鉴:无\n地上保:避雷针,\n养护复:其它,\n照片（:\n图片说:无\n调查人:熊国鹏\n调查日:20170509\n审核人:蔡孔瑜\n审核日:20180504\n特征编:1 3 2110 0125 200 0375 013 1 1\n名木类:纪念树\n名木栽:\n名木_1:20180519\n备注:\nF50:\nF51:\nF52:\n");
//    qt_write_csv_test();
    QStringList res = qt_read_csv_test();
    int s = res.size() - 1;
    while(s + 1){
        QString h;
        h.setNum(s + 1);
        GenerateQRcode(res[s].toStdString().c_str(), h);
        s--;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::GenerateQRcode(QString tempstr, QString h)
{
    QRcode *qrcode; //二维码数据
    //QR_ECLEVEL_Q 容错等级
    qrcode = QRcode_encodeString(tempstr.toStdString().data(), 2, QR_ECLEVEL_Q, QR_MODE_8, 1);
    qint32 temp_width=ui->label->width(); //二维码图片的大小
    qint32 temp_height=ui->label->height();
    qint32 qrcode_width = qrcode->width > 0 ? qrcode->width : 1;
    double scale_x = (double)temp_width / (double)qrcode_width; //二维码图片的缩放比例
    double scale_y =(double) temp_height /(double) qrcode_width;
    QImage mainimg=QImage(temp_width,temp_height,QImage::Format_ARGB32);
    QPainter painter(&mainimg);
    QColor background(Qt::white);
    painter.setBrush(background);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, temp_width, temp_height);
    QColor foreground(Qt::black);
    painter.setBrush(foreground);
    for( qint32 y = 0; y < qrcode_width; y ++)
    {
        for(qint32 x = 0; x < qrcode_width; x++)
        {
            unsigned char b = qrcode->data[y * qrcode_width + x];
            if(b & 0x01)
            {
                QRectF r(x * scale_x, y * scale_y, scale_x, scale_y);
                painter.drawRects(&r, 1);
            }
        }
    }
    QPixmap mainmap=QPixmap::fromImage(mainimg);
    ui->label->setPixmap(mainmap);
    ui->label->setVisible(true);
    QString path = ".jpg";
    path.insert(0, h);
    QFileInfo info(path);
        if (info.suffix() == "jpg" || info.suffix() == "JPG")   // 只支持保存为jpg格式
        {
            QDir().mkpath(info.path());   // 创建路径，防止路径不存在
            mainimg.save(path);
        }
}

void MainWindow::qt_write_csv_test()
{
   QFile outFile("3.csv");
   QStringList lines;
   lines << "00,01,02\n" << "10,11,12\n" << "20,21,22";
   /*如果以ReadWrite方式打开，不会把所有数据清除，如果文件原来的长度为100字节，写操作写入了30字节，那么还剩余70字节，并且这70字节和文件打开之前保持一直*/
   if (outFile.open(QIODevice::WriteOnly))
   {
    for (int i = 0; i < lines.size(); i++)
    {
        outFile.write(lines[i].toStdString().c_str());/*写入每一行数据到文件*/
    }
    outFile.close();
   }
}


QStringList MainWindow::qt_read_csv_test()
{
    QFile inFile("2.csv");
    QStringList lines, res;/*行数据*/

    if (inFile.open(QIODevice::ReadOnly))
    {
        QTextStream stream_text(&inFile);
        while (!stream_text.atEnd())
        {
            lines.push_back(stream_text.readLine());
        }
        for (int j = 0; j < lines.size(); j++)
        {
            QString line = lines.at(j);
            QStringList split = line.split(",");/*列数据*/
            QString tmp = "";
            for (int col = 0; col < split.size(); col++)
            {
                tmp += split.at(col);
                tmp += "\n";
                //cout << split.at(col).toStdString() << " ";
            }
            res.push_back(tmp);
            //cout << endl;
        }
        inFile.close();
    }
    return res;
}
