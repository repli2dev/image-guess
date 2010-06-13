#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helpdialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include <QDesktopWidget>
#include <QPixmap>
#include <QPushButton>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Restore position
    QSettings settings("JanDrabek", "ImageGuess");

    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());

    // Init
    this->fullscreen = FALSE;

    // Align picture to center in both directions
    ui->canvas->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    // Prepare for showing buttons
    int totalWidth = ui->canvas->width();
    int totalHeight = ui->canvas->height();
    int k = 0;

    // Set minimum column size
    for (int i = 0; i < 10; ++i) {
        ui->buttonGrid->setColumnMinimumWidth(i,totalWidth/10);
    }
    for (int i = 0; i < 10; ++i) {
        ui->buttonGrid->setRowMinimumHeight(i,totalHeight/10);
    }

    // Create buttons
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
                 this->buttons[i][j] = new QPushButton(tr("%1").arg(++k));
                 this->buttons[i][j]->setFixedHeight(totalHeight/10);
                 this->buttons[i][j]->setFixedWidth(totalWidth/10);
                 this->buttons[i][j]->hide();
                 QObject::connect(this->buttons[i][j], SIGNAL(clicked()), buttons[i][j], SLOT(hide()));
                 ui->buttonGrid->addWidget(this->buttons[i][j],i,j);
        }
    }

}
void MainWindow::closeEvent(QCloseEvent *event){
    QSettings settings("JanDrabek","ImageGuess");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* e)
{
    // Resize canvas
    ui->canvas->setGeometry(0,0,ui->centralWidget->width(),ui->centralWidget->height());
    int totalWidth = ui->centralWidget->width();
    int totalHeight = ui->canvas->height();

    // Set minimum column size
    for (int i = 0; i < 10; ++i) {
        ui->buttonGrid->setColumnMinimumWidth(i,totalWidth/10);
    }
    for (int i = 0; i < 10; ++i) {
        ui->buttonGrid->setRowMinimumHeight(i,totalHeight/10);
    }

    // Resize picture if it's displayed
    if(this->files.count() > 0 && this->current != 0) {
        this->showPicture();
    }

    // Resize buttons
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
                 this->buttons[i][j]->setFixedHeight(totalHeight/10);
                 this->buttons[i][j]->setFixedWidth(totalWidth/10);
             }
    }
}


void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_actionOpen_dir_triggered()
{
    // Ask for directory name
    QString path;
    this->path = QFileDialog::getExistingDirectory(this,tr("Open dir"),QDir::currentPath());

    if(!this->path.isEmpty()){
        // Show message in status bar
        ui->statusBar->showMessage(tr("Opened directory: ") + this->path);

        // Create directory object for reading from dir
        QDir directory = QDir(this->path);

        // Set filters to show only images
        QStringList filters;
        filters << "*.jpg" << "*.jpeg" << "*.gif" << "*.png" << "*.tiff";
        directory.setNameFilters(filters);

        // Get all files
        this->files = directory.entryList(QDir::Files | QDir::NoSymLinks, QDir::Name | QDir::IgnoreCase);

        // Reset counter
        this->current = 0;

        // For debuging
        //for (int i = 0; i < files.size(); ++i)
        //    std::cout << files.at(i).toLocal8Bit().constData() << std::endl;

        this->setWindowTitle(QString(this->path).append(tr(" - ImageGuess")));
    }
}

void MainWindow::on_actionClose_dir_triggered()
{
    if(!this->path.isEmpty()){
        // Set all variables to empty, revert window title
        this->current = 0;
        this->path = "";
        this->files.clear();
        this->setWindowTitle(tr("ImageGuess"));
        this->hideAndEmpty();

        // Show message in status bar
        ui->statusBar->showMessage(tr("Directory closed"),3000);
    }
}

void MainWindow::on_actionClose_triggered()
{
    this->close();
}

void MainWindow::on_actionFullscreen_triggered()
{
    if(this->fullscreen){ // Escape from full screen
        QWidget::showNormal();
        ui->mainToolBar->show();
        ui->statusBar->show();
        ui->toolBar->show();
        ui->menuBar->setStyleSheet("");
        this->fullscreen = FALSE;
    } else {
        QMessageBox::information(this,tr("Switching to fullscreen mode"),tr("You are now switching to fullscreen mode.\nFor return use F11 key."));
        QWidget::showFullScreen();
        ui->mainToolBar->hide();
        ui->statusBar->hide();
        ui->toolBar->hide();
        ui->menuBar->setStyleSheet("background: black; height: 0px;");
        this->fullscreen = TRUE;
    }

}

void MainWindow::hideAndEmpty(){
    // Hide buttons and empty canvas
    QPixmap empty;
    ui->canvas->setPixmap(empty);
    // Hide buttons
    this->on_actionUnhide_triggered();
}

void MainWindow::on_actionNext_triggered()
{
    if(this->files.count() <= this->current){
        QMessageBox::information(this,tr("Finished"),tr("Guessing was finished. Click on next for new round."));
        // Reset back to first picture
        this->current = 0;
        this->hideAndEmpty();
        return;
    }

    // Increase pointer
    this->current++;

    // Show picture
    this->showButtons();
    this->showPicture();

}

void MainWindow::showButtons(){
    // Show all buttons
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
                 this->buttons[i][j]->show();
             }
    }
}


void MainWindow::on_actionPrevious_triggered()
{
    // Decrease pointer
    if(this->current > 1){
        this->current = this->current - 1;
    }

    if(this->files.count() > 0 && this->current != 0){
        this->showButtons();
        this->showPicture();
    }
}

void MainWindow::showPicture(){

    // Prepare name and dir
    QString name;
    name = QString(this->files.at((this->current-1)).toLocal8Bit().constData());
    name.prepend('/');
    name.prepend(this->path);
    //std::cout << "'" << qPrintable(name) << "'" << std::endl;

    // Load picture
    QPixmap image;
    image.load(name);

    // Rescale
    QPixmap image2(image.scaledToHeight(ui->canvas->height()));
    if(image2.width() > ui->canvas->width()){
        QPixmap image3(image.scaledToWidth(ui->canvas->width()));
        ui->canvas->setPixmap(image3);
    } else {
        ui->canvas->setPixmap(image2);
    }

    // Show message in status bar
    ui->statusBar->showMessage(tr("Image ") + QString::number(this->current) + tr(" from ") + QString::number(this->files.count()));

}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About ImageGuess"),
                 tr("<p>The <b>ImageGuess</b> show picture part by part and it offers you\n"
                    "perfect oportunity to quess what is in the picture.</p><p>Author: Jan Drabek<br />E-mail: <a href=\"mailto: me@jandrabek.cz\">me@jandrabek.cz</a><br />Licence: GPL</p>"));

}

void MainWindow::on_actionUnhide_triggered()
{
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
                 this->buttons[i][j]->hide();
             }
    }
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this,tr("About Qt"));
}

void MainWindow::on_actionContents_triggered()
{
    QDialog *help = new HelpDialog;
    help->show();
}

void MainWindow::on_actionFirst_triggered()
{
    if(this->files.count() > 0){
        this->current = 1;
        this->showButtons();
        this->showPicture();
    }
}

void MainWindow::on_actionLast_triggered()
{
    if(this->files.count() > 0){
        this->current = this->files.count();
        this->showButtons();
        this->showPicture();
    }
}
