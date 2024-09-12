#ifndef CHATTINGRECORDVIEW_H
#define CHATTINGRECORDVIEW_H

#include <QWidget>
#include <QObject>
#include <QEvent>
#include <QPaintEvent>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QListWidget>

class ChattingRecordView : public QListWidget
{
    Q_OBJECT
public:
    explicit ChattingRecordView(QWidget *parent = nullptr);

public:
    /*back insert*/
    void pushBackItem(QWidget *item);

    /*front insert*/
    void pushFrontItem(QWidget *item);

    /*insert randomly*/
    void randomInsertItem(QWidget *pos, QWidget *item);;

    static constexpr std::size_t max_size = 16777215;

protected:
    bool eventFilter(QObject *object, QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void registerSignal();

private slots:
    void slot_rangeChanged(int min, int max);

private:
    bool m_newdataarrived;
    QVBoxLayout *m_layout;
    QScrollArea *m_scroll;
};

#endif // CHATTINGRECORDVIEW_H
