#ifndef PASSWORDDISPLAYSWITCHING_H
#define PASSWORDDISPLAYSWITCHING_H
#include <Qt>
#include <QLabel>
#include <QWidget>
#include "tools.h"

class PasswordDisplaySwitching : public QLabel
{
    Q_OBJECT

public:
    PasswordDisplaySwitching(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~PasswordDisplaySwitching();

    const LabelState & getState() const;

signals:
    void clicked();

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;

    /*mouse enter selected section*/
    virtual void enterEvent(QEnterEvent *event) override;

    /*mouse leave*/
    virtual void leaveEvent(QEvent *event) override;

private:
    LabelState m_state;
};

#endif // PASSWORDDISPLAYSWITCHING_H
