// util.h
// This file includes basic shapes and utility functions
#include "CSCIx229.h"
#define LEN 8192  //  Maximum length of text string
int local = 0;  // Local Viewer Model

static void Vertex(int th, int ph, double r, double g, double b)
{
   glColor3f(r, g, b);
   double x = -Sin(th)*Cos(ph);
   double y =  Cos(th)*Cos(ph);
   double z =          Sin(ph);
   glNormal3d(x,y,z);
   glTexCoord2d(th/360.0,ph/180.0+0.5);
   glVertex3d(x,y,z);
}

void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

//Draws a sphere!
static void sphere(int texture)
{
	int th,ph;
	//  Set texture 
	if(texture != 0){
	   glEnable(GL_TEXTURE_2D);
	   glBindTexture(GL_TEXTURE_2D, texture);
	}
	//  Latitude bands
	glColor3f(1,1,1);
	for (ph=-90;ph<90;ph+=5)
	{
	  glBegin(GL_QUAD_STRIP);
	  for (th=0;th<=360;th+=5)
	  {
		 Vertex(th,ph, 1,1,1);
		 Vertex(th,ph+5, 1,1,1);
	  }
	  glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}

static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
	//  Save transformation
	glPushMatrix();
	//  Offset
	glTranslated(x,y,z);
	glRotated(th,0,1,0);
	glScaled(dx,dy,dz);
	//  Cube
	glBegin(GL_QUADS);
	//  Front
	glColor3f(0.5,0.5,0.5);
	glNormal3f( 0, 0, 1);
	glVertex3f(-1,-1, 1);
	glVertex3f(+1,-1, 1);
	glVertex3f(+1,+1, 1);
	glVertex3f(-1,+1, 1);
	//  Back
	glNormal3f( 0, 0, -1);
	glVertex3f(+1,-1,-1);
	glVertex3f(-1,-1,-1);
	glVertex3f(-1,+1,-1);
	glVertex3f(+1,+1,-1);
	//  Right
	glNormal3f( 1, 0, 0);
	glVertex3f(+1,-1,+1);
	glVertex3f(+1,-1,-1);
	glVertex3f(+1,+1,-1);
	glVertex3f(+1,+1,+1);
	//  Left
	glNormal3f( -1, 0, 0);
	glVertex3f(-1,-1,-1);
	glVertex3f(-1,-1,+1);
	glVertex3f(-1,+1,+1);
	glVertex3f(-1,+1,-1);
	//  Top
	glNormal3f( 0, 1, 0);
	glVertex3f(-1,+1,+1);
	glVertex3f(+1,+1,+1);
	glVertex3f(+1,+1,-1);
	glVertex3f(-1,+1,-1);
	//  Bottom
	glNormal3f( 0, -1, 0);	
	glVertex3f(-1,-1,-1);
	glVertex3f(+1,-1,-1);
	glVertex3f(+1,-1,+1);
	glVertex3f(-1,-1,+1);
	//  End
	glEnd();
	//  Undo transformations
	glPopMatrix();
}

void drawAxes(double len){
		glBegin(GL_LINES);
		glVertex3d(0.0,0.0,0.0);
		glVertex3d(len,0.0,0.0);
		glVertex3d(0.0,0.0,0.0);
		glVertex3d(0.0,len,0.0);
		glVertex3d(0.0,0.0,0.0);
		glVertex3d(0.0,0.0,len);
		glEnd();
		//  Label axes
		glRasterPos3d(len,0.0,0.0);
		Print("X");
		glRasterPos3d(0.0,len,0.0);
		Print("Y");
		glRasterPos3d(0.0,0.0,len);
		Print("Z");
}

static void lightBall(float Position[4])
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(Position[0],Position[1],Position[2]);
   glScaled(Position[3],Position[3],Position[3]);

   //  Latitude bands
   for (ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         Vertex(th,ph, 1, 1, 1);
         Vertex(th,ph+d, 1, 1, 1);
      }
      glEnd();
   }

   //  Undo transformations
   glPopMatrix();
}

void lightsOn(float Ambient[], float Diffuse[], float Specular[], float Position[]){
	//  Draw light position as ball (still no lighting here)
	lightBall(Position);
	//  OpenGL should normalize normal vectors
	glEnable(GL_NORMALIZE);
	//  Enable lighting
	glEnable(GL_LIGHTING);
	//  Location of viewer for specular calculations
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
	//  glColor sets ambient and diffuse color materials
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	//  Enable light 0
	glEnable(GL_LIGHT0);
	//  Set ambient, diffuse, specular components and position of light 0
	glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
	glLightfv(GL_LIGHT0,GL_POSITION,Position);
}
