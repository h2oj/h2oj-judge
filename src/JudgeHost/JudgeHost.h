#include <sys/resource.h>
#ifndef _JUDGEHOST_H_
#define _JUDGEHOST_H_
typedef int judge_stat;
struct SessionLimit
{
    
    rlimit* hs;
    int type;
};
struct SessionData
{
    string in;
    string out;
};
class JudgeSession
{
public:
    
    SessionLimit* cpul;
    SessionLimit* meml;
    SessionLimit* procl;
    SessionData* data;

    JudgeSession()
    {
        cpul = new SessionLimit;
        meml = new SessionLimit;
        procl = new SessionLimit;
        data = new SessionData;
        cpul->type = RLIMIT_CPU;
        meml->type = RLIMIT_AS;
        procl->type = RLIMIT_NPROC;
        procl->hs->rlim_cur = 1;
        procl->hs->rlim_max = 1;
    }
    ~JudgeSession()
    {
        delete cpul;
        delete meml;
        delete procl;
        delete data;
    }
};

#endif 