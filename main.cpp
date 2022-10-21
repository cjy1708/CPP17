#include <iostream>
#include "mysql.h"
int main() {
    MYSQL mysqlConnect;
    mysql_init(&mysqlConnect);
    if (!mysql_real_connect(&mysqlConnect, "127.0.0.1", "user", "password", NULL, MYSQL_PORT, NULL, 0)) {
        printf("failed\n");
        return -1;
    }
    mysql_select_db(&mysqlConnect, "testdb");
    mysql_query(&mysqlConnect, "SELECT * FROM user");
    // ...
    printf("end\n");
    return 0;
}
