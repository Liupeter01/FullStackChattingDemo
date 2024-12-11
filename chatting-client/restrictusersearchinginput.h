#ifndef RESTRICTUSERSEARCHINGINPUT_H
#define RESTRICTUSERSEARCHINGINPUT_H

#include <QLineEdit>

class RestrictUserSearchingInput : public QLineEdit {
  Q_OBJECT

public:
  RestrictUserSearchingInput(QWidget *parent = nullptr);
  RestrictUserSearchingInput(std::size_t max_length, QWidget *parent = nullptr);
  virtual ~RestrictUserSearchingInput();
  void setMaxLength(std::size_t max_length);

protected:
  void constraintTextLength(const QString &text);

private:
  /*when lineedit text changed, then limit text length*/
  void registerTextChangedSignal();

private:
  /*constraints the max input length of username*/
  std::size_t m_maxLen;
};

#endif // RESTRICTUSERSEARCHINGINPUT_H
