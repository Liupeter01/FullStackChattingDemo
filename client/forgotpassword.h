#ifndef FORGOTPASSWORD_H
#define FORGOTPASSWORD_H

#include <QLabel>

class ForgotPassword : public QLabel
{
    Q_OBJECT;

public:
    ForgotPassword();

public:
    ForgotPassword(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~ForgotPassword();

signals:
    void clicked();

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;

    /*mouse enter selected section*/
    virtual void enterEvent(QEnterEvent *event) override;

    /*mouse leave*/
    virtual void leaveEvent(QEvent *event) override;
};

#endif // FORGOTPASSWORD_H
