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
#include "ftm.hpp"

extern "C" {


int main(void); // needed here to be callable in main.c (otherwise name mangeling mismatch)
}





int main()
{
    BOARD_BootClockRUN();

    // enable CLOCKS
    apply(gpio::clock_init,
    spi::clock_init,
    ftm::clock_init);

    // enable GPIO
    // apply(write(Kvasir::PortdPcr2::MuxValC::v001)); // set Pin to gpio
    apply(gpio::init);

    // enable SPI
    apply(spi::init);
    apply(spi::start);
    //apply(write(Kvasir::PortdPcr1::MuxValC::v010)); // set Pin to SPI clock

    // enable FTM for output (duty cycle 0%), FTM3_CH2
    apply(write(Kvasir::PortdPcr2::MuxValC::v100)); // set Pin to (disabled) FTM3_CH2
    
    // enable FTM I

    // enable FTM II

    // DMA for values (FTM I)

    // DMA for modechange (FTM II)

    // START
    //apply(write(Kvasir::PortdPcr2::MuxValC::v100)); // set Pin to FTM3_CH2 (disabled)

    while (1)
    {
        //apply(write(Kvasir::PortdPcr2::MuxValC::v001)); // set Pin to gpio
        //apply(write(Kvasir::PortdPcr2::MuxValC::v010)); // set Pin to SPI0_Output
        //apply(write(Kvasir::PortdPcr2::MuxValC::v100)); // set Pin to (disabled) FTM3_CH2
    }
}
