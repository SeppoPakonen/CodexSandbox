#include "EasyDemo.h"


int main (int, char**)
{
    OSReport("Hello World!!!\n");
#ifdef _DEBUG
    OSReport("This is a DEBUG build.\n");
#else
    OSReport("This is a NON-DEBUG/optimized build.\n");
#endif // _DEBUG
    OSReport("Calling SampleLib->SampleFunction with 42.\n");
    OSReport("Result is %d\n",SampleFunction(42));
    
    OSHalt("Sample complete");
    
    return 0;
}
