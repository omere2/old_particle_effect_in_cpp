// Elementary particle system, amit moskovich.

#include "dgfx2"  // Graphic functions
#include <conio>  // inp
#include <stdlib> // rand
#include <math>   // sin, cos, pi.
#include <timer>  // Timing code.

struct Vector2D {
    float X,Y;
};

struct Particle {
    Vector2D Position,Velocity;
};

const float Pi=3.1415926535897932384626;
const ParticleNumber=512;
Particle Particles[ParticleNumber];

void InitParticles()
{
    for (int i=0; i<ParticleNumber; i++) {
        // Set initial position 160,100 for all particle.
        Particles[i].Position.X=ScreenX/2;
        Particles[i].Position.Y=ScreenY/2;
        // Set random angle (0 - 2pi) and velocity (0 - 1).
        float Angle=float(rand())/32768*Pi*2;
        float Velocity=float(rand())/32768*1.5;
        Particles[i].Velocity.X=cos(Angle)*Velocity;
        Particles[i].Velocity.Y=sin(Angle)*Velocity;
    }
}

void DrawParticles(char* Buffer)
{
    for (int i=0; i<ParticleNumber; i++) {
        PutPixel(Particles[i].Position.X, Particles[i].Position.Y, 255, Buffer);
    }
}

void MoveParticles()
{
    // For each particle, add velocity to position.
    // Multiply velocity by 0.95 to simulate friction.
    for (int i=0; i<ParticleNumber; i++) {
        Particles[i].Position.X+=Particles[i].Velocity.X;
        Particles[i].Position.Y+=Particles[i].Velocity.Y;
        Particles[i].Velocity.Y*=0.99;
        Particles[i].Velocity.X*=0.99;
    }
}

void main()
{
    char* VirScr=new char[ScreenX*ScreenY];
    char* TempBuffer=new char[ScreenX*ScreenY];
    SetScreen(VirScr,0);
    SetScreen(TempBuffer,0);
    SetScreenMode(0x13); // Set Vga 320x200.
    Gradient(0,39,63,63,63,0,0,0);
    Gradient(40,147,0,0,0,0,63,0);
    Gradient(148,255,0,63,0,10,63,30);
    InitParticles();
    int LoopCounter=0;
    StartTimer();
    while (inp(0x60)!=1) // While escape not pressed.
    {
        DrawParticles(VirScr);
        BlurScreen(VirScr,TempBuffer);
        MoveScreen(TempBuffer,VirScr);
        MoveScreen(TempBuffer,VgaBuffer);
        MoveParticles();
        LoopCounter++;
    }
    SetScreenMode(3);
    DisplayTime(LoopCounter);
    delete[] TempBuffer;
    delete[] VirScr;
}