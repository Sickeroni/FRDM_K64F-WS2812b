namespace uart {
    // init all the clocks
    auto constexpr clock_init = write(
            Kvasir::SimScgc4::Uart0ValC::v1 //enable Uart0
    );

    auto constexpr init = Kvasir::MPL::list(
            write(Kvasir::Spi0Mcr::SmplptValC::v00), // 0 cycles between sck-edge


            clear(Kvasir::Spi0Mcr::mdis), // enable module (set 0 to module_disable)
            set(Kvasir::Spi0Mcr::halt), // halt everything
            set(Kvasir::Spi0Mcr::mstr),  // set mastermode
            write(Kvasir::Spi0Ctar0::PbrValC::v10), // divider Pbr is 5
            write(Kvasir::Spi0Ctar0::br,Kvasir::Register::value<2>()), // divider br is 6
            write(Kvasir::Spi0Ctar0::CphaValC::v1)    // phaseshift is on. (without: first bit is half length)
            ,write(Kvasir::Spi0Ctar0::asc,Kvasir::Register::value<0x3>())


    );
    auto constexpr start = clear(Kvasir::Spi0Mcr::halt); // remove halt

}