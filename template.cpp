//
// Created by Administrator on 2022/10/7.
//

#include <iostream>
using namespace std;

// One function works for all data types. This would work
// even for user defined types if operator '>' is overloaded
// 模板函数
template <typename T>
T myMax(T x, T y)
{
    return (x > y)? x: y;
}


//=====================================模板类====================================
//类模板: 与函数模板类似，当类的定义不依赖具体类型时类模板是非常有用的
template <typename T>
class Array {
private:
    T *ptr;
    int size;
public:
    Array(T arr[], int s);
    void print();
};

template <typename T>
Array<T>::Array(T arr[], int s) {
    ptr = new T[s];
    size = s;
    for(int i = 0; i < size; i++)
        ptr[i] = arr[i];
}

template <typename T>
void Array<T>::print() {
    for (int i = 0; i < size; i++)
        cout<<" "<<*(ptr + i);
    cout<<endl;
}

//int main()
//{
//    cout << myMax<int>(3, 7) << endl; // Call myMax for int
//    cout << myMax<double>(3.0, 7.0) << endl; // call myMax for double
//    cout << myMax<char>('g', 'e') << endl; // call myMax for char
//    int arr[5] = {1, 2, 3, 4, 5};
//    Array<int> a(arr, 5);
//    a.print();
//    return 0;
//}
