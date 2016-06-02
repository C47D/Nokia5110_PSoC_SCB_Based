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
void `$INSTANCE_NAME`_Init (const char * background)
#else  //DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_Init ( void )
#endif //DRAW_OVER_BACKGROUND
{
    int i;
/* wait before clear reset */

    `$INSTANCE_NAME`_Control_Write(SET_RST_CLEAR_DC);

/* Enable SPI */
    `$INSTANCE_NAME`_SPI_Start();
    `$INSTANCE_NAME`_SPI_SpiSetActiveSlaveSelect(`$INSTANCE_NAME`_SPI_SPI_SLAVE_SELECT0);
    `$INSTANCE_NAME`_SendCommand(0x21); // `$INSTANCE_NAME` Extended Commands.
    `$INSTANCE_NAME`_SendCommand(0xC0); // Set `$INSTANCE_NAME` Vop (Contrast).
    `$INSTANCE_NAME`_SendCommand(0x06); // Set Temp coefficent.
    `$INSTANCE_NAME`_SendCommand(0x13); // `$INSTANCE_NAME` bias mode 1:48.
    `$INSTANCE_NAME`_SendCommand(0x20); // `$INSTANCE_NAME` Standard Commands, Horizontal addressing mode.
    `$INSTANCE_NAME`_SendCommand(0x0C); // `$INSTANCE_NAME` in normal mode.
/* Clear display */
    `$INSTANCE_NAME`_SendCommand(0x40);
    `$INSTANCE_NAME`_SendCommand(0x80);

#ifdef DRAW_OVER_BACKGROUND //DRAW_OVER_BACKGROUND
    if (background == NULL){
        int16 n;
        `$INSTANCE_NAME`_Goto(0,0);
        for (n=0; n<504; n++) {
            background = 0x00;
            `$INSTANCE_NAME`_SendData(0x00);
        }//84 pixel x 6 lineas=504 
    }else{
        Background = background;
        for(i=0; i<504; i++) `$INSTANCE_NAME`_SendData(Background[i]);
        Background_cursor = 0;
    }
#else //DRAW_OVER_BACKGROUND
    for(i=0; i<504; i++) `$INSTANCE_NAME`_SendData(0x00);
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
void `$INSTANCE_NAME`_SetBackground ( const char *dataPtr ){
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
void `$INSTANCE_NAME`_Contrast ( char contrast ){
    if (contrast > 0x7f) {
        contrast = 0x7f;
    }
    /* `$INSTANCE_NAME` Extended Commands */
    `$INSTANCE_NAME`_SendCommand(0x20|0x01);
    /* Set `$INSTANCE_NAME` Vop (Contrast) */
    `$INSTANCE_NAME`_SendCommand(0x80|contrast);
    /* `$INSTANCE_NAME` Standard Commands, horizontal addressing mode. */
    `$INSTANCE_NAME`_SendCommand(0x20);
}

/***************************************************************************
* Name : `$INSTANCE_NAME`_Clear
* Description : Clears the display.
* Argument(s) : None.
* Return value : None.
***************************************************************************/
void `$INSTANCE_NAME`_Clear ( void ){
    int i;
    #ifdef DRAW_OVER_BACKGROUND
        `$INSTANCE_NAME`_SendCommand(0x40);
        `$INSTANCE_NAME`_SendCommand(0x80);
        for(i=0; i<504; i++) `$INSTANCE_NAME`_SendData(Background[i]);
    #else  //DRAW_OVER_BACKGROUND
        for(i=0; i<504; i++) `$INSTANCE_NAME`_SendData(0x00);
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
void `$INSTANCE_NAME`_Goto ( char x, char y ){
    px=x;
    py=y;
    `$INSTANCE_NAME`_SendCommand(0x40|(y & 0x07));
    `$INSTANCE_NAME`_SendCommand(0x80|(x & 0x7f));
    #ifdef DRAW_OVER_BACKGROUND
        Background_cursor = y*84 + x;
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
void `$INSTANCE_NAME`_Chr ( char ch, draw_type dtype, font_type ft ){
    int i;
    
    switch(ft){
    case 0: // Small Front
        if (dtype == DRAW_OR)  
            for(i=0; i<3; i++, Background_cursor++) `$INSTANCE_NAME`_SendData(SmallFont[ch-32][i] | Background[Background_cursor]);
        else if (dtype == DRAW_XOR)    
            for(i=0; i<3; i++, Background_cursor++) `$INSTANCE_NAME`_SendData(SmallFont[ch-32][i] ^ Background[Background_cursor]);
        else // DRAW_CLEAR
            for(i=0; i<3; i++ ) `$INSTANCE_NAME`_SendData(Background[Background_cursor]);
            `$INSTANCE_NAME`_SendData(Background[Background_cursor]); // empty space after character 
    break;
            
    default: // Default Middle Front
        if (dtype == DRAW_OR)  
            for(i=0; i<5; i++, Background_cursor++) `$INSTANCE_NAME`_SendData(MiddleFont[ch-32][i] | Background[Background_cursor]);
        else if (dtype == DRAW_XOR)    
            for(i=0; i<5; i++, Background_cursor++) `$INSTANCE_NAME`_SendData(MiddleFont[ch-32][i] ^ Background[Background_cursor]);
        else // DRAW_CLEAR
            for(i=0; i<5; i++ ) `$INSTANCE_NAME`_SendData(Background[Background_cursor]);
            `$INSTANCE_NAME`_SendData(Background[Background_cursor]); // empty space after character
    break;
    }
}
#else //DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_Chr ( char ch , font_type ft ){
    int i;
    switch(ft){
    case 0: // Small Front
        for(i=0; i<3; i++) `$INSTANCE_NAME`_SendData(SmallFront[ch-32][i]);// empty space after character 
        `$INSTANCE_NAME`_SendData(0x00);
    break;
        
    default: // Default Middle Front
    for(i=0; i<5; i++) `$INSTANCE_NAME`_SendData(MiddleFont[ch-32][i]);// empty space after character 
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
void `$INSTANCE_NAME`_Str ( char *dataPtr, draw_type dtype, font_type ft ){
    int i;
    while ( *dataPtr ){
        char ch = *dataPtr++; 
        switch(ft){
        case 0: // Small Front
            if (dtype == DRAW_OR)
                for(i=0; i<3; i++, Background_cursor++) `$INSTANCE_NAME`_SendData(SmallFont[ch-32][i] | Background[Background_cursor]);
            else if (dtype == DRAW_XOR)
                for(i=0; i<3; i++, Background_cursor++) `$INSTANCE_NAME`_SendData(SmallFont[ch-32][i] ^ Background[Background_cursor]);
            else
                for(i=0; i<3; i++, Background_cursor++) `$INSTANCE_NAME`_SendData(Background[Background_cursor]);// empty space after character 
                `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);
        break;
        default: //Default front Middle
            if (dtype == DRAW_OR)
                for(i=0; i<5; i++, Background_cursor++) `$INSTANCE_NAME`_SendData(MiddleFont[ch-32][i] | Background[Background_cursor]);
            else if (dtype == DRAW_XOR)
                for(i=0; i<5; i++, Background_cursor++) `$INSTANCE_NAME`_SendData(MiddleFont[ch-32][i] ^ Background[Background_cursor]);
            else
                for(i=0; i<5; i++, Background_cursor++) `$INSTANCE_NAME`_SendData(Background[Background_cursor]);// empty space after character 
                `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);
        break;
        }
    }
}
#else //DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_Str ( char *dataPtr , font_type ft  ){
    int i;
    while ( *dataPtr ){
    
        char ch = *dataPtr++; 
        switch(ft){
        case 0: // Small Front
            char ch = *dataPtr++; 
            for(i=0; i<5; i++) `$INSTANCE_NAME`_SendData(FontLookup[ch-32][i]);
            // empty space after character 
            `$INSTANCE_NAME`_SendData(0x00);
        default: // Default Middle Front
            char ch = *dataPtr++; 
            for(i=0; i<5; i++) `$INSTANCE_NAME`_SendData(FontLookup[ch-32][i]);
            // empty space after character 
            `$INSTANCE_NAME`_SendData(0x00);
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
void `$INSTANCE_NAME`_BigStr(char *dataPtr, draw_type dtype){

    char *dataPtrVar;
    int i;
    char ch;
    //draw upper half of text
    dataPtrVar = dataPtr;
    `$INSTANCE_NAME`_SendCommand(0x40 | (py & 0x07));
    `$INSTANCE_NAME`_SendCommand(0x80 | (px & 0x7f));
    Background_cursor = py*84+px;
    while ( *dataPtrVar ){

        ch = *dataPtrVar++; 
        if (dtype == DRAW_OR)
            for(i=0; i<8; i++,Background_cursor++) `$INSTANCE_NAME`_SendData(BigFont[ch-32][i][0] | Background[Background_cursor]); 
        else if (dtype == DRAW_XOR)
            for(i=0; i<8; i++,Background_cursor++ ) `$INSTANCE_NAME`_SendData(BigFont[ch-32][i][0] ^ Background[Background_cursor]); 
        else 
            for(i=0; i<8; i++,Background_cursor++ ) `$INSTANCE_NAME`_SendData(Background[Background_cursor]); 
            `$INSTANCE_NAME`_SendData(Background[Background_cursor]);
    };
    //draw lover half of text
    dataPtrVar = dataPtr;
    `$INSTANCE_NAME`_SendCommand(0x40 | ((py+1)& 0x07));
    `$INSTANCE_NAME`_SendCommand(0x80 | ( px& 0x7f));
    Background_cursor = (py+1)*84+px;
    while ( *dataPtrVar ){
    
        char ch = *dataPtrVar++; 
        if (dtype == DRAW_OR)
            for(i=0; i<8; i++,Background_cursor++) `$INSTANCE_NAME`_SendData(BigFont[ch-32][i][1] | Background[Background_cursor]); 
        else if (dtype == DRAW_XOR)
            for(i=0; i<8; i++,Background_cursor++) `$INSTANCE_NAME`_SendData(BigFont[ch-32][i][1] ^ Background[Background_cursor]); 
        else
            for(i=0; i<8; i++,Background_cursor++) `$INSTANCE_NAME`_SendData(Background[Background_cursor]); 
            `$INSTANCE_NAME`_SendData(Background[Background_cursor]);
    }
}
#else //DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_BigStr(char x, char y, char *dataPtr ){
    char *dataPtrVar;
    int i;
    char ch;
    //draw upper half of text
    dataPtrVar = dataPtr;
    `$INSTANCE_NAME`_SendCommand(0x40 | y);
    `$INSTANCE_NAME`_SendCommand(0x80 | x);
    while ( *dataPtrVar ){
        ch = *dataPtrVar++; 
        for(i=0; i<8; i++) `$INSTANCE_NAME`_SendData(BigFontLookup[ch-32][i][0]); 
        `$INSTANCE_NAME`_SendData(0x00);
    };
    //draw lover half of text
    dataPtrVar = dataPtr;
    `$INSTANCE_NAME`_SendCommand(0x40 | (y+1));
    `$INSTANCE_NAME`_SendCommand(0x80 | x);
    while ( *dataPtrVar ){        
        char ch = *dataPtrVar++; 
        for(i=0; i<8; i++) `$INSTANCE_NAME`_SendData(BigFontLookup[ch-32][i][1]); 
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
void `$INSTANCE_NAME`_Image (char x, char y, char xsize, char ysize, const char * dataPtr){
    int i, j;
    for (j=0; j<ysize; j++){
    // set start address
        `$INSTANCE_NAME`_SendCommand(0x40 | (y+j));
        `$INSTANCE_NAME`_SendCommand(0x80 | x);
        for (i=0; i<xsize; i++) `$INSTANCE_NAME`_SendData(dataPtr[j*xsize+i]); 
    } 
    #ifdef DRAW_OVER_BACKGROUND
        Background_cursor = (y+ysize)*84+x+xsize;
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
void `$INSTANCE_NAME`_VBargraph (char x, char ystart, char yend, char yposition, draw_type dt){
    char i,j,ch;
    `$INSTANCE_NAME`_SendCommand(0x40 | yend);
    `$INSTANCE_NAME`_SendCommand(0x80 | x);
    Background_cursor = yend*84+x;
    if (dt == DRAW_OR) 
        `$INSTANCE_NAME`_SendData(0xFF);
    else if (dt == DRAW_XOR) 
        `$INSTANCE_NAME`_SendData(0xFF ^ Background[Background_cursor++]);
    else   
        `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);
    for (i=0; i<6 ; i++, Background_cursor++){
        switch (yposition){
        case 0 : ch=0x80; break;
        case 1 : ch=0x80; break;
        case 2 : ch=0xC0; break;
        case 3 : ch=0xE0; break;
        case 4 : ch=0xF0; break;
        case 5 : ch=0xF8; break;
        case 6 : ch=0xFC; break;
        case 7 : ch=0xFE; break;
        default: ch=0xFF; break;
        };
    if (dt == DRAW_OR) 
        `$INSTANCE_NAME`_SendData(ch | Background[Background_cursor]);
    else if (dt == DRAW_XOR) 
        `$INSTANCE_NAME`_SendData(ch ^ Background[Background_cursor]);
    else   
        `$INSTANCE_NAME`_SendData(Background[Background_cursor]);
    }
    if (dt == DRAW_OR) 
        `$INSTANCE_NAME`_SendData(0xFF);
    else if (dt == DRAW_XOR) 
        `$INSTANCE_NAME`_SendData(0xFF ^ Background[Background_cursor++]);
    else   
        `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);

    for (j=yend-1; j>ystart; j--){
        if (yposition > 8)
            yposition -=8;
        else
            yposition=0;  
            `$INSTANCE_NAME`_SendCommand(0x40 | j);
            `$INSTANCE_NAME`_SendCommand(0x80 | x);
            Background_cursor = j*84+x;

        if (dt == DRAW_OR) 
            `$INSTANCE_NAME`_SendData(0xFF);
        else if (dt == DRAW_XOR) 
            `$INSTANCE_NAME`_SendData(0xFF ^ Background[Background_cursor++]);
        else   
            `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);
        for (i=0; i<6 ; i++, Background_cursor++){
            switch (yposition){
            case 0 : ch=0x00; break;
            case 1 : ch=0x80; break;
            case 2 : ch=0xC0; break;
            case 3 : ch=0xE0; break;
            case 4 : ch=0xF0; break;
            case 5 : ch=0xF8; break;
            case 6 : ch=0xFC; break;
            case 7 : ch=0xFE; break;
            default: ch=0xFF; break;
            };
        if (dt == DRAW_OR) 
            `$INSTANCE_NAME`_SendData(ch | Background[Background_cursor]);
        else if (dt == DRAW_XOR) 
            `$INSTANCE_NAME`_SendData(ch ^ Background[Background_cursor]);
        else   
            `$INSTANCE_NAME`_SendData(Background[Background_cursor]);
        };
        if (dt == DRAW_OR) 
            `$INSTANCE_NAME`_SendData(0xFF);
        else if (dt == DRAW_XOR) 
            `$INSTANCE_NAME`_SendData(0xFF ^ Background[Background_cursor++]);
        else   
            `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);
    };
    if (yposition > 8)
        yposition -=8;
    else
        yposition=0;  
    `$INSTANCE_NAME`_SendCommand(0x40 | ystart);
    `$INSTANCE_NAME`_SendCommand(0x80 | x);
    Background_cursor = ystart*84+x;
    if (dt == DRAW_OR) 
        `$INSTANCE_NAME`_SendData(0xFF);
    else if (dt == DRAW_XOR) 
        `$INSTANCE_NAME`_SendData(0xFF ^ Background[Background_cursor++]);
    else   
        `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);
    for (i=0; i<6 ; i++, Background_cursor++){
        switch (yposition){
        case 0 : ch=0x01; break;
        case 1 : ch=0x81; break;
        case 2 : ch=0xC1; break;
        case 3 : ch=0xE1; break;
        case 4 : ch=0xF1; break;
        case 5 : ch=0xF9; break;
        case 6 : ch=0xFD; break;
        case 7 : ch=0xFF; break;
        default: ch=0xFF; break;
        };
    if (dt == DRAW_OR) 
        `$INSTANCE_NAME`_SendData(ch | Background[Background_cursor]);
    else if (dt == DRAW_XOR) 
        `$INSTANCE_NAME`_SendData(ch ^ Background[Background_cursor]);
    else   
        `$INSTANCE_NAME`_SendData(Background[Background_cursor]);
    }
    if (dt == DRAW_OR) 
        `$INSTANCE_NAME`_SendData(0xFF);
    else if (dt == DRAW_XOR) 
        `$INSTANCE_NAME`_SendData(0xFF ^ Background[Background_cursor++]);
    else   
        `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);
}

#else //DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_VBargraph (char x, char ystart, char yend, char yposition){
    char i,j,ch;
    `$INSTANCE_NAME`_SendCommand(0x40 | yend);
    `$INSTANCE_NAME`_SendCommand(0x80 | x);
    `$INSTANCE_NAME`_SendData(0xFF);
    for (i=0; i<6 ; i++, Background_cursor++){
        switch (yposition){
        case 0 : ch=0x80; break;
        case 1 : ch=0x80; break;
        case 2 : ch=0xC0; break;
        case 3 : ch=0xE0; break;
        case 4 : ch=0xF0; break;
        case 5 : ch=0xF8; break;
        case 6 : ch=0xFC; break;
        case 7 : ch=0xFE; break;
        default: ch=0xFF; break;
        };
    `$INSTANCE_NAME`_SendData(ch);
    }
    `$INSTANCE_NAME`_SendData(0xFF);
    for (j=yend-1; j>ystart; j--){
        if (yposition > 8)
            yposition -=8;
        else
            yposition=0;  
    `$INSTANCE_NAME`_SendCommand(0x40 | j);
    `$INSTANCE_NAME`_SendCommand(0x80 | x);
    `$INSTANCE_NAME`_SendData(0xFF);
        for (i=0; i<6 ; i++, Background_cursor++){
            switch (yposition){
            case 0 : ch=0x00; break;
            case 1 : ch=0x80; break;
            case 2 : ch=0xC0; break;
            case 3 : ch=0xE0; break;
            case 4 : ch=0xF0; break;
            case 5 : ch=0xF8; break;
            case 6 : ch=0xFC; break;
            case 7 : ch=0xFE; break;
            default: ch=0xFF; break;
            };
        `$INSTANCE_NAME`_SendData(ch);
        };
        `$INSTANCE_NAME`_SendData(0xFF);
    };
    if (yposition > 8)
        yposition -=8;
    else
        yposition=0;  
    `$INSTANCE_NAME`_SendCommand(0x40 | ystart);
    `$INSTANCE_NAME`_SendCommand(0x80 | x);
    `$INSTANCE_NAME`_SendData(0xFF);
    for (i=0; i<6 ; i++, Background_cursor++){
        switch (yposition){
        case 0 : ch=0x01; break;
        case 1 : ch=0x81; break;
        case 2 : ch=0xC1; break;
        case 3 : ch=0xE1; break;
        case 4 : ch=0xF1; break;
        case 5 : ch=0xF9; break;
        case 6 : ch=0xFD; break;
        case 7 : ch=0xFF; break;
        default: ch=0xFF; break;
        };
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
void `$INSTANCE_NAME`_HBargraph (char y, char xstart, char xend, char xposition, draw_type dt){
    int i;
    `$INSTANCE_NAME`_SendCommand(0x40 | y);
    `$INSTANCE_NAME`_SendCommand(0x80 | xstart);
    Background_cursor = y*84+xstart;
    if (dt == DRAW_OR) 
        `$INSTANCE_NAME`_SendData(0x7E | Background[Background_cursor++]);
    else if (dt == DRAW_XOR) 
        `$INSTANCE_NAME`_SendData(0x7E ^ Background[Background_cursor++]);
    else   
        `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);
    for (i=1; i<xposition; i++,Background_cursor++) 
        if (dt == DRAW_OR) 
            `$INSTANCE_NAME`_SendData(0x7E | Background[Background_cursor]);
        else if (dt == DRAW_XOR) 
            `$INSTANCE_NAME`_SendData(0x7E ^ Background[Background_cursor]);
        else   
            `$INSTANCE_NAME`_SendData(Background[Background_cursor]);
    for (i=xposition+1; i<xend-xstart; i++,Background_cursor++) 
        if (dt == DRAW_OR) 
            `$INSTANCE_NAME`_SendData(0x42 | Background[Background_cursor]);
        else if (dt == DRAW_XOR) 
            `$INSTANCE_NAME`_SendData(0x42 ^ Background[Background_cursor]);
        else   
            `$INSTANCE_NAME`_SendData(Background[Background_cursor]);
        if (dt == DRAW_OR) 
            `$INSTANCE_NAME`_SendData(0x7E | Background[Background_cursor++]);
        else if (dt == DRAW_XOR) 
            `$INSTANCE_NAME`_SendData(0x7E ^ Background[Background_cursor++]);
        else   
            `$INSTANCE_NAME`_SendData(Background[Background_cursor++]);
}
#else //DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_HBargraph (char y, char xstart, char xend, char xposition){
    int i;
    `$INSTANCE_NAME`_SendCommand(0x40 | y);
    `$INSTANCE_NAME`_SendCommand(0x80 | xstart);
    `$INSTANCE_NAME`_SendData(0x7E);
    for (i=1; i<=xposition; i++){
        `$INSTANCE_NAME`_SendData(0x7E);
    }
    for (i=xposition+1; i<xend-xstart; i++){
        `$INSTANCE_NAME`_SendData(0x42);
    }
    `$INSTANCE_NAME`_SendData(0x7E);
}
#endif //DRAW_OVER_BACKGROUND


#ifdef DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_Line (char xb, char yb, char xe, char ye, draw_type dt)
#else //DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_Line (char xb, char yb, char xe, char ye)
#endif //DRAW_OVER_BACKGROUND
{
    char posx, posy, drwdata;
    int stepx, stepy, dx, dy, fraction;
    dy = ye - yb;
    dx = xe - xb;
    if (dy < 0){
        dy = -dy;
        stepy = -1;
    }
    else stepy = 1; 
    if (dx < 0){
        dx = -dx;
        stepx = -1;
    }
    else stepx = 1;
  
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

    if (dx > dy){
    
    fraction = dy - (dx >> 1);
    while (xb != xe){
        if (fraction >= 0 ){ 
            yb += stepy;
            fraction -= dx;
        }
        xb += stepx;
        fraction += dy;

        if (posx != xb || posy != yb >> 3){
            `$INSTANCE_NAME`_SendCommand(0x40 | posy);
            `$INSTANCE_NAME`_SendCommand(0x80 | posx); 
#ifdef DRAW_OVER_BACKGROUND
            Background_cursor = posy*84+posx;
            if (dt == DRAW_OR) `$INSTANCE_NAME`_SendData(drwdata | Background[Background_cursor++]);
            else if (dt == DRAW_XOR) `$INSTANCE_NAME`_SendData(drwdata ^ Background[Background_cursor++]); 
            else `$INSTANCE_NAME`_SendData(Background[Background_cursor++]); 
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
    while (yb != ye){
        if (fraction >=0 ){
            xb += stepx;
            fraction -= dy;
        }
        yb += stepy;
        fraction += dx;
        if (posx != xb || posy != yb >> 3){
            `$INSTANCE_NAME`_SendCommand(0x40 | posy);
            `$INSTANCE_NAME`_SendCommand(0x80 | posx); 
#ifdef DRAW_OVER_BACKGROUND
            Background_cursor = posy*84+posx;
            if (dt == DRAW_OR) `$INSTANCE_NAME`_SendData(drwdata | Background[Background_cursor++]);
            else if (dt == DRAW_XOR) `$INSTANCE_NAME`_SendData(drwdata ^ Background[Background_cursor++]); 
            else `$INSTANCE_NAME`_SendData(Background[Background_cursor++]); 
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
    if (dt == DRAW_OR) `$INSTANCE_NAME`_SendData(drwdata | Background[Background_cursor++]);
    else if (dt == DRAW_XOR) `$INSTANCE_NAME`_SendData(drwdata ^ Background[Background_cursor++]); 
    else `$INSTANCE_NAME`_SendData(Background[Background_cursor++]); 
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
void `$INSTANCE_NAME`_SendData( char data ){

    `$INSTANCE_NAME`_Control_Write(SET_RST_SET_DC);
    // Send data to display controller.
    `$INSTANCE_NAME`_SPI_SpiUartWriteTxData(data);
    // Wait until SPI cycle complete.
    while(`$INSTANCE_NAME`_SPI_SpiIsBusBusy());
    
}

/*--------------------------------------------------------------------------------------------------
// Name : `$INSTANCE_NAME`_SendCommand
// Description : Sends command to display controller.
// Argument(s) : command -> Command to be sent
// Return value : None.
//--------------------------------------------------------------------------------------------------*/
void `$INSTANCE_NAME`_SendCommand( char command ){
    
    `$INSTANCE_NAME`_Control_Write(SET_RST_CLEAR_DC);
    // Send command to display controller.
    `$INSTANCE_NAME`_SPI_SpiUartWriteTxData(command);
    // Wait until SPI cycle complete.
    while(`$INSTANCE_NAME`_SPI_SpiIsBusBusy());

}

/* END OF FILE */
