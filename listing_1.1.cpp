#include <iostream>
#include <thread>

void hello()
{
    std::cout<<"Hello Concurrent World\n";
}

int main()
{
    // 创建thread，参数是入口点
    std::thread t(hello);
    // 这个jion与pthread的join用处一样，perent进程会等待t进程，防止t进程成为孤儿进程
    t.join();
}
