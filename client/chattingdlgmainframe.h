#ifndef CHATTINGDLGMAINFRAME_H
#define CHATTINGDLGMAINFRAME_H

#include <map>
#include <QIcon>
#include <optional>
#include <QDialog>

namespace Ui {
class ChattingDlgMainFrame;
}

class ChattingDlgMainFrame : public QDialog
{
    Q_OBJECT

public:
    explicit ChattingDlgMainFrame(QWidget *parent = nullptr);
    ~ChattingDlgMainFrame();

private:
    void registerSignal();
    void updateSearchUserButton();

private:
    Ui::ChattingDlgMainFrame *ui;
};

#endif // CHATTINGDLGMAINFRAME_H
