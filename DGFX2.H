#ifndef __Dgfx2_H__
#define __Dgfx2_H__

/*
    This is the second version of droolgfx, this time more flexible and
    slightly more C++.

    Works only in a 256 color linearly buffered mode, overall number of
    pixels mus be dividable by 4 (SetScr and MoveScr).
    Compile under Watcom C++ in 386 flat memory model.

    Amit Moskovich, 12.3.98.
*/

#define VgaBuffer ((char *)0xA0000)
const int ScreenX=320;
const int ScreenY=200;

void SetScreenMode(char ModeNumber);
void PutPixel(long X, long Y, char Color, char* Buffer);

void SetScr(char* Buffer, char Color);
void MoveScr(char* Buffer1, char* Buffer2);
void BlurScreen(char* Buffer, char* TempBuffer);

void SwapChar(char* A, char* B);

void GetRgb(char Color, char* R, char* G, char* B);
void SetRgb(char Color, char R, char G, char B);
void Gradient(char Color1, char Color2, char R1, char G1, char B1, char R2, char G2, char B2);
void DisplayPalette(char* Buffer);

void SetScreenMode(char ModeNumber);
#pragma aux SetScreenMode=\
"mov ah,0         "\
"int 10h          "\
parm [al] modify [ah];

void SetScreen(char* Buffer, char Color); // edi=Buffer, bl=Color.
#pragma aux SetScreen=\
"mov eax,ScreenX  "\
"mov ecx,ScreenY  "\
"mul ecx          "\
"shr eax,2        "\
"mov ecx,eax      "/* ecx=ScreenY*ScreenX/4 */\
"mov al,bl        "\
"mov ah,bl        "\
"rol eax,16       "\
"mov al,bl        "\
"mov ah,bl        "/* fill eax's 4 bytes with bl */\
"cld              "\
"rep stosd        "\
parm [edi] [bl] modify [eax ecx edx];

void MoveScreen(char* Buffer1, char* Buffer2); // esi=Buffer1, edi=Buffer2.
#pragma aux MoveScreen=\
"mov eax,ScreenX   "\
"mov ecx,ScreenY   "\
"mul ecx           "\
"mov ecx,eax       "\
"shr ecx,2         "/* ecx=ScreenX*ScreenY/4 */\
"cld               "\
"rep movsd         "\
parm [esi] [edi] modify [eax ecx edx];

#endif