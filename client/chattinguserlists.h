#ifndef CHATTINGUSERLISTS_H
#define CHATTINGUSERLISTS_H

#include <QEvent>
#include <QListWidget>

class ChattingUserLists : public QListWidget
{
    Q_OBJECT

public:
    ChattingUserLists(QWidget *parent = nullptr);
    ~ChattingUserLists();

protected:
    /*customlized functions*/
    bool eventFilter(QObject *object, QEvent *event) override;

signals:
    void signal_load_more_record();

private:

};

#endif // CHATTINGUSERLISTS_H
