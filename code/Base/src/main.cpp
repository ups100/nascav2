#include "MyNscaMain.h"
#include "LogEntry.h"
#include <signal.h>

using namespace INZ_project::Base;

void handler(int signal)
{
    LOG_ENTRY(MyLogger::INFO, "Received SIGINT, exiting...")
    MyNscaMain::shutDown();
}

int main(int argc, char** argv)
{
    MyNscaMain instance(argc, argv);
    signal(SIGINT, handler);
    return instance.exec();
}
