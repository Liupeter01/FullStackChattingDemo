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

    /*register action for search edit ui item*/
    void registerSearchEditAction();
    void registerSearchEditSignal();
    void updateSearchUserButton();

private:
    /*reserve for search line edit*/
    QAction* m_searchAction;

    /*reserve for cancel user searching*/
    QAction* m_cancelAction;
    Ui::ChattingDlgMainFrame *ui;
};

#endif // CHATTINGDLGMAINFRAME_H
