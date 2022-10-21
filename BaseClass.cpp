#include <iostream>

//
// Created by Administrator on 2022/10/7.
//
class BaseClass
{
public:
    BaseClass() {}
//    ~BaseClass()
//    {
//        std::cout << "delete BaseClass" << std::endl;
//    }
    //虚析构函数
    virtual ~BaseClass()
    {
        std::cout << "delete BaseClass" << std::endl;
    }
};

class ChildClassA : public BaseClass
{
public:
    ChildClassA() {}
    ~ChildClassA()
    {
        std::cout << "ChildClassA Delete" << std::endl;
    }
};
class ChildClassB : public ChildClassA
{
public:
    ChildClassB() {}
    ~ChildClassB()
    {
        std::cout << "ChildClassB Delete" << std::endl;
    }
};

//int main(){
//    // 测试案例
//    BaseClass *a = new ChildClassA();
//    BaseClass *b = new ChildClassB();
//    ChildClassA *c = new ChildClassB();
//
//    delete a;
//    a = nullptr;
//    std::cout << std::endl;
//
//    delete b;
//    b = nullptr;
//    std::cout << std::endl;
//
//    delete c;
//    c = nullptr;
////    delete BaseClass
////
////    delete BaseClass
////
////    ChildClassA Delete
////    delete BaseClass
//
//    //结论：基类析构函数不定义为虚析构，当我们用基类指针指向子类对象的时候，只会调用基类析构函数。
//    // 当基类设置为虚析构，子类的析构函数会被调用，且子类的析构函数不需要设置为虚析构，孙子类的析构函数仍然会调用
//}