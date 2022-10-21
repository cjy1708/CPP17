#include <iostream>
#include <pthread.h>
#include "value.h"
#include "reader.h"
#include <cstdio>

int main() {
    // 输出json
    Json::Value user;
    user["id"] = 1;
    user["name"] = "luge";
    user["age"] = "30";
    for (int i = 0; i < 3; i++) {
        std::string temp = std::to_string(i);
        user["list"][i]["ids" + temp] = "10" + temp;
    }
    std::string json = user.toStyledString().c_str();
    std::cout<<json<<std::endl;

    // 解析json
    Json::Value root;
    Json::Reader reader;

    if (reader.parse(json, root)) {
        //解析成功,获取数据
        root["id"].asInt();
        root["name"].asCString();
    }




    return 0;
}
