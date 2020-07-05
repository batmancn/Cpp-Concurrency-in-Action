#include <thread>

void do_something(int& i)
{
    ++i;
}

struct func
{
    int& i;

    func(int& i_):i(i_){} // 初始化函数

    void operator()()
    {
        for(unsigned j=0;j<1000000;++j)
        {
            do_something(i); // 潜在访问隐患：悬空引用。因为“不等待”。
        }
    }
};


void oops()
{
    int some_local_state=0;
    func my_func(some_local_state); // class初始化，其实struct就是只有public成员的class
    std::thread my_thread(my_func);
    my_thread.detach(); // 不等待，产生“悬空引用”问题
}

int main()
{
    oops();
}
