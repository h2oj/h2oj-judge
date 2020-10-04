#include <iostream>
#include <unistd.h>
#include <ctime>
#include <climits>
#include <fstream>
#include <string>
#include <sys/wait.h>
#include <signal.h>
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
    std::string in;
    std::string out;
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
class Stack
{
    int bp = -1;
    void* valve[256] = {0};
public:
    int push(void* ptr)
    {
        if (bp <= 256)
        {
            bp++;
            valve[bp] = ptr;
            return 0;
        }
        else
            return -1;
    }
    void* pop(void)
    {
        if (bp >= 0)
        {
            bp--;
            return valve[bp+1];
        }
        else
            return NULL;
    }
};
class JudgeDataNode
{
public:
    SessionData* data;
    JudgeDataNode* next_node = NULL;
    JudgeDataNode(){data = new SessionData;}
    ~JudgeDataNode(){delete data;}
}; 
class JudgeLimits
{
public:
    SessionLimit* cpu;
    SessionLimit* mem;
    SessionLimit* proc;
    JudgeLimits()
    {
        cpu = new SessionLimit;
        mem = new SessionLimit;
        proc = new SessionLimit;
    }
    ~JudgeLimits()
    {
        delete cpu;
        delete mem;
        delete proc;
    }
};

class JudgeDataList
{
public:
    int num;
    JudgeDataNode* head_node;
    ~JudgeDataList()
    {
        Stack* __d_st = new Stack;
        JudgeDataNode* __d_ptr = head_node;
        while(!(__d_ptr->next_node == NULL))
        {
            __d_st->push(__d_ptr);
            __d_ptr = __d_ptr->next_node;
        }
        delete __d_ptr;
        __d_ptr = (JudgeDataNode*)__d_st->pop();
        while (!(__d_ptr == NULL))
        {
            delete __d_ptr;
            __d_ptr = (JudgeDataNode*)__d_st->pop();
        }
        delete __d_st;
    }
};

class JudgeTarget
{
public:
        JudgeDataList* data;
        JudgeLimits* limits;
        JudgeTarget()
        {
            data = new JudgeDataList;
            limits = new JudgeLimits;
        }
        ~JudgeTarget()
        {
            delete data;
            delete limits;
        }
};


#endif 