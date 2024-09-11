#ifndef LOADINGWAITDIALOG_H
#define LOADINGWAITDIALOG_H

#include <QDialog>

namespace Ui {
class LoadingWaitDialog;
}

class LoadingWaitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadingWaitDialog(QWidget *parent = nullptr);
    ~LoadingWaitDialog();

private:
    void setupWindowsAttribute();
    void setupLoadingImage();

private:
    Ui::LoadingWaitDialog *ui;
};

#endif // LOADINGWAITDIALOG_H
