#ifndef LIXIANCONTROLLER_H
#define LIXIANCONTROLLER_H

#include <string>
#include <vector>

#define ERROR_CHECK_BOOL(cond) \
do\
{\
    if (!cond)\
        return false;\
} while (0)

namespace
{
    const std::string DEFAULT_SCRIPT_PATH   = "/usr/local/share/xunlei-lixian/lixian_cli.py";
    const std::string CONFIG                = "config";
    const std::string USER_NAME             = "username";
    const std::string PASSWORD              = "password";
    const std::string OUTPUT_DIR            = "output-dir";
    const std::string LOGIN                 = "login";
    const std::string DOWNLOAD              = "download";
    const std::string TASK_INFO             = "[TaskInfo]";
    const std::string G_DRIVE_ID            = "gdriveid";
    const std::string FILE_PATH             = "filePath";
    const std::string URL                   = "url";
}

class LXController
{
public:
    struct DownloadFileInfo
    {
        std::string filePath;
        std::string url;
        std::string token;
    };

    typedef std::vector<DownloadFileInfo> DownloadFileList;

private:
    std::string _lxScriptPath;

public:
    explicit LXController(const std::string& lixianScriptPath = DEFAULT_SCRIPT_PATH);

public:
    bool config(const std::string& outputPath, const std::string& userName, const std::string& password);
    bool login();
    bool fetchUrl(const std::string& url, DownloadFileInfo& downloadInfo);
    bool fecthTorrent(const std::string& torrentContent, DownloadFileList& downloadList);

private:
    inline bool _execCommand(const std::vector<std::string>& params, std::string& result);
};

#endif // LIXIANCONTROLLER_H
