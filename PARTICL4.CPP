// Elementary particle system, amit moskovich.

#include "dgfx2"  // Graphic functions
#include <conio>  // inp
#include <stdlib> // rand
#include <math>   // sin, cos,
#include "timer"  // Timing code.
#include <iostream>

struct Vec2D {
    float X,Y;
};

struct Particle {
    Vec2D Position,Velocity;
};

#define Blur
//#define GravsMove

const int MaxColor=31;
const float Pi=3.1415926535897932384626;
const int ParticleNumber=2048;
const int GravNumber=2;
const float GravStrength=2;
//const Vec2D GravCenter={80,100};
//const Vec2D GravCenter2={220,100};
Particle Gravs[GravNumber]; // Gravitational centers.
Particle Particles[ParticleNumber];

float XPos,YPos,YVel,XVel,BlastStrength;

inline Vec2D VecAdd(Vec2D V1, Vec2D V2)
{
    V1.X+=V2.X;
    V1.Y+=V2.Y;
    return V1;
}

inline Vec2D VecSub(Vec2D V1, Vec2D V2)
{
    V1.X-=V2.X;
    V1.Y-=V2.Y;
    return V1;
}

inline Vec2D VecMul(Vec2D V, float Scalar)
{
    V.X*=Scalar;
    V.Y*=Scalar;
    return V;
}

inline float DotProduct(Vec2D V1, Vec2D V2)
{
    return V1.X*V2.X+V1.Y*V2.Y;
}

inline float VecLength(Vec2D V)
{
        return sqrt(DotProduct(V,V));
}

inline float FloatSqr(float F)
{
    return F*F;
}

void InitGravs()
{
    Gravs[0].Position.X=120; Gravs[0].Position.Y=100;
    Gravs[0].Velocity.X=0.5; Gravs[0].Velocity.Y=1;

    Gravs[1].Position.X=200; Gravs[1].Position.Y=100;
    Gravs[1].Velocity.X=0.7; Gravs[1].Velocity.Y=-1;

//    Gravs[2].Position.X=160; Gravs[2].Position.Y=70;
//    Gravs[2].Velocity.X=0; Gravs[2].Velocity.Y=0.5;

//    Gravs[3].Position.X=160; Gravs[3].Position.Y=130;
//    Gravs[3].Velocity.X=0; Gravs[3].Velocity.Y=-0.5;
}

void InitParticles()
{
    for (int i=0; i<ParticleNumber; i++) {

        Particles[i].Position.X=XPos; Particles[i].Position.Y=YPos;
        Particles[i].Velocity.X=XVel; Particles[i].Velocity.Y=YVel;

        // Set initial position 160,100 for all particle.
//        Particles[i].Position.X=ScreenX/2;
//        Particles[i].Position.Y=ScreenY/2;
        // Set random angle (0 - 2pi) and velocity (0 - 1).
        float Angle=float(rand())/32768*Pi*2;
        float Velocity=float(rand())/32768*BlastStrength;
        Particles[i].Velocity.X+=cos(Angle)*Velocity;
        Particles[i].Velocity.Y+=sin(Angle)*Velocity;
    }
}

void DrawParticles(char* Buffer)
{
    for (int i=0; i<ParticleNumber; i++) {
        PutPixel(Particles[i].Position.X, Particles[i].Position.Y, MaxColor, Buffer);
    }
}

void MoveParticles()
{
    // For each particle, add velocity to position.
    // Multiply velocity by 0.95 to simulate friction.
    for (int i=0; i<ParticleNumber; i++) {
        Particles[i].Position.X+=Particles[i].Velocity.X;
        Particles[i].Position.Y+=Particles[i].Velocity.Y;
//        Particles[i].Velocity.Y*=1;
//        Particles[i].Velocity.X*=1;
    }

    #ifdef GravsMove
        for (i=0; i<GravNumber; i++) {
            Gravs[i].Position.X+=Gravs[i].Velocity.X;
            Gravs[i].Position.Y+=Gravs[i].Velocity.Y;
        }
    #endif
}

inline Particle CalcGravity(Particle P, Vec2D Grav)
{
        Vec2D Dif=VecSub(Grav,P.Position);
        float Force=1/FloatSqr(VecLength(Dif));
        Vec2D AddedDif=VecMul(Dif,Force*GravStrength);
        P.Velocity=VecAdd(P.Velocity,AddedDif);
        return P;
}

void Gravity()
{
    float DifX, DifY, Force;

    for (int i=0; i<GravNumber; i++) {
        for (int j=0; j<ParticleNumber; j++) {
            // Difference=GravCenter-Position
            // Force=1/|Difference|ý
            // AddedDifference=Difference*Force;

            // Particles[j]=CalcGravity(Particles[j],Gravs[i].Position);

            // Dif=VecSub(Gravs[i].Position,Particles[j].Position);
            DifX=Gravs[i].Position.X-Particles[j].Position.X;
            DifY=Gravs[i].Position.Y-Particles[j].Position.Y;
            // Force=1/FloatSqr(VecLength(Dif));
            Force=GravStrength/(DifX*DifX+DifY*DifY);
              // AddedDif=VecMul(Dif,Force*GravStrength);
              // AddedDif.X=Dif.X*Force;
              // AddedDif.Y=Dif.Y*Force;
            // Particles[j].Velocity=VecAdd(Particles[j].Velocity,AddedDif);
            Particles[j].Velocity.X+=DifX*Force;
            Particles[j].Velocity.Y+=DifY*Force;
        }
    }

    #ifdef GravsMove
        for (i=0; i<GravNumber; i++)
            for (int j=0; j<GravNumber; j++)
                if (i!=j) Gravs[j]=CalcGravity(Gravs[j], Gravs[i].Position);
    #endif
}

void DarkenScreen(char* Buffer)
{
    for (int y=0; y<ScreenY; y++)
        for (int x=0; x<ScreenX; x++) {
            char Temp=Buffer[y*ScreenX+x];
            if (Temp>0) Temp--;
            Buffer[y*ScreenX+x]=Temp;
        }

}

void FastBlur(char* Buffer, char* TempBuffer); //320x200 only!!
#pragma aux FastBlur=\
"mov edx,320                           "\
"YLoop:                                "\
"    mov ecx,1                         "\
"    XLoop:                            "\
"        push edx                      "\
"        lea ebx,[edx+ecx]             "\
"        add ebx,esi                   "\
"        mov eax,0                     "\
"        mov dx,word ptr [ebx-321]     "\
"        mov al,dl                     "\
"        add al,dh                     "\
"        adc ah,0                      "\
"        mov dx,word ptr [ebx-1]       "\
"        add al,dl                     "\
"        adc ah,0                      "\
"        add al,dh                     "\
"        adc ah,0                      "\
"        mov dx,word ptr [ebx+319]     "\
"        add al,dl                     "\
"        adc ah,0                      "\
"        add al,dh                     "\
"        adc ah,0                      "\
"        add al,byte ptr [ebx-319]     "\
"        adc ah,0                      "\
"        add al,byte ptr [ebx+1]       "\
"        adc ah,0                      "\
"        pop edx                       "\
"        shr eax,3                     "\
"        lea ebx,[edx+ecx]             "\
"        mov [edi+ebx],al              "\
"                                      "\
"        inc ecx                       "\
"        cmp ecx,319                   "\
"    jb XLoop                          "\
"    add edx,320                       "\
"    cmp edx,64000-320                 "\
"jb YLoop                              "\
parm [esi] [edi] modify [eax ebx ecx edx edi esi];

void main()
{
    srand(*((unsigned short*)0x46c)); // set rand seed to current time

//__Random explosion:
//    XPos=rand()%ScreenX; YPos=rand()%ScreenY;
//    XVel=float(rand())/32768-0.5; YVel=float(rand())/32768-0.5;
//    BlastStrength=float(rand())/32768*2;

//__Fixed explosions:
//    XPos=260; YPos=70; XVel=-0.2145; YVel=0.0432; BlastStrength=1.60345;
//    XPos=186; YPos=108; XVel=-0.141632; YVel=0.069; BlastStrength=0.0534;
//    XPos=92; YPos=139; XVel=0.276611; YVel=0.150513; BlastStrength=0.0355408;
//        XPos=130; YPos=140; XVel=1.76611; YVel=-0.60513; BlastStrength=0.0355408;
//    XPos=145; YPos=140; XVel=1.76611; YVel=-0.60513; BlastStrength=0.0555408;
      XPos=133; YPos=140; XVel=1.76611; YVel=-0.60513; BlastStrength=0.055;

        char* VirScr=new char[ScreenX*ScreenY];
        char* TempScr=new char[ScreenX*ScreenY];
        SetScreen(VirScr,0);
        SetScreen(TempScr,0);
        SetScreenMode(0x13); // Set Vga 320x200.
        Gradient(0,MaxColor/2,0,0,0,32,0,63);
        Gradient(MaxColor/2+1,MaxColor,32,0,63,63,63,63);
        SetRgb(255,32,63,0);
//    DisplayPalette(VgaBuffer);
//    while (inp(0x60)>127);
    InitGravs();
    InitParticles();
    int LoopCounter=0;
    float Light=1;
    int FadingNow=0;
    StartTimer();
    while ((inp(0x60)!=1)&&(Light>0)) // While escape not pressed.
    {
        #ifdef Blur
            DrawParticles(VirScr);
            FastBlur(VirScr,TempScr);
            MoveScreen(TempScr,VgaBuffer);
            MoveScreen(TempScr,VirScr);
        #endif
        #ifndef Blur
            SetScreen(VirScr,0);
            DrawParticles(VirScr);
            MoveScreen(VirScr,VgaBuffer);
        #endif

//        for (int i=0; i<GravNumber; i++)
//            PutPixel(Gravs[i].Position.X, Gravs[i].Position.Y, 255, VgaBuffer);
        MoveParticles();
        Gravity();
        LoopCounter++;
    }
    SetScreenMode(3);
    DisplayTime(LoopCounter);
    delete[] TempScr;
    delete[] VirScr;
    cout << "\nNumber of particles: " << ParticleNumber << ".\n";
    cout << "Number of Gravitational poles: " << GravNumber << ".\n";
    cout << "Gravitational scaling factor: " << GravStrength << ".\n\n";
    cout << "Initial explosion position: " << XPos << ", " << YPos << ".\n";
    cout << "Initial speed: " << XVel << ", " << YVel << ".\n";
    cout << "Blast strength: " << BlastStrength << ".\n";
}