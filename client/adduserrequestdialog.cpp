#include <QDebug>
#include <QScrollBar>
#include <algorithm>
#include "tools.h"
#include "adduserrequestdialog.h"
#include "ui_adduserrequestdialog.h"

AddUserRequestDialog::AddUserRequestDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddUserRequestDialog)
    , m_existing_cur_pos(QPoint(COMPENSATION_WIDTH, COMPENSATION_HEIGHT))   /*init existing tag current pos*/
    , m_selected_cur_pos(QPoint(COMPENSATION_WIDTH, COMPENSATION_HEIGHT))
{
    ui->setupUi(this);

     /*register signal<->slot*/
    registerSignal();

    /*set up placeholder text*/
    setupDefaultInfo();

    /*set up ui style*/
    setupWindowStyle();

    /*load image for usertag widget*/
    Tools::loadImgResources({"unselect_tag.png"}, UserTagWidget::getImageSize().width(), UserTagWidget::getImageSize().height());
}

AddUserRequestDialog::~AddUserRequestDialog()
{
    delete ui;
}

void AddUserRequestDialog::registerSignal()
{
    /*bind with show more label click event*/
    //connect(ui->show_more_label, &OnceClickableQLabel::clicked,
    //    this, &AddUserRequestDialog::slot_show_more_label);

    /*bind with press enter inside tag_input widget*/
    connect(ui->tag_input, &RestrictUserSearchingInput::returnPressed, this, &AddUserRequestDialog::slot_input_tag_press_enter);

    /*bind with textchange inside tag_input*/
    connect(ui->tag_input, &RestrictUserSearchingInput::textChanged, this, &AddUserRequestDialog::slot_input_tag_textchange);

    /*bind with editfinished event inside tag_input*/
    //connect(ui->tag_input, &RestrictUserSearchingInput::editingFinished, this, &AddUserRequestDialog::slot_input_tag_finished);

    //connect(ui->tag_display, &OnceClickableQLabel::clicked, this, &AddUserRequestDialog::slo)
}

void AddUserRequestDialog::setupDefaultInfo()
{
    /*input label
     * 1. setup start pos
     * 2. setup widget height
     * 3. **limit user input ammount to 21**
     */
    ui->tag_input->move(2,2);
    ui->tag_input->setFixedHeight(36);
    ui->tag_input->setMaxLength(21);

    /*hide status display bar*/
    ui->user_tag_display_bar->hide();

    ui->nick_name_edit->setPlaceholderText(QString("I'am test_friend"));
    ui->request_msg_edit->setPlaceholderText(QString("I'am test_sender"));
    ui->tag_input->setPlaceholderText(QString("Select or search tags"));
}

void AddUserRequestDialog::setupWindowStyle()
{
    /*hide dialog title*/
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    ui->scrollArea->horizontalScrollBar()->setHidden(true);
    ui->scrollArea->verticalScrollBar()->setHidden(true);
    ui->scrollArea->installEventFilter(this);

    this->setModal(true);
}

bool AddUserRequestDialog::eventFilter(QObject *object, QEvent *event)
{
    if(object == ui->scrollArea){
        if(event->type() == QEvent::Enter){
            ui->scrollArea->verticalScrollBar()->setHidden(false);
        }
        else if(event->type() == QEvent::Leave){
            ui->scrollArea->verticalScrollBar()->setHidden(true);
        }
    }
    return QDialog::eventFilter(object, event);
}

void AddUserRequestDialog::addNewTag(const QString &text)
{
    /*clear input text*/
    ui->tag_input->clear();

    /*we find it inside existing key*/
    auto it = std::find(m_existing_key.begin(), m_existing_key.end(), text);
    if(it != m_existing_key.end()){
        return;
    }

    /*create a new widget*/
    UserTagWidget* tag(new UserTagWidget(ui->input_tag_widget));
    tag->setTagName(text);
    tag->setObjectName("UserDefTag");

    //connect signal slot for close signal
    connect(tag, &UserTagWidget::signal_close, this, &AddUserRequestDialog::slot_remove_selected_tag);

    /*add new tag to the vector container*/
    createSelectedTag(text, tag);

    /*calculate do we need to generate this object at a new line*/
    if(m_selected_cur_pos.x() + tag->width() > ui->input_tag_widget->width()){
        m_selected_cur_pos.setX(COMPENSATION_WIDTH);
        m_selected_cur_pos.setY(m_selected_cur_pos.y() + tag->height() + COMPENSATION_HEIGHT);
    }

    /*move it to new place*/
    tag->move(m_selected_cur_pos);
    tag->show();

    /*update m_selected_cur_pos to the next new place*/
    m_selected_cur_pos.setX(m_selected_cur_pos.x() + tag->width() + COMPENSATION_WIDTH);
    if(m_selected_cur_pos.x() + COMPENSATION_WIDTH > ui->input_tag_widget->width())  {
        /*move tag_input widget position to avoid collison with UserTagWidget*/
        m_selected_cur_pos.setX(COMPENSATION_WIDTH);
        m_selected_cur_pos.setY(m_selected_cur_pos.y() + tag->height() + COMPENSATION_HEIGHT);
    }

    /*move tag_input's position*/
    ui->tag_input->move(m_selected_cur_pos);

    /*extend the height of input tag widget*/
    if(ui->input_tag_widget->height() < m_selected_cur_pos.y() + tag->height() + COMPENSATION_HEIGHT){
        ui->input_tag_widget->setFixedHeight(2 * ui->input_tag_widget->height());
    }
}

void AddUserRequestDialog::createSelectedTag(const QString &text, UserTagWidget *widget)
{
    m_selected_key.push_back(text);
    m_selected_label.insert(std::pair<QString, std::shared_ptr<UserTagWidget>>(text,
         std::shared_ptr<UserTagWidget>(widget, [](UserTagWidget *){}))
    );
}

void AddUserRequestDialog::closeDialog()
{
    qDebug() << "closing AddUserRequestDialog";
    this->hide();
    deleteLater();
}

void AddUserRequestDialog::on_confirm_button_clicked()
{
    closeDialog();
}

void AddUserRequestDialog::on_cancel_button_clicked()
{
    closeDialog();
}

void AddUserRequestDialog::slot_input_tag_press_enter()
{
    /*no text*/
    if(ui->tag_input->text().isEmpty()){
        return;
    }

    /*add it to new tag*/
    addNewTag(ui->tag_input->text());

    /*hide input notification widget*/
    ui->user_tag_display_bar->hide();
}

void AddUserRequestDialog::slot_remove_selected_tag()
{

}

void AddUserRequestDialog::slot_input_tag_textchange(const QString &text)
{
    ui->tag_input->setFocus(Qt::FocusReason::ActiveWindowFocusReason);
}
