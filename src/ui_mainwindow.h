/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_16;
    QGridLayout *mainLayout;
    QGridLayout *gridLayout_13;
    QGridLayout *gridLayout_12;
    QGridLayout *gridLayout_10;
    QGridLayout *gridLayout_9;
    QLabel *strategyLabel;
    QComboBox *strategyBox;
    QPushButton *planButton;
    QPushButton *changePlanButton;
    QSpacerItem *horizontalSpacer_2;
    QGridLayout *gridLayout_11;
    QGridLayout *gridLayout_7;
    QLabel *startTimeLabel;
    QDateTimeEdit *startTime;
    QGridLayout *gridLayout_8;
    QLabel *latestArrivalTimeLabel;
    QDateTimeEdit *latestArrivalTime;
    QGridLayout *gridLayout_2;
    QLabel *depatureLabel;
    QComboBox *departureBox;
    QSpacerItem *horizontalSpacer_7;
    QLabel *destinationLabel;
    QComboBox *destinationBox;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label;
    QSpinBox *visitTime;
    QHBoxLayout *horizontalLayout_5;
    QLabel *passLable;
    QComboBox *cityBox;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *addCity;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *removeCity;
    QSpacerItem *horizontalSpacer_5;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_5;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_3;
    QGridLayout *gridLayout_4;
    QLabel *label_4;
    QLabel *label_3;
    QSlider *speedSlider;
    QPushButton *simButton;
    QSpacerItem *horizontalSpacer_8;
    QWidget *map;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_19;
    QDateTimeEdit *sysTime;
    QGridLayout *gridLayout_14;
    QPushButton *addPassenger;
    QComboBox *passengerBox;
    QPushButton *removePassenger;
    QGridLayout *gridLayout_20;
    QProgressBar *simulatedProgressBar;
    QLabel *statusLabel;
    QGridLayout *rightLayout;
    QTextBrowser *logBrowser;
    QTableWidget *passList;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(926, 627);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(3);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(0, 0));
        MainWindow->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	font: 75 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"}"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        centralWidget->setStyleSheet(QString::fromUtf8("QLabel,QPushButton,QProgressBar,QSpinBox,QComboBox,QDateTimeEdit,QListWidget{\n"
"	font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"}\n"
"\n"
"QTextEdit{\n"
"	font:\n"
"	font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"}\n"
"\n"
""));
        gridLayout_16 = new QGridLayout(centralWidget);
        gridLayout_16->setSpacing(6);
        gridLayout_16->setContentsMargins(11, 11, 11, 11);
        gridLayout_16->setObjectName(QString::fromUtf8("gridLayout_16"));
        mainLayout = new QGridLayout();
        mainLayout->setSpacing(6);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        gridLayout_13 = new QGridLayout();
        gridLayout_13->setSpacing(6);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        gridLayout_12 = new QGridLayout();
        gridLayout_12->setSpacing(6);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        gridLayout_10 = new QGridLayout();
        gridLayout_10->setSpacing(6);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        gridLayout_9 = new QGridLayout();
        gridLayout_9->setSpacing(6);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        strategyLabel = new QLabel(centralWidget);
        strategyLabel->setObjectName(QString::fromUtf8("strategyLabel"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(strategyLabel->sizePolicy().hasHeightForWidth());
        strategyLabel->setSizePolicy(sizePolicy2);

        gridLayout_9->addWidget(strategyLabel, 0, 0, 1, 1);

        strategyBox = new QComboBox(centralWidget);
        strategyBox->setObjectName(QString::fromUtf8("strategyBox"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(strategyBox->sizePolicy().hasHeightForWidth());
        strategyBox->setSizePolicy(sizePolicy3);
        strategyBox->setMaximumSize(QSize(174, 16777215));

        gridLayout_9->addWidget(strategyBox, 0, 1, 1, 1);


        gridLayout_10->addLayout(gridLayout_9, 0, 0, 1, 1);

        planButton = new QPushButton(centralWidget);
        planButton->setObjectName(QString::fromUtf8("planButton"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(planButton->sizePolicy().hasHeightForWidth());
        planButton->setSizePolicy(sizePolicy4);
        planButton->setMinimumSize(QSize(0, 25));
        planButton->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_10->addWidget(planButton, 1, 0, 1, 1);

        changePlanButton = new QPushButton(centralWidget);
        changePlanButton->setObjectName(QString::fromUtf8("changePlanButton"));
        sizePolicy4.setHeightForWidth(changePlanButton->sizePolicy().hasHeightForWidth());
        changePlanButton->setSizePolicy(sizePolicy4);
        changePlanButton->setMinimumSize(QSize(150, 25));
        changePlanButton->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_10->addWidget(changePlanButton, 2, 0, 1, 1);


        gridLayout_12->addLayout(gridLayout_10, 0, 4, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_12->addItem(horizontalSpacer_2, 0, 1, 1, 1);

        gridLayout_11 = new QGridLayout();
        gridLayout_11->setSpacing(6);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        gridLayout_7 = new QGridLayout();
        gridLayout_7->setSpacing(6);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        startTimeLabel = new QLabel(centralWidget);
        startTimeLabel->setObjectName(QString::fromUtf8("startTimeLabel"));
        sizePolicy2.setHeightForWidth(startTimeLabel->sizePolicy().hasHeightForWidth());
        startTimeLabel->setSizePolicy(sizePolicy2);

        gridLayout_7->addWidget(startTimeLabel, 0, 0, 1, 1);

        startTime = new QDateTimeEdit(centralWidget);
        startTime->setObjectName(QString::fromUtf8("startTime"));
        sizePolicy3.setHeightForWidth(startTime->sizePolicy().hasHeightForWidth());
        startTime->setSizePolicy(sizePolicy3);
        startTime->setMinimumSize(QSize(230, 0));
        startTime->setCalendarPopup(true);

        gridLayout_7->addWidget(startTime, 0, 1, 1, 1);


        gridLayout_11->addLayout(gridLayout_7, 1, 0, 1, 1);

        gridLayout_8 = new QGridLayout();
        gridLayout_8->setSpacing(6);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        latestArrivalTimeLabel = new QLabel(centralWidget);
        latestArrivalTimeLabel->setObjectName(QString::fromUtf8("latestArrivalTimeLabel"));
        sizePolicy2.setHeightForWidth(latestArrivalTimeLabel->sizePolicy().hasHeightForWidth());
        latestArrivalTimeLabel->setSizePolicy(sizePolicy2);

        gridLayout_8->addWidget(latestArrivalTimeLabel, 0, 0, 1, 1);

        latestArrivalTime = new QDateTimeEdit(centralWidget);
        latestArrivalTime->setObjectName(QString::fromUtf8("latestArrivalTime"));
        sizePolicy3.setHeightForWidth(latestArrivalTime->sizePolicy().hasHeightForWidth());
        latestArrivalTime->setSizePolicy(sizePolicy3);
        latestArrivalTime->setMinimumSize(QSize(230, 0));
        latestArrivalTime->setMinimumDateTime(QDateTime(QDate(1984, 1, 1), QTime(0, 0, 0)));
        latestArrivalTime->setCalendarPopup(true);

        gridLayout_8->addWidget(latestArrivalTime, 0, 1, 1, 1);


        gridLayout_11->addLayout(gridLayout_8, 2, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        depatureLabel = new QLabel(centralWidget);
        depatureLabel->setObjectName(QString::fromUtf8("depatureLabel"));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(depatureLabel->sizePolicy().hasHeightForWidth());
        depatureLabel->setSizePolicy(sizePolicy5);

        gridLayout_2->addWidget(depatureLabel, 0, 0, 1, 1);

        departureBox = new QComboBox(centralWidget);
        departureBox->setObjectName(QString::fromUtf8("departureBox"));
        sizePolicy4.setHeightForWidth(departureBox->sizePolicy().hasHeightForWidth());
        departureBox->setSizePolicy(sizePolicy4);
        departureBox->setMaxVisibleItems(12);

        gridLayout_2->addWidget(departureBox, 0, 1, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_7, 0, 2, 1, 1);

        destinationLabel = new QLabel(centralWidget);
        destinationLabel->setObjectName(QString::fromUtf8("destinationLabel"));
        sizePolicy5.setHeightForWidth(destinationLabel->sizePolicy().hasHeightForWidth());
        destinationLabel->setSizePolicy(sizePolicy5);

        gridLayout_2->addWidget(destinationLabel, 0, 3, 1, 1);

        destinationBox = new QComboBox(centralWidget);
        destinationBox->setObjectName(QString::fromUtf8("destinationBox"));
        sizePolicy4.setHeightForWidth(destinationBox->sizePolicy().hasHeightForWidth());
        destinationBox->setSizePolicy(sizePolicy4);
        destinationBox->setMaxVisibleItems(12);

        gridLayout_2->addWidget(destinationBox, 0, 4, 1, 1);


        gridLayout_11->addLayout(gridLayout_2, 0, 0, 1, 1);


        gridLayout_12->addLayout(gridLayout_11, 0, 2, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy5.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy5);

        horizontalLayout_7->addWidget(label);

        visitTime = new QSpinBox(centralWidget);
        visitTime->setObjectName(QString::fromUtf8("visitTime"));
        sizePolicy4.setHeightForWidth(visitTime->sizePolicy().hasHeightForWidth());
        visitTime->setSizePolicy(sizePolicy4);

        horizontalLayout_7->addWidget(visitTime);


        gridLayout->addLayout(horizontalLayout_7, 2, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        passLable = new QLabel(centralWidget);
        passLable->setObjectName(QString::fromUtf8("passLable"));
        sizePolicy5.setHeightForWidth(passLable->sizePolicy().hasHeightForWidth());
        passLable->setSizePolicy(sizePolicy5);
        passLable->setLocale(QLocale(QLocale::Chinese, QLocale::China));

        horizontalLayout_5->addWidget(passLable);

        cityBox = new QComboBox(centralWidget);
        cityBox->setObjectName(QString::fromUtf8("cityBox"));
        sizePolicy4.setHeightForWidth(cityBox->sizePolicy().hasHeightForWidth());
        cityBox->setSizePolicy(sizePolicy4);
        cityBox->setMaxVisibleItems(12);

        horizontalLayout_5->addWidget(cityBox);


        gridLayout->addLayout(horizontalLayout_5, 1, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        addCity = new QPushButton(centralWidget);
        addCity->setObjectName(QString::fromUtf8("addCity"));
        sizePolicy3.setHeightForWidth(addCity->sizePolicy().hasHeightForWidth());
        addCity->setSizePolicy(sizePolicy3);
        addCity->setMinimumSize(QSize(40, 40));
        addCity->setMaximumSize(QSize(70, 16777215));

        horizontalLayout_6->addWidget(addCity);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);

        removeCity = new QPushButton(centralWidget);
        removeCity->setObjectName(QString::fromUtf8("removeCity"));
        sizePolicy3.setHeightForWidth(removeCity->sizePolicy().hasHeightForWidth());
        removeCity->setSizePolicy(sizePolicy3);
        removeCity->setMinimumSize(QSize(40, 40));
        removeCity->setMaximumSize(QSize(70, 16777215));

        horizontalLayout_6->addWidget(removeCity);


        gridLayout->addLayout(horizontalLayout_6, 0, 0, 1, 1);


        gridLayout_12->addLayout(gridLayout, 0, 6, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_12->addItem(horizontalSpacer_5, 0, 3, 1, 1);

        gridLayout_6 = new QGridLayout();
        gridLayout_6->setSpacing(6);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setSpacing(6);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_4, 0, 5, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer, 0, 0, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_3, 0, 3, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy6(QSizePolicy::Minimum, QSizePolicy::Ignored);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy6);
        label_4->setMaximumSize(QSize(16777215, 30));

        gridLayout_4->addWidget(label_4, 1, 0, 1, 1);

        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy6.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy6);
        label_3->setMaximumSize(QSize(16777215, 30));

        gridLayout_4->addWidget(label_3, 0, 0, 1, 1);


        gridLayout_5->addLayout(gridLayout_4, 0, 1, 1, 1);

        speedSlider = new QSlider(centralWidget);
        speedSlider->setObjectName(QString::fromUtf8("speedSlider"));
        QSizePolicy sizePolicy7(QSizePolicy::Fixed, QSizePolicy::Ignored);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(speedSlider->sizePolicy().hasHeightForWidth());
        speedSlider->setSizePolicy(sizePolicy7);
        speedSlider->setMaximumSize(QSize(20, 100));
        speedSlider->setSizeIncrement(QSize(0, 0));
        speedSlider->setStyleSheet(QString::fromUtf8("color:rgb(85, 170, 255)"));
        speedSlider->setMinimum(0);
        speedSlider->setMaximum(10);
        speedSlider->setValue(7);
        speedSlider->setOrientation(Qt::Vertical);
        speedSlider->setTickPosition(QSlider::NoTicks);
        speedSlider->setTickInterval(10);

        gridLayout_5->addWidget(speedSlider, 0, 4, 1, 1);


        gridLayout_6->addLayout(gridLayout_5, 0, 0, 1, 1);

        simButton = new QPushButton(centralWidget);
        simButton->setObjectName(QString::fromUtf8("simButton"));
        sizePolicy4.setHeightForWidth(simButton->sizePolicy().hasHeightForWidth());
        simButton->setSizePolicy(sizePolicy4);
        simButton->setMinimumSize(QSize(100, 25));
        simButton->setMaximumSize(QSize(16777215, 16777215));
        simButton->setCheckable(false);
        simButton->setAutoDefault(false);
        simButton->setFlat(false);

        gridLayout_6->addWidget(simButton, 1, 0, 1, 1);


        gridLayout_12->addLayout(gridLayout_6, 0, 0, 1, 1);

        horizontalSpacer_8 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_12->addItem(horizontalSpacer_8, 0, 5, 1, 1);


        gridLayout_13->addLayout(gridLayout_12, 1, 0, 1, 1);

        map = new QWidget(centralWidget);
        map->setObjectName(QString::fromUtf8("map"));
        map->setEnabled(true);
        QSizePolicy sizePolicy8(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(map->sizePolicy().hasHeightForWidth());
        map->setSizePolicy(sizePolicy8);
        map->setMinimumSize(QSize(0, 0));
        map->setMaximumSize(QSize(16777215, 16777215));
        map->setStyleSheet(QString::fromUtf8("QWidget#map{\n"
"	border-image:url(:/image/china.png);\n"
"}"));
        layoutWidget = new QWidget(map);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 0, 561, 145));
        gridLayout_3 = new QGridLayout(layoutWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        gridLayout_19 = new QGridLayout();
        gridLayout_19->setSpacing(6);
        gridLayout_19->setObjectName(QString::fromUtf8("gridLayout_19"));
        sysTime = new QDateTimeEdit(layoutWidget);
        sysTime->setObjectName(QString::fromUtf8("sysTime"));
        QSizePolicy sizePolicy9(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy9.setHorizontalStretch(1);
        sizePolicy9.setVerticalStretch(0);
        sizePolicy9.setHeightForWidth(sysTime->sizePolicy().hasHeightForWidth());
        sysTime->setSizePolicy(sizePolicy9);
        sysTime->setMinimumSize(QSize(250, 0));

        gridLayout_19->addWidget(sysTime, 0, 0, 1, 1);

        gridLayout_14 = new QGridLayout();
        gridLayout_14->setSpacing(6);
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        addPassenger = new QPushButton(layoutWidget);
        addPassenger->setObjectName(QString::fromUtf8("addPassenger"));
        QSizePolicy sizePolicy10(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy10.setHorizontalStretch(1);
        sizePolicy10.setVerticalStretch(0);
        sizePolicy10.setHeightForWidth(addPassenger->sizePolicy().hasHeightForWidth());
        addPassenger->setSizePolicy(sizePolicy10);
        addPassenger->setMinimumSize(QSize(40, 40));
        addPassenger->setMaximumSize(QSize(40, 40));

        gridLayout_14->addWidget(addPassenger, 0, 0, 1, 1);

        passengerBox = new QComboBox(layoutWidget);
        passengerBox->setObjectName(QString::fromUtf8("passengerBox"));
        QSizePolicy sizePolicy11(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy11.setHorizontalStretch(6);
        sizePolicy11.setVerticalStretch(0);
        sizePolicy11.setHeightForWidth(passengerBox->sizePolicy().hasHeightForWidth());
        passengerBox->setSizePolicy(sizePolicy11);
        passengerBox->setMinimumSize(QSize(0, 40));
        passengerBox->setAutoFillBackground(true);
        passengerBox->setInsertPolicy(QComboBox::NoInsert);
        passengerBox->setDuplicatesEnabled(false);
        passengerBox->setFrame(true);
        passengerBox->setModelColumn(0);

        gridLayout_14->addWidget(passengerBox, 0, 2, 1, 1);

        removePassenger = new QPushButton(layoutWidget);
        removePassenger->setObjectName(QString::fromUtf8("removePassenger"));
        sizePolicy10.setHeightForWidth(removePassenger->sizePolicy().hasHeightForWidth());
        removePassenger->setSizePolicy(sizePolicy10);
        removePassenger->setMinimumSize(QSize(40, 40));
        removePassenger->setMaximumSize(QSize(40, 40));

        gridLayout_14->addWidget(removePassenger, 0, 1, 1, 1);


        gridLayout_19->addLayout(gridLayout_14, 1, 0, 1, 1);


        gridLayout_3->addLayout(gridLayout_19, 0, 0, 1, 1);

        gridLayout_20 = new QGridLayout();
        gridLayout_20->setSpacing(6);
        gridLayout_20->setObjectName(QString::fromUtf8("gridLayout_20"));
        simulatedProgressBar = new QProgressBar(layoutWidget);
        simulatedProgressBar->setObjectName(QString::fromUtf8("simulatedProgressBar"));
        sizePolicy10.setHeightForWidth(simulatedProgressBar->sizePolicy().hasHeightForWidth());
        simulatedProgressBar->setSizePolicy(sizePolicy10);
        simulatedProgressBar->setMinimumSize(QSize(250, 0));
        simulatedProgressBar->setStyleSheet(QString::fromUtf8("QProgressBar{background:white;} \n"
"QProgressBar::chunk{background:rgb(85, 170, 255)}"));
        simulatedProgressBar->setValue(88);
        simulatedProgressBar->setTextVisible(true);
        simulatedProgressBar->setOrientation(Qt::Horizontal);

        gridLayout_20->addWidget(simulatedProgressBar, 0, 0, 1, 1);

        statusLabel = new QLabel(layoutWidget);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        sizePolicy4.setHeightForWidth(statusLabel->sizePolicy().hasHeightForWidth());
        statusLabel->setSizePolicy(sizePolicy4);
        statusLabel->setMinimumSize(QSize(250, 40));
        statusLabel->setMaximumSize(QSize(16777215, 40));
        statusLabel->setAutoFillBackground(true);
        statusLabel->setStyleSheet(QString::fromUtf8("font: 75 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        statusLabel->setFrameShape(QFrame::NoFrame);

        gridLayout_20->addWidget(statusLabel, 1, 0, 1, 1);


        gridLayout_3->addLayout(gridLayout_20, 0, 1, 1, 1);


        gridLayout_13->addWidget(map, 0, 0, 1, 1);


        mainLayout->addLayout(gridLayout_13, 0, 0, 1, 1);

        rightLayout = new QGridLayout();
        rightLayout->setSpacing(6);
        rightLayout->setObjectName(QString::fromUtf8("rightLayout"));
        logBrowser = new QTextBrowser(centralWidget);
        logBrowser->setObjectName(QString::fromUtf8("logBrowser"));
        QSizePolicy sizePolicy12(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy12.setHorizontalStretch(0);
        sizePolicy12.setVerticalStretch(12);
        sizePolicy12.setHeightForWidth(logBrowser->sizePolicy().hasHeightForWidth());
        logBrowser->setSizePolicy(sizePolicy12);
        logBrowser->setStyleSheet(QString::fromUtf8("font: 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));

        rightLayout->addWidget(logBrowser, 0, 0, 1, 1);

        passList = new QTableWidget(centralWidget);
        if (passList->rowCount() < 2)
            passList->setRowCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        passList->setVerticalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        passList->setVerticalHeaderItem(1, __qtablewidgetitem1);
        passList->setObjectName(QString::fromUtf8("passList"));
        QSizePolicy sizePolicy13(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy13.setHorizontalStretch(0);
        sizePolicy13.setVerticalStretch(1);
        sizePolicy13.setHeightForWidth(passList->sizePolicy().hasHeightForWidth());
        passList->setSizePolicy(sizePolicy13);
        passList->setStyleSheet(QString::fromUtf8("font: 75 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
""));
        passList->setRowCount(2);
        passList->setColumnCount(0);
        passList->horizontalHeader()->setVisible(false);
        passList->horizontalHeader()->setMinimumSectionSize(60);
        passList->horizontalHeader()->setDefaultSectionSize(60);
        passList->verticalHeader()->setMinimumSectionSize(60);
        passList->verticalHeader()->setDefaultSectionSize(60);
        passList->verticalHeader()->setHighlightSections(false);

        rightLayout->addWidget(passList, 1, 0, 1, 1);


        mainLayout->addLayout(rightLayout, 0, 1, 1, 1);


        gridLayout_16->addLayout(mainLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        simButton->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Travel Simulation", nullptr));
        strategyLabel->setText(QCoreApplication::translate("MainWindow", "\347\255\226\347\225\245", nullptr));
        planButton->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\350\247\204\345\210\222", nullptr));
        changePlanButton->setText(QCoreApplication::translate("MainWindow", "\346\233\264\346\224\271\350\256\241\345\210\222", nullptr));
        startTimeLabel->setText(QCoreApplication::translate("MainWindow", "\351\242\204\350\256\241\345\207\272\345\217\221\346\227\266\351\227\264", nullptr));
        startTime->setDisplayFormat(QCoreApplication::translate("MainWindow", "yyyy/MM/dd HH:mm", nullptr));
        latestArrivalTimeLabel->setText(QCoreApplication::translate("MainWindow", "\345\270\214\346\234\233\345\210\260\350\276\276\346\227\266\351\227\264", nullptr));
        latestArrivalTime->setDisplayFormat(QCoreApplication::translate("MainWindow", "yyyy/MM/dd HH:mm", nullptr));
        depatureLabel->setText(QCoreApplication::translate("MainWindow", "\345\207\272\345\217\221\345\234\260", nullptr));
        destinationLabel->setText(QCoreApplication::translate("MainWindow", "\347\233\256\347\232\204\345\234\260", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\345\201\234\347\225\231", nullptr));
        visitTime->setSuffix(QCoreApplication::translate("MainWindow", " \346\227\266", nullptr));
        visitTime->setPrefix(QString());
        passLable->setText(QCoreApplication::translate("MainWindow", "\351\200\224\347\273\217", nullptr));
        cityBox->setCurrentText(QString());
        addCity->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
        removeCity->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\346\205\242", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\345\277\253", nullptr));
#if QT_CONFIG(tooltip)
        speedSlider->setToolTip(QCoreApplication::translate("MainWindow", "\350\260\203\346\225\264\346\250\241\346\213\237\351\200\237\345\272\246", nullptr));
#endif // QT_CONFIG(tooltip)
        simButton->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\346\250\241\346\213\237", nullptr));
        sysTime->setDisplayFormat(QCoreApplication::translate("MainWindow", "yyyy/MM/dd HH:mm", nullptr));
        addPassenger->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
        removePassenger->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "\345\260\232\346\234\252\345\274\200\345\247\213\346\227\205\350\241\214", nullptr));
        QTableWidgetItem *___qtablewidgetitem = passList->verticalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "\351\200\224\347\273\217\345\237\216\345\270\202", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = passList->verticalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "\345\201\234\347\225\231\346\227\266\351\227\264", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
