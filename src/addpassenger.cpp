#include "addpassenger.h"
#include "ui_addpassenger.h"

addPassenger::addPassenger(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addPassenger)
{
    ui->setupUi(this);
}

addPassenger::~addPassenger()
{
    delete ui;
}
