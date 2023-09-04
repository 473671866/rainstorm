#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

QString Dialog::GetHost()
{
    return ui->host_line->text();
}

QString Dialog::GetPort()
{
    return ui->port_line->text();
}

QString Dialog::GetPath()
{
    return this->file_path;
}

bool Dialog::Legal()
{
    return ui->path_line->text().size() > 0 && ui->host_line->text().size() > 0 && ui->path_line->text().size() > 0;
}

void Dialog::on_pushButton_clicked()
{
    this->file_path = QFileDialog::getOpenFileName(this,"选择文件", "../", "*.vmx");
    ui->path_line->setText(this->file_path);
}

