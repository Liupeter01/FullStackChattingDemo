#include "tools.h"
#include <QStyle>

void Tools::refreshQssStyle(QWidget *widget){
    widget->style()->unpolish(widget);
    widget->style()->polish(widget);
}
