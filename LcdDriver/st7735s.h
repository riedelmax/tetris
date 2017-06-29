#ifndef __ST7735S_H__
#define __ST7735S_H__

#include <stdint.h>
#include "grlib.h"
#include "gpio_msp432.h"
#include "spi_msp432.h"

// LCD Screen Dimensions
#define LCD_VERTICAL_MAX     128
#define LCD_HORIZONTAL_MAX   128

#define LCD_ORIENTATION_UP    0
#define LCD_ORIENTATION_LEFT  1
#define LCD_ORIENTATION_DOWN  2
#define LCD_ORIENTATION_RIGHT 3

class st7735s {
public:
    st7735s(spi_msp432      & spi,
    		gpio_msp432_pin & rst_pin,
			gpio_msp432_pin & dc_pin);

    static st7735s * _inst;

	void SetDrawFrame(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

	void SetOrientation(uint8_t orientation);

	void PixelDraw(void *pvDisplayData, int16_t lX, int16_t lY, uint16_t ulValue);


	void PixelDrawMultiple(void *pvDisplayData, int16_t lX,
	                              int16_t lY, int16_t lX0, int16_t lCount,
	                              int16_t lBPP,
	                              const uint8_t *pucData,
	                              const uint32_t *pucPalette);

	void LineDrawH(void *pvDisplayData, int16_t lX1, int16_t lX2,
	                      int16_t lY, uint16_t ulValue);

	void LineDrawV(void *pvDisplayData, int16_t lX, int16_t lY1,
	                      int16_t lY2, uint16_t ulValue);

	void RectFill(void *pvDisplayData, const Graphics_Rectangle *pRect,
	                     uint16_t ulValue);

	uint32_t ColorTranslate(void *pvDisplayData, uint32_t ulValue);

	void	Flush(void *pvDisplayData);

	void	ClearScreen (void *pvDisplayData, uint16_t ulValue);

private:

	void HAL_LCD_writeCommand(uint8_t command);
	void HAL_LCD_writeData(uint8_t data);

    spi_msp432      & spi;
	gpio_msp432_pin & lcd_rst;
	gpio_msp432_pin & lcd_dc;


	uint8_t  Lcd_Orientation;
	uint16_t Lcd_ScreenWidth, Lcd_ScreenHeigth;
	uint8_t  Lcd_PenSolid, Lcd_FontSolid, Lcd_FlagRead;
	uint16_t Lcd_TouchTrim;

};


#endif /* __ST7735S_H__ */

