// Author: Chris Fichman
// Assignment 3: CSCI 4229
// Draw an X-Wing & TIE Fighter
// This code uses excerpts/some shapes from ex8 in order to build parts 
// of the x-wing

#include "CSCIx229.h"

int axes=0;       //  Display axes
int mode =1;		//Display mode
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
double fov=50.0;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=30.0;   //  Size of world
int x_foils = 1;	// 	X-Foil attack mode
int len = 5;
int info = 0;		//Display info

//Lighting Variables
int light=1; 
int move 	  =   0;
int one       =   1;  // Unit value
int distance  =  40;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light 

//Asteroids
int N = 20;
int spread = 40;
typedef struct
{
	double r;
	double x;
	double y;
	double z;
	int t;
} Asteroid;

Asteroid field[20];

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

double randRange(double min, double max) {
   return rand() * (max - min) / RAND_MAX + min;
}

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
static void lightBall(double x,double y,double z,double r, double rd, double gr, double bl)
{
   const int d=5;
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
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

static void sphere(int tex_name)
{
   int th,ph;
   /*
    *  Draw surface of the sphere
    */
   //  Set texture
   
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, tex_name);
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

static void asteroidField(int n, int spread)
{
	int k;
	float yellow[] = {1.0,1.0,0.0,1.0};
	float Emission[]  = {0.0,0.0,0.01*emission,1.0};


	glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
    glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
	
	for(k = 0; k < n; k++){
		//  Save transformation	
		glPushMatrix();	
		glTranslated(field[k].x, field[k].y, field[k].z);
		glScaled(field[k].r, field[k].r, field[k].r);
		
		if (k>0 && light) glEnable(GL_LIGHTING); //  Lighting for planets

		sphere(field[k].t);
		
		glPopMatrix();
	}
}

static void tieWings(int r, int b, int g, double wingRad, 
					 double wingspan, double offset)
{
	int tieWing = LoadTexBMP("TIEWing.bmp");

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tieWing);

	
	//Wing(outside)
	glNormal3f(0, 0, wingspan);
	glBegin(GL_TRIANGLES);
	glColor3f(r, b, g);	
		glTexCoord2f(wingRad,0);	
		glVertex3d(wingRad, 0, wingspan+offset); //v1
		glTexCoord2f(wingRad*0.5, wingRad);
		glVertex3d(wingRad*0.5, wingRad, wingspan+offset); //v2
		glVertex3d(0, 0, wingspan+offset); //v0
		
		glVertex3d(0, 0, wingspan+offset); //v0
		glTexCoord2f(wingRad*0.5, wingRad);
		glVertex3d(wingRad*0.5, wingRad, wingspan+offset); //v2
		glTexCoord2f(-wingRad*0.5, wingRad);		
		glVertex3d(-wingRad*0.5, wingRad, wingspan+offset); //v3
		
		glVertex3d(0, 0, wingspan+offset); //v0
		glTexCoord2f(-wingRad*0.5, wingRad);		
		glVertex3d(-wingRad*0.5, wingRad, wingspan+offset); //v3
		glTexCoord2f(-wingRad, 0);		
		glVertex3d(-wingRad, 0, wingspan+offset); //v4
	
		glVertex3d(0, 0, wingspan+offset); //v0		
		glTexCoord2f(-wingRad, 0);		
		glVertex3d(-wingRad, 0, wingspan+offset); //v4
		glTexCoord2f(-wingRad*0.5, -wingRad);			
		glVertex3d(-wingRad*0.5, -wingRad, wingspan+offset); //v5
	
		glVertex3d(0, 0, wingspan+offset); //v0
		glTexCoord2f(-wingRad*0.5, -wingRad);					
		glVertex3d(-wingRad*0.5, -wingRad, wingspan+offset); //v5
		glTexCoord2f(wingRad*0.5, -wingRad);					
		glVertex3d(wingRad*0.5, -wingRad, wingspan+offset); //v6
		
		glVertex3d(0, 0, wingspan+offset); //v0
		glTexCoord2f(wingRad*0.5, -wingRad);					
		glVertex3d(wingRad*0.5, -wingRad, wingspan+offset); //v6
		glTexCoord2f(wingRad, 0);					
		glVertex3d(wingRad, 0, wingspan+offset); //v1		
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tieWing);
	
	//Wing(Inside)
	glNormal3f(0, 0, -wingspan);
	glBegin(GL_TRIANGLES);
	glColor3f(r, b, g);
		glVertex3d(0, 0, wingspan); //v0
		glTexCoord2f(wingRad*0.5, -wingRad);					
		glVertex3d(wingRad*0.5, -wingRad, wingspan); //v6
		glTexCoord2f(-wingRad*0.5, -wingRad);					
		glVertex3d(-wingRad*0.5, -wingRad, wingspan); //v5

		glVertex3d(0, 0, wingspan); //v0		
		glTexCoord2f(-wingRad*0.5, -wingRad);							
		glVertex3d(-wingRad*0.5, -wingRad, wingspan); //v5
		glTexCoord2f(-wingRad, 0);		
		glVertex3d(-wingRad, 0, wingspan); //v4
		
		glVertex3d(0, 0, wingspan); //v0
		glTexCoord2f(-wingRad, 0);		
		glVertex3d(-wingRad, 0, wingspan); //v4
		glTexCoord2f(-wingRad*0.5, wingRad);		
		glVertex3d(-wingRad*0.5, wingRad, wingspan); //v3

		glVertex3d(0, 0, wingspan); //v0
		glTexCoord2f(-wingRad*0.5, wingRad);		
		glVertex3d(-wingRad*0.5, wingRad, wingspan); //v3
		glTexCoord2f(wingRad*0.5, wingRad);
		glVertex3d(wingRad*0.5, wingRad, wingspan); //v2

		glVertex3d(0, 0, wingspan); //v0
		glTexCoord2f(wingRad*0.5, wingRad);
		glVertex3d(wingRad*0.5, wingRad, wingspan); //v2
		glTexCoord2f(wingRad, 0);							
		glVertex3d(wingRad, 0, wingspan); //v1

		glVertex3d(0, 0, wingspan); //v0
		glTexCoord2f(wingRad, 0);					
		glVertex3d(wingRad, 0, wingspan); //v1	
		glTexCoord2f(wingRad*0.5, -wingRad);					
		glVertex3d(wingRad*0.5, -wingRad, wingspan); //v6
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	
	//Connect parts
	glBegin(GL_QUADS);
	glColor3f(r, b, g);
			glNormal3f(0.50, 0.25, 0);
			glVertex3d(wingRad, 0, wingspan+offset); //v1_O	
			glVertex3d(wingRad, 0, wingspan); //v1_i
			glVertex3d(wingRad*0.5, wingRad, wingspan); //v2_i
			glVertex3d(wingRad*0.5, wingRad, wingspan+offset); //v2_O
			
			glNormal3f(0, 1, 0);
			glVertex3d(wingRad*0.5, wingRad, wingspan+offset); //v2_O
			glVertex3d(wingRad*0.5, wingRad, wingspan); //v2_i
			glVertex3d(-wingRad*0.5, wingRad, wingspan); //v3_i
			glVertex3d(-wingRad*0.5, wingRad, wingspan+offset); //v3_o
					
			glNormal3f(-0.50, 0.25, 0);	
			glVertex3d(-wingRad*0.5, wingRad, wingspan+offset); //v3_o
			glVertex3d(-wingRad*0.5, wingRad, wingspan); //v3_i		
			glVertex3d(-wingRad, 0, wingspan); //v4_i
			glVertex3d(-wingRad, 0, wingspan+offset); //v4_o

			glNormal3f(-0.50, -0.25, 0);				
			glVertex3d(-wingRad, 0, wingspan+offset); //v4_o
			glVertex3d(-wingRad, 0, wingspan); //v4_i
			glVertex3d(-wingRad*0.5, -wingRad, wingspan); //v5_i
			glVertex3d(-wingRad*0.5, -wingRad, wingspan+offset); //v5_o
			
			glNormal3f(0, -1, 0);				
			glVertex3d(-wingRad*0.5, -wingRad, wingspan+offset); //v5_o
			glVertex3d(-wingRad*0.5, -wingRad, wingspan); //v5_i
			glVertex3d(wingRad*0.5, -wingRad, wingspan); //v6_i
			glVertex3d(wingRad*0.5, -wingRad, wingspan+offset); //v6_o		

			glNormal3f(0.50, -0.25, 0);				
			glVertex3d(wingRad*0.5, -wingRad, wingspan+offset); //v6_o		
			glVertex3d(wingRad*0.5, -wingRad, wingspan); //v6_i
			glVertex3d(wingRad, 0, wingspan); //v1_i
			glVertex3d(wingRad, 0, wingspan+offset); //v1_O	
	glEnd();
	
	//  Undo transformations
	//glPopMatrix();
}
static void tieFighter(	double x, double y, double z, 
					double dx, double dy, double dz,
					double ux, double uy, double uz, double scale)
{
	float yellow[] = {1.0,1.0,0.0,1.0};
	float Emission[]  = {0.0,0.0,0.01*emission,1.0};
	int tieBody = LoadTexBMP("TIETex.bmp");
	
	glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
	glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
	glMaterialfv(GL_FRONT,GL_EMISSION,Emission);

	const double wingRad = 3.0;
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

	//Wings
	//Right(red)
	tieWings(1,1,1, wingRad, wingspan, 0.1);
	//Left(yellow)
	tieWings(1,1,1, wingRad, -wingspan, -0.1);
	
	//FIX THIS
	//Cockpit
	glScaled(0.9, 0.9, 0.9);
	sphere(tieBody);

	
	//Arms to wings
	glBegin(GL_QUADS);
	glColor3f(0.5,0.5,0.5);
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
		glNormal3f(2.0, 0, 0.2);				
		glVertex3d(armRad1,  armRad1,0.5); //v0
		glVertex3d(armRad2,  armRad2, wingspan); //v4
		glVertex3d(armRad2, -armRad2, wingspan); //v7
		glVertex3d(armRad1, -armRad1,0.5); //v3
	//Back of Arm
		glNormal3f(-2.0, 0, 0.2);				
		glVertex3d(-armRad2, armRad2, wingspan); //v5
		glVertex3d(-armRad1, armRad1,0.5); //v1
		glVertex3d(-armRad1,-armRad1,0.5); //v2
		glVertex3d(-armRad2,-armRad2, wingspan); //v6
	//Top
		glNormal3f(0, 2.0, 0.2);
		glVertex3d(armRad2,  armRad2, wingspan); //v4
		glVertex3d(armRad1,  armRad1,0.5); //v0
		glVertex3d(-armRad1, armRad1,0.5); //v1
		glVertex3d(-armRad2, armRad2, wingspan); //v5
	//Bottom
		glNormal3f(0, -2.0, 0.2);
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
		glNormal3f(2.0, 0, -0.2);				
		glVertex3d(armRad1,  armRad1,-0.5); //v0
		glVertex3d(armRad2,  armRad2, -wingspan); //v4
		glVertex3d(armRad2, -armRad2, -wingspan); //v7
		glVertex3d(armRad1, -armRad1,-0.5); //v3
	//Back of Arm
		glNormal3f(-2.0, 0, -0.2);				
		glVertex3d(-armRad2, armRad2, -wingspan); //v5
		glVertex3d(-armRad1, armRad1,-0.5); //v1
		glVertex3d(-armRad1,-armRad1,-0.5); //v2
		glVertex3d(-armRad2,-armRad2, -wingspan); //v6
	//Top
		glNormal3f(0, 2.0, -0.2);
		glVertex3d(armRad2,  armRad2, -wingspan); //v4
		glVertex3d(armRad1,  armRad1,-0.5); //v0
		glVertex3d(-armRad1, armRad1,-0.5); //v1
		glVertex3d(-armRad2, armRad2, -wingspan); //v5
	//Bottom
		glNormal3f(0, -2.0, -0.2);
		glVertex3d(-armRad1,-armRad1,-0.5); //v2
		glVertex3d(armRad1, -armRad1,-0.5); //v3
		glVertex3d(armRad2, -armRad2, -wingspan); //v7
		glVertex3d(-armRad2,-armRad2, -wingspan); //v6
	glEnd();
	


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
	
	// Light Switch!
	if (light)
	{
		 //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
        float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        lightBall(distance*Cos(zh),ylight,distance*Sin(zh),1.0, 1,1,1);
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
	else
		glDisable(GL_LIGHTING);
     
	//xWing(-8,0,0, 1,0,0, 0,1,1, 1);
	tieFighter(8,0,0, 1,0.5,0, 0, 0,1, 1);
	//xWing(4,-10,-15, 0.1,0.1,0.1, 0,1,-1, 1);	
	tieFighter(-27,+5,-4, 1,0,0, (20/35), -0.25, 0.1, 1);
	
	asteroidField(N, spread);
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
	
	//  Print program info
	if(info == 1){
		glWindowPos2i(10,65);
			Print("Angle(arrows), Projection(p), X-Foils(~), Reset(0)");
			glWindowPos2i(10,45);
			Print("Dim(pg_dwn,pg_up), FOV(-,+), Axes(x,X), Toggle Light(l)");
			glWindowPos2i(10,25);
			Print("Move Light(m), Up,Down(],[), CW,CCW(<,>), Emission(e,E)");
			glWindowPos2i(10,5);
			Print("Specularity(s,S), Diffuse(d,D), Ambient(a,A), Shininess(n,N)");
	}else{
		glWindowPos2i(5,5);
		Print("Press 'i' to toggle program info/control display");
	}

	//  Debug info
	ErrCheck("display");
	//  Render
	glFlush();
	//  Make the rendered scene visible
	glutSwapBuffers();
}

/*
 *  GLUT calls this routine when there is nothing else to do
 */
void idle()
{
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360);
   glutPostRedisplay();
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
      dim += 1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 1;
   //  Toggle ball increment
   else if (key == GLUT_KEY_F8)
      inc = (inc==10)?3:10;

   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(mode?fov:0,asp,dim);
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
	else if (ch == '0')
	  th = ph = 0;
	//  Toggle x-foils
	else if (ch == '~')
		x_foils = 1-x_foils;
	//  Toggle axes
	else if (ch == 'x' || ch == 'X')
	  axes = 1-axes;
	//  Toggle lighting
	else if (ch == 'l' || ch == 'L')
	  light = 1-light;
	//  Switch projection mode
	else if (ch == 'p' || ch == 'P')
	  mode = 1-mode;
	//  Toggle light movement
	else if (ch == 'm' || ch == 'M')
	  move = 1-move;
	//  Move light
	else if (ch == '<')
	  zh += 3;
	else if (ch == '>')
	  zh -= 3;
	//  Change field of view angle
	else if (ch == '-' && ch>1)
	  fov--;
	else if (ch == '+' && ch<179)
	  fov++;
	//  Light elevation
	else if (ch=='[')
	  ylight -= 1;
	else if (ch==']')
	  ylight += 1;
	//  Ambient level
	else if (ch=='a' && ambient>0)
	  ambient -= 5;
	else if (ch=='A' && ambient<100)
	  ambient += 5;
	//  Diffuse level
	else if (ch=='d' && diffuse>0)
	  diffuse -= 5;
	else if (ch=='D' && diffuse<100)
	  diffuse += 5;
	//  Specular level
	else if (ch=='s' && specular>0)
	  specular -= 5;
	else if (ch=='S' && specular<100)
	  specular += 5;
	//  Emission level
	else if (ch=='e' && emission>0)
	  emission -= 5;
	else if (ch=='E' && emission<100)
	  emission += 5;
	//  Shininess level
	else if (ch=='n' && shininess>-1)
	  shininess -= 1;
	else if (ch=='N' && shininess<7)
	  shininess += 1;
	// Toggle info pane
	else if (ch=='i' || ch=='I')
		info = 1-info;
	
	shinyvec[0] = shininess <0 ? 0 : pow(2.0, shininess);
	//  Reproject
	Project(mode?fov:0,asp,dim);
	//  Animate - only for light, not used for Xwing (yet)
	glutIdleFunc(move?idle:NULL);
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

void reshape(int width,int height)
{
	//  Ratio of the width to the height of the window
	asp = (height>0) ? (double)width/height : 1;
	//  Set the viewport to the entire window
	glViewport(0,0, width,height);
	//  Set projection
	Project(mode?fov:0, asp, dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
 
int main(int argc,char* argv[])
{
	//  Set up asteroid array
	int i;
	//  Initialize GLUT
	glutInit(&argc,argv);
	//  Request double buffered, true color window with Z buffering at 600x600
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(600,600);
	glutCreateWindow("HW 5 - Chris Fichman - X-Wing vs. TIE Fighter");
	for(i = 0 ; i < N; i++){
		double r = randRange(0, 3);
		double x = randRange(-spread, spread);
		double y = randRange(-spread, spread);
		double z = randRange(-spread, spread);
		field[i].r = r;
		field[i].x = x;
		field[i].y = y;
		field[i].z = z;
		field[i].t = LoadTexBMP("asteroid.bmp");
	}
	//  Set callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(key);
	//  Tell GLUT to call "idle" when there is nothing else to do
	glutIdleFunc(idle);
	//  Pass control to GLUT so it can interact with the user
	ErrCheck("init");
	glutMainLoop();
	return 0;
}
