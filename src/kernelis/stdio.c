#include<stdint.h>
typedef unsigned char Bool;
#define false 0
#define true 1

static int VGA_buffer_index;

void prtchr(char c, int index)
{
	char* VGA_BUFFER = (char*) 0xFFFFFFFFFFE95000 - 0xA0000 + 0xB8000;
	VGA_BUFFER[index << 1] = c;
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

void print(char* buffer, void* argv)
{
	int i = 0;
	int ai = 0;
	while (buffer[i] != '\0')
	{
		if (buffer[i] == '%')
		{
			i++;
			uint8_t* ptr = (uint8_t*) argv;
			int integer = *(int*)(ptr + ai);
			static char ibuffer[11];
			itoa(integer, ibuffer);
			uint8_t si = 0;
			while(ibuffer[si] == 0)
			{
				si++;
			}
			while(si < 11)
			{
				prtchr(ibuffer[si], i + VGA_buffer_index - 1);
				si++;
			 	i++;
			}
			ai += sizeof(int);
			i--;
		}
		prtchr(buffer[i], i + VGA_buffer_index);
		i++;
	}
	VGA_buffer_index += i;
	return;
}

