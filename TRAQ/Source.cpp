#include "Injector.h"




int main()
{
    Injector* injector = new Injector;

    injector->Attach(5256);

    printf("[) Current Image Base Address -> %p\n", injector->ExGetCurrentPeb()->ImageBaseAddress);
    





    delete injector;

    return 0;
}