#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::fillForm(unsigned int inRow, unsigned int inColumn){
    ui->inColumn->setValue(inColumn);
    ui->inRow->setValue(inRow);
}

void SettingsDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

unsigned int SettingsDialog::getInRow(){
    return this->inRow;
}
unsigned int SettingsDialog::getInColumn(){
    return this->inColumn;
}

void SettingsDialog::on_buttonBox_accepted()
{
    this->inRow = ui->inRow->value();
    this->inColumn = ui->inColumn->value();
}
