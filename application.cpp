#include "application.h"
#include "ui_application.h"
#include "button.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化
    this->setWindowTitle("rainstorm");
    this->button_count = 0;
    this->button_group = new QButtonGroup(this);
    this->profile = new ProFileA(PROFILE_NAME);

    //菜单栏
    QMenuBar* menu_bar = new QMenuBar(this);
    QMenu* menu = menu_bar->addMenu("管理");

    //全部开启
    QAction *power_on_all = menu->addAction("全部开启");
    connect(power_on_all, &QAction::triggered, this, &MainWindow::PowerOnAllVirtualMachine);

    //全部关闭
    QAction *power_off_all = menu->addAction("全部关闭");
    connect(power_off_all, &QAction::triggered, this, &MainWindow::PowerOffAllViatualMachine);

    //添加虚拟机
    QAction *add_virtual_machine = menu->addAction("添加虚拟机");
    connect(add_virtual_machine, &QAction::triggered, this, &MainWindow::AddVirtualMachine);
    this->setMenuBar(menu_bar);

    //加载配置文件
    std::list<std::string> sections = this->profile->ReadPrivateProfileSectionNames();
    for(std::string &section : sections)
    {
        std::map<std::string, std::string> res = this->profile->ReadProfileSectionsA(section);
        std::string path = res["FilePath"];
        std::string host = res["Host"];
        std::string port = res["Port"];

        //创建按钮
        VirtualMachineControl *contorl = new VirtualMachineControl(path.c_str(), this);
        ui->stackedWidget->addWidget(contorl);
        Button *button = new Button(contorl, path.c_str(), section.c_str(), host.c_str(), port.c_str(), ui, this);
        connect(button, &QPushButton::clicked, this, &MainWindow::SwitchVirtualMachine);
        ui->verticalLayout->addWidget(button, this->button_count);
        ui->verticalLayout->addStretch();
        this->button_group->addButton(button);
        this->button_count++;
    }
    return;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete profile;
}

void MainWindow::AddVirtualMachine()
{
    //打开对话框
    Dialog dialog(this);
    dialog.setWindowTitle("添加");
    dialog.exec();

    if(dialog.Legal()){
        QString path = dialog.GetPath();
        QString name = path.section("/", -1).section(".", 0, 0);
        QString host = dialog.GetHost();
        QString port = dialog.GetPort();

        //保存配置文件
        this->profile->WriteProfileStringsA(name.toLatin1().data(), "FilePath", path.toLatin1().data());
        this->profile->WriteProfileStringsA(name.toLatin1().data(), "Host", host.toLatin1().data());
        this->profile->WriteProfileStringsA(name.toLatin1().data(), "Port", port.toLatin1().data());

        //创建按钮
        VirtualMachineControl *contorl = new VirtualMachineControl(path, this);
        ui->stackedWidget->addWidget(contorl);

        Button *button = new Button(contorl, path, name, host, port, ui, this);
        connect(button, &QPushButton::clicked, this, &MainWindow::SwitchVirtualMachine);
        this->button_group->addButton(button);
        ui->verticalLayout->addWidget(button, this->button_count);
        this->button_count++;
    }

    ui->statusbar->showMessage("添加成功", 3000);
    return;
}

void MainWindow::PowerOnAllVirtualMachine()
{
    for(QWidget* item: this->button_group->buttons()){
        Button *button = static_cast<Button*>(item);
        button->PowerOnVirtualMachine();
    }
    return;
}

void MainWindow::PowerOffAllViatualMachine()
{
    for(QWidget* item: this->button_group->buttons()){
        Button *button = static_cast<Button*>(item);
        button->PowerOffVirtualMachine();
    }
    return;
}

void MainWindow::SwitchVirtualMachine()
{
    Button *btn = static_cast<Button *>(sender());
    ui->stackedWidget->setCurrentWidget(btn->m_control);
    return;
}
