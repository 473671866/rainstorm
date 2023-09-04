#include "button.h"
#include "profile.h"
#include "dialog.h"
#include "utils.h"

Button::Button(VirtualMachineControl *contorl, QString path, QString name, QString host, QString port, Ui::MainWindow *p, QWidget* parent)
    :QPushButton(parent),
    m_name(name),
    m_path(path),
    m_host(host),
    m_port(port),
    m_control(contorl),
    ui(p)
{
    QFont font = this->font();
    font.setPointSize(8);
    this->setFont(font);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setText(this->m_name);
    this->setFixedSize(110,40);
    this->setCheckable(true);
    this->setStyleSheet(

        /**正常情况下样式**/
        "QPushButton{"
        "background-color: rgba(0,0,0,0);"
        "border-radius: 3px;"
        "color: rgb(0,0,0);"
        "}"

        /**鼠标停留在按钮上的样式**/
        "QPushButton::hover{"
        "background-color: rgb(200, 200, 200);"
        "}"

        /**鼠标按压下去的样式**/
        "QPushButton::pressed,QPushButton::checked{"
        "background-color: rgb(200, 200, 200);"
        "}"
        );

    //添加右键菜单
    m_menu = new QMenu(this);
    connect(this ,&QPushButton::customContextMenuRequested, this, &Button::CustomContextMenuProc);

    //开机
    m_power_on_action = new QAction("开机",this);
    m_menu->addAction(m_power_on_action);
    connect(m_power_on_action, &QAction::triggered, this, &Button::PowerOnVirtualMachine);

    //关机
    m_power_off_action = new QAction("关机",this);
    m_menu->addAction(m_power_off_action);
    connect(m_power_off_action, &QAction::triggered, this, &Button::PowerOffVirtualMachine);

    //链接
    m_connect_action = new QAction("链接", this);
    m_menu->addAction(m_connect_action);
    connect(m_connect_action, &QAction::triggered, this, &Button::ConnectVirtualMachine);

    //断开
    m_disconnect_action = new QAction("断开", this);
    m_menu->addAction(m_disconnect_action);
    connect(m_disconnect_action, &QAction::triggered, this, &Button::DisconnectVirtualMachine);

    //编辑
    m_edit_action = new QAction("编辑", this);
    m_menu->addAction(m_edit_action);
    connect(m_edit_action, &QAction::triggered, this, &Button::EditVirtualMachine);

    //删除
    m_delete_action = new QAction("删除",this);
    m_menu->addAction(m_delete_action);
    connect(m_delete_action, &QAction::triggered, this, &Button::DeleteVirtualMachine);
}

void Button::CustomContextMenuProc(const QPoint &pos)
{
    this->m_menu->exec(QCursor::pos());
    return;
}

void Button::DeleteVirtualMachine()
{
    //删除控件
    ui->verticalLayout->removeWidget(this);
    ui->stackedWidget->removeWidget(this->m_control);

    //删除配置文件
    ProFileA profile(PROFILE_NAME);
    profile.DeleteProfileSection(this->m_name.toLatin1().data());

    this->deleteLater();
}

void Button::EditVirtualMachine()
{
    //对话框
    Dialog dialog(this);
    dialog.setWindowTitle("编辑");
    dialog.exec();

    if(dialog.Legal()){
        //删掉旧的
        ProFileA profile(PROFILE_NAME);
        profile.DeleteProfileSection(this->m_path.toLatin1().data());
        //文件路径
        this->m_path = dialog.GetPath();
        //文件名
        this->m_name = this->m_path.section("/", -1).section(".", 0, 0);
        //ip
        this->m_host = dialog.GetHost();
        //端口
        this->m_port = dialog.GetPath();
        //更新
        profile.WriteProfileStringsA(this->m_name.toLatin1().data(), "FilePath", this->m_path.toLatin1().data());
        profile.WriteProfileStringsA(this->m_name.toLatin1().data(), "Host", this->m_host.toLatin1().data());
        profile.WriteProfileStringsA(this->m_name.toLatin1().data(), "Port", this->m_port.toLatin1().data());
        ui->statusbar->showMessage("修改成功", 3000);
    }

    return;
}

void Button::PowerOnVirtualMachine()
{
    if(this->m_control->IsStart() == false){
         std::thread([=]{
             if(this->m_control->PowerOn()){
                 auto util = Utils::ProcessUtils::get_instance();
                 int pid = util->GetProcessId(PROCESS_NAME);
                 this->m_control->SetPid(pid);
             }
         }).detach();
    }
    return;
}

void Button::PowerOffVirtualMachine()
{
    if(this->m_control->IsStart() == false){
         this->m_control->PowerOff();
         Utils::ProcessUtils::get_instance()->RemoveProcessId(this->m_control->GetPid());
    }
    return;
}

void Button::ConnectVirtualMachine()
{
    if(this->m_control->IsLogin() == false){
        if(!this->m_control->Login(this->m_host, this->m_port)){
            ui->statusbar->showMessage("链接失败", 3000);
            return;
        }
    }
    this->m_control->Show();
    return;
}

void Button::DisconnectVirtualMachine()
{
    if(this->m_control->IsLogin() == true){
        this->m_control->Logout();
    }
    return;
}
