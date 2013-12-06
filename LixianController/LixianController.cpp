#include "LixianController.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include "pstreams/pstream.h"
#include <sys/signal.h>

LXController::LXController(const std::string &lixianScriptPath)
    : _lxScriptPath(lixianScriptPath)
{

}

bool LXController::config(const std::string& outputPath, const std::string& userName, const std::string& password)
{
    bool ret = true;

    // config only output url
    {
        std::vector<std::string> params;
        params.push_back(CONFIG);
        params.push_back("tool");
        params.push_back("stdout");

        std::string result;
        ret = _execCommand(params, result);
        ERROR_CHECK_BOOL(ret);
    }

    // no-hash
    {
        std::vector<std::string> params;
        params.push_back(CONFIG);
        params.push_back("no-hash");

        std::string result;
        ret = _execCommand(params, result);
        ERROR_CHECK_BOOL(ret);
    }

    // output path
    {
        std::vector<std::string> params;
        params.push_back(CONFIG);
        params.push_back(OUTPUT_DIR);
        params.push_back(outputPath);

        std::string result;
        ret = _execCommand(params, result);
        ERROR_CHECK_BOOL(ret);
    }

    // username
    {
        std::vector<std::string> params;
        params.push_back(CONFIG);
        params.push_back(USER_NAME);
        params.push_back(userName);

        std::string result;
        ret = _execCommand(params, result);
        ERROR_CHECK_BOOL(ret);
    }

    // password
    {
        std::vector<std::string> params;
        params.push_back(CONFIG);
        params.push_back(PASSWORD);
        params.push_back(password);

        std::string result;
        ret = _execCommand(params, result);
        ERROR_CHECK_BOOL(ret);
    }

    return ret;
}

bool LXController::login()
{
    bool ret = true;

    std::vector<std::string> params;
    params.push_back(LOGIN);

    std::string result;
    ret = _execCommand(params, result);
    ERROR_CHECK_BOOL(ret);

    return ret;
}

bool LXController::fetchUrl(const std::string& url, DownloadFileInfo& downloadInfo)
{
    bool ret = true;
    std::vector<std::string> params;
    params.push_back(DOWNLOAD);
    params.push_back(url);

    std::string result;
    ret = _execCommand(params, result);
    ERROR_CHECK_BOOL(ret);

    std::stringstream stream(result);
    std::string line;
    while (std::getline(stream, line))
    {
        if (TASK_INFO == line)
        {
            std::getline(stream, downloadInfo.url);
            std::getline(stream, downloadInfo.token);
            std::getline(stream, downloadInfo.filePath);

            break;
        }
    }

    return ret;
}

bool LXController::fecthTorrent(const std::string& torrentContent, DownloadFileList& downloadList)
{
    bool ret = true;

    std::ofstream ofs;

    std::stringstream ss;
    ss << "/tmp/" << time(NULL) << ".torrent";
    std::string tempTorrent = ss.str();

    ofs.open(tempTorrent.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
    ofs.write(torrentContent.c_str(), torrentContent.size());
    ofs.close();

    std::vector<std::string> params;
    params.push_back(DOWNLOAD);
    params.push_back(tempTorrent);

    std::string result;
    ret = _execCommand(params, result);
    ERROR_CHECK_BOOL(ret);

    std::stringstream stream(result);
    std::string line;
    while (std::getline(stream, line))
    {
        if (TASK_INFO == line)
        {
            DownloadFileInfo downloadInfo;
            std::getline(stream, downloadInfo.url);
            std::getline(stream, downloadInfo.token);
            std::getline(stream, downloadInfo.filePath);

            downloadList.push_back(downloadInfo);
        }
    }

    return ret;
}

struct temp_recv_child_signal
{
    temp_recv_child_signal()
    {
        signal(SIGCHLD, SIG_DFL);
    }

    ~temp_recv_child_signal()
    {
        signal(SIGCHLD, SIG_IGN);
    }
};

bool LXController::_execCommand(const std::vector<std::string> &params, std::string &result)
{
    temp_recv_child_signal trcs;

    std::vector<std::string> realParams;
    realParams.push_back("/usr/bin/python");
    realParams.push_back(_lxScriptPath);
    std::copy(params.begin(), params.end(), std::back_inserter(realParams));

    redi::ipstream pstream;
    pstream.open("python", realParams, redi::pstreambuf::pstdout);

    std::stringstream buffer;
    std::string line;
    while (pstream >> line)
        buffer << line << std::endl;
    result = buffer.str();

    pstream.close();
    int exitCode = pstream.rdbuf()->status();
    return !exitCode;
}
