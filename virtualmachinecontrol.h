#ifndef VIRTUALMACHINECONTROL_H
#define VIRTUALMACHINECONTROL_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QImage>
#include <QTimer>
#include <QDebug>
#include "vix.h"
#include "rfbclient.h"
#include "dispaly.h"

#pragma comment(lib,"vix.lib")
#pragma comment(lib,"vixd.lib")

#define  CONNTYPE    VIX_SERVICEPROVIDER_VMWARE_WORKSTATION
#define  HOSTNAME ""
#define  HOSTPORT 0
#define  USERNAME ""
#define  PASSWORD ""
#define  VMPOWEROPTIONS   VIX_VMPOWEROP_LAUNCH_GUI


namespace Ui {
class VirtualMachineControl;
}

class VirtualMachineControl : public QWidget
{
    Q_OBJECT

public:
    explicit VirtualMachineControl(QString path, QWidget *parent = nullptr);
    ~VirtualMachineControl();

public:
    //链接
    bool Link();
    //开机
    bool PowerOn();
    //关机
    bool PowerOff();
    //登录
    bool Login(QString h, QString p);
    //登出
    void Logout();
    //绘制
    bool Show();

    bool IsLogin();

    bool IsStart();

    int GetPid();

    void SetPid(int pid);

public slots:
    void FetchUpdate();

private:
    Ui::VirtualMachineControl *ui;
    DispalyLabel *dispaly;
    int pid;
    QString file_path;
    QString host;
    QString port;
    QImage *m_image;
    QTimer m_timer;
    rfbClient* client;
    VixHandle host_handle;
    bool power_on;
    bool login;
    bool show;
};

#endif // VIRTUALMACHINECONTROL_H
