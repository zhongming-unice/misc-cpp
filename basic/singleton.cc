#include <atomic>
#include <mutex>

// Lazy, Double Checked Locking, memory_order_seq_cst
class Singleton1
{
public:
  static Singleton1 *getSingleton1Instance();
  
private:
  Singleton1() {};
  static std::atomic<Singleton1 *> instance_;
  static std::mutex mutex_;
};

std::atomic<Singleton1 *> Singleton1::instance_;
std::mutex Singleton1::mutex_;

Singleton1 *Singleton1::getSingleton1Instance()
{
  Singleton1 *tmp = instance_.load();
  if (tmp == nullptr)
    {
      std::lock_guard<std::mutex> lk(mutex_);
      tmp = instance_.load();
      if (tmp == nullptr)
	{
	  tmp = new Singleton1();
	  instance_.store(tmp);
	}
    }
  return instance_;
}


int main(int argc, char *argv[])
{
  
  return 0;
}
