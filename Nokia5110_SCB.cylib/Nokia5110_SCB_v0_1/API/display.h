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
* DRAW_OVER_BACKGROUND: Compiler directive that allow using backgroung under
* painting, this feature is enabled by default, to disable it
* place #undef DRAW_OVER_BACKGROUND between the
*
* // `#START DRAW_OVER_BACKGROUND` //
*
* // `#END` //
*
* section bellow.
***************************************************************************/
#define DRAW_OVER_BACKGROUND
/* `#START DRAW_OVER_BACKGROUND` */

/* `#END` */

#ifdef DRAW_OVER_BACKGROUND

static const char *Background;

typedef enum{
    DRAW_OR = 0,
    DRAW_XOR = 1,
    DRAW_CLEAR = 2
}DRAW_TYPE_t;

#endif // DRAW_OVER_BACKGROUND

typedef enum{
    Small_Font = 0,
    Middle_Font = 1
}FONT_TYPE_t;

/***************************************************************************
* Function prototypes
***************************************************************************/

#ifdef DRAW_OVER_BACKGROUND
void `$INSTANCE_NAME`_Init(const char* dataPtr);
void `$INSTANCE_NAME`_SetBackground(const char* dataPtr);
void `$INSTANCE_NAME`_Chr(char ch, DRAW_TYPE_t dtype, FONT_TYPE_t ftype);
void `$INSTANCE_NAME`_Str(const char* dataPtr, DRAW_TYPE_t dtype, FONT_TYPE_t ftype);
void `$INSTANCE_NAME`_BigStr(char* dataPtr, DRAW_TYPE_t dtype);
void `$INSTANCE_NAME`_VBargraph(uint8_t x, uint8_t ystart, uint8_t yend, uint8_t yposition, DRAW_TYPE_t dtype);
void `$INSTANCE_NAME`_HBargraph(uint8_t y, uint8_t xstart, uint8_t xend, uint8_t xposition, DRAW_TYPE_t dtype);
void `$INSTANCE_NAME`_Line(uint8_t xb, uint8_t yb, uint8_t xe, uint8_t ye, DRAW_TYPE_t dtype);
#else /* DRAW_OVER_BACKGROUND */
void `$INSTANCE_NAME`_Init(void);
void `$INSTANCE_NAME`_Chr(char ch, FONT_TYPE_t ftype);
void `$INSTANCE_NAME`_Str(const char* dataPtr, FONT_TYPE_t ftype);
void `$INSTANCE_NAME`_VBargraph(uint8_t x, uint8_t ystart, uint8_t yend, uint8_t yposition);
void `$INSTANCE_NAME`_HBargraph(uint8_t y, uint8_t xstart, uint8_t xend, uint8_t xposition);
void `$INSTANCE_NAME`_Line (uint8_t xb, uint8_t yb, uint8_t xe, uint8_t ye);
#endif /* DRAW_OVER_BACKGROUND */

void `$INSTANCE_NAME`_Clear(void);
void `$INSTANCE_NAME`_Contrast(uint8_t contrast);
void `$INSTANCE_NAME`_Goto(uint8_t column, uint8_t row);
void `$INSTANCE_NAME`_Image(uint8_t x, uint8_t y, uint8_t xsize, uint8_t ysize, const char* dataPtr);
void `$INSTANCE_NAME`_SendData(uint8_t data);
void `$INSTANCE_NAME`_SendCommand(uint8_t command);

#endif // `$INSTANCE_NAME`_DISPLAY_H_