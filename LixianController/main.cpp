#include <iostream>
#include <fstream>
#include <assert.h>

#include "LixianController.h"

const std::string TEST_URL = "http://dldir1.qq.com/qqfile/qq/QQ2013/QQ2013SP5/9050/QQ2013SP5.exe";
const std::string TEST_TORRENT = "/home/twotrees/Documents/AriaThunder/test/testxxx.torrent";

int main()
{
    LXController lx;
    bool ret = lx.config("/home/twotrees/Downloads/", "zhangfancc", "zf299792458");
    assert(ret);
    ret = lx.login();
    assert(ret);

    LXController::DownloadFileInfo downloadInfo;
    ret = lx.fetchUrl(TEST_URL, downloadInfo);
    assert(ret);

    std::ifstream ifs(TEST_TORRENT.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
    unsigned int size = ifs.tellg();
    std::string torrent;
    torrent.resize(size);
    ifs.seekg(0, std::ios::beg);
    ifs.read((char*)torrent.data(), size);
    ifs.close();

    LXController::DownloadFileList downloadList;
    ret = lx.fecthTorrent(torrent, downloadList);
    assert(ret);

    return 0;
}

