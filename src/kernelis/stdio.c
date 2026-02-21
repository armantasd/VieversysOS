#include<stdint.h>
#include<stdarg.h>
#include"stdlib.h"
typedef unsigned char Bool;
#define false 0
#define true 1

static int VGA_buffer_indeksas;

void Prideti_eilute();
inline void Perkrauti_zymekli(int zymeklio_pos);

void prtchr(char c, int index)
{
	char* VGA_BUFFER = (char*) 0xFFFFFFFFFFE95000 - 0xA0000 + 0xB8000;
	VGA_BUFFER[index << 1] = c;
	if (index >= 2000)
	{
		Prideti_eilute();
	}
	uint16_t zymeklio_pos = (c != '\0') ? VGA_buffer_indeksas + 1 : VGA_buffer_indeksas;
	Perkrauti_zymekli(zymeklio_pos);
}

void itoa(int a, char* string)
{
	char letter;
	Bool is_signed = false;
	uint8_t index = 0;
	if (a > 2147483647)
	{
		is_signed = true;
		a--;
		a = ~a;
	}
	letter = (a % 10) + '0';
	a /= 10;
	string[10] = letter;
	while (a != 0)
	{
		index++;
		letter = (a % 10) + '0';
		a /= 10;
		string[10 - index] = letter;
	}
	index++;
	if(is_signed)
	{
		string[10 - index] = '-';
	}
}

void print(char* tekstas, ...)
{
	va_list argumentai;
	va_start(argumentai, tekstas);
	int i = 0;
	while (tekstas[i] != '\0')
	{
		if (tekstas[i] == '%')
		{
			switch(tekstas[i + 1])
			{
				case 'r':
					char raide = va_arg(argumentai, int);
					if(raide == '\b')
					{
						if (VGA_buffer_indeksas == 0)
						{
							i += 2;
						}
						else
						{
							VGA_buffer_indeksas--;
							prtchr('\0', VGA_buffer_indeksas);
							i += 2;
						}
						
					}
					else if(raide == '\n')
					{
						if (VGA_buffer_indeksas > 1920)
						{
							Prideti_eilute();
						}
						int Stulpelis = VGA_buffer_indeksas % 80;
						VGA_buffer_indeksas += 80 - Stulpelis;
						Perkrauti_zymekli(VGA_buffer_indeksas);
						i += 2;
					}
					else
					{
						prtchr(raide, VGA_buffer_indeksas);
						VGA_buffer_indeksas++;
						i += 2;
					}
					break;
				case 's':
					char skaicius[11];
					itoa(va_arg(argumentai, int), skaicius);
					for(int j = 0; j < 11; j++)
					{
						prtchr(skaicius[j], VGA_buffer_indeksas);
						VGA_buffer_indeksas++;
					}
					i += 2;
					break;
			}
		}
		else if(tekstas[i] == '\n')
		{
			if (VGA_buffer_indeksas > 1920)
			{
				Prideti_eilute();
			}
			int Stulpelis = VGA_buffer_indeksas % 80;
			VGA_buffer_indeksas += 80 - Stulpelis;
			i++;
		}
		else
		{
			prtchr(tekstas[i], VGA_buffer_indeksas);
			VGA_buffer_indeksas++;
			i++;
		}
	}
	va_end(argumentai);
}

inline void Prideti_eilute()
{
	char* VGA_BUFFER = (char*) 0xFFFFFFFFFFE95000 - 0xA0000 + 0xB8000;
	for (int i = 0; i < 24; i++)
	{
		for(int j = 0; j < 160; j++)
		{
			VGA_BUFFER[j + i * 160] = VGA_BUFFER[j + i * 160 + 160];
		}
	}
	for (int i = 0; i < 80; i++)
	{
		VGA_BUFFER[(i + 24 * 80) << 1] = '\0';
	}
	VGA_buffer_indeksas -= 80;
}

void Perkrauti_zymekli(int zymeklio_pos)
{
	irasyti_i_porta(0x3d4, 0x0f);
	irasyti_i_porta(0x3d5, (uint8_t)(zymeklio_pos & 0xff));
	irasyti_i_porta(0x3d4, 0x0e);
	irasyti_i_porta(0x3d5, (uint8_t)(zymeklio_pos >> 8) & 0xff);
}



