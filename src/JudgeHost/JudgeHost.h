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
struct session_limit
{

    rlimit *hs;
    int type;
};
struct session_data
{
    std::string in;
    std::string out;
};
class judge_session
{
public:
    session_limit *cpul;
    session_limit *meml;
    session_limit *procl;
    session_data *data;

    judge_session()
    {
        cpul = new session_limit;
        meml = new session_limit;
        procl = new session_limit;
        data = new session_data;
        cpul->type = RLIMIT_CPU;
        meml->type = RLIMIT_AS;
        procl->type = RLIMIT_NPROC;
        procl->hs->rlim_cur = 1;
        procl->hs->rlim_max = 1;
    }
    ~judge_session()
    {
        delete cpul;
        delete meml;
        delete procl;
        delete data;
    }
};
class stack_256
{
    int bp = -1;
    void *valve[256] = {0};

public:
    int push(void *ptr)
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
    void *pop()
    {
        if (bp >= 0)
        {
            bp--;
            return valve[bp + 1];
        }
        else
            return NULL;
    }
};
class judge_data_node
{
public:
    session_data *data;
    judge_data_node *next_node = NULL;
    judge_data_node() { data = new session_data; }
    ~judge_data_node() { delete data; }
};
class JudgeLimits
{
public:
    session_limit *cpu;
    session_limit *mem;
    session_limit *proc;
    JudgeLimits()
    {
        cpu = new session_limit;
        mem = new session_limit;
        proc = new session_limit;
    }
    ~JudgeLimits()
    {
        delete cpu;
        delete mem;
        delete proc;
    }
};

class judge_data_list
{
public:
    int num;
    judge_data_node *head_node;
    ~judge_data_list()
    {
        stack_256 *d_st = new stack_256;
        judge_data_node *d_ptr = head_node;
        while (!(d_ptr->next_node == NULL))
        {
            d_st->push(d_ptr);
            d_ptr = d_ptr->next_node;
        }
        delete d_ptr;
        d_ptr = (judge_data_node *)d_st->pop();
        while (!(d_ptr == NULL))
        {
            delete d_ptr;
            d_ptr = (judge_data_node *)d_st->pop();
        }
        delete d_st;
    }
};

class judge_target
{
public:
    judge_data_list *data;
    JudgeLimits *limits;
    judge_target()
    {
        data = new judge_data_list;
        limits = new JudgeLimits;
    }
    ~judge_target()
    {
        delete data;
        delete limits;
    }
};

#endif