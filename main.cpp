#include <iostream>
#include "RunSession.h"

int main() {
    std::cout << "Balala Prototype\n";
    RunSession run;          // seed random, jadi kartu yang keluar beda tiap program run
    run.Start();
    std::cout << "Selesai\n";
    return 0;
}