//
// Created by Administrator on 2022/10/7.
//

#include <cstdio>

/***
 * struct的使用
 * strut是一个结构体
 * 声明里面的所有都是public
 * 现在在下面弄BinaNode,相当于声明了一个BinaryNode BinaNode对象
 * 指针 声明一个指针，但是没有实例化，使用的时候需要实例化
 * 相当于 BinaryNode *BiNode = new BiaryNode();
 * ***/

struct BinaryNode{
    int val;
    BinaryNode *pLeft;
    BinaryNode *pRight;
}BinaNode,*BiNode;


struct {
    int val;
}Node;
/***
 * 也就是说明了一个结构为Node的结构
 * 这个的后面不能加指针
 * ***/

//int main(){
//    BinaNode.val = 12;
//    printf("%d\n",BinaNode.val);
//    BiNode = new BinaryNode();
//    BiNode->val = 42;
//    printf("%d\n",BiNode->val);
//    Node.val = 32;
//    printf("%d\n",Node.val);
////    P = new Node(); 这里是不行的
//    return 0;
//}
