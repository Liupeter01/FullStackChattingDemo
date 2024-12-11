#ifndef MAINFRAMESEARCHLISTS_H
#define MAINFRAMESEARCHLISTS_H

#include "addusernamecarddialog.h"
#include "def.hpp"
#include "mainframeshowlists.h"
#include <memory>
#include <optional>

// forward declearation
class QListWidgetItem;

class MainFrameSearchLists : public MainFrameShowLists {
  Q_OBJECT

public:
  MainFrameSearchLists(QWidget *parent = nullptr);
  virtual ~MainFrameSearchLists();

signals:
  /*
   * waiting for data from remote server
   * status = true: activate
   * status = false: deactivate
   */
  void signal_waiting_for_data(bool status);

  /*if target user has already became a auth friend with current user*/
  void signal_switch_user_profile(std::shared_ptr<UserNameCard> info);

private slots:
  void slot_search_username(std::optional<std::shared_ptr<UserNameCard>> info,
                            ServiceStatus status);

private:
  /*register signal*/
  void registerSignal();

  /*add a startup widget inside the list*/
  void addNewUserWidget();

  /*add style sheet*/
  void addStyleSheet();

private:
  /*close status dialog*/
  std::shared_ptr<QDialog> m_Dlg;
};

#endif // MAINFRAMESEARCHLISTS_H
