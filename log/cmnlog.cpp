#include "cmnlog.h"
#include <cstdio>
#include <cstdlib>
#define LOG_PATH "./"
namespace cmnlog{
    void writelog(const char * szText)
    {
        printf(szText);
    }
}