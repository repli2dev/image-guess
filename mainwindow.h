#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QPushButton>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    void showPicture();
    void resizeEvent(QResizeEvent* e);
    void closeEvent(QCloseEvent *event);
    void hideAndEmpty();
    void showButtons();
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    QPushButton *buttons[10][10];
    bool fullscreen;

private:
    Ui::MainWindow *ui;
    QStringList files;  // List of images found in opened dir
    unsigned int current; // Offset in files array of current displayed image
    QString path;  // Name of opened directory

private slots:
    void on_actionLast_triggered();
    void on_actionFirst_triggered();
    void on_actionContents_triggered();
    void on_actionAbout_Qt_triggered();
    void on_actionUnhide_triggered();
    void on_actionAbout_triggered();
    void on_actionPrevious_triggered();
    void on_actionNext_triggered();
    void on_actionFullscreen_triggered();
    void on_actionClose_triggered();
    void on_actionClose_dir_triggered();
    void on_actionOpen_dir_triggered();
};

#endif // MAINWINDOW_H
