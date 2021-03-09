#include <mutex>
#include <thread>
 
struct bank_account {
    explicit bank_account(int balance) : balance(balance) {}
    int balance;
    std::mutex m;
};
 
void transfer(bank_account &from, bank_account &to, int amount)
{
    // 锁定两个互斥而不死锁
    std::lock(from.m, to.m);
    // 保证二个已锁定互斥在作用域结尾解锁
    std::lock_guard<std::mutex> lock1(from.m, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(to.m, std::adopt_lock);
 
// 等价方法：
//    std::unique_lock<std::mutex> lock1(from.m, std::defer_lock);
//    std::unique_lock<std::mutex> lock2(to.m, std::defer_lock);
//    std::lock(lock1, lock2);
 
    from.balance -= amount;
    to.balance += amount;
}
 
int main()
{
    bank_account my_account(100);
    bank_account your_account(50);
 
    std::thread t1(transfer, std::ref(my_account), std::ref(your_account), 10);
    std::thread t2(transfer, std::ref(your_account), std::ref(my_account), 5);
 
    t1.join();
    t2.join();
}
