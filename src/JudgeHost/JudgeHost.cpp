#include "JudgeHost.h"
#include "../OJConf.h"
using namespace std;
int judge(judge_session *session);
int judgeAll(judge_target *target)
{
    ifstream data_in;
    data_in.open(JUDGE_CONFIG);
    //TODO
    judge_data_node *d_ptr;
    judge_session *this_session = new judge_session;
    this_session->cpul = target->limits->cpu;
    this_session->meml = target->limits->mem;
    this_session->procl = target->limits->proc;
    for (size_t si = 0; si < (unsigned int)target->data->num; si++)
    {
        if (si == 0)
        {
            this_session->data = target->data->head_node->data;
            judge(this_session);
        }
        else
        {
            d_ptr = target->data->head_node;
            for (size_t i = 0; i < si; i++)
                d_ptr = d_ptr->next_node;
            this_session->data = d_ptr->data;
            judge(this_session);
        }
    }

    return 0;
}
int judge(judge_session *session) //单次sessionjudge
{
#if 0
    int start_time,end_time;
    start_time = time(NULL);
#endif
    ofstream data_out;
    ifstream data_in;
    data_out.open(JUDGE_IN); //写输入文件
    data_out << session->data->in;
    data_out.close();

    int pid = fork(); //fork子进程

    if (pid == 0)
    {
        /*                        评测进程源码                            */
        setrlimit(session->cpul->type, session->cpul->hs);   //配置cpu时间限制
        setrlimit(session->meml->type, session->meml->hs);   //配置内存限制
        setrlimit(session->procl->type, session->procl->hs); //防止fork,限制不允许多于1个进程
        chroot(JUDGE_DIR);                                   //chroot SANDBOX
        setuid(JUDGE_USER_UID);                              //降级,不再拥有root权限
        execl("/exe", "exe", NULL);                          //执行目标
        _exit(0);
    }
    /*                        HOST进程源码                            */
    else
    {
        int end_stat;
        wait(&end_stat);
        if (end_stat == 0) //正常退出
        {
            data_in.open(JUDGE_OUT); //读输出声音
            string program_out = "";
            string tmp_out = "";
            do
            {
                getline(data_in, tmp_out);
                if (tmp_out != "")
                {
                    tmp_out += '/n';
                    program_out += tmp_out;
                }
            } while (tmp_out != "");
            if (session->data->out.compare(program_out))
                return OJ_AC;
            else
                return OJ_WA;
        }
        else if (end_stat == SIGKILL || end_stat == SIGSTOP) //被迫中止,应为触碰RLIMIT
            return OJ_LE;
        else
            return OJ_UKE;
    } //我也不知道怎么错的,我也不敢问,那就给个UKE回去(不是)
}