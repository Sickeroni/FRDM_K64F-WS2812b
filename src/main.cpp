// kinetis SDK header
#include <MK64F12.h>
#include "fsl_device_registers.h"

// kvasir header
#include "Chip/MK64F12.hpp"

// TODO remove
#include "clock_config.h"

// local header
#include "gpio.hpp"
#include "spi.hpp"

extern "C" {


int main(void); // needed here to be callable in main.c (otherwise name mangeling mismatch)
}





int main()
{

    SYSMPU_Type *base = SYSMPU;
    BOARD_BootClockRUN();
    // Disable SYSMPU.
   //base->CESR &= ~SYSMPU_CESR_VLD_MASK;

    // enable CLOCKS
    apply(gpio::clock_init,
    spi::clock_init);

    // enable GPIO
    // apply(write(Kvasir::PortdPcr2::MuxValC::v001)); // set Pin to gpio
    apply(gpio::init);

    // enable SPI
    apply(spi::init);
    apply(spi::start);
    apply(write(Kvasir::PortdPcr2::MuxValC::v010)); // set Pin to SPI
    apply(write(Kvasir::PortdPcr1::MuxValC::v010)); // set Pin to SPI clock
    apply(write(Kvasir::Spi0Mcr::ContsckeValC::v1));

    // enable FTM I

    // enable FTM II

    // DMA for values (FTM I)

    // DMA for modechange (FTM II)

    // START
    //apply(write(Kvasir::PortdPcr2::MuxValC::v100)); // set Pin to FTM3_CH2 (disabled)

    while (1)
    {

    }
}
