#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    SettingsDialog(QWidget *parent = 0);
    void fillForm(unsigned int inRow, unsigned int inColumn);
    unsigned int getInRow();
    unsigned int getInColumn();
    ~SettingsDialog();

protected:
    void changeEvent(QEvent *e);
    unsigned int inRow;
    unsigned int inColumn;

private:
    Ui::SettingsDialog *ui;

private slots:
    void on_buttonBox_accepted();
};

#endif // SETTINGSDIALOG_H
