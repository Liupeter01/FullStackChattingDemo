#ifndef CHATTINGPAGEWIDGET_H
#define CHATTINGPAGEWIDGET_H

#include <QWidget>

namespace Ui {
class ChattingPageWidget;
}

class ChattingPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChattingPageWidget(QWidget *parent = nullptr);
    ~ChattingPageWidget();

private:
    Ui::ChattingPageWidget *ui;
};

#endif // CHATTINGPAGEWIDGET_H
