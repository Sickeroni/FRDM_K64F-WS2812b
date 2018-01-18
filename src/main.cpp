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

static inline void set_dma1(unsigned arrayaddr) { // TODO cleanup Donnerstag
    // dma4
    apply(write(Kvasir::DmaTcd1Attr::smod, Kvasir::Register::value<0>()),
          write(Kvasir::DmaTcd1Attr::SsizeValC::v000), // source size = 8bit
          write(Kvasir::DmaTcd1Attr::dmod, Kvasir::Register::value<0>()), write(Kvasir::DmaTcd1Attr::dsize, Kvasir::Register::value<0>()),
          write(Kvasir::DmaTcd1Soff::soff, Kvasir::Register::value<1>()),
          write(Kvasir::DmaTcd1NbytesMloffno::SmloeValC::v0),
          write(Kvasir::DmaTcd1NbytesMloffno::DmloeValC::v0),
          write(Kvasir::DmaTcd1NbytesMloffno::nbytes, Kvasir::Register::value<1>()),
            //write(Kvasir::DmaTcd1Daddr::daddr,arrayaddr), // destination = adc0SC1a, not changed
          write(Kvasir::DmaTcd1Doff::doff, Kvasir::Register::value<0>()),
          write(Kvasir::DmaTcd1CiterElinkno::ElinkValC::v0),
          write(Kvasir::DmaTcd1Dlastsga::dlastsga, Kvasir::Register::value<0>()), write(Kvasir::DmaTcd1Csr::BwcValC::v00),
          write(Kvasir::DmaTcd1Csr::MajorelinkValC::v0),
          write(Kvasir::DmaTcd1Csr::majorlinkch, Kvasir::Register::value<0>()), write(Kvasir::DmaTcd1Csr::EsgValC::v0),
          write(Kvasir::DmaTcd1Csr::DreqValC::v0), write(Kvasir::DmaTcd1Csr::InthalfValC::v0),
          write(Kvasir::DmaTcd1Csr::IntmajorValC::v0), write(Kvasir::DmaTcd1Csr::StartValC::v0),
          write(Kvasir::DmaTcd1BiterElinkno::ElinkValC::v0));
    // TCD1 Laufzeitvariablen
    apply(
            write(Kvasir::DmaTcd1Saddr::saddr, arrayaddr),
            write(Kvasir::DmaTcd1Slast::slast, -3),
            write(Kvasir::DmaTcd1CiterElinkno::citer, Kvasir::Register::value<3>()),
            write(Kvasir::DmaTcd1BiterElinkno::biter, Kvasir::Register::value<3>()),
            write(Kvasir::DmaTcd1Dlastsga::dlastsga, Kvasir::Register::value<0>()),
            write(Kvasir::DmaTcd1Daddr::daddr, 0x4004C009)); // pinalternatemode...
}

template<unsigned no_of_leds>
static inline void set_dma2(unsigned arrayaddr) { // TODO cleanup Donnerstag
    // dma4
    apply(write(Kvasir::DmaTcd2Attr::smod, Kvasir::Register::value<0>()),
          write(Kvasir::DmaTcd2Attr::SsizeValC::v000), // source size = 8bit
          write(Kvasir::DmaTcd2Attr::dmod, Kvasir::Register::value<0>()), write(Kvasir::DmaTcd2Attr::dsize, Kvasir::Register::value<0>()),
          write(Kvasir::DmaTcd2Soff::soff, Kvasir::Register::value<1>()),
          write(Kvasir::DmaTcd2NbytesMloffno::SmloeValC::v0),
          write(Kvasir::DmaTcd2NbytesMloffno::DmloeValC::v0),
          write(Kvasir::DmaTcd2NbytesMloffno::nbytes, Kvasir::Register::value<1>()),
            //write(Kvasir::DmaTcd2Daddr::daddr,arrayaddr), // destination = adc0SC1a, not changed
          write(Kvasir::DmaTcd2Doff::doff, Kvasir::Register::value<0>()),
          write(Kvasir::DmaTcd2CiterElinkno::ElinkValC::v0),
          write(Kvasir::DmaTcd2Dlastsga::dlastsga, Kvasir::Register::value<0>()), write(Kvasir::DmaTcd2Csr::BwcValC::v00),
          write(Kvasir::DmaTcd2Csr::MajorelinkValC::v0),
          write(Kvasir::DmaTcd2Csr::majorlinkch, Kvasir::Register::value<0>()), write(Kvasir::DmaTcd2Csr::EsgValC::v0),
          write(Kvasir::DmaTcd2Csr::DreqValC::v0), write(Kvasir::DmaTcd2Csr::InthalfValC::v0),
          write(Kvasir::DmaTcd2Csr::IntmajorValC::v0), write(Kvasir::DmaTcd2Csr::StartValC::v0),
          write(Kvasir::DmaTcd2BiterElinkno::ElinkValC::v0));
    // TCD1 Laufzeitvariablen
    apply(
            write(Kvasir::DmaTcd2Saddr::saddr, arrayaddr),
            write(Kvasir::DmaTcd2Slast::slast, -(no_of_leds*3)),
            write(Kvasir::DmaTcd2CiterElinkno::citer, Kvasir::Register::value<no_of_leds*3>()),
            write(Kvasir::DmaTcd2BiterElinkno::biter, Kvasir::Register::value<no_of_leds*3>()),
            write(Kvasir::DmaTcd2Dlastsga::dlastsga, Kvasir::Register::value<0>()),
            write(Kvasir::DmaTcd2Daddr::daddr, 0x4002c034)); // spidataout
}

struct led_t {
    uint8_t R;
    uint8_t G;
    uint8_t B;
};

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
    apply(write(Kvasir::PortdPcr1::MuxValC::v010)); // set Pin to SPI clock
    //apply(write(Kvasir::Spi0Mcr::ContsckeValC::v1));
    // enable FTM for output (duty cycle 0%), FTM3_CH2
    apply(write(Kvasir::PortdPcr2::MuxValC::v100)); // set Pin to (disabled) FTM3_CH2
    //apply(write(Kvasir::Ftm3Sc::ClksValC::v01));
    apply(set(Kvasir::Ftm3C2sc::elsb),set(Kvasir::Ftm3C2sc::msb));

    // enable FTM for outputchange


    // DMA for values (FTM I)
    //set_dmamux();


    //char PinMuxValues[3] = {1U,2U,4U};
    led_t Led_Framebuffer[] = {led_t{0xAA,0x55,0xAA},led_t{0x55,0xAA,0x55},led_t{0xAA,0x55,0xAA}};
    char PinMuxValues[3] = {2U,2U,2U};
    //char PinMuxValues[3] = {4U,4U,4U};

    apply(write(Kvasir::SimScgc6::DmamuxValC::v1,
                Kvasir::SimScgc7::DmaValC::v1));


    set_dma1(reinterpret_cast<unsigned int>(&PinMuxValues[0]));
    set_dma2<3>(reinterpret_cast<unsigned int>(&Led_Framebuffer[0]));


    //ftm3

        apply(write(Kvasir::Ftm3C0sc::DmaValC::v1, // enable dma request :)
                    Kvasir::Ftm3C0sc::ChieValC::v1),
              set(Kvasir::Ftm3C0sc::elsa, Kvasir::Ftm3C0sc::elsb,
                  Kvasir::Ftm3C0sc::msa)
        ); // channel interupt for dma
        apply(write(Kvasir::Ftm3Mod::mod, Kvasir::Register::value<(5)-1>()));
        apply(write(Kvasir::Ftm3Sc::ClksValC::v01, Kvasir::Ftm3Sc::PsValC::v000)); // start through clock (system clock)



    //ftm2
    apply(write(Kvasir::Ftm2C0sc::DmaValC::v1, // enable dma request :)
                Kvasir::Ftm2C0sc::ChieValC::v1),
          set(Kvasir::Ftm2C0sc::elsa, Kvasir::Ftm2C0sc::elsb,
              Kvasir::Ftm2C0sc::msa)
    ); // channel interupt for dma
    apply(write(Kvasir::Ftm2Mod::mod, Kvasir::Register::value<(5*8*3)-1>()));
    apply(write(Kvasir::Ftm2Sc::ClksValC::v01, Kvasir::Ftm2Sc::PsValC::v000)); // start through clock (system clock)

    // enable FTM II


    // DMA for modechange (FTM II)

    // START
    //apply(write(Kvasir::PortdPcr2::MuxValC::v100)); // set Pin to FTM3_CH2 (disabled)
    apply(write(Kvasir::DmaErq::Erq1ValC::v1));
    apply(write(Kvasir::DmaErq::Erq2ValC::v1));
    apply(write(Kvasir::Spi0Ctar0::fmsz,Kvasir::Register::value<7>()));

    (*(volatile uint8_t *) (0x40021001U)) = (1 << 7) + 32;  // Set dma 1 mux to source32 aka FTM3 Channel 0
    (*(volatile uint8_t *) (0x40021002U)) = (1 << 7) + 30;  // Set dma 2 mux to source30 aka FTM2 Channel 0


apply(write(Kvasir::PortdPcr2::MuxValC::v010));

    //apply(write(Kvasir::Spi0Pushr::txdata,Kvasir::Register::value<0xAA>()));

    while (1)
    {
      //  apply(write(Kvasir::Spi0Pushr::txdata,i));
       // i++;
 //       apply(write(Kvasir::PortdPcr2::MuxValC::v001)); // set Pin to gpio
        //    (*(volatile uint8_t *)(0x4004C009)) = 0x1U;
        //(*(volatile uint8_t *)(0x4004C009)) = 0x2U;
        //(*(volatile uint8_t *)(0x4004C009)) = 0x4U;
 //       asm("NOP");
 //       apply(write(Kvasir::PortdPcr2::MuxValC::v010)); // set Pin to SPI0_Output
 //       asm("NOP");
 //       apply(write(Kvasir::PortdPcr2::MuxValC::v100)); // set Pin to (disabled) FTM3_CH2
 //       asm("NOP");
    }
}
