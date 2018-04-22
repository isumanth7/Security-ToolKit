#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QFileDialog>
#include <QMessageBox>
#include "Security.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void updateModImageView();
    ~MainWindow();
public slots:
    void onOpenDialogClick();
    void onSaveDialogClick();
    void onMessageChanged();
    void onDecodeBtnClicked();
    void onZoomChanged(int factor);
    void onAboutBtnClicked();
private:
    Ui::MainWindow *ui;
    bool isEncrypting;
    Security* currentSession;
};

#endif // MAINWINDOW_H


