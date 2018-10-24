#include <iostream>
#include "Singleton.h"
#include "SocketFd.h"
#include <netinet/in.h>
#include <signal.h>
#include <dirent.h>
#include "Singleton.h"
#include "FileManager.h"

using namespace net;

int main() {
    signal(SIGCHLD,SIG_DFL);
    signal(SIGPIPE,SIG_IGN);

    return  0;
}