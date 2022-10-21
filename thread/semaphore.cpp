//
// Created by Administrator on 2022-10-11.
//
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
using namespace std;



sem_t semPackage;



void* makeOneSheet(void*) {
    for (int i = 0; i < 4; ++i) {
        cout << "Make 1 sheet" << endl;
        sleep(1);
        sem_post(&semPackage);
    }

    pthread_exit(nullptr);
    return nullptr;
}



void* combineOnePackage(void*) {
    for (int i = 0; i < 4; ++i) {
        sem_wait(&semPackage);
        sem_wait(&semPackage);
        cout << "Combine 2 sheets into 1 package" << endl;
    }

    pthread_exit(nullptr);
    return nullptr;
}



int main() {
    pthread_t tidMakeSheetA, tidMakeSheetB, tidCombinePackage;
    int ret = 0;

    ret = sem_init(&semPackage, 0, 0);

    ret = pthread_create(&tidMakeSheetA, nullptr, &makeOneSheet, nullptr);
    ret = pthread_create(&tidMakeSheetB, nullptr, &makeOneSheet, nullptr);
    ret = pthread_create(&tidCombinePackage, nullptr, &combineOnePackage, nullptr);

    ret = pthread_join(tidMakeSheetA, nullptr);
    ret = pthread_join(tidMakeSheetB, nullptr);
    ret = pthread_join(tidCombinePackage, nullptr);

    ret = sem_destroy(&semPackage);
    return 0;
}