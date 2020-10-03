#include <iostream>
#include <unistd.h>
#include <ctime>
#include <climits>
#include <fstream>
#include <string>
#include <sys/wait.h>
#include <signal.h>
#include "JudgeHost.h"
#include "../OJConf.h"
using namespace std;
int judgeStart(JudgeSession* Session)                           //单次sessionjudge
{
    int start_time,end_time;
    start_time = time(NULL);
    ofstream data_out;
    ifstream data_in;
    data_out.open(JUDGE_IN);
    data_out << Session->data->in;
    data_out.close();
    int pid = vfork();                                          //fork子进程

    if(pid == 0) 
    {
/*                        评测进程源码                            */
        setrlimit(Session->cpul->type,Session->cpul->hs);       //配置cpu时间限制
        setrlimit(Session->meml->type,Session->meml->hs);       //配置内存限制
        setrlimit(Session->procl->type,Session->procl->hs);     //防止fork,限制不允许多于1个进程
        chroot(JUDGE_DIR);                                      //chroot SANDBOX
        setuid(JUDGE_USER_UID);                                 //降级,不再拥有root权限
        execl("/exe","exe",NULL);                               //执行目标
        _exit(0);
    }
/*                        HOST进程源码                            */
    else
    {
        int* end_stat = new int;
        wait(end_stat);
        if (*end_stat == 0)                                     //正常退出
        {
            data_in.open(JUDGE_OUT);
            string program_out;
            data_in >> program_out;
            if(Session->data->out.compare(program_out))
                return OJ_AC;
            else
            {
                return OJ_WA;
            }
            
        }
        else if(*end_stat == SIGKILL || *end_stat == SIGSTOP)   //被迫中止,应为触碰RLIMIT
        {
            return OJ_LE;
        }
        else
        {
            return OJ_UKE;                                      //我也不知道怎么错的,我也不敢问,那就给个UKE回去(不是)
        }
        
        

    }
}