#ifndef BUTTONDISPLAYSWITCHING_H
#define BUTTONDISPLAYSWITCHING_H

#include "tools.h"
#include <QWidget>
#include <QPushButton>

class ButtonDisplaySwitching
    : public QPushButton
{
    Q_OBJECT

public:
    explicit ButtonDisplaySwitching(QWidget *parent = nullptr);
    ~ButtonDisplaySwitching();

signals:
    void update_display();

public:
    const PushButtonState &getState() const;

protected:
    /*press then switch icon*/
    virtual void mousePressEvent(QMouseEvent *event) override;

    /*release button then switch to hover status*/
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

    /*mouse enter selected section*/
    virtual void enterEvent(QEnterEvent *event) override;

    /*mouse leave*/
    virtual void leaveEvent(QEvent *event) override;

private:
    PushButtonState m_state;
};

#endif // BUTTONDISPLAYSWITCHING_H
