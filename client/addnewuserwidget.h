#ifndef ADDNEWUSERWIDGET_H
#define ADDNEWUSERWIDGET_H

#include "listitemwidgetbase.h"

namespace Ui {
class AddNewUserWidget;
}

class AddNewUserWidget : public ListItemWidgetBase {
  Q_OBJECT

public:
  explicit AddNewUserWidget(QWidget *parent = nullptr);
  ~AddNewUserWidget();

private:
  Ui::AddNewUserWidget *ui;
};

#endif // ADDNEWUSERWIDGET_H
