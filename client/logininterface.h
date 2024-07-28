#ifndef LOGININTERFACE_H
#define LOGININTERFACE_H

#include <QDialog>

namespace Ui {
class LoginInterface;
}

class LoginInterface : public QDialog
{
    Q_OBJECT

public:
    explicit LoginInterface(QWidget *parent = nullptr);
    ~LoginInterface();

private:
    void registerSignal();

signals:
    void switchWindow();

private:
    Ui::LoginInterface *ui;
};

#endif // LOGININTERFACE_H
