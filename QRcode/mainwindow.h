#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <qtextstream.h>
#include <qfile.h>
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void GenerateQRcode(QString tempstr, QString h);
    void qt_write_csv_test();
    QStringList qt_read_csv_test();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
