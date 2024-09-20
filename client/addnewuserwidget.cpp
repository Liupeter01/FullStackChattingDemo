#include "tools.h"
#include "addnewuserwidget.h"
#include "ui_addnewuserwidget.h"

AddNewUserWidget::AddNewUserWidget(QWidget *parent)
    : ListItemWidgetBase(parent)
    , ui(new Ui::AddNewUserWidget)
{
    ui->setupUi(this);

    /*set item type to add new user*/
    this->setItemType(ListItemType::AddNewusr);

    /*load qimage for image display*/
    Tools::loadImgResources({"proceed_image.png", "add_image.png"},
                            (ui->proceed_image->width() + ui->add_image->width()) / 2,
                            (ui->proceed_image->height() + ui->add_image->height()) / 2);

    Tools::setQLableImage(ui->add_image, "add_image.png");
    Tools::setQLableImage(ui->proceed_image, "proceed_image.png");
}

AddNewUserWidget::~AddNewUserWidget()
{
    delete ui;
}
