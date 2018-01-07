namespace spi {
    // init all the clocks
    auto constexpr clock_init = write(
            Kvasir::SimScgc6::Spi0ValC::v1  //enable SPI0
    );

    auto constexpr init = Kvasir::MPL::list(
            clear(Kvasir::Spi0Mcr::mdis), // enable module (set 0 to module_disable)
            set(Kvasir::Spi0Mcr::halt), // halt everything
            set(Kvasir::Spi0Mcr::mstr),  // set mastermode
            write(Kvasir::Spi0Ctar0::PbrValC::v10),
            write(Kvasir::Spi0Ctar0::br,Kvasir::Register::value<2>())
    );
    auto constexpr start = clear(Kvasir::Spi0Mcr::halt); // remove halt

}