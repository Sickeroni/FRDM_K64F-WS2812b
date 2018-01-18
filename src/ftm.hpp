namespace ftm {
    auto constexpr clock_init = write(
            Kvasir::SimScgc3::Ftm3ValC::v1,  //enable FTM3
            Kvasir::SimScgc3::Ftm2ValC::v1,
            Kvasir::SimScgc6::Ftm2ValC::v1
    );

}