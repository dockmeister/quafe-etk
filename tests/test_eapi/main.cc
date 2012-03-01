
#include <iostream>
#include <glibmm.h>
#include <glibmm/thread.h>
#include <glibmm/random.h>
#include <glibmm/threadpool.h>
#include <glibmm/timer.h>


namespace
{


void print_char(char c)
{
  Glib::Rand rand;

  for(int i = 0; i < 10; ++i)
  {
    {
   //   std::cout << c;
    //  std::cout.flush();
    }
    Glib::usleep(rand.get_int_range(10000, 100000));
  }
}

} // anonymous namespace


int main(int, char**)
{
	std::cout << "starting: " << std::endl;
	Glib::init();
	Glib::thread_init();
	if(!Glib::thread_supported()) {
		std::cout << "test";
	}
  Glib::ThreadPool pool (10);

  for(char c = 'a'; c <= 'z'; ++c)
  {
    pool.push(sigc::bind<1>(sigc::ptr_fun(&print_char), c));
  }

  while(pool.get_num_threads() > 0) {
	  std::cout << "numthreads: " << pool.get_num_threads() << std::endl;
	  Glib::usleep(100000);
  }
  pool.shutdown();
  return 0;
}

