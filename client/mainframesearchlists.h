#ifndef MAINFRAMESEARCHLISTS_H
#define MAINFRAMESEARCHLISTS_H

#include <memory>
#include "addusernamecarddialog.h"
#include "mainframeshowlists.h"

// forward declearation
class QListWidgetItem;

class MainFrameSearchLists : public MainFrameShowLists {
  Q_OBJECT

public:
  MainFrameSearchLists(QWidget *parent = nullptr);
  virtual ~MainFrameSearchLists();

private:
  /*add a startup widget inside the list*/
  void addNewUserWidget();

  /*add style sheet*/
  void addStyleSheet();
};

#endif // MAINFRAMESEARCHLISTS_H
