#include <iostream>
#include <unistd.h>
#include <ctime>
#include <climits>
#include <sys/wait.h>
#include <signal.h>
#include <sys/resource.h>
using namespace std;
void catch_sig(int num){cout <<endl<<"Catch A SIG:"<<num<<endl;}

int main()
{
    cout<<"USET"<<endl;
    rlimit* rl = new rlimit;
    rl->rlim_cur=2;
    rl->rlim_max=2;
    cout<<"ASET"<<endl;
    setrlimit(RLIMIT_NPROC,rl);
    cout<<"SET"<<endl;
    signal(1,catch_sig);
    signal(2,catch_sig);
    signal(3,catch_sig);
    signal(4,catch_sig);
    signal(5,catch_sig);
    signal(6,catch_sig);
    signal(7,catch_sig);
    signal(8,catch_sig);
    signal(9,catch_sig);
    signal(10,catch_sig);
    signal(11,catch_sig);
    signal(12,catch_sig);
    signal(13,catch_sig);
    signal(14,catch_sig);
    signal(15,catch_sig);
    signal(16,catch_sig);
    signal(17,catch_sig);
    signal(18,catch_sig);
    signal(19,catch_sig);
    signal(20,catch_sig);
    while (1)
    cout<<"WAF"<<endl;
    sleep(1000);
    fork();
    cout<<"AAF"<<endl;
;
    
}