/** @file FONTY.H Proportional fonts
*/

#ifndef FONTY_H
#define FONTY_H

#define FIRST_CHAR 32 ///< Vsechny fonty zacinaji od mezery
#define FontsInstalled 4 ///< Pocet instalovanych fontu

#define txtNormal 0
#define txtNeg 1

#define f_Tahoma_16_Bold 0
#define f_Free_Sans_24_Bold 1
#define f_Free_Sans_72_Bold 2
#define f_Free_Sans_36_Bold 3

/// Struktura popisujici jeden znak fontu
typedef struct tFontInfo {
 const unsigned char* Ptr; ///< Ukazatel na bitovou mapu znaku
 unsigned short Width; ///< Sirka znaku v pixelech
 unsigned short Height; ///< Vyska znaku v pixelech
} TFontInfo;

extern const TFontInfo* FontTable[FontsInstalled];

extern unsigned short GetCharW(unsigned char C, unsigned short Font);
extern void CalcPixelSize(unsigned char* Line, unsigned short Font, unsigned short* Width, unsigned short* Height);

#endif

