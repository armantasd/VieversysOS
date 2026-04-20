#include<stdint.h>
#include<stdbool.h>
#include"failai.h"
#include"stdio.h"

fd* fd_lent;

uint16_t atidaryti(char* failo_lokacija)
{
	direktorija* dir = (void*)RAMDISK + 18 * 512;
	void* Data = (void*)RAMDISK + 28 * 512;
	int j = 0;
	uint16_t klasteris = 0;
	while (true)
	{
		int i = j + 1;
		while (true)
		{
			char raide = failo_lokacija[i + j];
			if(raide == '\0')
			{
				failo_lokacija[i + j - 4] = '\0';
				for(int k = 0; k < 224; k++)
				{
					direktorija failas = dir[k];
					if (palyginti_teksta(failo_lokacija + j + 1, failas.pav) && palyginti_teksta(failo_lokacija + i + j - 3, failas.pav + 8))
					{
						return failas.klasteris;
					}
				}
				return -1;
			}
			if(raide == '/')
			{
				break;
			}
			i++;
		}
		direktorija kita_dir;
		for(int i = 0; i < 225; i++)
		{
			if (i == 224)
			{
				return -1;
			}
			kita_dir = dir[i];
			if (palyginti_teksta(failo_lokacija + j + 1, kita_dir.pav))
			{
				break;
			}
		}
		klasteris = kita_dir.klasteris;
		j = i;
		dir = Data + klasteris * 1024;
	}
}
bool palyginti_teksta(char* tekstas1, char* tekstas2)
{
	int i = 0;
	while(true)
	{
		char raide1 = tekstas1[i];
		char raide2 = tekstas2[i];
		if(raide1 == '\0')
		{
			break;
		}
		if (raide1 != raide2)
		{
			return false;
		}
		i++;
	}
	return true;
}
void skaityti(void* isvestis, int baitu_sk, int paslinkimas, uint16_t failas)
{
	void* Data = (void*)RAMDISK + 28 * 512;
	uint8_t* fat_lent = (uint8_t*)RAMDISK;
	uint16_t klasteris = failas + paslinkimas / 1024;
	uint16_t baitas = paslinkimas % 1024;
	int i = 0;
	while(baitu_sk > i)
	{
		if(baitas == 1024)
		{
			if(klasteris % 2 == 0)
			{
				klasteris = fat_lent[3 * klasteris / 2] + (fat_lent[3 * klasteris / 2 + 1] & 0xf);
			}
			else
			{
				klasteris = (fat_lent[3 * klasteris / 2] >> 4) + (((uint16_t)fat_lent[3 * klasteris / 2 + 1]) << 4);
			}
			if(klasteris == 0xfff)
			{
				return;
			}
			baitas = 0;
		}
		*(uint8_t*)(isvestis + i) = *(uint8_t*)(Data + klasteris * 1024 + baitas);
		i++;
		baitas++;
	}
}
