#ifndef CONTROL_H
#define CONTROL_H

#include <QWidget>

namespace Ui {
class VirtualMachineControl;
}

class VirtualMachineControl : public QWidget
{
    Q_OBJECT

public:
    explicit VirtualMachineControl(QWidget *parent = nullptr);
    ~VirtualMachineControl();

private:
    Ui::VirtualMachineControl *ui;
};

#endif // CONTROL_H
