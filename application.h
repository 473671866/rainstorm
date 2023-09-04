#ifndef APPLICATION_H
#define APPLICATION_H

//qt
#include <QMainWindow>
#include <QAction>
#include <QMenuBar>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QButtonGroup>

//stl
#include <list>
#include <vector>
#include <algorithm>
#include <thread>

//self
#include "virtualmachinecontrol.h"
#include "profile.h"
#include "dialog.h"


struct VirtualMachineManager
{
    QPushButton* button;
    QMenu * menu;
    QAction *delete_action;
    QAction *edit_action;
    QAction *power_on_action;
    QAction *power_off_action;
    QAction *connect_action;
    QAction *disconnect_action;
    QString file_path;
    QString file_name;
    QString host;
    QString port;
    VirtualMachineControl *control;
};


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    //添加
    void AddVirtualMachine();
    //开启全部
    void PowerOnAllVirtualMachine();
    //关闭全部
    void PowerOffAllViatualMachine();
    //切换
    void SwitchVirtualMachine();
private:
    Ui::MainWindow *ui;
    //切换按钮数量
    int button_count;
    //切换按钮组
    QButtonGroup* button_group;
    //配置文件
    ProFileA *profile;
};
#endif // APPLICATION_H
