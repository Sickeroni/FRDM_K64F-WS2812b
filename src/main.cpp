// kinetis SDK header
#include <MK64F12.h>
#include "fsl_device_registers.h"

// kvasir header
#include "Chip/MK64F12.hpp"

// local header
#include "gpio.hpp"


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
    apply(gpio::clock_init);
    // enable GPIO
    apply(write(Kvasir::PortdPcr2::MuxValC::v001)); // set Pin to gpio..
    apply(gpio::init);
    // enable SPI

    // enable FTM I

    // enable FTM II

    // DMA for values (FTM I)

    // DMA for modechange (FTM II)

    // START


    while (1)
    {

    }
}
