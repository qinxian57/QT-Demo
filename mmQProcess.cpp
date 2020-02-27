#include "mmQProcess.h"


#ifdef WIN32
#include<windows.h>
#include <shellapi.h>
#else
#include <unistd.h>
#endif // !WIN32

CQProcess::CQProcess()
{

}
/*
 * ShellExecute and system of Linux
*/
int CQProcess::StartApp(const std::wstring FileName)
{
#ifdef WIN32
    ShellExecute(nullptr, L"open", FileName.c_str(), nullptr, nullptr, SW_SHOW);

#else
    system(FileName);
#endif
    return 0;
}


bool CQProcess::CheckAppStatus(const QString &appName)
{
#if defined(Q_OS_WIN)      //表示如果在windows下

    QProcess process;
    process.start("TASKLIST" ,QStringList()<<"/FI"<<"imagename eq " +appName);   //执行tasklist程序
    process.waitForFinished(5000);    //阻塞5秒等待tasklist程序执行完成，超过五秒则直接返回
    QString outputStr = QString::fromLocal8Bit(process.readAllStandardOutput()); //把tasklist程序读取到的进程信息输出到字符串中
    if(outputStr.contains(appName))
    {
        process.close(); //用完记得把process关闭了，否则如果重新调用这个函数可以会失败
        return true;
    }
    else
    {
        process.close();
        return false;
    }


#elif defined(Q_OS_LINUX)

    QProcess process;
    //connect(&process,SIGNAL(readyReadStandardOutput()),this,SLOT(readProcessStandardOutput(process)));

    QStringList args;
    args<<"-c";
    args<<"ps | grep " + appName;
    //process.start("sh",args);
    process.start("sh", QStringList()<<"-c"<<"ps | grep "+appName);

    process.waitForFinished(5000);    //阻塞5秒等待执行完成，超过五秒则直接返回
    QString outputStr = QString::fromLocal8Bit(process.readAllStandardOutput()); //把读取到的进程信息输出到字符串中
    if(outputStr.contains(appName))
    {
        process.close(); //用完记得把process关闭了，否则如果重新调用这个函数可以会失败
        return true;
    }
    else
    {
        process.close();
        return false;
    }

#endif

}

std::shared_ptr<QProcess> CQProcess::StartApp(const QString &appPath, const bool isDetach)       //name可以是程序名也可以程序所在的完整路径
{
    std::shared_ptr<QProcess> process = std::make_shared<QProcess>();

    if(isDetach){
        process->startDetached(QString("\"%1\"").arg(appPath));
    }
    else
        process->start(QString("\"%1\"").arg(appPath));

    process->waitForStarted(5000);
    //process.close();

    return process;

    //QDir::setCurrent("d:\a b\test.exe")将当前目录设置到外部执行文件目录
    //QDir::setCurrent(QApplication::applicationDirPath())

}

int CQProcess::KillApp(const QString &appName)
{

#if defined(Q_OS_WIN)
    QString command="TASKKILL /IM " + appName + " /F";
#elif defined(Q_OS_LINUX)
    QString command="pkill " + appName;
#endif

    QProcess process;
    int result = process.execute(command);                 //execute执行后会一直阻塞到返回外部程序退出的代码，0-success, 1-fail, 无法关闭会返回-2
    process.close();

    return result;


}


