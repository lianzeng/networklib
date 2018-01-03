//compile: g++ -std=c++11 -pthread c11test.cpp 

#include <forward_list>
#include<iostream>
#include<thread>
using namespace std;

void thread_task()
{
  std::cout<< "\n another thread \n";
}

int main()
{


std::thread t(thread_task);
t.join();


std::cout <<"test c++11 ok "<<endl;
return 0;

}
