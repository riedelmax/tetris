/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//*****************************************************************************
//
// Crystalfontz128x128.c - Display driver for the Crystalfontz
//                         128x128 display with ST7735 controller.
//
//*****************************************************************************

#include "grlib.h"
#include "st7735s.h"
#include <stdint.h>
#include "gpio_msp432.h"
#include "spi_msp432.h"

// ST7735 LCD controller Command Set
#define CM_NOP             0x00
#define CM_SWRESET         0x01
#define CM_RDDID           0x04
#define CM_RDDST           0x09
#define CM_SLPIN           0x10
#define CM_SLPOUT          0x11
#define CM_PTLON           0x12
#define CM_NORON           0x13
#define CM_INVOFF          0x20
#define CM_INVON           0x21
#define CM_GAMSET          0x26
#define CM_DISPOFF         0x28
#define CM_DISPON          0x29
#define CM_CASET           0x2A
#define CM_RASET           0x2B
#define CM_RAMWR           0x2C
#define CM_RGBSET          0x2d
#define CM_RAMRD           0x2E
#define CM_PTLAR           0x30
#define CM_MADCTL          0x36
#define CM_COLMOD          0x3A
#define CM_SETPWCTR        0xB1
#define CM_SETDISPL        0xB2
#define CM_FRMCTR3         0xB3
#define CM_SETCYC          0xB4
#define CM_SETBGP          0xb5
#define CM_SETVCOM         0xB6
#define CM_SETSTBA         0xC0
#define CM_SETID           0xC3
#define CM_GETHID          0xd0
#define CM_SETGAMMA        0xE0
#define CM_MADCTL_MY       0x80
#define CM_MADCTL_MX       0x40
#define CM_MADCTL_MV       0x20
#define CM_MADCTL_ML       0x10
#define CM_MADCTL_BGR      0x08
#define CM_MADCTL_MH       0x04

#define HAL_LCD_delay(x)
//__delay_cycles(x * 48)

st7735s * st7735s::_inst;

st7735s::st7735s(spi_msp432       & s,
				 gpio_msp432_pin  & rst_pin,
				 gpio_msp432_pin  & dc_pin)
: spi(s), lcd_rst(rst_pin), lcd_dc(dc_pin) {

	_inst = this;

	// Initialize Reset & D/C pins
	lcd_rst.gpioMode(GPIO::OUTPUT | GPIO::INIT_HIGH);
	lcd_dc.gpioMode (GPIO::OUTPUT | GPIO::INIT_HIGH);

    lcd_rst.gpioWrite(LOW);
    HAL_LCD_delay(50);
    lcd_rst.gpioWrite(HIGH);
    HAL_LCD_delay(120);

    HAL_LCD_writeCommand(CM_SLPOUT);
    HAL_LCD_delay(200);

    HAL_LCD_writeCommand(CM_GAMSET);
    HAL_LCD_writeData(0x04);

    HAL_LCD_writeCommand(CM_SETPWCTR);
    HAL_LCD_writeData(0x0A);
    HAL_LCD_writeData(0x14);

    HAL_LCD_writeCommand(CM_SETSTBA);
    HAL_LCD_writeData(0x0A);
    HAL_LCD_writeData(0x00);

    HAL_LCD_writeCommand(CM_COLMOD);
    HAL_LCD_writeData(0x05);
    HAL_LCD_delay(10);

    HAL_LCD_writeCommand(CM_MADCTL);
    HAL_LCD_writeData(CM_MADCTL_BGR);

    HAL_LCD_writeCommand(CM_NORON);

    Lcd_ScreenWidth  = LCD_VERTICAL_MAX;
    Lcd_ScreenHeigth = LCD_HORIZONTAL_MAX;
    Lcd_PenSolid  = 0;
    Lcd_FontSolid = 1;
    Lcd_FlagRead  = 0;
    Lcd_TouchTrim = 0;

    SetDrawFrame(0, 0, 127, 127);
    HAL_LCD_writeCommand(CM_RAMWR);
    int i;
    for (i = 0; i < 16384; i++)
    {
        HAL_LCD_writeData(0xFF);
        HAL_LCD_writeData(0xFF);
    }

    HAL_LCD_delay(10);
    HAL_LCD_writeCommand(CM_DISPON);
}


void st7735s::SetDrawFrame(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    switch (Lcd_Orientation) {
        case 0:
            x0 += 2;
            y0 += 3;
            x1 += 2;
            y1 += 3;
            break;
        case 1:
            x0 += 3;
            y0 += 2;
            x1 += 3;
            y1 += 2;
            break;
        case 2:
            x0 += 2;
            y0 += 1;
            x1 += 2;
            y1 += 1;
            break;
        case 3:
            x0 += 1;
            y0 += 2;
            x1 += 1;
            y1 += 2;
            break;
        default:
            break;
    }

    HAL_LCD_writeCommand(CM_CASET);
    HAL_LCD_writeData((uint8_t)(x0 >> 8));
    HAL_LCD_writeData((uint8_t)(x0));
    HAL_LCD_writeData((uint8_t)(x1 >> 8));
    HAL_LCD_writeData((uint8_t)(x1));

    HAL_LCD_writeCommand(CM_RASET);
    HAL_LCD_writeData((uint8_t)(y0 >> 8));
    HAL_LCD_writeData((uint8_t)(y0));
    HAL_LCD_writeData((uint8_t)(y1 >> 8));
    HAL_LCD_writeData((uint8_t)(y1));
}


//*****************************************************************************
//
//! Sets the LCD Orientation.
//!
//! \param orientation is the desired orientation for the LCD. Valid values are:
//!           - \b LCD_ORIENTATION_UP,
//!           - \b LCD_ORIENTATION_LEFT,
//!           - \b LCD_ORIENTATION_DOWN,
//!           - \b LCD_ORIENTATION_RIGHT,
//!
//! This function sets the orientation of the LCD
//!
//! \return None.
//
//*****************************************************************************
void st7735s::SetOrientation(uint8_t orientation)
{
    Lcd_Orientation = orientation;
    HAL_LCD_writeCommand(CM_MADCTL);
    switch (Lcd_Orientation) {
        case LCD_ORIENTATION_UP:
            HAL_LCD_writeData(CM_MADCTL_MX | CM_MADCTL_MY | CM_MADCTL_BGR);
            break;
        case LCD_ORIENTATION_LEFT:
            HAL_LCD_writeData(CM_MADCTL_MY | CM_MADCTL_MV | CM_MADCTL_BGR);
            break;
        case LCD_ORIENTATION_DOWN:
            HAL_LCD_writeData(CM_MADCTL_BGR);
            break;
        case LCD_ORIENTATION_RIGHT:
            HAL_LCD_writeData(CM_MADCTL_MX | CM_MADCTL_MV | CM_MADCTL_BGR);
            break;
    }
}


//*****************************************************************************
//
//! Draws a pixel on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the pixel.
//! \param lY is the Y coordinate of the pixel.
//! \param ulValue is the color of the pixel.
//!
//! This function sets the given pixel to a particular color.  The coordinates
//! of the pixel are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
void st7735s::PixelDraw(void *pvDisplayData, int16_t lX, int16_t lY,
                                   uint16_t ulValue)
{

    SetDrawFrame(lX,lY,lX,lY);

    //
    // Write the pixel value.
    //
    HAL_LCD_writeCommand(CM_RAMWR);
    HAL_LCD_writeData(ulValue>>8);
    HAL_LCD_writeData(ulValue);
}


//*****************************************************************************
//
//! Draws a horizontal sequence of pixels on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the first pixel.
//! \param lY is the Y coordinate of the first pixel.
//! \param lX0 is sub-pixel offset within the pixel data, which is valid for 1
//! or 4 bit per pixel formats.
//! \param lCount is the number of pixels to draw.
//! \param lBPP is the number of bits per pixel; must be 1, 4, or 8.
//! \param pucData is a pointer to the pixel data.  For 1 and 4 bit per pixel
//! formats, the most significant bit(s) represent the left-most pixel.
//! \param pucPalette is a pointer to the palette used to draw the pixels.
//!
//! This function draws a horizontal sequence of pixels on the screen, using
//! the supplied palette.  For 1 bit per pixel format, the palette contains
//! pre-translated colors; for 4 and 8 bit per pixel formats, the palette
//! contains 24-bit RGB values that must be translated before being written to
//! the display.
//!
//! \return None.
//
//*****************************************************************************
void st7735s::PixelDrawMultiple(void *pvDisplayData, int16_t lX,
                                           int16_t lY, int16_t lX0, int16_t lCount,
                                           int16_t lBPP,
                                           const uint8_t *pucData,
                                           const uint32_t *pucPalette)
{
    uint16_t Data;

    //
    // Set the cursor increment to left to right, followed by top to bottom.
    //
    SetDrawFrame(lX,lY,lX+lCount,127);
    HAL_LCD_writeCommand(CM_RAMWR);

    //
    // Determine how to interpret the pixel data based on the number of bits
    // per pixel.
    //
    switch(lBPP)
    {
        // The pixel data is in 1 bit per pixel format
        case 1:
        {
            // Loop while there are more pixels to draw
            while(lCount > 0)
            {
                // Get the next byte of image data
                Data = *pucData++;

                // Loop through the pixels in this byte of image data
                for(; (lX0 < 8) && lCount; lX0++, lCount--)
                {
                    // Draw this pixel in the appropriate color
                    HAL_LCD_writeData((((uint32_t *)pucPalette)[(Data >>
                                                             (7 - lX0)) & 1])>>8);
                    HAL_LCD_writeData(((uint32_t *)pucPalette)[(Data >>
                                                             (7 - lX0)) & 1]);
                }

                // Start at the beginning of the next byte of image data
                lX0 = 0;
            }
            // The image data has been drawn

            break;
        }

        // The pixel data is in 4 bit per pixel format
        case 4:
        {
            // Loop while there are more pixels to draw.  "Duff's device" is
            // used to jump into the middle of the loop if the first nibble of
            // the pixel data should not be used.  Duff's device makes use of
            // the fact that a case statement is legal anywhere within a
            // sub-block of a switch statement.  See
            // http://en.wikipedia.org/wiki/Duff's_device for detailed
            // information about Duff's device.
            switch(lX0 & 1)
            {
                case 0:

                    while(lCount)
                    {
                        // Get the upper nibble of the next byte of pixel data
                        // and extract the corresponding entry from the palette
                        Data = (*pucData >> 4);
                        Data = (*(uint16_t *)(pucPalette + Data));
                        // Write to LCD screen
                        HAL_LCD_writeData(Data>>8);
                        HAL_LCD_writeData(Data);

                        // Decrement the count of pixels to draw
                        lCount--;

                        // See if there is another pixel to draw
                        if(lCount)
                        {
                case 1:
                            // Get the lower nibble of the next byte of pixel
                            // data and extract the corresponding entry from
                            // the palette
                            Data = (*pucData++ & 15);
                            Data = (*(uint16_t *)(pucPalette + Data));
                            // Write to LCD screen
                            HAL_LCD_writeData(Data>>8);
                            HAL_LCD_writeData(Data);

                            // Decrement the count of pixels to draw
                            lCount--;
                        }
                    }
            }
            // The image data has been drawn.

            break;
        }

        // The pixel data is in 8 bit per pixel format
        case 8:
        {
            // Loop while there are more pixels to draw
            while(lCount--)
            {
                // Get the next byte of pixel data and extract the
                // corresponding entry from the palette
                Data = *pucData++;
                Data = (*(uint16_t *)(pucPalette + Data));
                // Write to LCD screen
                HAL_LCD_writeData(Data>>8);
                HAL_LCD_writeData(Data);
            }
            // The image data has been drawn
            break;
        }

        //
        // We are being passed data in the display's native format.  Merely
        // write it directly to the display.  This is a special case which is
        // not used by the graphics library but which is helpful to
        // applications which may want to handle, for example, JPEG images.
        //
        case 16:
        {
            uint16_t usData;

            // Loop while there are more pixels to draw.

            while(lCount--)
            {
                // Get the next byte of pixel data and extract the
                // corresponding entry from the palette
                usData = *((uint16_t *)pucData);
                pucData += 2;

                // Translate this palette entry and write it to the screen
                HAL_LCD_writeData(usData>>8);
                HAL_LCD_writeData(usData);
            }
        }
    }
}


//*****************************************************************************
//
//! Draws a horizontal line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX1 is the X coordinate of the start of the line.
//! \param lX2 is the X coordinate of the end of the line.
//! \param lY is the Y coordinate of the line.
//! \param ulValue is the color of the line.
//!
//! This function draws a horizontal line on the display.  The coordinates of
//! the line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
void st7735s::LineDrawH(void *pvDisplayData, int16_t lX1, int16_t lX2,
                                   int16_t lY, uint16_t ulValue)
{
    SetDrawFrame(lX1, lY, lX2, lY);
    //
    // Write the pixel value.
    //
    int16_t i;
    HAL_LCD_writeCommand(CM_RAMWR);
    for (i = lX1; i <= lX2; i++)
    {
        HAL_LCD_writeData(ulValue>>8);
        HAL_LCD_writeData(ulValue);
    }
}


//*****************************************************************************
//
//! Draws a vertical line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the line.
//! \param lY1 is the Y coordinate of the start of the line.
//! \param lY2 is the Y coordinate of the end of the line.
//! \param ulValue is the color of the line.
//!
//! This function draws a vertical line on the display.  The coordinates of the
//! line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
void st7735s::LineDrawV(void *pvDisplayData, int16_t lX, int16_t lY1,
                                   int16_t lY2, uint16_t ulValue)
{
    SetDrawFrame(lX, lY1, lX, lY2);
    //
    // Write the pixel value.
    //
    int16_t i;
    HAL_LCD_writeCommand(CM_RAMWR);
    for (i = lY1; i <= lY2; i++)
    {
        HAL_LCD_writeData(ulValue>>8);
        HAL_LCD_writeData(ulValue);
    }
}


//*****************************************************************************
//
//! Fills a rectangle.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param pRect is a pointer to the structure describing the rectangle.
//! \param ulValue is the color of the rectangle.
//!
//! This function fills a rectangle on the display.  The coordinates of the
//! rectangle are assumed to be within the extents of the display, and the
//! rectangle specification is fully inclusive (in other words, both sXMin and
//! sXMax are drawn, along with sYMin and sYMax).
//!
//! \return None.
//
//*****************************************************************************
void st7735s::RectFill(void *pvDisplayData, const Graphics_Rectangle *pRect,
                                  uint16_t ulValue)
{
    int16_t x0 = pRect->sXMin;
    int16_t x1 = pRect->sXMax;
    int16_t y0 = pRect->sYMin;
    int16_t y1 = pRect->sYMax;

    SetDrawFrame(x0, y0, x1, y1);
    //
    // Write the pixel value.
    //
    int16_t i;
    int16_t pixels = (x1 - x0 + 1) * (y1 - y0 + 1);
    HAL_LCD_writeCommand(CM_RAMWR);
    for (i = 0; i <= pixels; i++)
    {
        HAL_LCD_writeData(ulValue>>8);
        HAL_LCD_writeData(ulValue);
    }
}

//*****************************************************************************
//
//! Translates a 24-bit RGB color to a display driver-specific color.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param ulValue is the 24-bit RGB color.  The least-significant byte is the
//! blue channel, the next byte is the green channel, and the third byte is the
//! red channel.
//!
//! This function translates a 24-bit RGB color into a value that can be
//! written into the display's frame buffer in order to reproduce that color,
//! or the closest possible approximation of that color.
//!
//! \return Returns the display-driver specific color.
//
//*****************************************************************************
uint32_t st7735s::ColorTranslate(void *pvDisplayData,
                                    uint32_t ulValue)
{
    //
    // Translate from a 24-bit RGB color to a 5-6-5 RGB color.
    //
    return(((((ulValue) & 0x00f80000) >> 8) |
            (((ulValue) & 0x0000fc00) >> 5) |
            (((ulValue) & 0x000000f8) >> 3)));
}


//*****************************************************************************
//
//! Flushes any cached drawing operations.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//!
//! This functions flushes any cached drawing operations to the display.  This
//! is useful when a local frame buffer is used for drawing operations, and the
//! flush would copy the local frame buffer to the display.  For the SSD2119
//! driver, the flush is a no operation.
//!
//! \return None.
//
//*****************************************************************************
void st7735s::Flush(void *pvDisplayData)
{
    //
    // There is nothing to be done.
    //
}


//*****************************************************************************
//
//! Send command to clear screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//!
//! This function does a clear screen and the Display Buffer contents
//! are initialized to the current background color.
//!
//! \return None.
//
//*****************************************************************************
void st7735s::ClearScreen (void *pvDisplayData, uint16_t ulValue)
{
    Graphics_Rectangle rect = { 0, 0, LCD_VERTICAL_MAX-1, LCD_VERTICAL_MAX-1};
    RectFill(&pvDisplayData, &rect, ulValue);
}



//*****************************************************************************
//
// Writes a command to the CFAF128128B-0145T.  This function implements the basic SPI
// interface to the LCD display.
//
//*****************************************************************************
void st7735s::HAL_LCD_writeCommand(uint8_t command)
{
	lcd_dc.gpioWrite(LOW);
	uint8_t rcv_data;
	spi.transfer(&command, &rcv_data, 1);
	lcd_dc.gpioWrite(HIGH);
}


//*****************************************************************************
//
// Writes a data to the CFAF128128B-0145T.  This function implements the basic SPI
// interface to the LCD display.
//
//*****************************************************************************
void st7735s::HAL_LCD_writeData(uint8_t data)
{
	uint8_t rcv_data;
	spi.transfer(&data, &rcv_data, 1);
}





void gPixelDraw(void *pvDisplayData, int16_t lX, int16_t lY, uint16_t ulValue) {
	st7735s::_inst->PixelDraw(pvDisplayData, lX, lY, ulValue);
}

void gPixelDrawMultiple(void *pvDisplayData, int16_t lX,
                              int16_t lY, int16_t lX0, int16_t lCount,
                              int16_t lBPP,
                              const uint8_t *pucData,
                              const uint32_t *pucPalette) {
	st7735s::_inst->PixelDrawMultiple(pvDisplayData, lX, lY, lX0, lCount,
                              lBPP, pucData, pucPalette);
}

void gLineDrawH(void *pvDisplayData, int16_t lX1, int16_t lX2,
                      int16_t lY, uint16_t ulValue) {
	st7735s::_inst->LineDrawH(pvDisplayData, lX1, lX2,lY, ulValue);
}

void gLineDrawV(void *pvDisplayData, int16_t lX, int16_t lY1,
                      int16_t lY2, uint16_t ulValue) {
	st7735s::_inst->LineDrawV(pvDisplayData, lX, lY1, lY2, ulValue);
}

void gRectFill(void *pvDisplayData, const Graphics_Rectangle *pRect,
                     uint16_t ulValue) {
	st7735s::_inst->RectFill(pvDisplayData, pRect, ulValue);
}

uint32_t gColorTranslate(void *pvDisplayData, uint32_t ulValue) {
	return st7735s::_inst->ColorTranslate(pvDisplayData, ulValue);
}

void gFlush(void *pvDisplayData) {
	st7735s::_inst->Flush(pvDisplayData);
}

void gClearScreen (void *pvDisplayData, uint16_t ulValue) {
	st7735s::_inst->ClearScreen(pvDisplayData, ulValue);
}


extern const Graphics_Display g_st7735s =
{
    sizeof(tDisplay),
    0,
    LCD_VERTICAL_MAX,
    LCD_HORIZONTAL_MAX,
	gPixelDraw,
	gPixelDrawMultiple,
	gLineDrawH,
	gLineDrawV,
	gRectFill,
	gColorTranslate,
	gFlush,
	gClearScreen
};


