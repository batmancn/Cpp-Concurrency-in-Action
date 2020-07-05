#include <exception>
#include <stack>
#include <mutex>
#include <memory>

struct empty_stack: std::exception
{
    const char* what() const throw()
    {
        return "empty stack";
    }

};

// threadsafe_stack实现方式：
// 1. 修改api，减少无用的api，例如top
// 2.
template<typename T>
class threadsafe_stack
{
private:
    std::stack<T> data;
    mutable std::mutex m;
public:
    threadsafe_stack(){}
    threadsafe_stack(const threadsafe_stack& other)
    {
        std::lock_guard<std::mutex> lock(other.m);
        data=other.data;
    }
    // 不要赋值构造函数
    threadsafe_stack& operator=(const threadsafe_stack&) = delete;

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(m);
        data.push(new_value);
    }
    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw empty_stack();
        // 使用top获得数据
        // 使用shared_ptr引用这个数据
        // 在Java中就没这个问题，因为Java自动管理内存
        std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
        // 注意pop只是从data中删除了数据，而数据本身并没有被删除
        data.pop();
        return res;
    }
    void pop(T& value)
    {
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw empty_stack();
        value=data.top();
        data.pop();
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};

int main()
{
    threadsafe_stack<int> si;
    si.push(5);
    si.pop();
    if(!si.empty())
    {
        int x;
        si.pop(x);
    }

}
