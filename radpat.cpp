#include "radpat.h"
#include "ui_radpat.h"

RadPat::RadPat(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RadPat)
{
    ui->setupUi(this);
}

RadPat::~RadPat()
{
    delete ui;
}
