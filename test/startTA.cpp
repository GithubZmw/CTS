#include "TA.h"

int main(int argc, char *argv[]) {
    initParams();// 初始化系统公钥
    startServerTA(argc, argv);// 启动服务器
    return 0;
}
