#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/wait.h>

std::mutex g_display_mutex;

void foo()
{
  std::thread::id this_id = std::this_thread::get_id();
  auto tid = static_cast<pid_t>(::syscall(SYS_gettid));
  auto pthread_id = pthread_self();
  g_display_mutex.lock();

  std::cout << "thread " << this_id << " sleeping...\n";
  std::cout << "thread " << tid << std::endl;
  std::cout << "thread " << pthread_id << std::endl;
  g_display_mutex.unlock();

  std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main()
{
  auto pid = fork();
  if (pid == 0)
    std::cout << "-----------------------" << std::endl;
  std::thread t1(foo);
  std::thread t2(foo);

  t1.join();
  t2.join();

  int status;
  wait(&status);
}
