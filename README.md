#  What?
NOT WORKING proof of concept to control a LEDStrip with WS2912b-LEDs with a Devboard called FRDM_K64F. 
Super fast, spibased PoC for using LEDStrips with WS2812b. Only using CPU for initialisation.

# Idea
Sending out Data through a PIN with SPI and changing Pinstate with different alternate Pinfunctions. That way a Signal can be genrated, that is compatible with WS2812b. 

# Problem (that fails it)
between SPI-Datapackats there are short gaps.  Because of this the SPI-signal desyncs with the pinchanges.  

# Possible Solutions
* replace SPI with I2S
* trying to find a SPI-configuration (config gap and clock) that is compatible. (Part of the output that is a gap can maybe be hidden in a different Pinstate) 
