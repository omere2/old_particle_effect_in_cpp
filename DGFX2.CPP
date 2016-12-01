#include <conio> // inp, outp.
#include "dgfx2.h"

void PutPixel(long X, long Y, char Color, char* Buffer)
{
    if (X>=0 && X<ScreenX && Y>=0 && Y<ScreenY) Buffer[Y*ScreenX+X]=Color;
}

void SwapChar(char* A, char* B)
{
    char Temp=*A;
    *A=*B;
    *B=Temp;
}

void GetRgb(char Color, char* R, char* G, char* B)
{
        outp(0x3C7,Color);
        *R=inp(0x3C9);
        *G=inp(0x3C9);
        *B=inp(0x3C9);
}

void SetRgb(char Color, char R, char G, char B)
{
        outp(0x3C8,Color);
        outp(0x3C9,R);
        outp(0x3C9,G);
        outp(0x3C9,B);
}

void Gradient(char Color1, char Color2, char R1, char G1, char B1, char R2, char G2, char B2)
{
    R1&=63; G1&=63; B1&=63;
    R2&=63; G2&=63; B2&=63; // Make sure values don't overflow.

    if (Color1 != Color2)
    {
        if (Color1 > Color2) // Make sure colors are in correct order
        {
            SwapChar(&Color1,&Color2);
            SwapChar(&R1,&R2); SwapChar(&G1,&G2); SwapChar(&B1,&B2);
        }

        int GradientLength=Color2-Color1+1;
        float RSlope=(float(R2)-R1)/GradientLength;
        float GSlope=(float(G2)-G1)/GradientLength;
        float BSlope=(float(B2)-B1)/GradientLength;
        float R=R1, G=G1, B=B1;
        for (int i=Color1; i<=Color2; i++)
        {
            R+=RSlope;
            G+=GSlope;
            B+=BSlope;
            SetRgb(i,R,G,B);
        }
    }
}

void DisplayPalette(char* Buffer)
{
    for (int y=0; y<ScreenY; y++)
        for (int x=0; x<256; x++)
            PutPixel(x,y,x,Buffer);
}

void BlurScreen(char* Buffer, char* TempBuffer)
{
    for (int y=1; y<ScreenY-1; y++)
        for (int x=1; x<ScreenX-1; x++)
        {
            int Sum=Buffer[(y-1)*ScreenX+(x-1)]+Buffer[(y-1)*ScreenX+(x)]
            +Buffer[(y-1)*ScreenX+(x+1)]+Buffer[(y)*ScreenX+(x-1)]
            +Buffer[(y)*ScreenX+(x)]+Buffer[(y)*ScreenX+(x+1)]
            +Buffer[(y+1)*ScreenX+(x-1)]+Buffer[(y+1)*ScreenX+(x)];

            TempBuffer[y*ScreenX+x]=Sum>>3;
        }
}