/********************************************************************************
** Form generated from reading UI file 'ihm.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IHM_H
#define UI_IHM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IHM
{
public:
    QPushButton *pushButton_regler;
    QPushButton *pushButton_quitter;
    QLineEdit *lineEdit;
    QDateTimeEdit *dateTimeEdit;

    void setupUi(QWidget *IHM)
    {
        if (IHM->objectName().isEmpty())
            IHM->setObjectName("IHM");
        IHM->resize(415, 347);
        pushButton_regler = new QPushButton(IHM);
        pushButton_regler->setObjectName("pushButton_regler");
        pushButton_regler->setGeometry(QRect(280, 160, 71, 21));
        pushButton_quitter = new QPushButton(IHM);
        pushButton_quitter->setObjectName("pushButton_quitter");
        pushButton_quitter->setGeometry(QRect(280, 180, 71, 21));
        lineEdit = new QLineEdit(IHM);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(50, 60, 301, 51));
        dateTimeEdit = new QDateTimeEdit(IHM);
        dateTimeEdit->setObjectName("dateTimeEdit");
        dateTimeEdit->setGeometry(QRect(50, 110, 301, 51));
        QFont font;
        font.setPointSize(20);
        font.setBold(true);
        dateTimeEdit->setFont(font);

        retranslateUi(IHM);

        QMetaObject::connectSlotsByName(IHM);
    } // setupUi

    void retranslateUi(QWidget *IHM)
    {
        IHM->setWindowTitle(QCoreApplication::translate("IHM", "IHM", nullptr));
        pushButton_regler->setText(QCoreApplication::translate("IHM", "R\303\251gler", nullptr));
        pushButton_quitter->setText(QCoreApplication::translate("IHM", "Quitter", nullptr));
    } // retranslateUi

};

namespace Ui {
    class IHM: public Ui_IHM {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IHM_H
