#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    currentSession = NULL;
    connect(ui->openBtn,       SIGNAL(clicked()),         this, SLOT(onOpenDialogClick()));
    connect(ui->message,       SIGNAL(textChanged()),     this, SLOT(onMessageChanged()));
    connect(ui->pushButton_3,       SIGNAL(clicked()),         this, SLOT(onSaveDialogClick()));
    connect(ui->pushButton_2,     SIGNAL(clicked()),         this, SLOT(onDecodeBtnClicked()));
    connect(ui->zoomSlider,    SIGNAL(valueChanged(int)), this, SLOT(onZoomChanged(int)));
    connect(ui->pushButton,      SIGNAL(clicked()),         this, SLOT(onAboutBtnClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onOpenDialogClick()
{
    const char* action;
    if (this->isEncrypting)action = "Open image to be encoded...";
    else action = "Open image to be decoded...";
    QString filename = QFileDialog::getOpenFileName(this, tr(action), "/", tr("Image Files (*.png *.jpg *.bmp)"));
    if (filename == NULL)return;
    this->setWindowTitle("Security_ToolKit_Steganography");
    QImage* workingImage = new QImage();
    workingImage->load(filename);
    QGraphicsScene* scene = new QGraphicsScene;
    scene->addPixmap(QPixmap::fromImage(*workingImage));
    ui->modImageView->setScene(scene);
    if (currentSession != NULL)delete currentSession;
    currentSession = new Security(*workingImage,1);
    currentSession->setMessage(ui->message->toPlainText());
}

void MainWindow::onSaveDialogClick() {
    if (currentSession == NULL)return;
    QString filename = QFileDialog::getSaveFileName(this, tr("Save encoded image..."), "/home/mde/Pictures/untitled.png", tr("Images (*.png *.bmp *.jpg)"));
    currentSession->encodeImage()->save(filename);
}


void MainWindow::onMessageChanged()
{
    const QString& message = ui->message->toPlainText();
    if (currentSession == NULL)return;
    currentSession->setMessage(message);
    updateModImageView();
}

void MainWindow::onDecodeBtnClicked() {
    if (currentSession == NULL)return;
    ui->message->setPlainText(currentSession->decodeImage());
}

void MainWindow::updateModImageView() {
    ui->modImageView->scene()->clear();
    ui->modImageView->scene()->addPixmap(QPixmap::fromImage(*(currentSession->encodeImage())));
}

void MainWindow::onZoomChanged(int factor) {
    QMatrix matrix;
    double rfactor = factor / 100.0;
    matrix.scale(rfactor, rfactor);
    ui->modImageView->setMatrix(matrix);
}

void MainWindow::onAboutBtnClicked() {
    QMessageBox msgBox;
    msgBox.setText("Security Tool Kit Written in Qt  and C++\n\n"
                   "Special Thanks to Pushpendra sir\n");
    msgBox.setWindowTitle("About our Project");
    msgBox.exec();
}
