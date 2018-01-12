//compile: g++ -std=c++11 -pthread c11test.cpp 

#include <forward_list>
#include<iostream>
#include<thread>
#include<functional>
using namespace std;

void thread_task()
{
  std::cout<< "\n value is 0 \n";
}

void thread_task2(int value)
{
  std::cout <<"\n value is "<< value <<"\n";
}


class classA
{
public:
classA(int i):m(i)
{}

void func(int v)
{
 std::cout <<"\n value is "<< m <<" and "<<v <<"\n";
}

private:
int m;

};

int main()
{


std::thread t(thread_task);
t.join();


std::thread t2(std::bind(thread_task2, 2));
t2.join();

std::thread t3(thread_task2, 3);
t3.join();


classA obja(4);
std::thread t4(std::bind(&classA::func, &obja, 4));
t4.join();

std::cout <<"test c++11 ok "<<endl;
return 0;

}
