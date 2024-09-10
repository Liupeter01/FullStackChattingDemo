#include "restrictusersearchinginput.h"

RestrictUserSearchingInput::RestrictUserSearchingInput(QWidget *parent)
    : RestrictUserSearchingInput(0, parent) {}

RestrictUserSearchingInput::RestrictUserSearchingInput(std::size_t max_length,
                                                       QWidget *parent)
    : m_maxLen(max_length), QLineEdit(parent) {
  /*when lineedit text changed, then limit text length*/
  registerTextChangedSignal();
}

RestrictUserSearchingInput::~RestrictUserSearchingInput() {}

void RestrictUserSearchingInput::setMaxLength(std::size_t max_length) {
  m_maxLen = max_length;
}

void RestrictUserSearchingInput::constraintTextLength(const QString &text) {
  if (m_maxLen <= 0) {
    return;
  }
  QByteArray arr = text.toUtf8();

  /*the size should not exceed m_maxLen*/
  if (arr.size() > m_maxLen) {
    qDebug() << "Input Exceeded! Length = " << arr.size();
    setText(QString::fromUtf8(arr.left(m_maxLen)));
  }
}

void RestrictUserSearchingInput::registerTextChangedSignal() {
  connect(this, &QLineEdit::textChanged, this,
          &RestrictUserSearchingInput::constraintTextLength);
}
