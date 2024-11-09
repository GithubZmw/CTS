#include "Aggregator.h"



int main(int argc, char *argv[]) {
    getPpAndSk(argc, argv);
    collectParSigs(argc, argv);
    startServerTran(argc, argv);
    return 0;
}
