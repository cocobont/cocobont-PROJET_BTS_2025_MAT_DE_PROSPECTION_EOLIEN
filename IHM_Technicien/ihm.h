#ifndef IHM_H
#define IHM_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class IHM;
}
QT_END_NAMESPACE

class IHM : public QWidget
{
    Q_OBJECT

public:
    IHM(QWidget *parent = nullptr);
    ~IHM();

private slots:
    void on_pushButton_quitter_clicked();
    void on_pushButton_regler_clicked();

private:
    Ui::IHM *ui;
};
#endif // IHM_H
