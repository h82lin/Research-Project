#include <iostream>
#include "../Ed25519.h"

using namespace std;

int main(){
    struct P
    {
        limb_t x[32 / sizeof(limb_t)];
        limb_t y[32 / sizeof(limb_t)];
        limb_t z[32 / sizeof(limb_t)];
        limb_t t[32 / sizeof(limb_t)];
    };
    P p1;
    P p2;
	return 0;
}
