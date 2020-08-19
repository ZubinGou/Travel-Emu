#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/image/logo.ico"));
    this->setFixedSize(1400, 900);
    QStringList strategyList = {"最少风险", "最少时间", "最少费用", "限时最少风险", "限时最少费用"};
    ui->strategyBox->addItems(strategyList);
    ui->departureBox->addItems(cityList);
    ui->destinationBox->addItems(cityList);
    ui->cityBox->addItems(cityList);
    ui->map->installEventFilter(this);
    sysTime = QDateTime::currentDateTime();
    ui->sysTime->setDateTime(sysTime);
    setUiDefault();
    qtimer = new QTimer;

    Idx = 0;
    Passenger psg;
    psg.status = "尚未开始旅行";
    psg.id = "Default User";
    psg.color = QColor::fromHsv(200, 160, 255);
    psg.planReady = false;

    psgList.push_back(psg);
    psgList[Idx].started = false;
    ui->passengerBox->addItem(psg.id);
    QStandardItemModel *pItemModel = qobject_cast<QStandardItemModel*>(ui->passengerBox->model());
    pItemModel->item(ui->passengerBox->currentIndex())->setBackground(psg.color);

    ui->mainLayout->setColumnStretch(0, 2);
    ui->mainLayout->setColumnStretch(1, 1);
//    ui->rightLayout->setColumnStretch(0, 3);
//    ui->rightLayout->setColumnStretch(1, 1);
//    ui->leftLayout->setColumnStretch(0, 5);
//    ui->leftLayout->setColumnStretch(1, 1);
//    ui->downLayout->setColumnStretch(0, 5);
//    ui->downLayout->setColumnStretch(1, 1);

    connect(this->ui->departureBox,SIGNAL(currentTextChanged(QString)),this,SLOT(changeDepartCity()));
    connect(this->ui->destinationBox,SIGNAL(currentTextChanged(QString)),this,SLOT(changeDestCity()));
    connect(this->ui->passengerBox,SIGNAL(currentTextChanged(QString)),this,SLOT(changePassenger()));
    connect(qtimer,SIGNAL(timeout()),this,SLOT(changeTravelStatus()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUiDefault()
{
    ui->departureBox->setCurrentIndex(0);
    ui->destinationBox->setCurrentIndex(0);
    ui->strategyBox->setCurrentIndex(0);
    ui->startTime->setDateTime(ui->sysTime->dateTime());
    ui->latestArrivalTime->setDateTime(ui->sysTime->dateTime());
    ui->simulatedProgressBar->setRange(0,1000);
    ui->simulatedProgressBar->setValue(0);
    ui->latestArrivalTime->setEnabled(false);
//    ui->pauseButton->setEnabled(false);
    ui->sysTime->setEnabled(false);
    ui->changePlanButton->setEnabled(false);

}

// 开始计划
void MainWindow::on_planButton_clicked()
{
    psgList[Idx].passedPoints.clear();
    repaint();
    if (ui->departureBox->currentText() == ui->destinationBox->currentText()){
        QMessageBox::information(this, "小提示", "出发地和目的地应该不同哦",
                                 QMessageBox::Ok,QMessageBox::NoButton);
        qDebug()<<"出发地和目的地不能相同";
        return;
    }
    if (ui->startTime->dateTime() < ui->sysTime->dateTime()) {
        QMessageBox::information(this, "小提示", "出发时间不能早于系统当前时间",
                                 QMessageBox::Ok,QMessageBox::NoButton);
        qDebug()<<"出发地和目的地不能相同";
        return;
    }

    QStringList passCity; // 途经城市
    vector<int> stayTime; // 途经城市停留时间
    int colCount = ui->passList->columnCount();
    if (colCount > 0) {
        for (int i = 0; i < colCount; i++) {
            passCity.push_back(ui->passList->item(0, i)->text());
            stayTime.push_back(ui->passList->item(1, i)->text().toInt());
        }
        qDebug() << "passCity and stayTime:";
        qDebug() << passCity;
        qDebug() << stayTime;
    }

    // 获取最迟到达时间
    DayTime startTime(0, ui->startTime->time().hour(), ui->startTime->time().minute());
    uint intervalTime = 0;
    unsigned short day;
    unsigned short hour;
    unsigned short minute;
    if (ui->latestArrivalTime->dateTime().toTime_t() > ui->startTime->dateTime().toTime_t())
        intervalTime = ui->latestArrivalTime->dateTime().toTime_t() - ui->startTime->dateTime().toTime_t();
    day = intervalTime / (24 * 60 * 60);
    hour = (intervalTime % (24 * 60 * 60)) / (60 * 60);
    minute = (intervalTime % (24 * 60 * 60)) % (60 * 60) / 60;
    DayTime maxTime(day, hour, minute);
    DayTime latestArrivalTime = startTime + maxTime; // 限时策略中的最迟到达时间
    qDebug()<<"latestArrivalTime";
    latestArrivalTime.print();

    psgList[Idx].setPlan(ui->departureBox->currentText(), ui->destinationBox->currentText(), passCity, stayTime, startTime, latestArrivalTime, ui->strategyBox->currentIndex() + 1);
    psgList[Idx].getPlan();
    if (psgList[Idx].plan.moneyCost == -1) { // 表示异常
        return;
    }
    psgList[Idx].expBeginTime = ui->startTime->dateTime();
    psgList[Idx].expEndTime = ui->latestArrivalTime->dateTime();

    psgList[Idx].plan.beginTime = psgList[Idx].plan.route.front().beginTime;
    psgList[Idx].plan.endTime = psgList[Idx].plan.route.back().endTime;
    psgList[Idx].plan.timeCost = psgList[Idx].plan.endTime - psgList[Idx].plan.beginTime;

    Plan plan = psgList[Idx].plan;
    QDateTime beginDate = ui->startTime->dateTime();
    beginDate.setTime(QTime::fromString("00:00", "hh:mm"));
    psgList[Idx].beginTime = beginDate.addDays(plan.beginTime.day).addSecs(plan.beginTime.hour * 3600 + plan.beginTime.minute *60);
    psgList[Idx].endTime = beginDate.addDays(plan.endTime.day).addSecs(plan.endTime.hour * 3600 + plan.endTime.minute *60);
    psgList[Idx].minuteCost = psgList[Idx].beginTime.secsTo(psgList[Idx].endTime) / 60;

    // show plan
    QString log;
    QString spaces = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
    log.append("========路线规划完毕=======<br>");
    log.append(QString(spaces + "<img src=\":\\image\\risk.png\" width=\"70\" height=\"70\"/>") + spaces + spaces + spaces + QString("<img src=\":\\image\\money.png\" width=\"70\" height=\"70\"/><br>"));
    log.append(QString(" 风险系数：") + QString::number(plan.risk, 'f', 2) + spaces);
    log.append(QString("费用开销：￥%1<br>").arg(plan.moneyCost));
    log.append(QString("%1%1%1<img src=\":\\image\\time.png\" width=\"70\" height=\"70\"/><br>").arg(spaces));
    log.append(QString(spaces + "预计出发时间：") + psgList[Idx].beginTime.toString("yyyy-MM-dd hh:mm<br>"));
    log.append(QString(spaces + "预计到达时间：") + psgList[Idx].endTime.toString("yyyy-MM-dd hh:mm<br>"));
    log.append(QString(spaces + "本行程总耗时：%1天%2时%3分<br>").arg(plan.timeCost.day).arg(plan.timeCost.hour).arg(plan.timeCost.minute));
    log.append("<br>=========行程安排=========<br>");

    QMap<QString, QString> vehicleName = {
        {"飞机", "plane"},
        {"火车", "train"},
        {"汽车", "bus"}
    };

    int pathCount = 0;
    QDateTime nowDateTime;
    for (auto path : plan.route) {
        pathCount++;
        log.append(QString("<img src=\":\\image\\%1.png\" width=\"50\" height=\"50\"/>#%2：").arg(vehicleName[path.vehicle]).arg(pathCount));
        log.append(QString("%1 → %2<br>").arg(path.begin).arg(path.end));
        log.append(QString(spaces + "交通工具：%1&nbsp;&nbsp;&nbsp;").arg(path.vehicle));
        log.append(QString("班次：%1 <br>").arg(path.number));
        log.append(QString(spaces + "费用：￥%1&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;").arg(path.moneyCost));
        log.append(QString("风险：") + QString::number(path.risk, 'f', 2) + "<br>");
        nowDateTime = beginDate.addDays(path.beginTime.day).addSecs(path.beginTime.hour * 3600 + path.beginTime.minute * 60);
        log.append(QString(spaces + "出发时间：") + nowDateTime.toString("yyyy-MM-dd hh:mm<br>"));
        nowDateTime = beginDate.addDays(path.endTime.day).addSecs(path.endTime.hour * 3600 + path.endTime.minute *60);
        log.append(QString(spaces + "到达时间：") + nowDateTime.toString("yyyy-MM-dd hh:mm<br>"));
        log.append(QString(spaces + "交通耗时：%1天%2时%3分<br>").arg(path.timeCost.day).arg(path.timeCost.hour).arg(path.timeCost.minute));
    }
    psgList[Idx].planReady = true;
    psgList[Idx].log = log;

    ui->logBrowser->setHtml(log);
    qDebug() << log;
}

void MainWindow::on_strategyBox_currentIndexChanged(int index) {
    if(index == 3 || index == 4)
        ui->latestArrivalTime->setEnabled(true);
    else
        ui->latestArrivalTime->setEnabled(false);
}

void MainWindow::on_addPassenger_clicked()
{
    QStringList defaultNameList = {"Barbara Liskov", "Elon Musk", "Larry Page", "Carl Sassenrath",
                                   "Guido Van Rassum", "Mark Zuckerburg", "Brendan Eich", "Tim Berners-Lee",
                                   "Hedy Lamarr", "Bill Gates", "Linus Torvalds", "Sergey Brin",
                                   "John D. Carmack", "David Axmark", "James Gosling", "Ken Thompson"};
    QString dlgTitle="添加旅客";
    QString txtLabel="旅客ID";
    QString defaultInput = defaultNameList[rand() % defaultNameList.size()];
    QLineEdit::EchoMode echoMode=QLineEdit::Normal;//正常文字输入
    bool ok=false;
    QString id = QInputDialog::getText(this, dlgTitle,txtLabel, echoMode,defaultInput, &ok);
    if (ok && !id.isEmpty()) {
        for (auto psg : psgList) {
            if (psg.id == id) {
                QMessageBox::information(this, "错误", "该旅客已存在！",
                                         QMessageBox::Ok,QMessageBox::NoButton);
                return;
            }
        }
        Passenger psg;
        psg.id = id;
        psg.status = "尚未开始旅行";
//        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        psg.color = QColor::fromHsv(qrand()%360, 160, qrand()%100 + 155); // 随机颜色用于作图
        psg.planReady = false;
        psgList.push_back(psg);
        ui->passengerBox->addItem(id);
        ui->passengerBox->setCurrentIndex(ui->passengerBox->count() - 1);
        QStandardItemModel *pItemModel = qobject_cast<QStandardItemModel*>(ui->passengerBox->model());
        pItemModel->item(ui->passengerBox->currentIndex())->setBackground(psg.color);
        setUiDefault();
    }
}

void MainWindow::on_removePassenger_clicked()
{
    if (ui->passengerBox->count() > 1) { // 1位乘客时，不可删除
        psgList.erase(psgList.begin() + ui->passengerBox->currentIndex());
        ui->passengerBox->removeItem(ui->passengerBox->currentIndex());
    }
}

void MainWindow::changePassenger()
{
    Idx = ui->passengerBox->currentIndex();
    Passenger psg = psgList[Idx];
//    ui->passengerBox->setStyleSheet("QComboBox{background:yellow}")
    ui->logBrowser->setText(psg.log);
    ui->statusLabel->setText(psg.status);
    if (psgList[Idx].planReady) {
        ui->departureBox->setCurrentText(psg.begin);
        ui->destinationBox->setCurrentText(psg.end);
        ui->strategyBox->setCurrentIndex(psg.strategy - 1);
        ui->startTime->setDateTime(psg.expBeginTime);
        ui->latestArrivalTime->setDateTime(psg.expEndTime);

        ui->passList->clearContents();
        ui->passList->setColumnCount(0);
        for (int i = 0; i < psgList[Idx].passCity.size(); i++) {
            ui->passList->insertColumn(i);
            ui->passList->setItem(0, i, new QTableWidgetItem(psgList[Idx].passCity[i]));
            ui->passList->setItem(1, i, new QTableWidgetItem(QString::number(psgList[Idx].passHours[i])));
        }
    }
}

//途经城市列表添加城市
void MainWindow::on_addCity_clicked()
{
    QString departCity = ui->departureBox->currentText();
    QString destCity =ui->destinationBox->currentText();
    QString city = ui->cityBox->currentText();
    int stayTime = ui->visitTime->value();
    int colCount = ui->passList->columnCount();
    for (int i = 0; i < colCount; i++) {
        if (ui->passList->item(0, i)->text() == city)
            return;
    }
    if (city == departCity || city == destCity) {
        return;
    }

    ui->passList->insertColumn(colCount);
    ui->passList->setItem(0, colCount, new QTableWidgetItem(city));
    ui->passList->setItem(1, colCount, new QTableWidgetItem(QString::number(stayTime)));
    qDebug() << "添加中途城市：" << city;
}

//途经城市列表删除城市
void MainWindow::on_removeCity_clicked()
{
//    QListWidgetItem *currentItem = ui->passList->currentItem();
//    if (currentItem) {
//        qDebug()<<"删除中途城市：" << (*currentItem).text().left(2);
//        addedCities.removeAll((*currentItem).text().left(2));
//        delete currentItem;
//    }
    int colCount = ui->passList->columnCount();
    if (colCount) {
        ui->passList->removeColumn(colCount - 1);
    }
}

void MainWindow::changeDepartCity()
{
//    addedCities.removeAll(departCity);
    ui->passList->clearContents();
    ui->passList->setColumnCount(0);
//    addedCities.append(departCity);
    QString departCity = ui->departureBox->currentText();
    qDebug() << "出发城市改为：" << departCity;
}

void MainWindow::changeDestCity()
{
//    addedCities.removeAll(destCity);
    ui->passList->clearContents();
    ui->passList->setColumnCount(0);
//    addedCities.append(destCity);
    QString destCity = ui->destinationBox->currentText();
    qDebug() << "终点城市改为：" << destCity;
}

//点击开始模拟旅行
void MainWindow::on_simButton_clicked()
{
    if (ui->simButton->text() == "开始模拟" || ui->simButton->text() == "继续模拟") {
//        for (auto psg : psgList) {
//            if (psg.planReady == false) {
//                QMessageBox::information(this,"错误","请先为所有旅客规划方案，再开始模拟","确定");
//                return;
//            }
//        }
        ui->departureBox->setEnabled(false);
        ui->destinationBox->setEnabled(false);
        ui->cityBox->setEnabled(false);
        ui->visitTime->setEnabled(false);
        ui->startTime->setEnabled(false);
        ui->planButton->setEnabled(false);
        ui->strategyBox->setEnabled(false);
        ui->addCity->setEnabled(false);
        ui->removeCity->setEnabled(false);
        ui->changePlanButton->setEnabled(false);
        ui->addPassenger->setEnabled(false);
        for (int index = 0; index < (int)psgList.size(); index++) {
            if (psgList[index].planReady) {
                if (!psgList[index].started) { // 尚未开始模拟
                    psgList[index].passedPoints.clear();
    //                repaint();
                    qDebug()<<"开始模拟";
                    psgList[index].pathIndex = 0;
                    psgList[index].curPath = psgList[index].plan.route[0];
                    psgList[index].onPath = false;
                    psgList[index].curMoneyCost = 0;
                    psgList[index].status = "您目前在：" + QString("%1").arg(psgList[index].curPath.begin);
                    if (index == Idx)
                        this->ui->statusLabel->setText(psgList[Idx].status);
                }
                else {
                    qDebug() << psgList[index].id << "继续旅行";
                }
                psgList[index].started = true;
            }
        }
        qDebug() << "hr12";
        qtimer->start(10);                   //可以用来调节模拟进度的快慢
        ui->passengerBox->showPopup();
        ui->simButton->setText("暂停模拟");
    }
    else if (ui->simButton->text() == "暂停模拟") {
        qtimer->stop();
        qDebug()<<"暂停模拟";
        ui->departureBox->setEnabled(true);
        ui->destinationBox->setEnabled(true);
        ui->cityBox->setEnabled(true);
        ui->visitTime->setEnabled(true);
        ui->startTime->setEnabled(true);
        ui->planButton->setEnabled(true);
        ui->strategyBox->setEnabled(true);
        ui->addCity->setEnabled(true);
        ui->removeCity->setEnabled(true);
        ui->changePlanButton->setEnabled(true);
        ui->addPassenger->setEnabled(true);
        ui->simButton->setText("继续模拟");
    }
}

void MainWindow::changeTravelStatus()
{
    ui->sysTime->setDateTime(sysTime);
    repaint();
    for (int index = 0; index < (int)psgList.size(); index++) {
        if (psgList[index].started) { // 开始旅行的旅客更新状态
            psgList[index].curMinuteCost = psgList[index].beginTime.secsTo(sysTime) / 60;
            int progress = 1000 * psgList[index].curMinuteCost / psgList[index].minuteCost; // 进度条
            if (index == Idx)
                ui->simulatedProgressBar->setValue(progress);

            qDebug() << "psgList[Idx].curMinuteCost: " << psgList[Idx].curMinuteCost;
            qDebug() << "psgList[Idx].minuteCost" << psgList[Idx].minuteCost;
            qDebug() << "psgList[Idx].curPath.beginTime" << (psgList[Idx].curPath.beginTime - psgList[Idx].plan.beginTime).toMinute();
            qDebug() << "psgList[Idx].curPath.endTime" << (psgList[Idx].curPath.endTime - psgList[Idx].plan.beginTime).toMinute();

            if(psgList[index].curMinuteCost == psgList[index].minuteCost) { //模拟完成
    //            qtimer->stop();
                qDebug() << "旅客" << psgList[index].id << "模拟旅行结束";
                psgList[index].onPath = false;
                QPointF curPathEnd = cityPoint[cityList.indexOf(psgList[index].curPath.end)];
                psgList[index].passedPoints.append(curPathEnd);
                repaint();
                psgList[index].started = false;
                psgList[index].planReady = false;
                ui->planButton->setEnabled(true);
//                ui->simButton->setEnabled(true);
//                ui->simButton->setText("开始模拟");
        //        ui->pauseButton->setEnabled(false);
                QMessageBox::information(this,"提示", QString("旅客%1已到达目的地：%2").arg(psgList[index].id, psgList[index].end), "确定");
                ui->passengerBox->showPopup();
                psgList[index].status = "已到达目的地：" + psgList[index].end;
//                ui->departureBox->setEnabled(true);
//                ui->destinationBox->setEnabled(true);
//                ui->cityBox->setEnabled(true);
//                ui->visitTime->setEnabled(true);
//                ui->startTime->setEnabled(true);
//                ui->planButton->setEnabled(true);
//                ui->strategyBox->setEnabled(true);
//                ui->simButton->setEnabled(true);
//                ui->addCity->setEnabled(true);
//                ui->removeCity->setEnabled(true);
//                ui->addPassenger->setEnabled(true);
                if (index == Idx) {
                    for (int i = 0; i < ui->passList->columnCount(); i++) {
                        ui->passList->item(0, i)->setBackground(Qt::NoBrush);
                        ui->passList->item(1, i)->setBackground(Qt::NoBrush);
                    }
                }
                continue;
            }
            //到达一个城市
            if(psgList[index].curMinuteCost == (psgList[index].curPath.endTime - psgList[index].plan.beginTime).toMinute()) {
                QPointF curPathEnd = cityPoint[cityList.indexOf(psgList[index].curPath.end)];
                psgList[index].passedPoints.append(curPathEnd);
                psgList[index].onPath = false;
                psgList[index].pathIndex++;
                psgList[index].curPath = psgList[index].plan.route[psgList[index].pathIndex];
                psgList[index].status = "目前停留在：" + psgList[index].curPath.begin;
                if (index == Idx) {
                    ui->statusLabel->setText(psgList[index].status);
                }
                qDebug() << psgList[index].id << "目前停留在：" << psgList[index].curPath.begin;
            }
            //到达新的path
            if(psgList[index].curMinuteCost == (psgList[index].curPath.beginTime - psgList[index].plan.beginTime).toMinute()) {
                psgList[index].onPath = true;
                if (index == Idx) {
                    for (int i = 0; i < ui->passList->columnCount(); i++) {
                        if (psgList[index].curPath.begin == ui->passList->item(0, i)->text()) {
                            ui->passList->item(0, i)->setBackground(Qt::Dense7Pattern);
                            ui->passList->item(1, i)->setBackground(Qt::Dense7Pattern);
                        }
                    }
                }
                Path path = psgList[index].curPath;
                psgList[index].status = QString("%1->%2, %3 %4").arg(path.begin).arg(path.end).arg(path.vehicle).arg(path.number);
                if (index == Idx) {
                    this->ui->statusLabel->setText(psgList[index].status);
                }
                qDebug() << sysTime << ": " << psgList[index].id << "在" << psgList[index].status;    //旅客状态变化写入日志文件
                psgList[index].curMoneyCost += psgList[index].curPath.moneyCost;
                qDebug() << "旅客" << psgList[index].id << "已经花费￥" << psgList[index].curMoneyCost;
            }
        }
    }
    sysTime = sysTime.addSecs(60); // 每次加1分钟
}



//初始化城市绘图坐标点
void MainWindow::initCityPoint() {
    // {"北京", "上海", "广州" , "深圳", "成都", "杭州", "重庆", "武汉", "南京", "郑州", "西安", "青岛"};
    //北京
    cityPoint[0].setX(687-4);
    cityPoint[0].setY(295+4);
    //上海
    cityPoint[1].setX(801-4);
    cityPoint[1].setY(449+7);
    //广州
    cityPoint[2].setX(675-4);
    cityPoint[2].setY(629+9);
    //深圳
    cityPoint[3].setX(691-4);
    cityPoint[3].setY(638+5);
    //成都
    cityPoint[4].setX(494-4);
    cityPoint[4].setY(490+7);
    //杭州
    cityPoint[5].setX(782-4);
    cityPoint[5].setY(473+6);
    //重庆
    cityPoint[6].setX(538-4);
    cityPoint[6].setY(511+7);
    //武汉
    cityPoint[7].setX(675-4);
    cityPoint[7].setY(481+6);
    //南京
    cityPoint[8].setX(750-4);
    cityPoint[8].setY(441+6);
    //郑州
    cityPoint[9].setX(655-4);
    cityPoint[9].setY(400+4);
    //西安
    cityPoint[10].setX(576-4);
    cityPoint[10].setY(419+4);
    //青岛
    cityPoint[11].setX(763-4);
    cityPoint[11].setY(360+4);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->map && event->type() == QEvent::Paint)
    {
        paintMap(); //响应函数
    }
    return QWidget::eventFilter(watched,event);
}

void MainWindow::paintMap()
{
    initCityPoint();
    QPainter painter(ui->map);
    QPen pen;
    pen.setWidth(5);
    static double wid = rect().width();
    static double hei = rect().height();
    painter.setRenderHints(QPainter::Antialiasing, true); //抗锯齿
    double new_wid = rect().width()/wid;
    double new_hei = rect().height()/hei;
    double min = qMin(new_wid,new_hei);
    painter.scale(min, min);

    // 城市点和名称
    for(int i = 0; i < cityList.size(); i++) {
        pen.setColor(Qt::blue);
        pen.setWidth(6);
        painter.setPen(pen);
        painter.drawPoint(cityPoint[i]);
//        pen.setColor(Qt::blue);
//        painter.setPen(pen);
//        painter.drawText(cityPoint[i].x() - 12, cityPoint[i].y() - 10, cityList[i]);
    }

    for (int index = 0; index < (int)psgList.size(); index++) {
        if (!psgList[index].started)
            continue;
        QPointF currentPoint;
        QPointF curPathBegin = cityPoint[cityList.indexOf(psgList[index].curPath.begin)];
        QPointF curPathEnd = cityPoint[cityList.indexOf(psgList[index].curPath.end)];
        // 在Path上
        if(psgList[index].started && psgList[index].onPath) {
            double xLength = curPathEnd.x() - curPathBegin.x();
            double yLength = curPathEnd.y() - curPathBegin.y();
            double currentProgress = double(psgList[index].curMinuteCost - (psgList[index].curPath.beginTime - psgList[index].plan.beginTime).toMinute()) / psgList[index].curPath.timeCost.toMinute();

    //        qDebug() << "currentProgress" << currentProgress;
            currentPoint.setX(curPathBegin.x() + xLength * currentProgress);
            currentPoint.setY(curPathBegin.y() + yLength * currentProgress);
            if(psgList[index].passedPoints.empty())
                psgList[index].passedPoints.append(curPathBegin);
        }
        // 走过的路线乘客颜色（降低饱和度）
        for(int i = 0; i < psgList[index].passedPoints.count() - 1;i++) {
            pen.setColor(QColor::fromHsv(psgList[index].color.hsvHue(), 80, 170));
            painter.setPen(pen);
            painter.drawLine(psgList[index].passedPoints[i], psgList[index].passedPoints[i+1]);
        }
        // 走过的点设为乘客颜色
        for(int i = 0; i < psgList[index].passedPoints.count()-1; i++) {
            pen.setColor(psgList[index].color);
            pen.setWidth(6);
            painter.setPen(pen);
            painter.drawPoint(psgList[index].passedPoints[i]);
            painter.drawPoint(psgList[index].passedPoints[i+1]);
        }
        // 交通工具、当前Path
        if(psgList[index].started && psgList[index].onPath) {
            pen.setColor(Qt::black);
            pen.setWidth(6);
            painter.setPen(pen);
            painter.drawPoint(curPathBegin);
            painter.drawPoint(curPathEnd);
//            pen.setColor(psgList[index].color);
            pen.setColor(psgList[index].color);
            painter.setPen(pen);
            painter.drawLine(curPathBegin, currentPoint);
            QPixmap transferTool;
            QString curVehicle = psgList[index].curPath.vehicle;
            if(curVehicle == "飞机")
                transferTool = QPixmap(":/image/plane.png").scaled(QSize(50, 50), Qt::KeepAspectRatio);
            else if(curVehicle == "火车")
                transferTool = QPixmap(":/image/train.png").scaled(QSize(50, 50), Qt::KeepAspectRatio);
            else if(curVehicle == "汽车")
                transferTool = QPixmap(":/image/bus.png").scaled(QSize(50, 50), Qt::KeepAspectRatio);
            painter.drawPixmap(currentPoint.x() - 25, currentPoint.y() - 25, transferTool);
        }
        // TODO乘客头像
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{

}

//模拟中途点击更改计划
void MainWindow::on_changePlanButton_clicked()
{
    if(ui->changePlanButton->text() == "更改计划") {
        this->ui->changePlanButton->setText("按新计划模拟");
//        this->ui->simButton->setEnabled(false);
        this->ui->destinationBox->setEnabled(true);
        this->ui->cityBox->setEnabled(true);
        this->ui->visitTime->setEnabled(true);
        this->ui->addCity->setEnabled(true);
        this->ui->removeCity->setEnabled(true);
        this->ui->strategyBox->setEnabled(true);
        ui->startTime->setDateTime(ui->sysTime->dateTime());
        ui->latestArrivalTime->setDateTime(ui->sysTime->dateTime());
    }
    else {
        //qDebug()<<"修改计划";
        //获取新的出发时间和出发城市

        QString newDepartCity;
        DayTime newStartTime(0,0,0);
//        bool stopOnPath = false;
        int oldCurMoneyCost = psgList[Idx].curMoneyCost;
        Path remainPath;
        if(psgList[Idx].onPath) { //正在某一条path上，以path的终点为起点城市，以到达时间为新期望出发日期
            remainPath = psgList[Idx].curPath;
//            stopOnPath = true;
            newDepartCity = psgList[Idx].curPath.end;
            newStartTime = psgList[Idx].curPath.endTime;
        }
        else { //正停留在城市，以当前城市为起点，以当前时间为为新期望出发日期
            newDepartCity = psgList[Idx].curPath.begin;
            DayTime _newStartTime(sysTime.daysTo(psgList[Idx].beginTime), ui->sysTime->time().hour(), ui->sysTime->time().minute());
            newStartTime = _newStartTime;
        }

        //获取途经城市列表
        QStringList newPassCity;
        vector<int> newstayTime;
        int colCount = ui->passList->columnCount();
        if (colCount > 0) {
            for (int i = psgList[Idx].pathIndex; i < colCount; i++) {
                newPassCity.push_back(ui->passList->item(0, i)->text());
                newstayTime.push_back(ui->passList->item(1, i)->text().toInt());
            }
            qDebug() << "passCity and stayTime:";
            qDebug() << newPassCity;
            qDebug() << newstayTime;
        }
//        int count = ui->passList->count();
//        if(count > 1) {
//            for(int i = psgList[Idx].pathIndex + 1 ;i < count; i++) {
//              QListWidgetItem *temp = ui->passList->item(i);
//              QString tempText = (*temp).text();
//              QString stayTimeString,cityString;
//              for(int i = 0;i < 2; i++) {
//                  cityString.append(tempText[i]);
//              }
//              for(int j = 0; j < tempText.length(); j++) {
//                  if(tempText[j] >= '0' && tempText[j] <= '9')
//                      stayTimeString.append(tempText[j]);
//              }
//              newPassCity.append(cityString);
//              newstayTime.push_back(stayTimeString.toInt());
//            }
//        }

        // 获取最迟到达时间
        DayTime startTime(0, ui->startTime->time().hour(), ui->startTime->time().minute());
        uint intervalTime = 0;
        unsigned short day;
        unsigned short hour;
        unsigned short minute;
        if (ui->latestArrivalTime->dateTime().toTime_t() > ui->startTime->dateTime().toTime_t())
            intervalTime = ui->latestArrivalTime->dateTime().toTime_t() - ui->startTime->dateTime().toTime_t();
        day = intervalTime / (24 * 60 * 60);
        hour = (intervalTime % (24 * 60 * 60)) / (60 * 60);
        minute = (intervalTime % (24 * 60 * 60)) % (60 * 60) / 60;
        DayTime maxTime(day, hour, minute);
        DayTime latestArrivalTime = startTime + maxTime; // 限时策略中的最迟到达时间
        qDebug()<<"latestArrivalTime";
        latestArrivalTime.print();

        //重新设置Passenger，计算新的策略
        psgList[Idx].setPlan(newDepartCity, ui->destinationBox->currentText(), newPassCity, newstayTime, newStartTime,latestArrivalTime, ui->strategyBox->currentIndex() + 1);
        psgList[Idx].getPlan();
        if (psgList[Idx].plan.moneyCost == -1) { // 表示异常
            qDebug()<<"计划异常";
            return;
        }
        psgList[Idx].passedPoints.clear();

        psgList[Idx].expBeginTime = ui->startTime->dateTime();
        psgList[Idx].expEndTime = ui->latestArrivalTime->dateTime();





        psgList[Idx].plan.beginTime = psgList[Idx].plan.route.front().beginTime;
        psgList[Idx].plan.endTime = psgList[Idx].plan.route.back().endTime;
        psgList[Idx].plan.timeCost = psgList[Idx].plan.endTime - psgList[Idx].plan.beginTime;

        Plan plan = psgList[Idx].plan;
        QDateTime beginDate = ui->startTime->dateTime();
        beginDate.setTime(QTime::fromString("00:00", "hh:mm"));
        psgList[Idx].beginTime = beginDate.addDays(plan.beginTime.day).addSecs(plan.beginTime.hour * 3600 + plan.beginTime.minute *60);
        psgList[Idx].endTime = beginDate.addDays(plan.endTime.day).addSecs(plan.endTime.hour * 3600 + plan.endTime.minute *60);
        psgList[Idx].minuteCost = psgList[Idx].beginTime.secsTo(psgList[Idx].endTime) / 60;

        psgList[Idx].curMoneyCost += oldCurMoneyCost;

//        if (stopOnPath) {
//            psgList[Idx].onPath = true;
//            psgList[Idx].pathIndex = -1;
//            psgList[Idx].curPath = remainPath;
//        }
//        else {
//            psgList[Idx].onPath = false;
//            psgList[Idx].pathIndex = 0;
//            psgList[Idx].curPath = psgList[Idx].plan.route[psgList[Idx].pathIndex];
//        }

        psgList[Idx].onPath = false;
        psgList[Idx].pathIndex = 0;
        psgList[Idx].curPath = psgList[Idx].plan.route[psgList[Idx].pathIndex];



        // show plan
        QString log;
        QString spaces = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
        log.append("========路线规划完毕=======<br>");
        log.append(QString(spaces + "<img src=\":\\image\\risk.png\" width=\"70\" height=\"70\"/>") + spaces + spaces + spaces + QString("<img src=\":\\image\\money.png\" width=\"70\" height=\"70\"/><br>"));
        log.append(QString(" 风险系数：") + QString::number(plan.risk, 'f', 2) + spaces);
        log.append(QString("费用开销：￥%1<br>").arg(plan.moneyCost));
        log.append(QString("%1%1%1<img src=\":\\image\\time.png\" width=\"70\" height=\"70\"/><br>").arg(spaces));
        log.append(QString(spaces + "预计出发时间：") + psgList[Idx].beginTime.toString("yyyy-MM-dd hh:mm<br>"));
        log.append(QString(spaces + "预计到达时间：") + psgList[Idx].endTime.toString("yyyy-MM-dd hh:mm<br>"));
        log.append(QString(spaces + "本行程总耗时：%1天%2时%3分<br>").arg(plan.timeCost.day).arg(plan.timeCost.hour).arg(plan.timeCost.minute));
        log.append("<br>=========行程安排=========<br>");

        QMap<QString, QString> vehicleName = {
            {"飞机", "plane"},
            {"火车", "train"},
            {"汽车", "bus"}
        };

        int pathCount = 0;
        QDateTime nowDateTime;
        for (auto path : plan.route) {
            pathCount++;
            log.append(QString("<img src=\":\\image\\%1.png\" width=\"50\" height=\"50\"/>#%2：").arg(vehicleName[path.vehicle]).arg(pathCount));
            log.append(QString("%1 → %2<br>").arg(path.begin).arg(path.end));
            log.append(QString(spaces + "交通工具：%1&nbsp;&nbsp;&nbsp;").arg(path.vehicle));
            log.append(QString("班次：%1 <br>").arg(path.number));
            log.append(QString(spaces + "费用：￥%1&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;").arg(path.moneyCost));
            log.append(QString("风险：") + QString::number(path.risk, 'f', 2) + "<br>");
            nowDateTime = beginDate.addDays(path.beginTime.day).addSecs(path.beginTime.hour * 3600 + path.beginTime.minute * 60);
            log.append(QString(spaces + "出发时间：") + nowDateTime.toString("yyyy-MM-dd hh:mm<br>"));
            nowDateTime = beginDate.addDays(path.endTime.day).addSecs(path.endTime.hour * 3600 + path.endTime.minute *60);
            log.append(QString(spaces + "到达时间：") + nowDateTime.toString("yyyy-MM-dd hh:mm<br>"));
            log.append(QString(spaces + "交通耗时：%1天%2时%3分<br>").arg(path.timeCost.day).arg(path.timeCost.hour).arg(path.timeCost.minute));
        }
        psgList[Idx].planReady = true;
        psgList[Idx].log = log;

        ui->logBrowser->setHtml(log);
        qDebug() << log;

        this->ui->changePlanButton->setText("更改计划");
        on_simButton_clicked();
//        qtimer->start(10);
//        ui->departureBox->setEnabled(false);
//        ui->destinationBox->setEnabled(false);
//        ui->cityBox->setEnabled(false);
//        ui->visitTime->setEnabled(false);
//        ui->startTime->setEnabled(false);
//        ui->planButton->setEnabled(false);
//        ui->strategyBox->setEnabled(false);
////        ui->simButton->setEnabled(false);
//        ui->addCity->setEnabled(false);
//        ui->removeCity->setEnabled(false);
//        ui->changePlanButton->setEnabled(false);
//        ui->pauseButton->setEnabled(true);
    }
}

void MainWindow::on_speedSlider_valueChanged()
{
    int speed = (int)ui->speedSlider->value();
    qtimer->setInterval((10 - speed) * (10 - speed) + 1);
}
