#include "usertagwidget.h"
#include "ui_usertagwidget.h"
#include <QFontMetricsF>

UserTagWidget::UserTagWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::UserTagWidget), m_tagSize(width(), height()) {
  ui->setupUi(this);

  /*load image for unselected*/
  loadImage();

  /*style sheet*/
  addStyleSheet();
}

UserTagWidget::~UserTagWidget() { delete ui; }

void UserTagWidget::setTagName(const QString &text) {
  ui->tag_label->setText(text);
  ui->tag_label->adjustSize();

  /*update tag size according to label input*/
  updateTagSize();
}

QSize UserTagWidget::sizeHint() const { return m_tagSize; }

const QSize UserTagWidget::getImageSize() {
  return QSize(image_width, image_height);
}

void UserTagWidget::slot_close(QString str, LabelState state) {
  /*emit signal to the outside*/
  emit signal_close(str, state);
}

void UserTagWidget::loadImage() {
  Tools::setQLableImage(ui->unselected_click, "unselect_tag.png");
}

void UserTagWidget::addStyleSheet() {
  ui->widget->setStyleSheet(QString(
      "#UserTagWidget "
      "QWidget{background-color:#daf6e7;color:#48bf56;border-radius:10px;}"));
}

void UserTagWidget::registerSignal() {
  connect(ui->unselected_click, &OnceClickableQLabel::clicked, this,
          &UserTagWidget::slot_close);
}

void UserTagWidget::updateTagSize() {
  QFontMetricsF fontMetrics(ui->tag_label->font());

  auto text_width = fontMetrics.horizontalAdvance(ui->tag_label->text());
  auto text_height = fontMetrics.height();

  qDebug() << "text_width = " << text_width << "\ntext_height = " << text_height
           << "\n";

  this->setFixedWidth(text_width + image_width + COMPENSATION_WIDTH);
  this->setFixedHeight(text_height + COMPENSATION_HEIGHT);

  m_tagSize = QSize(this->width(), this->height());
}
