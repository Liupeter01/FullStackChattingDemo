#ifndef LISTITEMWIDGET_H
#define LISTITEMWIDGET_H

#endif // LISTITEMWIDGET_H

#include <QWidget>

namespace Ui{
class ListItemWidget;
}

class ListItemWidget : public QWidget
{
    Q_OBJECT

public:
    ListItemWidget(QWidget *parent = nullptr);
    ~ListItemWidget();

    virtual QSize sizeHint() const;

    void setItemDisplay(const QString &_username,
                        const QString &_avator,
                        const QString &_last_message);

public:
    static constexpr std::size_t width = 250;
    static constexpr std::size_t height= 70;

private:
    Ui::ListItemWidget *ui;
    QString m_username;
    QString m_avatorpath;
    QString m_lastmsg;
};
