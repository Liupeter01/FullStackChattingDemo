#ifndef ADDNEWUSERSTACKWIDGET_H
#define ADDNEWUSERSTACKWIDGET_H

#include <QWidget>
#include <unordered_map>

namespace Ui {
class AddNewUserStackWidget;
}

/*declare*/
class AddUserNameCardWidget;

class AddNewUserStackWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AddNewUserStackWidget(QWidget *parent = nullptr);
    virtual ~AddNewUserStackWidget();

signals:

private slots:

private:
    Ui::AddNewUserStackWidget *ui;
    std::unordered_map<
        /*uuid*/
        std::size_t,
        /*namecard widget*/
        std::shared_ptr<AddUserNameCardWidget>
    > m_friendList;
};

#endif // ADDNEWUSERSTACKWIDGET_H
