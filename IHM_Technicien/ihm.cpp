#include "ihm.h"
#include "ui_ihm.h"
#include <QApplication>
#include <QDebug>


IHM::IHM(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::IHM)
{
    ui->setupUi(this);
}

IHM::~IHM()
{
    delete ui;
}

void IHM::on_pushButton_quitter_clicked()
{

    qApp->quit();

}

void IHM::on_pushButton_regler_clicked()
{
    QDateTime dateHeure = ui->dateTimeEdit->dateTime();
    ui->lineEdit->setText(dateHeure.toString("dd/MM/yyyy HH:mm:ss"));
}

