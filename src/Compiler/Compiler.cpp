#include <iostream>
#include <unistd.h>
#include "../OJConf.h"
using namespace std;
int compTarget()
{
    execl(CPP_COMPILER_PATH, "g++", SOURCE_FILE_PATH CPP_EXTN, CPP_COMPILER_FLAGS, "-o", OUTPUT_FILE_PATH, NULL);
    return 0;
}