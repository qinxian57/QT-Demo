#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QIcon>
#include <QStyleFactory>
#include <QFileDialog>
#include <QPushButton>
#include <QLabel>
#include <QPointer>


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

    //Tree Widget
    initTree();
    //Tree设置右键菜单
    initTreeMenu();

    //List Widget
    initList();





//    //更改窗口菜单栏样式
//    QFont font("consolas", 10, QFont::Normal);
//    ui->menuBar->setFont(font);
//    ui->menuBar->setStyle(QStyleFactory::create("fusion"));



}

MainWindow::~MainWindow()
{
    ui->treeFiles->clear();
    ui->listWidget->clear();
    delete ui;
}

void MainWindow::initTrayIcon()
{
    //新建QSystemTrayIcon对象,
    mSysTrayIcon = new QSystemTrayIcon(this);

    //将icon设到QSystemTrayIcon对象中
    mSysTrayIcon->setIcon(QIcon(":/images/tray_icon.png"));
    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    mSysTrayIcon->setToolTip("Demo");

    //给QSystemTrayIcon添加槽函数
    connect(mSysTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));

    //建立托盘操作的菜单
    createTrayActions();
}

void MainWindow::createTrayActions()
{
    mMenu = new QMenu(this);

    //新增菜单项---显示主界面
    mShowMainAction = new QAction("显示", this);
    connect(mShowMainAction,SIGNAL(triggered()),this,SLOT(on_showMainAction()));
    mMenu->addAction(mShowMainAction);

    //增加分隔符
    mMenu->addSeparator();

    //新增菜单项---退出程序
    mExitAppAction = new QAction("关闭", this);
    connect(mExitAppAction,SIGNAL(triggered()),this,SLOT(on_exitAppAction()));
    mExitAppAction->setIcon(QIcon(":/images/close.png"));
    mMenu->addAction(mExitAppAction);

//    //等宽字体12号
//    QFont font("consolas", 12, QFont::Normal);
//    mMenu->setFont(font);
//    mMenu->setStyle(QStyleFactory::create("fusion"));

    //把QMenu赋给QSystemTrayIcon对象
    mSysTrayIcon->setContextMenu(mMenu);
}

void MainWindow::closeEvent ( QCloseEvent * e )
{
    e->ignore();
    //隐藏窗口到托盘
    this->hide();
    mSysTrayIcon->show();
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type()!=QEvent::WindowStateChange) return;
    if(this->windowState()==Qt::WindowMinimized)
    {
        //隐藏窗口到托盘
        event->ignore();
        this->hide();
        mSysTrayIcon->show();
    }
}

void MainWindow::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
    case QSystemTrayIcon::DoubleClick:
        //双击托盘图标，显示主程序窗口
        this->activateWindow();
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
    this->activateWindow();
    this->show();
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
    mTreeMenu = new QMenu(this);

    mAddFolderAction = new QAction("添加目录", this);
    connect(mAddFolderAction,SIGNAL(triggered()),this, SLOT(on_actAddFolder()));
    mTreeMenu->addAction(mAddFolderAction);

    mAddFileAction = new QAction("添加文件", this);
    connect(mAddFileAction,SIGNAL(triggered()),this, SLOT(on_actAddFiles()));
    mTreeMenu->addAction(mAddFileAction);

    mDltNodeAction = new QAction("删除节点", this);
    connect(mDltNodeAction,SIGNAL(triggered()),this, SLOT(on_actDeleteItem()));
    mTreeMenu->addAction(mDltNodeAction);

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

void MainWindow::on_dockWidget_closeEvent(QCloseEvent * event)
{
    ui->actionDock->trigger();
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
    QTreeWidgetItem *item=new QTreeWidgetItem(MainWindow::itTopItem); //新建节点时设定类型为 itTopItem

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
    QTreeWidgetItem *item=new QTreeWidgetItem(MainWindow::itGroupItem); //新建节点, 设定type为 itGroupItem
    //item->setIcon(colItem,icon); //设置图标
    item->setText(colItem,NodeText); //最后的文件夹名称，第1列
    item->setText(colItemType,"type=itGroupItem"); //完整目录名称，第2列
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate); //设置节点选项
    item->setCheckState(colItem,Qt::Checked); //节点选中
    item->setData(colItem,Qt::UserRole,QVariant(dirName)); //设置角色为Qt::UserRole的Data,存储完整目录名称
    parItem->addChild(item); //在父节点下面添加子节点

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
    QTreeWidgetItem *item = new QTreeWidgetItem(MainWindow::itImageItem);
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
{
    QString filename = item->data(colItem,Qt::UserRole).toString();//获取节点data里存的文件名

    QListWidgetItem *img_item = new QListWidgetItem(ui->listWidget);
    img_item->setIcon(QIcon(":/images/tray_icon.png"));
    img_item->setText(filename);
    //设置item项中的文字位置
    img_item->setTextAlignment(Qt::AlignLeft);
    img_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsEditable);

    img_item->setCheckState(Qt::Checked); //设置为选中状态


//    //显示图片,节点item存储的图片文件名
//    QString filename=item->data(colItem,Qt::UserRole).toString();//获取节点data里存的文件名
//    //LabFileName->setText(filename);
//    curPixmap.load(filename); //从文件载入图片
//    //on_actZoomFitH_triggered(); //自动适应高度显示
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

void MainWindow::initList()
{
    //ui->listWidget->resize(200,300);
    //ui->listWidget->setFixedWidth(300);


    //设置item图标大小
    ui->listWidget->setIconSize(QSize(30,30));

    QListWidgetItem *add_item = new QListWidgetItem(ui->listWidget);
    add_item->setIcon(QIcon(":/images/tray_icon.png"));
    add_item->setText(tr("Add"));
    //设置item项中的文字位置
    add_item->setTextAlignment(Qt::AlignHCenter);
    add_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
    //ui->listWidget->addItem(add_item);
    //delete add_item;

    //设置viewModel，来确定使用不同的视图进行显示
    //使ListWidgetItem中的图标在上，文字在下
    //ui->listWidget->setViewMode(QListView::IconMode);
    //ui->listWidget->setViewMode(QListWidget::IconMode);  //这样的形式也可以
    //ui->listWidget->setViewMode(QListView::ListMode);


    //改变item大小，使用QListWidgetItem::setSizeHint()
    //add_item->setSizeHint(QSize(60,60));


    //设置ListWidget可以选择多个item
    //ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);


    //有两种方法在ListWidget中添加item
    //一种在构造item的时候，指定其父Widget
    QListWidgetItem *cubby_item = new QListWidgetItem(QIcon(":/images/tray_icon.png"),
                                                      tr("Cubby"),ui->listWidget);
    //第二种方法是在构造完item后，使用QListWidget::additem()来添加item
    QListWidgetItem *dropbox_item = new QListWidgetItem();
    dropbox_item->setIcon(QIcon(":/images/tray_icon.png"));
    dropbox_item->setText(tr("Dropbox"));
    ui->listWidget->addItem(dropbox_item);


    //向QListWidget中指定的位置插入itemm，使用QListWidget::addItem()
    QListWidgetItem *google_item = new QListWidgetItem(QIcon(":/images/tray_icon.png"),
                                                         tr("Google"));
    ui->listWidget->insertItem(1,google_item);


    //**使用QListWidget::takeItem(int index)来删除表中的某一项
    //**第二句的内存删除是一定要的，根据Qt文档，takeItem只是在视图（视觉）上实现了移除此项，但其实并没有释放内存
    //QListWidgetItem *deletedItem = ui->listWidget->takeItem(0);
    //delete deletedItem;

    //**removeItemWidget这个函数可以同时移除项和释放内存
    //ui->listWidget->removeItemWidget(add_item);

    //**打开和关闭item是否可以编辑,默认不可编辑
    //**使用QListWidget::openPersistenEditor(QListWidgetItem*)和
    //**QListWidget::closePersistentEditor(QListWidgetItem*)
    //ui->listWidget->openPersistentEditor(cubby_item);


    //设置当前的item是第几行
    //初始化ListWidget显示时，指向哪一行
    ui->listWidget->setCurrentRow(1);

    //设置单元项不可拖动，（Static、Free、Snap）
    ui->listWidget->setMovement(QListView::Static);
    //拖动item，进行复制
    //ui->listWidget->setMovement(QListWidget::Free);

    //设置垂直滚动条显示方式（ScrollBarAsNeeded：按需显示，ScrollBarAlwaysOff：隐藏，ScrollBarAlwaysOn：常显）
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);


    //设置ListWidget是否可以自动排序,默认是false
    ui->listWidget->setSortingEnabled(false);


    //设置QLisView大小改变时，图标的调整模式，默认是固定的，可以改成自动调整
    //ui->listWidget->setResizeMode(QListView::Adjust);



    QListWidgetItem *computer_item = new QListWidgetItem();
    QString str(tr("Computer"));
    computer_item->setData(Qt::DisplayRole,str);
    computer_item->setIcon(QIcon(":/images/tray_icon.png"));
    ui->listWidget->addItem(computer_item);


    //auto button = new QPushButton>(tr("Button"));
    QPointer<QPushButton> button = new QPushButton();
    QListWidgetItem *button_item = new QListWidgetItem();
    ui->listWidget->addItem(button_item);
    //实现替换，自定义item
    ui->listWidget->setItemWidget(button_item,button);

    //使用QListWidget::count()来统计ListWidget中总共的item数目
    //int item_count = ui->listWidget->count();
    //qDebug()<<item_count;

    //加入多个项，项是预先放在QStringList容器中的,或者使用临时容器存放,示例如下:
    //void addItems(const QStringList &);
    ui->listWidget->addItems(QStringList()<<QString:: fromUtf8("北京")<<QString::fromUtf8("上海")<<QString::fromUtf8("杭州"));

    //设置样式，直接在函数中设置
    ui->listWidget->setStyleSheet("QListWidget{border:1px solid gray; color:black; }"
                               "QListWidget::Item{padding-top:20px; padding-bottom:4px; }"
                               "QListWidget::Item:hover{background:skyblue; }"
                               "QListWidget::item:selected{background:lightgray; color:red; }"
                               "QListWidget::item:selected:!active{border-width:0px; background:lightgreen; }"
                               );


    //int currentRow(); //返回当前选择项的行.从0开始，可以看成是当前项在QListWidget中的下标.


    //void sortItems(Qt::SortOrder); //设置项排序的方式.默认是从AAA-ZZZ排序.(当使用此函数后，自动激活排序).
    //第一种:Qt::DescendingOrder: 从ZZZ到AAA排序.
    //第二种:Qt::AscendingOrder: 从AAA到ZZZ排序.
    //ui.listWidget->sortItems(Qt::DescendingOrder);


    //QListWidgetItem* item(int); //返回指定下标的项的指针.

    //int row(QListWidgetItem*); //返回指定项的下标.

    //清空QListWidget对象的所有内容.
    //void clear();

    //QListWIdget信号
    //void itemChanged(QListWidgetItem * item); //项的内容被改变发出信号.
    //void itemClicked(QListWidgetItem * item); //项被点击发出信号.
    //void itemSelectionChanged(); //项的选择清空发生改变发出信号.



}
