#ifndef REGISTERINTERFACE_H
#define REGISTERINTERFACE_H

#include <QDialog>
#include <QLabel>
//#include <type_traits>

namespace Ui {
class registerinterface;
}

class registerinterface : public QDialog
{
    Q_OBJECT

public:
    explicit registerinterface(QWidget *parent = nullptr);
    ~registerinterface();

private slots:
    void on_verification_button_clicked();

private:
    void setRegisterAttribute();

private:
    Ui::registerinterface *ui;
};

#endif // REGISTERINTERFACE_H
