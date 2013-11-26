// Author: Chris Fichman
// Assignment 3: CSCI 4229
// Draw an X-Wing & TIE Fighter
// This code uses excerpts/some shapes from ex8 in order to build parts 
// of the x-wing
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int axes=0;       //  Display axes
int mode=0;       //  Start in Projection mode
int th=0;         //  Azimuth of view angle
int zh=0;
int ph=0;         //  Elevation of view angle
double fov=50.0;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=30.0;   //  Size of world
int x_foils = 1;	// 	X-Foil attack mode
int len = 5;
int info = 0;


#define Cos(th) cos(3.1415927/180*(th))
#define Sin(th) sin(3.1415927/180*(th))

#define LEN 8192  //  Maximum length of text string
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

/*
 *  Set projection
 */
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (mode)
      gluPerspective(fov,asp,dim/4,4*dim);
   //  Orthogonal projection
   else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

static void Vertex(double th,double ph, double r, double g, double b)
{
	glColor3f(r, g, b);
	glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}
/*
 *  Draw a sphere (version 2)
 *     at (x,y,z)
 *     radius (r)
 */
static void sphere(double x,double y,double z,double r, double rd, double gr, double bl)
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   //  Latitude bands
   for (ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         Vertex(th,ph, rd, gr, bl);
         Vertex(th,ph+d, rd, gr, bl);
      }
      glEnd();
   }

   //  Undo transformations
   glPopMatrix();
}

/*
 *  Draw a cube for the base of the X-Wing
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
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
	glVertex3f(-1,-1, 1);
	glVertex3f(+1,-1, 1);
	glVertex3f(+1,+1, 1);
	glVertex3f(-1,+1, 1);
	//  Back
	glVertex3f(+1,-1,-1);
	glVertex3f(-1,-1,-1);
	glVertex3f(-1,+1,-1);
	glVertex3f(+1,+1,-1);
	//  Right
	glVertex3f(+1,-1,+1);
	glVertex3f(+1,-1,-1);
	glVertex3f(+1,+1,-1);
	glVertex3f(+1,+1,+1);
	//  Left
	glVertex3f(-1,-1,-1);
	glVertex3f(-1,-1,+1);
	glVertex3f(-1,+1,+1);
	glVertex3f(-1,+1,-1);
	//  Top
	glVertex3f(-1,+1,+1);
	glVertex3f(+1,+1,+1);
	glVertex3f(+1,+1,-1);
	glVertex3f(-1,+1,-1);
	//  Bottom
	glVertex3f(-1,-1,-1);
	glVertex3f(+1,-1,-1);
	glVertex3f(+1,-1,+1);
	glVertex3f(-1,-1,+1);
	//  End
	glEnd();
	//  Undo transformations
	glPopMatrix();
}

static void asteroidField(double x, double y, double z, 
					double dx, double dy, double dz, double th)
{
	//  Save transformation
	glPushMatrix();
	
	glTranslated(x,y,z);
	glRotated(th,0,1,0);
	glScaled(dx,dy,dz);
	
	sphere(0,0,0, 3, 0.345, 0.345, 0.345);
	sphere(-12,-9,-7, 2, 0.490, 0.490, 0.490);
	sphere(3.8,3.8,3.8, 1.5, 0.299, 0.299, 0.299);
	sphere(5.5,7.3,-10.3, 1.75, 0.545, 0.545, 0.545);

	glPopMatrix();
}

static void tieFighter(	double x, double y, double z, 
					double dx, double dy, double dz,
					double ux, double uy, double uz, double scale)
{

	const double wingRad = 3.00;
	const double wingspan = 2.00;
	const double armRad1 = 0.4;
	const double armRad2 = 0.2;
	
	//  Rotation Matrix Setup from ex8.c
	//  Unit vector in direction of flght
	double D0 = sqrt(dx*dx+dy*dy+dz*dz);
	double X0 = dx/D0;
	double Y0 = dy/D0;
	double Z0 = dz/D0;
	//  Unit vector in "up" direction
	double D1 = sqrt(ux*ux+uy*uy+uz*uz);
	double X1 = ux/D1;
	double Y1 = uy/D1;
	double Z1 = uz/D1;
	//  Cross product gives the third vector
	double X2 = Y0*Z1-Y1*Z0;
	double Y2 = Z0*X1-Z1*X0;
	double Z2 = X0*Y1-X1*Y0;
	//  Rotation matrix
	double mat[16];
	mat[0] = X0;   mat[4] = X1;   mat[ 8] = X2;   mat[12] = 0;
	mat[1] = Y0;   mat[5] = Y1;   mat[ 9] = Y2;   mat[13] = 0;
	mat[2] = Z0;   mat[6] = Z1;   mat[10] = Z2;   mat[14] = 0;
	mat[3] =  0;   mat[7] =  0;   mat[11] =  0;   mat[15] = 1;
	//  Save current transforms
	glPushMatrix();
	//  Offset, scale and rotate
	glTranslated(x,y,z);
	glScaled(scale,scale,scale);
	glMultMatrixd(mat);

	
	//Cockpit
	sphere(0,0,0, 0.8, 0.7,0.7,0.7);
	//Cockpit window
	sphere(0.15,0,0,0.7, 0,0,1);
	
	//Arms to wings
	glBegin(GL_QUADS);
	glColor3f(0.5,0.5, 0.5);
	//Left Arm
	//Base of arm
		glVertex3d(armRad1,  armRad1,0.5); //v0
		glVertex3d(-armRad1, armRad1,0.5); //v1
		glVertex3d(-armRad1,-armRad1,0.5); //v2
		glVertex3d(armRad1, -armRad1,0.5); //v3
	//End of Arm
		glVertex3d(armRad2,  armRad2, wingspan); //v4
		glVertex3d(-armRad2, armRad2, wingspan); //v5
		glVertex3d(-armRad2,-armRad2, wingspan); //v6
		glVertex3d(armRad2, -armRad2, wingspan); //v7
	//Front of Arm
		glVertex3d(armRad1,  armRad1,0.5); //v0
		glVertex3d(armRad2,  armRad2, wingspan); //v4
		glVertex3d(armRad2, -armRad2, wingspan); //v7
		glVertex3d(armRad1, -armRad1,0.5); //v3
	//Back of Arm
		glVertex3d(-armRad2, armRad2, wingspan); //v5
		glVertex3d(-armRad1, armRad1,0.5); //v1
		glVertex3d(-armRad1,-armRad1,0.5); //v2
		glVertex3d(-armRad2,-armRad2, wingspan); //v6
	//Top
		glVertex3d(armRad2,  armRad2, wingspan); //v4
		glVertex3d(armRad1,  armRad1,0.5); //v0
		glVertex3d(-armRad1, armRad1,0.5); //v1
		glVertex3d(-armRad2, armRad2, wingspan); //v5
	//Bottom
		glVertex3d(-armRad1,-armRad1,0.5); //v2
		glVertex3d(armRad1, -armRad1,0.5); //v3
		glVertex3d(armRad2, -armRad2, wingspan); //v7
		glVertex3d(-armRad2,-armRad2, wingspan); //v6
		
	//Right Arm
	//Base of arm
		glVertex3d(armRad1,  armRad1,-0.5); //v0
		glVertex3d(-armRad1, armRad1,-0.5); //v1
		glVertex3d(-armRad1,-armRad1,-0.5); //v2
		glVertex3d(armRad1, -armRad1,-0.5); //v3
	//End of Arm
		glVertex3d(armRad2,  armRad2, -wingspan); //v4
		glVertex3d(-armRad2, armRad2, -wingspan); //v5
		glVertex3d(-armRad2,-armRad2, -wingspan); //v6
		glVertex3d(armRad2, -armRad2, -wingspan); //v7
	//Front of Arm
		glVertex3d(armRad1,  armRad1,-0.5); //v0
		glVertex3d(armRad2,  armRad2, -wingspan); //v4
		glVertex3d(armRad2, -armRad2, -wingspan); //v7
		glVertex3d(armRad1, -armRad1,-0.5); //v3
	//Back of Arm
		glVertex3d(-armRad2, armRad2, -wingspan); //v5
		glVertex3d(-armRad1, armRad1,-0.5); //v1
		glVertex3d(-armRad1,-armRad1,-0.5); //v2
		glVertex3d(-armRad2,-armRad2, -wingspan); //v6
	//Top
		glVertex3d(armRad2,  armRad2, -wingspan); //v4
		glVertex3d(armRad1,  armRad1,-0.5); //v0
		glVertex3d(-armRad1, armRad1,-0.5); //v1
		glVertex3d(-armRad2, armRad2, -wingspan); //v5
	//Bottom
		glVertex3d(-armRad1,-armRad1,-0.5); //v2
		glVertex3d(armRad1, -armRad1,-0.5); //v3
		glVertex3d(armRad2, -armRad2, -wingspan); //v7
		glVertex3d(-armRad2,-armRad2, -wingspan); //v6
	glEnd();
	
	//Wings
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1, 0, 0);
		glVertex3d(0, 0, wingspan+0.0001); //v0
		glVertex3d(wingRad, 0, wingspan+0.0001); //v1
		glVertex3d(wingRad*0.5, wingRad, wingspan+0.0001); //v2
		glVertex3d(-wingRad*0.5, wingRad, wingspan+0.0001); //v3
		glVertex3d(-wingRad, 0, wingspan+0.0001); //v4
		glVertex3d(-wingRad*0.5, -wingRad, wingspan+0.0001); //v5
		glVertex3d(wingRad*0.5, -wingRad, wingspan+0.0001); //v6
		glVertex3d(wingRad, 0, wingspan+0.0001); //v1
	glEnd();
	
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1, 1, 0);
		glVertex3d(0, 0, -wingspan-0.0001); //v0
		glVertex3d(wingRad, 0, -wingspan-0.0001); //v1
		glVertex3d(wingRad*0.5, wingRad, -wingspan-0.0001); //v2
		glVertex3d(-wingRad*0.5, wingRad, -wingspan-0.0001); //v3
		glVertex3d(-wingRad, 0, -wingspan-0.0001); //v4
		glVertex3d(-wingRad*0.5, -wingRad, -wingspan-0.0001); //v5
		glVertex3d(wingRad*0.5, -wingRad, -wingspan-0.0001); //v6
		glVertex3d(wingRad, 0, -wingspan-0.0001); //v1
	glEnd();
	
	//  Undo transformations
	glPopMatrix();
}
static void xWing(	double x, double y, double z, 
					double dx, double dy, double dz,
					double ux, double uy, double uz, double scale	)
{
	//Dimensions used to size the Xwing
	
	const double wid_base = 0.35;
	const double wid_tip = 0.20;
	const double len_tip = 0.70;
	const double nose = +3.30;
	const double wingspan = 3.50;
	
	//  Rotation Matrix Setup from ex8.c
	//  Unit vector in direction of flght
	double D0 = sqrt(dx*dx+dy*dy+dz*dz);
	double X0 = dx/D0;
	double Y0 = dy/D0;
	double Z0 = dz/D0;
	//  Unit vector in "up" direction
	double D1 = sqrt(ux*ux+uy*uy+uz*uz);
	double X1 = ux/D1;
	double Y1 = uy/D1;
	double Z1 = uz/D1;
	//  Cross product gives the third vector
	double X2 = Y0*Z1-Y1*Z0;
	double Y2 = Z0*X1-Z1*X0;
	double Z2 = X0*Y1-X1*Y0;
	//  Rotation matrix
	double mat[16];
	mat[0] = X0;   mat[4] = X1;   mat[ 8] = X2;   mat[12] = 0;
	mat[1] = Y0;   mat[5] = Y1;   mat[ 9] = Y2;   mat[13] = 0;
	mat[2] = Z0;   mat[6] = Z1;   mat[10] = Z2;   mat[14] = 0;
	mat[3] =  0;   mat[7] =  0;   mat[11] =  0;   mat[15] = 1;
	//  Save current transforms
	glPushMatrix();
	//  Offset, scale and rotate
	glTranslated(x,y,z);
	glScaled(scale,scale,scale);
	glMultMatrixd(mat);

	
	// Draw Nose
	glColor3f(0.745,0.745,0.745);
	glBegin(GL_QUADS);
	
	//Top
	glVertex3d((len_tip + nose), 0.0, -wid_tip); 			//v0
	glVertex3d((len_tip + nose), 0.0, wid_tip);				//v1
	glVertex3d(nose, (wid_tip + 0.08), (wid_tip+ 0.08));	//v2
	glVertex3d(nose, (wid_tip + 0.08), -(wid_tip+ 0.08)); 	//v3
	
	//Bottom
	glVertex3d((len_tip + nose), 0.0, wid_tip);				//v1
	glVertex3d((len_tip + nose), 0.0, -wid_tip); 			//v0
	glVertex3d(nose, (-wid_tip-0.08), -(wid_tip+ 0.08));	//v5
	glVertex3d(nose, (-wid_tip-0.08), (wid_tip+ 0.08));		//v4

	//Right
	glVertex3d((len_tip + nose), 0.0, wid_tip);				//v1
	glVertex3d((len_tip + nose), 0.0, wid_tip);				//v1
	glVertex3d(nose, (-wid_tip-0.08), (wid_tip+ 0.08));		//v4
	glVertex3d(nose, (wid_tip + 0.08), (wid_tip+ 0.08));	//v2
	
	//Left
	glVertex3d((len_tip + nose), 0.0, -wid_tip); 			//v0
	glVertex3d((len_tip + nose), 0.0, -wid_tip); 			//v0
	glVertex3d(nose, (wid_tip + 0.08), -(wid_tip+ 0.08)); 	//v3
	glVertex3d(nose, (-wid_tip-0.08), -(wid_tip+ 0.08));	//v5

	//Back
	glVertex3d(nose, (wid_tip + 0.08), (wid_tip+ 0.08));	//v2
	glVertex3d(nose, (wid_tip + 0.08), -(wid_tip+ 0.08)); 	//v3
	glVertex3d(nose, (-wid_tip-0.08), (wid_tip+ 0.08));		//v4
	glVertex3d(nose, (-wid_tip-0.08), -(wid_tip+ 0.08));	//v5
	glEnd();
	
	// Draw Fusolage
	glColor3f(0.70,0.70,0.70);
	glBegin(GL_QUADS);
	
	//Top
	glVertex3d(nose, wid_tip, wid_tip); 	//v0
	glVertex3d(nose, wid_tip, -wid_tip);	//v1
	glVertex3d(0, wid_base, -wid_base);		//v2
	glVertex3d(0, wid_base, wid_base); 		//v3
	
	//Bottom
	glVertex3d(0, -wid_base, -wid_base);	//v4
	glVertex3d(0, -wid_base, wid_base); 	//v5
	glVertex3d(nose, -wid_tip, wid_tip);	//v6
	glVertex3d(nose, -wid_tip, -wid_tip);	//v7
	
	//Right
	glVertex3d(nose, wid_tip, -wid_tip);	//v1
	glVertex3d(nose, -wid_tip, -wid_tip);	//v7
	glVertex3d(0, -wid_base, -wid_base);	//v4
	glVertex3d(0, wid_base, -wid_base);		//v2

	//Left
	glVertex3d(0, wid_base, wid_base); 		//v3
	glVertex3d(0, -wid_base, wid_base); 	//v5
	glVertex3d(nose, -wid_tip, wid_tip);	//v6
	glVertex3d(nose, wid_tip, wid_tip); 	//v0


	//Back
	glVertex3d(0, wid_base, wid_base); 		//v3
	glVertex3d(0, wid_base, -wid_base);		//v2
	glVertex3d(0, -wid_base, -wid_base);	//v4
	glVertex3d(0, -wid_base, wid_base); 	//v5
	
	//Front
	glVertex3d(nose, wid_tip, -wid_tip);	//v1
	glVertex3d(nose, wid_tip, wid_tip); 	//v0
	glVertex3d(nose, -wid_tip, wid_tip);	//v6
	glVertex3d(nose, -wid_tip, -wid_tip);	//v7
	glEnd();
	
	//Cockpit base
	cube(-1, 0.05, 0, 1, 0.4, 0.4,0);
	
	//Cockpit
	glColor3f(0,0,1);
	glBegin(GL_QUADS);
	
	//Top
	glVertex3d(0, 0.5, -wid_base); 			//v0
	glVertex3d(0, 0.5, wid_base);			//v1
	glVertex3d(2.5, 0, 0.225);					//v2
	glVertex3d(2.5, 0, -0.225);				//v3
	
	//Bottom
	glVertex3d(0, 0, wid_base);		//V4			
	glVertex3d(0, 0, -wid_base); 	//V5	
	glVertex3d(2.5, 0, -0.225);		//V3
	glVertex3d(2.5, 0, 0.225);			//V2

	//Right
	glVertex3d(0, 0.5, -wid_base); 			//v0
	glVertex3d(2.5, 0, -0.225);				//v3
	glVertex3d(0, 0, -wid_base); 			//V5	
	glVertex3d(0, 0.5, -wid_base); 			//v0
	
	//Left
	glVertex3d(0, 0.5, wid_base);			//v1
	glVertex3d(0, 0, wid_base);				//V4
	glVertex3d(2.5, 0, 0.225);					//v2
	glVertex3d(0, 0.5, wid_base);			//v1
	
	//Back
	glVertex3d(0, 0.5, wid_base);			//v1
	glVertex3d(0, 0.5, -wid_base); 			//v0
	glVertex3d(0, 0, -wid_base); 			//V5
	glVertex3d(0, 0, wid_base);				//V4
	glEnd();
	
	//Wings in attack position
	if(x_foils == 1){
		glBegin(GL_QUADS);
		glColor3f(0.8,0,0);
		
		//top left wing (from front)
		glVertex3d(-0.20, 0.2, 0.35); 	//v0
		glVertex3d(-1.80, 0.2, 0.35);		//v1
		glVertex3d(-1.50, 0.80, wingspan);		//v2
		glVertex3d(-0.50, 0.80, wingspan);		//v3
		
		glColor3f(0,0.8,0);
		//bottom left wing (from front)
		glVertex3d(-0.20, -0.2, 0.35); 	//v0
		glVertex3d(-1.80, -0.2, 0.35);		//v1
		glVertex3d(-1.50, -0.80, wingspan);		//v2
		glVertex3d(-0.50, -0.80, wingspan);		//v3
		
		glColor3f(0,0,0.8);
		//bottom right wing (from front)
		glVertex3d(-0.20, -0.2, -0.35); 	//v0
		glVertex3d(-1.80, -0.2, -0.35);		//v1
		glVertex3d(-1.50, -0.80, -wingspan);		//v2
		glVertex3d(-0.50, -0.80, -wingspan);		//v3
		
		glColor3f(0.8,0.8,0);
		//top right wing (from front)
		glVertex3d(-0.20, 0.2, -0.35); 	//v0
		glVertex3d(-1.80, 0.2, -0.35);		//v1
		glVertex3d(-1.50, 0.80, -wingspan);		//v2
		glVertex3d(-0.50, 0.80, -wingspan);		//v3
		
		
		glEnd();
	}else{
		glBegin(GL_QUADS);	
				glColor3f(0.8,0,0);
		
		//top left wing (from front)
		glVertex3d(-0.20, 0.20, 0.35); 	//v0
		glVertex3d(-1.80, 0.20, 0.35);		//v1
		glVertex3d(-1.50, 0.20, wingspan);		//v2
		glVertex3d(-0.50, 0.20, wingspan);		//v3
		
		glColor3f(0,0.8,0);
		//bottom left wing (from front)
		glVertex3d(-0.20, -0.20, 0.35); 	//v0
		glVertex3d(-1.80, -0.20, 0.35);		//v1
		glVertex3d(-1.50, -0.20, wingspan);		//v2
		glVertex3d(-0.50, -0.20, wingspan);		//v3
		
		glColor3f(0,0,0.8);
		//bottom right wing (from front)
		glVertex3d(-0.20, -0.20, -0.35); 	//v0
		glVertex3d(-1.80, -0.20, -0.35);		//v1
		glVertex3d(-1.50, -0.20, -wingspan);		//v2
		glVertex3d(-0.50, -0.20, -wingspan);		//v3
		
		glColor3f(0.8,0.8,0);
		//top right wing (from front)
		glVertex3d(-0.20, 0.20, -0.35); 	//v0
		glVertex3d(-1.80, 0.20, -0.35);		//v1
		glVertex3d(-1.50, 0.20, -wingspan);		//v2
		glVertex3d(-0.50, 0.20, -wingspan);		//v3
		glEnd();	
	}
	//  Undo transformations
	glPopMatrix();
	
}
/*
 * Display the scene
 */
void display()
{
	const double len =  5;
	//  Erase the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//  Enable Z-buffering in OpenGL
	glEnable(GL_DEPTH_TEST);
	//  Undo previous transformations
	glLoadIdentity();
	//  Perspective - set eye position
	if (mode){
		double Ex = -2*dim*Sin(th)*Cos(ph);
		double Ey = +2*dim        *Sin(ph);
		double Ez = +2*dim*Cos(th)*Cos(ph);
		gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
	}else{
		glRotatef(ph,1,0,0);
		glRotatef(th,0,1,0);
	}
	
	xWing(-8,0,0, 1,0,0, 0,1,1, 1);
	tieFighter(8,0,0, 1,0.5,0, 0, 0,1, 1);
	xWing(4,-10,-15, 0.1,0.1,0.1, 0,1,-1, 1);	
	tieFighter(-27,+5,-4, 1,0,0, (20/35), -0.25, 0.1, 1);
	
	asteroidField(-15, 12, 8, 1,1,1, 45);
	asteroidField(12, -4, 0, 0.6,0.6,0.6, 20);
	asteroidField(-10, -12, -13, 1,1,1, -30);
	asteroidField(18, -14, 15, 2,2,2, 120);
	//  White
	glColor3f(1,1,1);
	//  Draw axes
	if (axes == 1)
	{
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
	//  Print program info/directions
	if(info == 1){
		glWindowPos2i(10,45);
			Print("Angle(arrows):%d,%d || Mode(m): %s || X-Foils(x): %s", 
				th,ph, mode?"Perspective":"Ortho", x_foils?"Attack Pos":"Closed");
			glWindowPos2i(10,25);
			Print("Dim(+|-):%d || FOV(pg-u|pg-d): %d || Axes(~): %s", 
				dim, fov, axes?"On":"Off");
	}else{
		glWindowPos2i(5,5);
		Print("Press 'i' to toggle program info/control display");
	}
	//  Render
	glFlush();
	//  Make the rendered scene visible
	glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 3;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 3;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 3;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 3;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
	//  Exit on ESC
	if (ch == 27)
		exit(0);
	//  Reset view angle
	else if (ch == '0'){
		th = ph = 0;
		dim = 30.0;
		fov = 50.0;
	}
	//  Switch X-foils into and out of attack position
	else if (ch == 'x' || ch == 'X')
		x_foils = 1-x_foils;
	// Toggle Axes
	else if (ch == 'a' || ch == 'A')
		axes = 1-axes;
	//  Switch display mode
	else if (ch == 'm' || ch == 'M')
		mode = 1-mode;
	//  Change field of view angle
	else if (ch == '-' && ch>1)
		fov--;
	else if (ch == '+' && ch<179)
		fov++;
	else if (ch=='i' || ch=='I')
		info = 1-info;
	//  Reproject
	Project();
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}
/*
 *  GLUT calls this toutine when there is nothing else to do
 */
void idle()
{
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360);
   glutPostRedisplay();
}


void reshape(int width,int height)
{
	//  Ratio of the width to the height of the window
	asp = (height>0) ? (double)width/height : 1;
	//  Set the viewport to the entire window
	glViewport(0,0, width,height);
	//  Set projection
	Project();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
	//  Initialize GLUT
	glutInit(&argc,argv);
	//  Request double buffered, true color window with Z buffering at 600x600
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(600,600);
	glutCreateWindow("HW 4 - Chris Fichman - X-Wing vs. TIE Fighter");
	//  Tell GLUT to call "idle" when there is nothing else to do
   glutIdleFunc(idle);
	//  Set callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(key);
	//  Pass control to GLUT so it can interact with the user
	glutMainLoop();
	return 0;
}
