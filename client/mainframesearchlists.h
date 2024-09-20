#ifndef MAINFRAMESEARCHLISTS_H
#define MAINFRAMESEARCHLISTS_H

#include "mainframeshowlists.h"

// forward declearation
class QListWidgetItem;

class MainFrameSearchLists : public MainFrameShowLists {
  Q_OBJECT

public:
  MainFrameSearchLists(QWidget *parent = nullptr);
  ~MainFrameSearchLists();

private:
  void registerSignal();

  /*add a startup widget inside the list*/
  void addNewUserWidget();

  /*add style sheet*/
  void addStyleSheet();

private slots:
  /*user click the item shown in the ListWidget*/
  void slot_item_clicked(QListWidgetItem *clicked_item);

private:
};

#endif // MAINFRAMESEARCHLISTS_H
