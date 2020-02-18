#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtEvents>
#include <QtDebug>

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QPointer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT



public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    QPointer<QSystemTrayIcon> mSysTrayIcon;
    QPointer<QMenu> mMenu;
    QPointer<QAction> mShowMainAction;
    QPointer<QAction> mExitAppAction;

    QPointer<QMenu> mTreeMenu;
    QPointer<QAction> mAddFolderAction;
    QPointer<QAction> mAddFileAction;
    QPointer<QAction> mDltNodeAction;

    void initTrayIcon();
    void createTrayActions();
    void closeEvent ( QCloseEvent * e );
    void changeEvent(QEvent * event);
    void initTreeMenu();

private slots:
    void on_showMainAction();
    void on_exitAppAction();
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);

    void on_actionExit_triggered();
    void on_actionActive_triggered();

    void on_actionDock_triggered(bool checked);

    void on_treeFiles_customContextMenuRequested(QPoint pos);
    void on_treeFiles_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_dockWidget_closeEvent(QCloseEvent * event);

    void    on_actAddFolder();
    void    on_actAddFiles();
    void    on_actDeleteItem();

private:
    Ui::MainWindow *ui;

private:
    bool mDockStatus;//dock widget显示状态

    //枚举类型treeItemType， 用于创建 QTreeWidgetItem 时作为节点的type, 自定义类型必须大于1000
    //itTopItem 顶层节点;  itGroupItem 组节点； itImageItem 图片
    enum    treeItemType{itTopItem=1001,itGroupItem,itImageItem};
    //枚举类型，表示列号
    enum    treeColNum{colItem=0, colItemType=1}; //目录树列的编号定义

    QLabel  *LabFileName;
    QPixmap curPixmap; //当前的图片
    float   pixRatio;//当前图片缩放比例

    void    initTree();//目录树初始化
    void    addFolderItem(QTreeWidgetItem *parItem, QString dirName);//添加一个目录节点
    QString getFinalFolderName(const QString &fullPathName);//从目录全名称中获取最后的文件夹名称
    void    addImageItem(QTreeWidgetItem *parItem,QString aFilename);//添加一个图片节点
    void    displayImage(QTreeWidgetItem *item); //显示一个图片节点的图片
    void    changeItemCaption(QTreeWidgetItem *item); //遍历改变节点标题

    void    initList();
};

#endif // MAINWINDOW_H
