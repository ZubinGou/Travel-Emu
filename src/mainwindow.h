#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "PlanAlgorithm.h"
#include "Passenger.h"
#include <QSet>
#include <QDate>
#include <QDebug>
#include <QQueue>
#include <QTimer>
#include <QString>
#include <QPainter>
#include <QDateTime>
#include <QLineEdit>
#include <QInputDialog>
#include <QTextStream>
#include <QMainWindow>
#include <QMessageBox>
#include <QStringList>
#include <QListWidgetItem>
#include <QStandardItemModel>
#include <cmath>
#include <queue>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *event);
    void paintMap();
    void mapPaint();
    void initCityPoint();

    void setUiDefault();

private slots:
    void changeDepartCity();
    void changeDestCity();
    void changePassenger();
    void on_addPassenger_clicked();
    void on_removePassenger_clicked();

    void on_addCity_clicked();
    void on_removeCity_clicked();
    void changeTravelStatus();
    void on_strategyBox_currentIndexChanged(int index);
    void on_planButton_clicked();
    void on_simButton_clicked();
//    void on_pauseButton_clicked();
    void on_changePlanButton_clicked();

    void on_speedSlider_valueChanged();

private:
    Ui::MainWindow *ui;
//    QList<QString> addedCities;
//    QString departCity;
//    QString destCity;
    QStringList cityList = {"北京", "上海", "广州" , "深圳", "成都", "杭州", "重庆", "武汉", "南京", "郑州", "西安", "青岛"};
    QPointF cityPoint[12];
    QTimer *qtimer;

    vector<Passenger> psgList; // 乘客名单
    int Idx; // 当前乘客序号
    QDateTime sysTime; // 系统时间
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // MAINWINDOW_H
