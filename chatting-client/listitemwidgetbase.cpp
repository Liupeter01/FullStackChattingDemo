#include "listitemwidgetbase.h"

ListItemWidgetBase::ListItemWidgetBase(QWidget *parent)
    : QWidget{parent}, m_type(ListItemType::Default) {}

ListItemWidgetBase::~ListItemWidgetBase() {}

const ListItemType ListItemWidgetBase::getItemType() const { return m_type; }

void ListItemWidgetBase::setItemType(ListItemType type) { m_type = type; }

QSize ListItemWidgetBase::sizeHint() const { return QSize(width, height); }
