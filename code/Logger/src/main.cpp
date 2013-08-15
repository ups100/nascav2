#include "LogEntry.h"

int main(int argc, char** argv)
{
    LOG_ENTRY(MyLogger::FATAL, "dziala na "<<5<<" jest ok.");
    return 0;
}
