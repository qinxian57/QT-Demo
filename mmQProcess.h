#ifndef MMQPROCESS_H
#define MMQPROCESS_H

#include <QProcess>
#include <memory>

class CQProcess
{
public:
    CQProcess();

    static int StartApp(std::wstring FileName);
    static bool CheckAppStatus(const QString &appName);
    static std::shared_ptr<QProcess> StartApp(const QString &appPath, const bool isDetach);
    static int KillApp(const QString &appName);
};

#endif // MMQPROCESS_H
