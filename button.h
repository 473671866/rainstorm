#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>
#include <QMenu>
#include <QAction>
#include "ui_application.h"
#include "virtualmachinecontrol.h"

class Button:public QPushButton
{
public:
    Button(VirtualMachineControl *contorl, QString path, QString name, QString host, QString port, Ui::MainWindow *p, QWidget* parent = nullptr);

public slots:
    //右键菜单
    void CustomContextMenuProc(const QPoint &pos);
    //删除
    void DeleteVirtualMachine();
    //编辑
    void EditVirtualMachine();
    //开机
    void PowerOnVirtualMachine();
    //关机
    void PowerOffVirtualMachine();
    //链接
    void ConnectVirtualMachine();
    //断开
    void DisconnectVirtualMachine();

public:
    //菜单
    QMenu *m_menu;
    //删除菜单
    QAction *m_delete_action;
    //编辑菜单
    QAction *m_edit_action;
    //开机
    QAction *m_power_on_action;
    //关机
    QAction *m_power_off_action;
    //链接
    QAction *m_connect_action;
    //断开
    QAction *m_disconnect_action;
    //文件名
    QString m_name;
    //文件路径
    QString m_path;
    //ip
    QString m_host;
    //端口
    QString m_port;
    //控制面板
    VirtualMachineControl *m_control;
    //ui
    Ui::MainWindow *ui;
};

#endif // BUTTON_H
