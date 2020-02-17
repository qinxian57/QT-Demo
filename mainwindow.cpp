#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QIcon>
#include <QStyleFactory>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //托盘初始化
    initTrayIcon();

    //左侧停靠窗口 Dock Widget, Tree Widget, List Widget
    this->setCentralWidget(ui->listWidget);
    ui->dockWidget->setWidget(ui->treeFiles);
    //dock状态，显示/隐藏，对应true/false
    mDockStatus = true;

    //
    initTree();
    //Tree设置右键菜单
    initTreeMenu();





//    //更改窗口菜单栏样式
//    QFont font("consolas", 10, QFont::Normal);
//    ui->menuBar->setFont(font);
//    ui->menuBar->setStyle(QStyleFactory::create("fusion"));



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initTrayIcon()
{
    //新建QSystemTrayIcon对象,
    //mSysTrayIcon = new QSystemTrayIcon(this);
    mSysTrayIcon = std::make_shared<QSystemTrayIcon>(this);

    //将icon设到QSystemTrayIcon对象中
    mSysTrayIcon->setIcon(QIcon(":/images/tray_icon.png"));
    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    mSysTrayIcon->setToolTip("Demo");

    //给QSystemTrayIcon添加槽函数
    connect(mSysTrayIcon.get(), SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));

    //建立托盘操作的菜单
    createTrayActions();
}

void MainWindow::createTrayActions()
{
    mMenu = std::make_shared<QMenu>(this);

    //新增菜单项---显示主界面
    mShowMainAction = std::make_shared<QAction>("显示", this);
    connect(mShowMainAction.get(),SIGNAL(triggered()),this,SLOT(on_showMainAction()));
    mMenu->addAction(mShowMainAction.get());

    //增加分隔符
    mMenu->addSeparator();

    //新增菜单项---退出程序
    mExitAppAction = std::make_shared<QAction>("关闭", this);
    connect(mExitAppAction.get(),SIGNAL(triggered()),this,SLOT(on_exitAppAction()));
    mExitAppAction->setIcon(QIcon(":/images/close.png"));
    mMenu->addAction(mExitAppAction.get());

//    //等宽字体12号
//    QFont font("consolas", 12, QFont::Normal);
//    mMenu->setFont(font);
//    mMenu->setStyle(QStyleFactory::create("fusion"));

    //把QMenu赋给QSystemTrayIcon对象
    mSysTrayIcon->setContextMenu(mMenu.get());
}

void MainWindow::closeEvent ( QCloseEvent * e )
{
    e->ignore();
    //隐藏窗口到托盘
    this->hide();
    mSysTrayIcon->show();
}

void MainWindow::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
    case QSystemTrayIcon::DoubleClick:
        //双击托盘图标，显示主程序窗口
        this->show();
        break;
    default:
        break;
    }
}


/*
* 当在系统托盘点击菜单内的显示主界面操作
*/
void MainWindow::on_showMainAction()
{
    this->show();
    this->activateWindow();
}

/*
* 当在系统托盘点击菜单内的退出程序操作
*/
void MainWindow::on_exitAppAction()
{
    qApp->exit(0);
}

void MainWindow::initTreeMenu()
{
    mTreeMenu = std::make_shared<QMenu>(this);

    mAddFolderAction = std::make_shared<QAction>("添加目录", this);
    connect(mAddFolderAction.get(),SIGNAL(triggered()),this, SLOT(on_actAddFolder()));
    mTreeMenu->addAction(mAddFolderAction.get());

    mAddFileAction = std::make_shared<QAction>("添加文件", this);
    connect(mAddFileAction.get(),SIGNAL(triggered()),this, SLOT(on_actAddFiles()));
    mTreeMenu->addAction(mAddFileAction.get());

    mDltNodeAction = std::make_shared<QAction>("删除节点", this);
    connect(mDltNodeAction.get(),SIGNAL(triggered()),this, SLOT(on_actDeleteItem()));
    mTreeMenu->addAction(mDltNodeAction.get());

    //给控件设置上下文菜单策略
    ui->treeFiles->setContextMenuPolicy(Qt::CustomContextMenu);
    //方式一
//    //鼠标右键点击控件时会发送一个void QWidget::customContextMenuRequested(const QPoint &pos)信号
//    //给信号设置相应的槽函数
//    connect(ui->treeFiles,&QTreeWidget::customContextMenuRequested,[=](const QPoint &pos)
//    {
//        //qDebug()<<pos;//参数pos用来传递右键点击时的鼠标的坐标，这个坐标一般是相对于控件左上角而言的
//        mTreeMenu->exec(QCursor::pos());
//    });
}

//方式二
void MainWindow::on_treeFiles_customContextMenuRequested(QPoint pos)
{
    //QTreeWidgetItem* curItem = ui->treeFiles->itemAt(pos);  //获取当前被点击的节点

    mTreeMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
}

void MainWindow::on_treeFiles_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{ //当前节点选择变化时触发
    Q_UNUSED(previous);
//    if  (current==NULL)
//        return;

    int var=current->type();//节点的类型

    switch(var)
    {
        case  itTopItem: //顶层节点
          mAddFolderAction->setEnabled(true);
          mAddFileAction->setEnabled(true);
          mDltNodeAction->setEnabled(false);    //顶层节点不能删除
          break;

        case  itGroupItem: //文件组节点
          mAddFolderAction->setEnabled(true);
          mAddFileAction->setEnabled(true);
          mDltNodeAction->setEnabled(true);
          break;

        case  itImageItem: //文件节点
          mAddFolderAction->setEnabled(false); //图片节点下不能添加目录节点
          mAddFileAction->setEnabled(true);
          mDltNodeAction->setEnabled(true);
          displayImage(current); //显示图片
          break;
    }
}

void MainWindow::initTree()
{
    //初始化Tree
    QString dataStr=""; // Item的Data 存储的string
    ui->treeFiles->clear();//清除目录树所有节点
    QIcon icon;
    icon.addFile(":/images/tray_icon.png"); //设置ICON的图标
    QTreeWidgetItem*  item=new QTreeWidgetItem(MainWindow::itTopItem); //新建节点时设定类型为 itTopItem
    //item->setIcon(MainWindow::colItem,icon); //设置第1列的图标
    item->setText(MainWindow::colItem,"图片管理"); //设置第1列的文字
    item->setText(MainWindow::colItemType,"type=itTopItem");  //设置第2列的文字
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
    item->setCheckState(colItem,Qt::Checked);//设置为选中
    item->setData(MainWindow::colItem,Qt::UserRole,QVariant(dataStr)); //设置节点第1列的Qt::UserRole的Data
    ui->treeFiles->addTopLevelItem(item);//添加顶层节点

}

void MainWindow::on_actAddFolder()
{// 选择一个文件夹,作为当前节点的子节点加入
    QString dir=QFileDialog::getExistingDirectory();//选择目录
    if (!dir.isEmpty()) //选择目录名称不为空
    {
        //QTreeWidgetItem *parItem; //节点
        QTreeWidgetItem* parItem=ui->treeFiles->currentItem(); //当前节点
        addFolderItem(parItem,dir);//在父节点下面添加一个组节点
    }
}

void MainWindow::addFolderItem(QTreeWidgetItem *parItem, QString dirName)
{
    //添加一个目录节点
    //QIcon   icon(":/images/icons/open3.bmp");//设置ICON的图标
    QString NodeText=getFinalFolderName(dirName); //从一个完整目录名称里，获得最后的文件夹名称
    QTreeWidgetItem *item; //节点
    item=new QTreeWidgetItem(MainWindow::itGroupItem); //新建节点, 设定type为 itGroupItem
    //item->setIcon(colItem,icon); //设置图标
    item->setText(colItem,NodeText); //最后的文件夹名称，第1列
    item->setText(colItemType,"type=itGroupItem"); //完整目录名称，第2列
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate); //设置节点选项
    item->setCheckState(colItem,Qt::Checked); //节点选中
    item->setData(colItem,Qt::UserRole,QVariant(dirName)); //设置角色为Qt::UserRole的Data,存储完整目录名称
    parItem->addChild(item); //在父节点下面添加子节点

    //addImageItem(item, "tray_icon.png");
}
QString MainWindow::getFinalFolderName(const QString &fullPathName)
{
    //从一个完整目录名称里，获得最后的文件夹名称
    int cnt=fullPathName.length(); //字符串长度
    int i=fullPathName.lastIndexOf("/");//  最后一次出现的位置
    QString str=fullPathName.right(cnt-i-1); //获得最后的文件夹的名称
    return str;
}

void MainWindow::on_actAddFiles()
{//添加图片文件节点
    QStringList files=QFileDialog::getOpenFileNames(this,"选择一个或多个文件","","Images(*.png)");//多选文件
    if (files.isEmpty()) //如果一个文件都没选
        return;
    QTreeWidgetItem *parItem,*item; //节点
    item=ui->treeFiles->currentItem(); //当前节点
    if (item->type()==itImageItem) //若当前节点是图片节点，取其父节点作为父节点
       parItem=item->parent();
    else //否则取当前节点为父节点
       parItem=item;
    for (int i = 0; i < files.size(); ++i)
    {
        QString aFilename=files.at(i); //得到StringList里的一行，也就是一个文件名
        addImageItem(parItem,aFilename); //添加一个图片节点
    }
}

void MainWindow::addImageItem(QTreeWidgetItem *parItem, QString aFilename)
{
    //添加一个图片文件节点
    QIcon   icon(":/images/tray_icon.png");//ICON的图标
    QString NodeText=getFinalFolderName(aFilename); //获得最后的文件名称
    QTreeWidgetItem *item; //节点
    item=new QTreeWidgetItem(MainWindow::itImageItem); //新建节点时设定类型为 itImageItem
    item->setIcon(colItem,icon); //设置图标
    item->setText(colItem,NodeText); //最后的文件夹名称
    //item->setText(colSecond,"type=itImageItem; data="+aFilename); //完整目录名称
    item->setText(colItemType,"type=itImageItem"); //完整目录名称
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate); //设置节点选项
    item->setCheckState(colItem,Qt::Checked); //节点选中
    item->setData(colItem,Qt::UserRole,QVariant(aFilename)); //设置节点Qt::UserRole的Data,存储完整文件名称
    parItem->addChild(item); //在父节点下面添加子节点
}

void MainWindow::on_actDeleteItem()
{//删除节点
    QTreeWidgetItem *item =ui->treeFiles->currentItem() ; //当前节点
    QTreeWidgetItem *parItem=item->parent () ; //父节点
    parItem->removeChild(item) ;//移除一个子节点，并不会删除

    delete item;
}

void MainWindow::displayImage(QTreeWidgetItem *item)
{//显示图片,节点item存储的图片文件名
    QString filename=item->data(colItem,Qt::UserRole).toString();//获取节点data里存的文件名
    //LabFileName->setText(filename);
    curPixmap.load(filename); //从文件载入图片
    //on_actZoomFitH_triggered(); //自动适应高度显示
}

void MainWindow::on_actionExit_triggered()
{
   on_exitAppAction();
}

void MainWindow::on_actionActive_triggered()
{
}

void MainWindow::on_actionDock_triggered(bool checked)
{
    if(!checked){
        ui->dockWidget->hide();
        mDockStatus = false;
    }
    else{
        ui->dockWidget->show();
        mDockStatus = true;
    }
}
