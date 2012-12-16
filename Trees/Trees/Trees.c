/*
 * Trees.c
 *
 * Created: 12/15/2012 2:55:41 PM
 *  Author: kmm
 */ 

#define F_CPU 9000000

#include <avr/io.h>
#include <util/delay.h>
// PB3 row 2 cathode, yellow LED cathode
// PB4 row 1 cathode, yellow LED anode
// PB0 RGB{red} anode
// PB1 RGB{green} anode
// PB2 RGB{blue} anode
#define ROW1 PB3
#define ROW2 PB4
#define RED PB0
#define GREEN PB1
#define BLUE PB2
unsigned const char *top;
unsigned const char sine[] = {24, 49, 73, 97, 120, 141, 161, 180, 197, 211, 224, 235, 240, 244, 250, 255, 250, 244, 240, 235, 224, 211, 197, 180, 161, 141, 120, 97, 73, 49, 24, 0};

void color_rgb(unsigned long color)
{
	signed short dr = 0, dg = 0, db = 0;
	unsigned char r, g, b;
	if(r = (color >> 24) & 0xFF) {
		dr = r - (color & 0xFF);
		dr = dr < 3 ? 3 % 3 : dr;
		dr = dr > 255 ? 255 : dr;
	}
	if(g = (color >> 16) & 0xFF) {
		dg = (g - (color & 0xFF));
		dg = dg < 3 ? 3 : dg;
		dg = dg > 255 ? 255 : dg;
	}
	if((b = (color >> 8) & 0xFF) != 0)
	{
		db = b - (color & 0xFF);
		db = db < 3 ? 3 : db;
		db = db > 255 ? 255 : db;
	}

	for(unsigned char i = 255; i != 0; i--)
	{
		if(dg > i) { PORTB |= _BV(GREEN); } else { PORTB &= ~(_BV(GREEN)); }
		if(dr > i) { PORTB |= _BV(RED); } else { PORTB &= ~(_BV(RED)); }
		if(db > i) { PORTB |= _BV(BLUE); } else { PORTB &= ~(_BV(BLUE)); }
	}
}

// this is kinda broken, not sure where
unsigned long hue(unsigned char val)
{
	unsigned char r = 0, g = 0, b = 0;
	val = val % 252 % 200 % 124;
	if(val < 42) //
	{
		g = (val * 6) | 0x01;
		r = 255;
	}
	else if(val >= 42 && val < 84)
	{
		r = (255 - ((val - 42) * 6)) | 0x01;
		g = 255;
	}
	else if(val >= 84 && val < 126)
	{
		b = ((val - 84) * 6) | 0x01;
		g = 255;
	}
	else if(val >= 126 && val < 168)
	{
		g = (255 - ((val - 126) * 6)) | 0x01;
		b = 255;
	}
	else if(val >= 168 && val < 210)
	{
		r = ((val - 168) * 6) | 0x01;
		b = 255;
	}
	else if(val >= 210 && val <= 252)
	{
		b = (255 - ((val - 210) * 6)) | 0x01;
		r = 255;
	}
	return 0x00000000 | ((unsigned long)r << 24) | ((unsigned long)g << 16) | ((unsigned long)b << 8);
}

int main(void)
{
	DDRB = 0xFF;
	
	uint8_t c, i;

    while(1)
    {
		//color_rgb((uint32_t)255 << 24 | (uint32_t)255 << 16 | (uint32_t)255 << 8);
		PORTB = ~(_BV(ROW2)) | _BV(ROW1);
		color_rgb((uint32_t)top[i] << 24 | (uint32_t)top[i + 1] << 16 | (uint32_t)top[i + 2] << 8);
		PORTB = ~(_BV(ROW1)) | _BV(ROW2);
		color_rgb((uint32_t)top[1024 - i] << 24 | (uint32_t)top[1023 - i] << 16 | (uint32_t)top[1022 - i] << 8);
		if(c % 20 == 0) { i++; }
		c++;
    }
}