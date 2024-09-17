#ifndef CHATTINGDLGMAINFRAME_H
#define CHATTINGDLGMAINFRAME_H

#include <QDialog>
#include <QIcon>

namespace Ui {
class ChattingDlgMainFrame;
}

class ChattingDlgMainFrame : public QDialog {
  Q_OBJECT

public:
  explicit ChattingDlgMainFrame(QWidget *parent = nullptr);
  ~ChattingDlgMainFrame();

protected:
  void addItemToShowLists();

private:
  void registerSignal();

  /*register action for search edit ui item*/
  void registerSearchEditAction();
  void registerSearchEditSignal();
  void updateSearchUserButton();

  /*register handler for chatting*/
  void updateMyChat();

  /*register handler for Contact list*/
  void updateMyContact();


private slots:
  void slot_load_more_record();

private:
  /*reserve for search line edit*/
  QAction *m_searchAction;

  /*reserve for cancel user searching*/
  QAction *m_cancelAction;
  Ui::ChattingDlgMainFrame *ui;

  enum class ChattingDlgMode {
    ChattingDlgChattingMode, // show multiple user chatting dialog
    ChattingDlgSearchingMode // allow user to add new/search exists friend
  } m_dlgMode;
};

#endif // CHATTINGDLGMAINFRAME_H
