#include "UAV.h"


int main(int argc, char* argv[]) {
    registerUAV(argc,argv);
    startUAVServer(argc,argv);
    return 0;
}