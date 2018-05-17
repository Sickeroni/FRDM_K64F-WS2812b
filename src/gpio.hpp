namespace gpio {
    // init all the clocks
    auto constexpr clock_init = write(
            // enable all Ports
            Kvasir::SimScgc5::PortaValC::v1,
            Kvasir::SimScgc5::PortbValC::v1,
            Kvasir::SimScgc5::PortcValC::v1,
            Kvasir::SimScgc5::PortdValC::v1,
            Kvasir::SimScgc5::PorteValC::v1
    );

    auto constexpr init = Kvasir::MPL::list(
            write(Kvasir::PortdPcr2::PeValC::v1,    // Pull-Resistor enable
                  Kvasir::PortdPcr2::PsValC::v1),   // Pull-Resistor ist Pullup
            write(Kvasir::PtdPddr::pdd, Kvasir::Register::value<(1<<2)>()),    // GPIO is Output
            write(Kvasir::PtdPcor::ptco,Kvasir::Register::value<(1<<2)>()) // Pin D2 set (logical 1) in setregister
                );

}