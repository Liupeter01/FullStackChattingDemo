#ifndef CHATTINGDLGMAINFRAME_H
#define CHATTINGDLGMAINFRAME_H

#include <QDialog>
#include <QIcon>
#include <QLabel>
#include <QVector>
#include <memory>

class SideBarWidget;
class QMouseEvent;

namespace Ui {
class ChattingDlgMainFrame;
}

class ChattingDlgMainFrame : public QDialog {
  Q_OBJECT

public:
  explicit ChattingDlgMainFrame(QWidget *parent = nullptr);
  virtual ~ChattingDlgMainFrame();

protected:
  /*chat list test*/
  void addItemToChatListTest();

  /*customlized functions*/
  bool eventFilter(QObject *object, QEvent *event) override;

private:
  void registerSignal();

  /*register action for search edit ui item*/
  void registerSearchEditAction();
  void registerSearchEditSignal();
  void updateSearchUserButton();

  /*windows switcher(accroding to m_dlgMode)*/
  void switchRelevantListWidget();

  /*register handler for chatting*/
  void updateMyChat();

  /*register handler for Contact list*/
  void updateMyContact();

  /*delegate sidebar widget*/
  void addLabel(SideBarWidget *widget);
  void resetAllLabels(SideBarWidget *new_widget);

  /*clear search_edit according to mouse position*/
  void clearSearchByMousePos(QMouseEvent *event);

  /* switch to chatting page by using stackedWidget */
  void switchChattingPage();

  /* switch to new user page by using stackedWidget */
  void switchNewUserPage();

private slots:
  /*search text changed*/
  void slot_search_text_changed();

  void slot_load_more_record();

  /*when side bar button activated, then display relevant info on show_list*/
  void slot_display_chat_list();
  void slot_display_contact_list();

private:
  /*reserve for search line edit*/
  QAction *m_searchAction;

  /*reserve for cancel user searching*/
  QAction *m_cancelAction;
  Ui::ChattingDlgMainFrame *ui;

  QVector<std::shared_ptr<SideBarWidget>> m_qlabelSet;

  /*cur qlabel*/
  SideBarWidget *m_curQLabel;

  enum class ChattingDlgMode {
    ChattingDlgChattingMode,  // show multiple user chatting dialog
    ChattingDlgSearchingMode, // allow user to search
    chattingDlgContactMode    // show contacts
  } m_dlgMode;
};

#endif // CHATTINGDLGMAINFRAME_H
