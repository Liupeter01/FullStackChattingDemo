#ifndef ADDNEWUSERWIDGET_H
#define ADDNEWUSERWIDGET_H

#include <QWidget>

namespace Ui {
class AddNewUserWidget;
}

class AddNewUserWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AddNewUserWidget(QWidget *parent = nullptr);
    ~AddNewUserWidget();
    virtual QSize sizeHint() const;

public:
    static constexpr std::size_t width = 250;
    static constexpr std::size_t height = 70;

private:
    Ui::AddNewUserWidget *ui;
};

#endif // ADDNEWUSERWIDGET_H
