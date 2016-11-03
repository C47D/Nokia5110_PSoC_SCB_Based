# PSoC 4 Nokia5110 Custom Component

Custom Component for Nokia5110 displays, this component is based on SPI peripheral, this version is a SCB based SPI on PSoC 4 devices and there´s also UDB based SPI on both PSoC 4 and PSoC 5LP devices.
SCB based have some glitches on the /SS line, this need more work but at the moment the component is usefull.
Internal driver of the Nokia5110 display works at 4MHz so SPI must work at that freq. aswell.

## Files inside the component:

### x_Display.h

Contain the function declarations and also an important define, DRAW_OVER_BACKGROUND, this define enable the capability of draw over a background, if you don't need it you can disable it by #undef DRAW_OVER_BACKGROUND on the specific region indicated by the comments.

### x_Display.c

Source code of the implementation, most of it taked from an old Cypress AppNote.


Improvements are welcome.

PSoC Rocks!

