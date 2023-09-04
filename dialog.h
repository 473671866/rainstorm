#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    QString GetHost();
    QString GetPort();
    QString GetPath();
    bool Legal();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialog *ui;
    QString file_path;
};

#endif // DIALOG_H
