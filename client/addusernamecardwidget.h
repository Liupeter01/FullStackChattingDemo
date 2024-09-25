#ifndef ADDUSERNAMECARDWIDGET_H
#define ADDUSERNAMECARDWIDGET_H

#include <QFrame>

namespace Ui {
class AddUserNameCardWidget;
}

class AddUserNameCardWidget : public QFrame
{
    Q_OBJECT

public:
    explicit AddUserNameCardWidget(QWidget *parent = nullptr);
    ~AddUserNameCardWidget();
    void setFriendName(const QString &name);
    void setFriendAvator(const QPixmap &pic);
    void setRequestMsg(const QString &msg);

    virtual QSize sizeHint() const;

    static constexpr std::size_t height = 30;
    static constexpr std::size_t width = 30;

private:
    void registerSignal();

signals:
    void signal_close_clicked();

private slots:
    void slot_close_clicked();

private:
    Ui::AddUserNameCardWidget *ui;
};

#endif // ADDUSERNAMECARDWIDGET_H
