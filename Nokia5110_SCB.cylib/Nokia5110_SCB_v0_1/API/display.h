#ifndef `$INSTANCE_NAME`_DISPLAY_H_
#define `$INSTANCE_NAME`_DISPLAY_H_
    
/***************************************************************************
* Include font files
***************************************************************************/
#include "`$INSTANCE_NAME`_fonts.h"

#include "cytypes.h"
#include "cyfitter.h"
    
/* PSoC Component only */
#define SET_RST_PIN     0x01 /* RST pin is on the first bit of the Control Register */
#define CLEAR_RST_PIN   0x00
#define SET_DC_PIN      0x02 /* DC pin is in the second bit of the Control Register */
#define CLEAR_DC_PIN    0x00
    
#define CLEAR_RST_CLEAR_DC  (CLEAR_RST_PIN | CLEAR_DC_PIN)
#define SET_RST_CLEAR_DC    (SET_RST_PIN | CLEAR_DC_PIN)
#define SET_RST_SET_DC      (SET_RST_PIN | SET_DC_PIN)
#define CLEAR_RST_SET_DC    (CLEAR_RST_PIN | SET_DC_PIN)

/***************************************************************************
* Compiler directive that allow using backgroung under painting
* To disable this feature comment it
***************************************************************************/
#define DRAW_OVER_BACKGROUND

/***************************************************************************
* Background variable
***************************************************************************/
#ifdef DRAW_OVER_BACKGROUND
    const char *Background;
    typedef enum{
        Small_Font = 0,
        Middle_Font = 1
    } font_type;

    typedef enum{
        DRAW_OR = 0,
        DRAW_XOR = 1,
        DRAW_CLEAR = 2
    } draw_type;

    typedef enum{
    	Invert_Yes,
    	Invert_No
    } Invert_t;

#endif // DRAW_OVER_BACKGROUND

/***************************************************************************
* Public function prototypes
***************************************************************************/
void `$INSTANCE_NAME`_Clear (void);
void `$INSTANCE_NAME`_Contrast (char contrast);
void `$INSTANCE_NAME`_Goto (char x, char y);
void `$INSTANCE_NAME`_Image (char x, char y, char xsize, char ysize, const char * dataPtr);
void `$INSTANCE_NAME`_SendData (char data);
void `$INSTANCE_NAME`_SendCommand (char command);

#ifdef DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_Init (const char * dataPtr);
void `$INSTANCE_NAME`_SetBackground (const char * dataPtr);
void `$INSTANCE_NAME`_Chr (char ch, draw_type dt, font_type ft);
void `$INSTANCE_NAME`_Str (char *dataPtr, draw_type dt, font_type ft);
void `$INSTANCE_NAME`_BigStr(char *dataPtr, draw_type dtype);
void `$INSTANCE_NAME`_VBargraph (char x, char ystart, char yend, char yposition, draw_type dt);
void `$INSTANCE_NAME`_HBargraph (char y, char xstart, char xend, char xposition, draw_type dt);
void `$INSTANCE_NAME`_Line (char xb, char yb, char xe, char ye, draw_type dt);
#else  // DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_Init (void);
void `$INSTANCE_NAME`_Chr (char ch, font_type ft);
void `$INSTANCE_NAME`_Str (char *dataPtr, font_type ft);
void `$INSTANCE_NAME`_VBargraph (char x, char ystart, char yend, char yposition);
void `$INSTANCE_NAME`_HBargraph (char y, char xstart, char xend, char xposition);
void `$INSTANCE_NAME`_Line (char xb, char yb, char xe, char ye);

#endif // DRAW_OVER_BACKGROUND

#endif // `$INSTANCE_NAME`_DISPLAY_H_