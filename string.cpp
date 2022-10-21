//
// Created by Administrator on 2022/10/9.
//

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>

using namespace std;

//int main(){
//    char greeting[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
//    char greeting_2[] = "Hello";
////    char str1[11] = "Hello";
////    char str2[11] = "World";
////    char str3[11];
////    int  len ;
////
////    // 复制 str1 到 str3
////    strcpy( str3, str1);
////    cout << "strcpy( str3, str1) : " << str3 << endl;
////
////    // 连接 str1 和 str2
////    strcat( str1, str2);
////    cout << "strcat( str1, str2): " << str1 << endl;
////
////    // 连接后，str1 的总长度
////    len = strlen(str1);
////    cout << "strlen(str1) : " << len << endl; //不包含'\0'
//
//
//    string str1 = "Hello";
//    string str2 = "World";
//    string str3;
//    int  len ;
//
//    // 复制 str1 到 str3
//    str3 = str1;
//    cout << "str3 : " << str3 << endl;
//
//    // 连接 str1 和 str2
//    str3 = str1 + str2;
//    cout << "str1 + str2 : " << str3 << endl;
//
//    // 连接后，str3 的总长度
//    len = str3.size();
//    cout << "str3.size() :  " << len << endl;
//
//       //核心API
////    str.assign(“ABC”) ——把str清空，重新赋值”ABC”
////    str.assign(“ABC”, 2) ——把str清空，重新赋值”ABC”，然后保留2位，str=“AB”
////    str.length() ——返回字符串长度、
////    str.size() ——和length一样
////    str**.reasize**(10) ——设置当前 str 的大小为10，若大小大与当前串的长度，\0 来填充
////    str**.reasize**(10,char c) ——设置当前 str 的大小为10，若大小大与当前串的长度，字0符c 来填充
////    str.reserve(10) ——设置str的容量 10，不会填充数据
////    str.swap(str1) ——交换 str1 和 str 的字符串
////    str.push_back(‘A’) ——在str末尾添加一个字符 ‘A’ ，参数必须是字符形式
////    str.append(“ABC”) ——在str末尾添加一个字符串 “ABC”，参数必须是字符串形式
//
//
//    return 0;
//}