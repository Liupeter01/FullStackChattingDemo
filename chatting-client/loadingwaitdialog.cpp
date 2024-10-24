#include "loadingwaitdialog.h"
#include "ui_loadingwaitdialog.h"
#include <QMovie>

LoadingWaitDialog::LoadingWaitDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::LoadingWaitDialog) {
  ui->setupUi(this);

  /*setup ui display*/
  setupWindowsAttribute();

  /*set up ui display*/
  setupLoadingImage();
}

LoadingWaitDialog::~LoadingWaitDialog() { delete ui; }

void LoadingWaitDialog::setupWindowsAttribute() {
  setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint |
                 Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);

  /*set background to transparent*/
  setAttribute(Qt::WA_TranslucentBackground);

  // setFixedSize()
}

void LoadingWaitDialog::setupLoadingImage() {
  /*add loading gif*/
  QMovie *movie = new QMovie(QT_DEMO_HOME "/res/loading.gif");
  ui->loading_label->setMovie(movie);
  movie->start();
}
