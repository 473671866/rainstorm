#include "virtualmachinecontrol.h"
#include "ui_virtualmachinecontrol.h"

VirtualMachineControl::VirtualMachineControl(QString path, QWidget *parent):
    QWidget(parent),
    ui(new Ui::VirtualMachineControl),
    file_path(path)
{
    ui->setupUi(this);
    this->host_handle = VIX_INVALID_HANDLE;
    this->power_on = false;
    this->login = false;
    this->show = false;
    this->dispaly = nullptr;
}

VirtualMachineControl::~VirtualMachineControl()
{
    delete ui;
    Vix_ReleaseHandle(this->host_handle);
}

bool VirtualMachineControl::Link()
{
    if(this->host_handle != VIX_INVALID_HANDLE){
        return true;
    }

    bool success = false;
    VixHandle handle = VixHost_Connect(VIX_API_VERSION, CONNTYPE, HOSTNAME, HOSTPORT, USERNAME, PASSWORD, 0, VIX_INVALID_HANDLE, NULL, NULL);
    VixError error = VixJob_Wait(handle, VIX_PROPERTY_JOB_RESULT_HANDLE, &this->host_handle, VIX_PROPERTY_NONE);
    if(!VIX_FAILED(error)){
        success = true;
    }
    Vix_ReleaseHandle(handle);
    return success;
}

bool VirtualMachineControl::PowerOn()
{
    if(this->Link())
    {

        //启动
        VixHandle handle = VixVM_Open(this->host_handle, this->file_path.toLatin1().data(), NULL, NULL);

        //等待
        VixHandle vmx_handle = VIX_INVALID_HANDLE;
        VixError error = VixJob_Wait(handle,VIX_PROPERTY_JOB_RESULT_HANDLE,&vmx_handle, VIX_PROPERTY_NONE);

        //启动失败
        if (VIX_FAILED(error)){
            Vix_ReleaseHandle(handle);
            return false;
        }

        //释放
        Vix_ReleaseHandle(handle);
        handle = VIX_INVALID_HANDLE;


        //获取虚拟机当前状态
        int powerState = 0;
        error = Vix_GetProperties(vmx_handle, VIX_PROPERTY_VM_POWER_STATE,&powerState,VIX_PROPERTY_NONE);
        if(VIX_OK == error){
            //已经是开机状态了
            if(powerState & VIX_POWERSTATE_POWERED_ON){
                Vix_ReleaseHandle(handle);
                Vix_ReleaseHandle(vmx_handle);
                this->power_on = true;
                return true;
            }
        }

        //开机
        handle = VixVM_PowerOn(vmx_handle,VMPOWEROPTIONS,VIX_INVALID_HANDLE, NULL, NULL);

        //等待开机
        error = VixJob_Wait(handle, VIX_PROPERTY_NONE);

        //开机失败
        if (VIX_FAILED(error)){
            Vix_ReleaseHandle(handle);
            return false;
        }

        //释放
        Vix_ReleaseHandle(handle);
        Vix_ReleaseHandle(vmx_handle);
        this->power_on = true;
        return true;
    }

    return false;
}

bool VirtualMachineControl::PowerOff()
{

    if(this->Link())
    {
        //启动
        VixHandle handle = VixVM_Open(this->host_handle, this->file_path.toLatin1().data(), NULL, NULL);

        //等待
        VixHandle vmx_handle = VIX_INVALID_HANDLE;
        VixError error = VixJob_Wait(handle,VIX_PROPERTY_JOB_RESULT_HANDLE,&vmx_handle, VIX_PROPERTY_NONE);

        //启动失败
        if (VIX_FAILED(error))return false;

        //释放
        Vix_ReleaseHandle(handle);
        handle = VIX_INVALID_HANDLE;

        //获取虚拟机当前状态
        int powerState = 0;
        error = Vix_GetProperties(vmx_handle, VIX_PROPERTY_VM_POWER_STATE,&powerState,VIX_PROPERTY_NONE);
        if(VIX_OK == error){
            //已经是关机状态了
            if(powerState & VIX_POWERSTATE_POWERED_OFF){
                Vix_ReleaseHandle(handle);
                Vix_ReleaseHandle(vmx_handle);
                this->power_on = false;;
                return true;
            }
        }

        //关机
        handle = VixVM_PowerOff(vmx_handle,VIX_VMPOWEROP_NORMAL,NULL, NULL);

        //等待开机
        error = VixJob_Wait(handle, VIX_PROPERTY_NONE);

        //开机失败
        if (VIX_FAILED(error))return false;

        //释放
        Vix_ReleaseHandle(handle);
        Vix_ReleaseHandle(vmx_handle);
        this->power_on = false;
        return true;
    }

    return false;
}

bool VirtualMachineControl::Login(QString h, QString p)
{
    this->host = h;
    this->port = p;

    this->client = rfbGetClient(8, 3, 4);
    client->serverHost = _strdup(h.toLatin1().data());
    client->serverPort = p.toInt();
    client->programName = _strdup("VMware VNC");
    this->login = rfbInitClient(client, NULL, NULL);
    return this->login;
}

void VirtualMachineControl::Logout()
{
    if(this->client && this->dispaly){
        ui->verticalLayout_2->removeWidget(this->dispaly);
        this->dispaly->deleteLater();
        this->dispaly = nullptr;

        rfbClientCleanup(this->client);
        this->client = nullptr;

        this->login = false;
        this->show = false;
    }
}

bool VirtualMachineControl::Show()
{
    if(this->client){
        if(this->show == false){
            this->dispaly = new DispalyLabel(this->client, this);
            ui->verticalLayout_2->addWidget(this->dispaly);
            connect(&this->m_timer, &QTimer::timeout, this, &VirtualMachineControl::FetchUpdate);
            this->m_timer.start(16);
            this->show = true;
            return true;
        }
    }
    return false;
}

bool VirtualMachineControl::IsLogin()
{
    return this->login;
}

bool VirtualMachineControl::IsStart()
{
    return this->power_on;
}

int VirtualMachineControl::GetPid()
{
    return this->pid;
}

void VirtualMachineControl::SetPid(int pid)
{
    this->pid = pid;
}

void VirtualMachineControl::FetchUpdate()
{
    if (client && WaitForMessage(client, 10) > 0) {
        if (HandleRFBServerMessage(client)) {
            this->m_image = new QImage (client->frameBuffer, client->width, client->height, QImage::Format_RGB32);
            this->dispaly->setPixmap(QPixmap::fromImage(*this->m_image));
            delete this->m_image;
            this->m_image = nullptr;
        }
    }
}

