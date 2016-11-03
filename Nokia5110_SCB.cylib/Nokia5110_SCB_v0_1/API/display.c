#include <project.h>
#include "`$INSTANCE_NAME`_display.h"

int Background_cursor, py, px;

/***************************************************************************
// Private function prototypes
****************************************************************************/

/***************************************************************************
* Name : `$INSTANCE_NAME`_Init
* Description : Performs SPI & `$INSTANCE_NAME`_ controller initialization.
* Argument(s) : None if you don't use DRAW_OVER_BACKGROUND
*               background -> Pointer to background in another case
* Return value : None.
***************************************************************************/
#ifdef DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_Init(const char* background)
#else  //DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_Init(void)
#endif //DRAW_OVER_BACKGROUND
{
    uint16_t i;

    `$INSTANCE_NAME`_Control_Write(SET_RST_CLEAR_DC);

    `$INSTANCE_NAME`_SPI_Start(); // Enable SPI
    `$INSTANCE_NAME`_SPI_SpiSetActiveSlaveSelect(`$INSTANCE_NAME`_SPI_SPI_SLAVE_SELECT0);
    `$INSTANCE_NAME`_SendCommand(0x21); // `$INSTANCE_NAME` Extended Commands.
    `$INSTANCE_NAME`_SendCommand(0xC0); // Set `$INSTANCE_NAME` Vop (Contrast).
    `$INSTANCE_NAME`_SendCommand(0x06); // Set Temp coefficent.
    `$INSTANCE_NAME`_SendCommand(0x13); // `$INSTANCE_NAME` bias mode 1:48.
    `$INSTANCE_NAME`_SendCommand(0x20); // `$INSTANCE_NAME` Standard Commands, Horizontal addressing mode.
    `$INSTANCE_NAME`_SendCommand(0x0C); // `$INSTANCE_NAME` in normal mode.

    `$INSTANCE_NAME`_SendCommand(0x40); // Clear display
    `$INSTANCE_NAME`_SendCommand(0x80);

#ifdef DRAW_OVER_BACKGROUND //DRAW_OVER_BACKGROUND
    if(background == NULL){
        uint16_t n;
        `$INSTANCE_NAME`_Goto(0, 0);
        for(n = 0; n < 504; n++){
            background = 0x00;
            `$INSTANCE_NAME`_SendData(0x00);
        } //84 pixel x 6 lines = 504 
    }else{
        Background = background;
        for(i = 0; i < 504; i++){
            `$INSTANCE_NAME`_SendData(Background[i]);
        }
        Background_cursor = 0;
    }
#else //DRAW_OVER_BACKGROUND
    for(i = 0; i < 504; i++){
        `$INSTANCE_NAME`_SendData(0x00);
    }
#endif
}

/***************************************************************************
* Name : SetBackground
* Description : SetPointer to dispaly background.
* Argument(s) : dataPtr -> Pointer on aray that contains background.
* Return value : None.
* Note : Availble only with DRAW_OVER_BACKGROUND directive
***************************************************************************/
#ifdef DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_SetBackground(const char* dataPtr){
    Background = dataPtr;
}
#endif //DRAW_OVER_BACKGROUND

/***************************************************************************
* Name : `$INSTANCE_NAME`_Contrast
* Description : Set display contrast.
* Argument(s) : contrast -> Contrast value from 0x00 to 0x7F.
* Return value : None.
* Notes : No change visible at ambient temperature.
***************************************************************************/
void `$INSTANCE_NAME`_Contrast(uint8_t contrast){
    `$INSTANCE_NAME`_SendCommand(0x20 | 0x01); // `$INSTANCE_NAME`_ Extended Commands.
    `$INSTANCE_NAME`_SendCommand(0x80 | (contrast & 0x7F)); // Set `$INSTANCE_NAME`_ Vop (Contrast).
    `$INSTANCE_NAME`_SendCommand(0x20); // `$INSTANCE_NAME`_ Standard Commands, horizontal addressing mode.
}

/***************************************************************************
* Name : `$INSTANCE_NAME`_Clear
* Description : Clears the display.
* Argument(s) : None.
* Return value : None.
***************************************************************************/
void `$INSTANCE_NAME`_Clear(void){
    uint16_t i;

    #ifdef DRAW_OVER_BACKGROUND
        `$INSTANCE_NAME`_SendCommand(0x40);
        `$INSTANCE_NAME`_SendCommand(0x80);
        for(i = 0; i < 504; i++){
            `$INSTANCE_NAME`_SendData(Background[i]);
        }
    #else  //DRAW_OVER_BACKGROUND
        for(i = 0; i < 504; i++){
            `$INSTANCE_NAME`_SendData(0x00);
        }
    #endif //DRAW_OVER_BACKGROUND
    `$INSTANCE_NAME`_SendCommand(0x40);
    `$INSTANCE_NAME`_SendCommand(0x80);
}

/***************************************************************************
* Name : `$INSTANCE_NAME`_GotoXY
* Description : Sets cursor location to xy location corresponding to basic font size.
* Argument(s) : x, y -> Coordinate for new cursor position. x range 0..83, y - 0..5
* Return value : None.
***************************************************************************/
void `$INSTANCE_NAME`_Goto(uint8_t column, uint8_t row){
    px = column; // To keep track of the cursor location
    py = row;

    `$INSTANCE_NAME`_SendCommand(0x40|(row & 0x07));
    `$INSTANCE_NAME`_SendCommand(0x80|(column & 0x7f));
    #ifdef DRAW_OVER_BACKGROUND
        Background_cursor = (row * 84) + column;
    #endif //DRAW_OVER_BACKGROUND
}

/***************************************************************************
* Name : `$INSTANCE_NAME`_Chr
* Description : Displays a character at current cursor location and increment cursor location.
* Argument(s) : ch -> Character to write.
*               dt (available only with DRAW_OVER_BACKGROUND directive) -> drawing type  
* Return value : None.
***************************************************************************/
#ifdef DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_Chr(char ch, DRAW_TYPE_t dtype, FONT_TYPE_t ft){
    uint16_t i;
    
    switch(ft){
    case Small_Font: // Small font
        if(dtype == DRAW_OR){
            for(i = 0; i < 3; i++, Background_cursor++){
                `$INSTANCE_NAME`_SendData(SmallFont[ch-32][i] | Background[Background_cursor]);
            }
        }else if(dtype == DRAW_XOR){
            for(i = 0; i < 3; i++, Background_cursor++){
                `$INSTANCE_NAME`_SendData(SmallFont[ch-32][i] ^ Background[Background_cursor]);
                }
        }else{ // DRAW_CLEAR
            for(i = 0; i < 3; i++ ){
                `$INSTANCE_NAME`_SendData(Background[Background_cursor]);
            }
        }
        `$INSTANCE_NAME`_SendData(Background[Background_cursor]); // empty space after character 
    break;
    case Middle_Font: default:// Default Middle font
        if(dtype == DRAW_OR){  
            for(i = 0; i < 5; i++, Background_cursor++){
                `$INSTANCE_NAME`_SendData(MiddleFont[ch-32][i] | Background[Background_cursor]);
            }
        }else if (dtype == DRAW_XOR){
            for(i = 0; i < 5; i++, Background_cursor++){
                `$INSTANCE_NAME`_SendData(MiddleFont[ch-32][i] ^ Background[Background_cursor]);
            }
        }else{ // DRAW_CLEAR
            for(i = 0; i < 5; i++){
                `$INSTANCE_NAME`_SendData(Background[Background_cursor]);
            }
        }
        `$INSTANCE_NAME`_SendData(Background[Background_cursor]); // empty space after character
    break;
    }
}
#else //DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_Chr(char ch, FONT_TYPE_t ft){
    uint16_t i;

    switch(ft){
    case Small_Font: // Small font
        for(i = 0; i < 3; i++){
            `$INSTANCE_NAME`_SendData(SmallFont[ch - 32][i]);// empty space after character
        }
        `$INSTANCE_NAME`_SendData(0x00);
    break;
    case Middle_Font: default: // Default Middle font
        for(i = 0; i < 5; i++){
            `$INSTANCE_NAME`_SendData(MiddleFont[ch - 32][i]);// empty space after character
        }
    `$INSTANCE_NAME`_SendData(0x00);
    break;
    }
}
#endif //DRAW_OVER_BACKGROUND

/***************************************************************************
* Name : `$INSTANCE_NAME`_Str
* Description : Displays a character at current cursor location and increment cursor location
* according to font size.
* Argument(s) : dataPtr -> Pointer to null terminated ASCII string to display.
*               dt (available only with DRAW_OVER_BACKGROUND directive) -> drawing type  
* Return value : None.
***************************************************************************/
#ifdef DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_Str(const char* dataPtr, DRAW_TYPE_t dtype, FONT_TYPE_t ft){
    uint16_t i;
    char ch;

    while(*dataPtr){
        ch = *dataPtr++; 
        switch(ft){
        case Small_Font: // Small font
            if (dtype == DRAW_OR){
                for(i = 0; i < 3; i++, Background_cursor++){
                    `$INSTANCE_NAME`_SendData(SmallFont[ch-32][i] | Background[Background_cursor]);
                }
            }else if(dtype == DRAW_XOR){
                for(i = 0; i < 3; i++, Background_cursor++){
                    `$INSTANCE_NAME`_SendData(SmallFont[ch-32][i] ^ Background[Background_cursor]);
                }
            }else{
                for(i = 0; i < 3; i++, Background_cursor++){
                    `$INSTANCE_NAME`_SendData(Background[Background_cursor]); // empty space after character
                }
            }
            `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);
        break;
        case Middle_Font: default: //Default font Middle
            if(dtype == DRAW_OR){
                for(i = 0; i < 5; i++, Background_cursor++){
                    `$INSTANCE_NAME`_SendData(MiddleFont[ch-32][i] | Background[Background_cursor]);
                }
            }else if(dtype == DRAW_XOR){
                for(i = 0; i < 5; i++, Background_cursor++){
                    `$INSTANCE_NAME`_SendData(MiddleFont[ch-32][i] ^ Background[Background_cursor]);
                }
            }else{
                for(i = 0; i < 5; i++, Background_cursor++){
                    `$INSTANCE_NAME`_SendData(Background[Background_cursor]); // empty space after character
                }
            }
            `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);
        break;
        }
    }
}
#else //DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_Str(const char* dataPtr, FONT_TYPE_t ftype){
    uint16_t i;
    char ch;

    while(*dataPtr){
        ch = *dataPtr++; 
        switch(ftype){
        case Small_Font: // Small font
            char ch = *dataPtr++; 
            for(i = 0; i < 5; i++){
                `$INSTANCE_NAME`_SendData(FontLookup[ch - 32][i]);
            }
            `$INSTANCE_NAME`_SendData(0x00); // empty space after character
        case Middle_Font: default: // Default Middle font
            ch = *dataPtr++; 
            for(i = 0; i < 5; i++){
                `$INSTANCE_NAME`_SendData(FontLookup[ch - 32][i]);
            }
            `$INSTANCE_NAME`_SendData(0x00); // empty space after character
        break;
        }
    }
}
#endif //DRAW_OVER_BACKGROUND

/***************************************************************************
* Name : `$INSTANCE_NAME`_BigStr
* Description : Displays a character at current cursor location and increment cursor location
* according to font size.
* Argument(s) : dataPtr -> Pointer to null terminated ASCII string to display.
*               dt (available only with DRAW_OVER_BACKGROUND directive) -> drawing type  
* Return value : None.
***************************************************************************/
#ifdef DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_BigStr(char* dataPtr, DRAW_TYPE_t dtype){
    char *dataPtrVar, ch;
    uint16_t i;

    dataPtrVar = dataPtr; //draw upper half of text
    `$INSTANCE_NAME`_SendCommand(0x40 | (py & 0x07));
    `$INSTANCE_NAME`_SendCommand(0x80 | (px & 0x7f));
    Background_cursor = (py * 84) + px;

    while(*dataPtrVar){
        ch = *dataPtrVar++;

        if(dtype == DRAW_OR){
            for(i = 0; i < 8; i++, Background_cursor++){
                `$INSTANCE_NAME`_SendData(BigFont[ch-32][i][0] | Background[Background_cursor]); 
            }
        }else if(dtype == DRAW_XOR){
            for(i = 0; i < 8; i++, Background_cursor++){
                `$INSTANCE_NAME`_SendData(BigFont[ch-32][i][0] ^ Background[Background_cursor]); 
            }
        }else{
            for(i = 0; i < 8; i++,Background_cursor++){
                `$INSTANCE_NAME`_SendData(Background[Background_cursor]);
            }
        }
        `$INSTANCE_NAME`_SendData(Background[Background_cursor]);
    };
    dataPtrVar = dataPtr; //draw lover half of text
    `$INSTANCE_NAME`_SendCommand(0x40 | ((py + 1) & 0x07));
    `$INSTANCE_NAME`_SendCommand(0x80 | (px & 0x7f));
    Background_cursor = ((py + 1) * 84) + px;
    while(*dataPtrVar){
        ch = *dataPtrVar++; 

        if(dtype == DRAW_OR){
            for(i = 0; i < 8; i++, Background_cursor++){
                `$INSTANCE_NAME`_SendData(BigFont[ch - 32][i][1] | Background[Background_cursor]); 
            }
        }else if(dtype == DRAW_XOR){
            for(i = 0; i < 8; i++, Background_cursor++){
                `$INSTANCE_NAME`_SendData(BigFont[ch-32][i][1] ^ Background[Background_cursor]);
            }
        }else{
            for(i=0; i<8; i++,Background_cursor++){
                `$INSTANCE_NAME`_SendData(Background[Background_cursor]);
            }
        }
        `$INSTANCE_NAME`_SendData(Background[Background_cursor]);
    }
}
#else //DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_BigStr(uint8_t x, uint8_t y, char* dataPtr){
    char *dataPtrVar, ch;
    uint16_t i;

    dataPtrVar = dataPtr; //draw upper half of text
    `$INSTANCE_NAME`_SendCommand(0x40 | y);
    `$INSTANCE_NAME`_SendCommand(0x80 | x);
    while(*dataPtrVar){
        ch = *dataPtrVar++; 
        for(i = 0; i < 8; i++){
            `$INSTANCE_NAME`_SendData(BigFontLookup[ch - 32][i][0]);
        }
        `$INSTANCE_NAME`_SendData(0x00);
    }
    dataPtrVar = dataPtr; //draw lover half of text
    `$INSTANCE_NAME`_SendCommand(0x40 | (y + 1));
    `$INSTANCE_NAME`_SendCommand(0x80 | x);
    while(*dataPtrVar){        
        ch = *dataPtrVar++; 
        for(i = 0; i < 8; i++){
            `$INSTANCE_NAME`_SendData(BigFontLookup[ch - 32][i][1]);
        }
        `$INSTANCE_NAME`_SendData(0x00);
    }
}
#endif //DRAW_OVER_BACKGROUND

/***************************************************************************
* Name : `$INSTANCE_NAME`_Image
* Description : DrawBitmap.
* Argument(s) : x -> Start x coord
*               y -> Start y coord
*               xsize -> Image x size
*               ysize -> Image y size
*               dataPrt -> pointer to linear array that contains bitmap	
* Return value : None.
***************************************************************************/
void `$INSTANCE_NAME`_Image(uint8_t x, uint8_t y, uint8_t xsize, uint8_t ysize, const char* dataPtr){
    uint16_t i, j;

    for(j = 0; j < ysize; j++){
        `$INSTANCE_NAME`_SendCommand(0x40 | (y + j)); // set start address
        `$INSTANCE_NAME`_SendCommand(0x80 | x);
        for (i = 0; i < xsize; i++){
            `$INSTANCE_NAME`_SendData(dataPtr[j * xsize + i]);
        }
    } 
    #ifdef DRAW_OVER_BACKGROUND
        Background_cursor = (y + ysize) * 84 + x + xsize;
    #else //DRAW_OVER_BACKGROUND
    #endif //DRAW_OVER_BACKGROUND
}

/***************************************************************************
// Name : `$INSTANCE_NAME`_VBargraph
// Description : Draw vertical Bargraph.
// Argument(s) : x -> bargraph's left coord
//               ystart -> Start y coord (by bank)
//               yend -> End y size (by bank, yend must be > ystart)
//               yposition -> bargraph position (by pixel, must  be <= (yend-ystart+1)*8)
//               dt (available only with DRAW_OVER_BACKGROUND directive) -> drawing type  
// Return value : None.
***************************************************************************/
#ifdef DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_VBargraph(uint8_t x, uint8_t ystart, uint8_t yend, uint8_t yposition, DRAW_TYPE_t dtype){
    char i, j, ch;

    `$INSTANCE_NAME`_SendCommand(0x40 | yend);
    `$INSTANCE_NAME`_SendCommand(0x80 | x);
    Background_cursor = (yend * 84) + x;
    if(dtype == DRAW_OR){
        `$INSTANCE_NAME`_SendData(0xFF);
    }else if(dtype == DRAW_XOR){ 
        `$INSTANCE_NAME`_SendData(0xFF ^ Background[Background_cursor++]);
    }else{
        `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);
    }
    for(i = 0; i < 6 ; i++, Background_cursor++){
        switch(yposition){
            case 0 : ch = 0x80; break;
            case 1 : ch = 0x80; break;
            case 2 : ch = 0xC0; break;
            case 3 : ch = 0xE0; break;
            case 4 : ch = 0xF0; break;
            case 5 : ch = 0xF8; break;
            case 6 : ch = 0xFC; break;
            case 7 : ch = 0xFE; break;
            default: ch = 0xFF; break;
        }
        if(dtype == DRAW_OR){ 
            `$INSTANCE_NAME`_SendData(ch | Background[Background_cursor]);
        }else if(dtype == DRAW_XOR){
            `$INSTANCE_NAME`_SendData(ch ^ Background[Background_cursor]);
        }else{
            `$INSTANCE_NAME`_SendData(Background[Background_cursor]);
        }
    }
    if(dtype == DRAW_OR){
        `$INSTANCE_NAME`_SendData(0xFF);
    }else if(dtype == DRAW_XOR){
        `$INSTANCE_NAME`_SendData(0xFF ^ Background[Background_cursor++]);
    }else{
        `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);
    }
    for(j = yend - 1; j > ystart; j--){
        if(yposition > 8){
            yposition -=8;
        }else{
            yposition=0;
        }
        `$INSTANCE_NAME`_SendCommand(0x40 | j);
        `$INSTANCE_NAME`_SendCommand(0x80 | x);
        Background_cursor = (j * 84) + x;

        if(dtype == DRAW_OR){
            `$INSTANCE_NAME`_SendData(0xFF);
        }else if(dtype == DRAW_XOR){
            `$INSTANCE_NAME`_SendData(0xFF ^ Background[Background_cursor++]);
        }else{
            `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);
        }
        for(i=0; i<6 ; i++, Background_cursor++){
            switch(yposition){
                case 0 : ch = 0x00; break;
                case 1 : ch = 0x80; break;
                case 2 : ch = 0xC0; break;
                case 3 : ch = 0xE0; break;
                case 4 : ch = 0xF0; break;
                case 5 : ch = 0xF8; break;
                case 6 : ch = 0xFC; break;
                case 7 : ch = 0xFE; break;
                default: ch = 0xFF; break;
            }
            if(dtype == DRAW_OR){
                `$INSTANCE_NAME`_SendData(ch | Background[Background_cursor]);
            }else if(dtype == DRAW_XOR){
                `$INSTANCE_NAME`_SendData(ch ^ Background[Background_cursor]);
            }else{
                `$INSTANCE_NAME`_SendData(Background[Background_cursor]);
            }
        }
        if(dtype == DRAW_OR){
            `$INSTANCE_NAME`_SendData(0xFF);
        }else if(dtype == DRAW_XOR){
            `$INSTANCE_NAME`_SendData(0xFF ^ Background[Background_cursor++]);
        }else{   
            `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);
        }
    }

    if(yposition > 8){
        yposition -=8;
    }else{
        yposition=0;  
    }
    `$INSTANCE_NAME`_SendCommand(0x40 | ystart);
    `$INSTANCE_NAME`_SendCommand(0x80 | x);
    Background_cursor = (ystart * 84) + x;
    if(dtype == DRAW_OR){
        `$INSTANCE_NAME`_SendData(0xFF);
    }else if(dtype == DRAW_XOR){
        `$INSTANCE_NAME`_SendData(0xFF ^ Background[Background_cursor++]);
    }else{
        `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);
    }
    for(i = 0; i < 6 ; i++, Background_cursor++){
        switch(yposition){
            case 0 : ch = 0x01; break;
            case 1 : ch = 0x81; break;
            case 2 : ch = 0xC1; break;
            case 3 : ch = 0xE1; break;
            case 4 : ch = 0xF1; break;
            case 5 : ch = 0xF9; break;
            case 6 : ch = 0xFD; break;
            case 7 : ch = 0xFF; break;
            default: ch = 0xFF; break;
        }
        if(dtype == DRAW_OR){
            `$INSTANCE_NAME`_SendData(ch | Background[Background_cursor]);
        }else if(dtype == DRAW_XOR){
            `$INSTANCE_NAME`_SendData(ch ^ Background[Background_cursor]);
        }else{
            `$INSTANCE_NAME`_SendData(Background[Background_cursor]);
        }
    }
    if(dtype == DRAW_OR){
        `$INSTANCE_NAME`_SendData(0xFF);
    }else if(dtype == DRAW_XOR){
        `$INSTANCE_NAME`_SendData(0xFF ^ Background[Background_cursor++]);
    }else{
        `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);
    }
}
#else //DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_VBargraph(uint8_t x, uint8_t ystart, uint8_t yend, uint8_t yposition){
    char i, j, ch;

    `$INSTANCE_NAME`_SendCommand(0x40 | yend);
    `$INSTANCE_NAME`_SendCommand(0x80 | x);
    `$INSTANCE_NAME`_SendData(0xFF);

    for(i = 0; i < 6 ; i++, Background_cursor++){
        switch(yposition){
            case 0 : ch = 0x80; break;
            case 1 : ch = 0x80; break;
            case 2 : ch = 0xC0; break;
            case 3 : ch = 0xE0; break;
            case 4 : ch = 0xF0; break;
            case 5 : ch = 0xF8; break;
            case 6 : ch = 0xFC; break;
            case 7 : ch = 0xFE; break;
            default: ch = 0xFF; break;
        }
    `$INSTANCE_NAME`_SendData(ch);
    }
    `$INSTANCE_NAME`_SendData(0xFF);
    for(j = yend - 1; j > ystart; j--){
        if(yposition > 8){
            yposition -=8;
        }else{
            yposition=0;
        }
    `$INSTANCE_NAME`_SendCommand(0x40 | j);
    `$INSTANCE_NAME`_SendCommand(0x80 | x);
    `$INSTANCE_NAME`_SendData(0xFF);
        for(i = 0; i < 6 ; i++, Background_cursor++){
            switch(yposition){
                case 0 : ch = 0x00; break;
                case 1 : ch = 0x80; break;
                case 2 : ch = 0xC0; break;
                case 3 : ch = 0xE0; break;
                case 4 : ch = 0xF0; break;
                case 5 : ch = 0xF8; break;
                case 6 : ch = 0xFC; break;
                case 7 : ch = 0xFE; break;
                default: ch = 0xFF; break;
            }
        `$INSTANCE_NAME`_SendData(ch);
        }
        `$INSTANCE_NAME`_SendData(0xFF);
    }
    if(yposition > 8){
        yposition -=8;
    }else{
        yposition=0;  
    }
    `$INSTANCE_NAME`_SendCommand(0x40 | ystart);
    `$INSTANCE_NAME`_SendCommand(0x80 | x);
    `$INSTANCE_NAME`_SendData(0xFF);
    for(i=0; i<6 ; i++, Background_cursor++){
        switch (yposition){
            case 0 : ch = 0x01; break;
            case 1 : ch = 0x81; break;
            case 2 : ch = 0xC1; break;
            case 3 : ch = 0xE1; break;
            case 4 : ch = 0xF1; break;
            case 5 : ch = 0xF9; break;
            case 6 : ch = 0xFD; break;
            case 7 : ch = 0xFF; break;
            default: ch = 0xFF; break;
        }
    `$INSTANCE_NAME`_SendData(ch);
    }
    `$INSTANCE_NAME`_SendData(0xFF);
}
#endif //DRAW_OVER_BACKGROUND

/***************************************************************************
* Name : `$INSTANCE_NAME`_HBargraph
* Description : Draw vertical Bargraph.
* Argument(s) : y -> bargraph's top coord
*               xstart -> Start x coord 
*		         xend -> End x coord (xend must be > xstart)
*               xposition -> bargraph position (must  be xstart <= xposition <= xend)
*               dt (available only with DRAW_OVER_BACKGROUND directive) -> drawing type  
* Return value : None.
***************************************************************************/
#ifdef DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_HBargraph(uint8_t y, uint8_t xstart, uint8_t xend, uint8_t xposition, DRAW_TYPE_t dtype){
    uint16_t i;

    `$INSTANCE_NAME`_SendCommand(0x40 | y);
    `$INSTANCE_NAME`_SendCommand(0x80 | xstart);
    Background_cursor = y * 84 + xstart;

    if(dtype == DRAW_OR){
        `$INSTANCE_NAME`_SendData(0x7E | Background[Background_cursor++]);
    }else if(dtype == DRAW_XOR){
        `$INSTANCE_NAME`_SendData(0x7E ^ Background[Background_cursor++]);
    }else{
        `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);
    }

    for(i = 1; i < xposition; i++, Background_cursor++)
        if(dtype == DRAW_OR){
            `$INSTANCE_NAME`_SendData(0x7E | Background[Background_cursor]);
        }else if(dtype == DRAW_XOR){
            `$INSTANCE_NAME`_SendData(0x7E ^ Background[Background_cursor]);
        }else{
            `$INSTANCE_NAME`_SendData(Background[Background_cursor]);
        }
    for(i = xposition + 1; i < xend - xstart; i++, Background_cursor++)
        if(dtype == DRAW_OR){
            `$INSTANCE_NAME`_SendData(0x42 | Background[Background_cursor]);
        }else if(dtype == DRAW_XOR){
            `$INSTANCE_NAME`_SendData(0x42 ^ Background[Background_cursor]);
        }else{
            `$INSTANCE_NAME`_SendData(Background[Background_cursor]);
        }
        if(dtype == DRAW_OR){
            `$INSTANCE_NAME`_SendData(0x7E | Background[Background_cursor++]);
        }else if(dtype == DRAW_XOR){
            `$INSTANCE_NAME`_SendData(0x7E ^ Background[Background_cursor++]);
        }else{
            `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);
        }
}
#else //DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_HBargraph(uint8_t y, uint8_t xstart, uint8_t xend, uint8_t xposition){
    uint16_t i;

    `$INSTANCE_NAME`_SendCommand(0x40 | y);
    `$INSTANCE_NAME`_SendCommand(0x80 | xstart);
    `$INSTANCE_NAME`_SendData(0x7E);

    for(i = 1; i <= xposition; i++){
        `$INSTANCE_NAME`_SendData(0x7E);
    }
    for(i = xposition+1; i < xend - xstart; i++){
        `$INSTANCE_NAME`_SendData(0x42);
    }
    `$INSTANCE_NAME`_SendData(0x7E);
}
#endif //DRAW_OVER_BACKGROUND

/***************************************************************************
* Name : `$INSTANCE_NAME`_Line
* Description : Draw line
* Argument(s) : y -> bargraph's top coord
*               xstart -> Start x coord 
*		         xend -> End x coord (xend must be > xstart)
*               xposition -> bargraph position (must  be xstart <= xposition <= xend)
*               dt (available only with DRAW_OVER_BACKGROUND directive) -> drawing type  
* Return value : None.
***************************************************************************/
#ifdef DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_Line(uint8_t xb, uint8_t yb, uint8_t xe, uint8_t ye, DRAW_TYPE_t dtype)
#else //DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_Line(uint8_t xb, uint8_t yb, uint8_t xe, uint8_t ye)
#endif //DRAW_OVER_BACKGROUND
{
    char posx, posy, drwdata;
    int stepx, stepy, dx, dy, fraction;
    dy = ye - yb;
    dx = xe - xb;
    
    if(dy < 0){
        dy = -dy;
        stepy = -1;
    }else{
        stepy = 1;
    }
    
    if(dx < 0){
        dx = -dx;
        stepx = -1;
    }else{
        stepx = 1;
    }
  
    dx <<= 1;
    dy <<= 1;
    posx = xb;
    posy = yb >> 3;
    switch(yb & 0x07){
        case 0x00 : drwdata = 0x01; break;
        case 0x01 : drwdata = 0x02; break;
        case 0x02 : drwdata = 0x04; break;
        case 0x03 : drwdata = 0x08; break;
        case 0x04 : drwdata = 0x10; break;
        case 0x05 : drwdata = 0x20; break;
        case 0x06 : drwdata = 0x40; break;
        default : drwdata = 0x80;
    } 

    if(dx > dy){
        fraction = dy - (dx >> 1);

        while(xb != xe){
            if(fraction >= 0 ){ 
                yb += stepy;
                fraction -= dx;
            }
            xb += stepx;
            fraction += dy;

            if(posx != xb || posy != yb >> 3){
                `$INSTANCE_NAME`_SendCommand(0x40 | posy);
                `$INSTANCE_NAME`_SendCommand(0x80 | posx); 
#ifdef DRAW_OVER_BACKGROUND
                Background_cursor = (posy * 84) + posx;
                if(dtype == DRAW_OR){
                    `$INSTANCE_NAME`_SendData(drwdata | Background[Background_cursor++]);
                }else if(dtype == DRAW_XOR){
                    `$INSTANCE_NAME`_SendData(drwdata ^ Background[Background_cursor++]);
                }else{
                    `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);
                }
#else //DRAW_OVER_BACKGROUND
                `$INSTANCE_NAME`_SendData(drwdata);
#endif //DRAW_OVER_BACKGROUND
                posx = xb;
                posy = yb >> 3;
                switch(yb & 0x07){
                    case 0x00 : drwdata = 0x01; break;
                    case 0x01 : drwdata = 0x02; break;
                    case 0x02 : drwdata = 0x04; break;
                    case 0x03 : drwdata = 0x08; break;
                    case 0x04 : drwdata = 0x10; break;
                    case 0x05 : drwdata = 0x20; break;
                    case 0x06 : drwdata = 0x40; break;
                    default : drwdata = 0x80;
                }
            }else{
                switch(yb & 0x07){
                    case 0x00 : drwdata |= 0x01; break;
                    case 0x01 : drwdata |= 0x02; break;
                    case 0x02 : drwdata |= 0x04; break;
                    case 0x03 : drwdata |= 0x08; break;
                    case 0x04 : drwdata |= 0x10; break;
                    case 0x05 : drwdata |= 0x20; break;
                    case 0x06 : drwdata |= 0x40; break;
                    default : drwdata |= 0x80; break;
                }
            }
        }
    }else{
        fraction = dx - (dy >> 1);
        while(yb != ye){
            if(fraction >=0 ){
                xb += stepx;
                fraction -= dy;
            }
            yb += stepy;
            fraction += dx;
            if(posx != xb || posy != yb >> 3){
                `$INSTANCE_NAME`_SendCommand(0x40 | posy);
                `$INSTANCE_NAME`_SendCommand(0x80 | posx);
#ifdef DRAW_OVER_BACKGROUND
                Background_cursor = posy*84+posx;
                if(dtype == DRAW_OR){
                    `$INSTANCE_NAME`_SendData(drwdata | Background[Background_cursor++]);
                }else if(dtype == DRAW_XOR){
                    `$INSTANCE_NAME`_SendData(drwdata ^ Background[Background_cursor++]);
                }else{
                    `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);
                }
#else //DRAW_OVER_BACKGROUND
                `$INSTANCE_NAME`_SendData(drwdata); 
#endif //DRAW_OVER_BACKGROUND
                posx = xb;
                posy = yb >> 3;
                switch(yb & 0x07){
                    case 0x00: drwdata = 0x01; break;
                    case 0x01 : drwdata = 0x02; break;
                    case 0x02 : drwdata = 0x04; break;
                    case 0x03 : drwdata = 0x08; break;
                    case 0x04 : drwdata = 0x10; break;
                    case 0x05 : drwdata = 0x20; break;
                    case 0x06 : drwdata = 0x40; break;
                    default : drwdata = 0x80;
                }
            }else{
                switch(yb & 0x07){
                    case 0x00: drwdata |= 0x01; break;
                    case 0x01 : drwdata |= 0x02; break;
                    case 0x02 : drwdata |= 0x04; break;
                    case 0x03 : drwdata |= 0x08; break;
                    case 0x04 : drwdata |= 0x10; break;
                    case 0x05 : drwdata |= 0x20; break;
                    case 0x06 : drwdata |= 0x40; break;
                    default : drwdata |= 0x80;
                }
            }
        }
    }
    `$INSTANCE_NAME`_SendCommand(0x40 | posy);
    `$INSTANCE_NAME`_SendCommand(0x80 | posx); 
#ifdef DRAW_OVER_BACKGROUND
    Background_cursor = posy*84+posx;
    if(dtype == DRAW_OR){
        `$INSTANCE_NAME`_SendData(drwdata | Background[Background_cursor++]);
    }else if(dtype == DRAW_XOR){
        `$INSTANCE_NAME`_SendData(drwdata ^ Background[Background_cursor++]);
    }else{
        `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);
    }
#else //DRAW_OVER_BACKGROUND
    `$INSTANCE_NAME`_SendData(drwdata); 
#endif //DRAW_OVER_BACKGROUND
}

/*--------------------------------------------------------------------------------------------------
// Name : `$INSTANCE_NAME`_SendData
// Description : Sends data to display controller.
// Argument(s) : data -> Data to be sent
// Return value : None.
//--------------------------------------------------------------------------------------------------*/
void `$INSTANCE_NAME`_SendData(uint8_t data){
    `$INSTANCE_NAME`_Control_Write(SET_RST_SET_DC);
    `$INSTANCE_NAME`_SPI_SpiUartWriteTxData(data); // Send data to display controller.
    while(`$INSTANCE_NAME`_SPI_SpiIsBusBusy()); // Wait until SPI cycle complete.
}

/*--------------------------------------------------------------------------------------------------
// Name : `$INSTANCE_NAME`_SendCommand
// Description : Sends command to display controller.
// Argument(s) : command -> Command to be sent
// Return value : None.
//--------------------------------------------------------------------------------------------------*/
void `$INSTANCE_NAME`_SendCommand(uint8_t command){
    `$INSTANCE_NAME`_Control_Write(SET_RST_CLEAR_DC);
    `$INSTANCE_NAME`_SPI_SpiUartWriteTxData(command); // Send command to display controller.
    while(`$INSTANCE_NAME`_SPI_SpiIsBusBusy()); // Wait until SPI cycle complete.
}

/* END OF FILE */
