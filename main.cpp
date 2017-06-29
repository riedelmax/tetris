
#include "msp.h"
#include <grlib.h>
#include <stdio.h>
#include "gpio_msp432.h"
#include "st7735s.h"

// Graphic library context
Graphics_Context g_sContext;

// Graphics display structure
extern const Graphics_Display g_st7735s;

int main()
{
	// SPI interface
	gpio_msp432_pin lcd_cs ( PORT_PIN(5,0) );
	spi_msp432      spi(EUSCI_B0_SPI, lcd_cs);

	// ST7735s driver
	gpio_msp432_pin lcd_rst( PORT_PIN(5,7) );
	gpio_msp432_pin lcd_dc ( PORT_PIN(3,7) );
	st7735s         lcd_drv(spi, lcd_rst, lcd_dc);

	// Backlight
	gpio_msp432_pin lcd_bl ( PORT_PIN(2,6) );
    lcd_bl.gpioMode(GPIO::OUTPUT | GPIO::INIT_HIGH);

    // Set default screen orientation
    lcd_drv.SetOrientation(LCD_ORIENTATION_UP);

    // Initializes graphics context
    Graphics_initContext(&g_sContext, &g_st7735s);

    // Set graphic context
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
    Graphics_clearDisplay(&g_sContext);

    // Draw a string and circles
    Graphics_drawStringCentered(&g_sContext, "Hallo MATSE", AUTO_STRING_LENGTH, 64, 10, OPAQUE_TEXT);
    Graphics_drawCircle(&g_sContext, 64, 64, 20);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_fillCircle(&g_sContext, 64, 64, 17);
}
