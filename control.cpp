#include "control.h"
#include "ui_control.h"

VirtualMachineControl::VirtualMachineControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::control)
{
    ui->setupUi(this);
}

VirtualMachineControl::~control()
{
    delete ui;
}
