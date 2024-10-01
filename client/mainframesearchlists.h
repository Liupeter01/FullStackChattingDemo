#ifndef MAINFRAMESEARCHLISTS_H
#define MAINFRAMESEARCHLISTS_H

#include "addusernamecarddialog.h"
#include "mainframeshowlists.h"
#include <memory>

// forward declearation
class QListWidgetItem;

class MainFrameSearchLists : public MainFrameShowLists {
  Q_OBJECT

public:
  MainFrameSearchLists(QWidget *parent = nullptr);
  virtual ~MainFrameSearchLists();

private:
  void registerSignal();

  /*add a startup widget inside the list*/
  void addNewUserWidget();

  /*add style sheet*/
  void addStyleSheet();

  /*close status dialog*/
  void closeDialog();

private slots:
  /*user click the item shown in the ListWidget*/
  void slot_item_clicked(QListWidgetItem *clicked_item);

private:
  /*close status dialog*/
  std::shared_ptr<QDialog> m_Dlg;
};

#endif // MAINFRAMESEARCHLISTS_H
