#include <iostream>       // std::cout
#include <chrono>	  // std::chrono
#include <functional>     // std::ref
#include <thread>         // std::thread
#include <future>         // std::promise, std::future


void WaitForMilkTea(std::future<int> &f_notice) {
  int notice = f_notice.get();
  std::cout << "收到通知，回来取奶茶" << std::endl;
}

void MakeMilkTea(std::promise<int> &p_notice) {
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << "奶茶做好了，通知顾客" << std::endl;
  p_notice.set_value(1);
  
}

int main() {
  
  // future from a packaged_task
  std::packaged_task<int()> task([]{ return 7; }); // wrap the function
  std::future<int> f1 = task.get_future();  // get a future
  std::thread t(std::move(task)); // launch on a thread
 
  // future from an async()
  std::future<int> f2 = std::async(std::launch::async, []{ return 8; });
 
  // future from a promise
  std::promise<int> p;
  std::future<int> f3 = p.get_future();
  std::thread( [&p]{ p.set_value_at_thread_exit(9); }).detach();
 
  std::cout << "Waiting..." << std::flush;
  f1.wait();
  f2.wait();
  f3.wait();
  std::cout << "Done!\nResults are: "
	    << f1.get() << ' ' << f2.get() << ' ' << f3.get() << '\n';
  t.join();

  
  std::promise<int> p_notice;
  std::future<int> f_notice = p_notice.get_future();  
  std::thread Customer(WaitForMilkTea, std::ref(f_notice));
  std::thread Waiter(MakeMilkTea, std::ref(p_notice));
  Customer.join();
  Waiter.join();
}
