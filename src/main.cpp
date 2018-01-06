#include <MK64F12.h>
#include "fsl_device_registers.h"

extern "C" {


int main(void); // needed here to be callable in main.c (otherwise name mangeling mismatch)
}





int main()
{
    /*
    SYSMPU_Type *base = SYSMPU;
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    // Disable SYSMPU.
    base->CESR &= ~SYSMPU_CESR_VLD_MASK; */

    // enable CLOCKS

    // enable GPIO

    // enable SPI

    // enable FTM I

    // enable FTM II

    // START


    while (1)
    {

    }
}
