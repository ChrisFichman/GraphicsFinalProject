// Author: Chris Fichman
// Final Project: CSCI 4229
// Draw an X-Wing & TIE Fighter

//~ 1) Fix textures for TIE, add textures for X-Wing (Done)
//~ 2) Add background (stars, nearby sun, & planet/moons) (Done)
//~ 
//~ FOR TG CHECK-IN:
//~ 
//~ 3) Motion for spacecrafts
//~ 4) Change Lighting according to the background(where the sun is ect)
//~ 5) Add Lasers (and corresponding lighting)
//~ 6) Make the asteroids move about the scene according to randomly 
//~ generated initial vectors

//~ AFTER TG BREAK:
//~ 7) Collisions
//~ 8) First person/third person camera, with a cockpit in the foreground.
//~ 9) data structure(may reuse the one that I'll need for collisions) 
//~ that keeps track of all of the ships on their paths, which will allow 
//~ the user to swap between crafts
//~ 10) If I have time, allow user control of a ship.
#include "CSCIx229.h"
#include "textures.h"
#include "util.h"
#include "asteroids.h"
#include "wings.h"

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
int move 	  =   1;
int one       =   1;  // Unit value
int distance  =  40;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light 

AField field;
Textures textures;

static void asteroidField(int n, int spread)
{
	int k;
	
	for(k = 0; k < n; k++){
		//  Save transformation	
		glPushMatrix();	
		glTranslated(field.asteroids[k].x, field.asteroids[k].y, field.asteroids[k].z);
		glScaled(field.asteroids[k].r, field.asteroids[k].r, field.asteroids[k].r);
		
		if (k>0 && light) glEnable(GL_LIGHTING); //  Lighting for planets

		sphere(textures.asteroid);
		
		glPopMatrix();
	}
}
static void xWingFoils(int x_foils, double wingspan, int wing){	
	double x_y;
	double x_z;
	double x_offset;
	
	if(wing == 0){ //Top Left from front
		x_y = 1.0;
		x_z = 1.0;
		x_offset = 0.1;
	}else if(wing == 1){ //Bottom Left from front
		x_y = -1.0;
		x_z = 1.0;
		x_offset = -0.1;
	}else if(wing == 2){ //Bottom Right from front
		x_y = -1.0;
		x_z = -1.0;
		x_offset = -0.1;
	}else{ //Top Right!
		x_y = 1.0;
		x_z = -1.0;
		x_offset = 0.1;
	}
	
	if(x_foils == 1){
		glBegin(GL_QUADS);
		glColor3f(0.8,0,0);			
			//bottom part of wing (from front)
			glNormal3f(0,-x_y*1.67,-x_z*0.32);
			glVertex3d(-0.20, x_y*0.2, x_z*0.35); 	//v0
			glVertex3d(-1.80, x_y*0.2, x_z*0.35);		//v1
			glVertex3d(-1.50, x_y*0.80, x_z*wingspan);		//v2
			glVertex3d(-0.50, x_y*0.80, x_z*wingspan);		//v3
			
			//top part of wing (from front)
			glNormal3f(0,x_y*1.67,x_z*0.32);
			glVertex3d(-0.20, x_y*0.2+x_offset, x_z*0.35); 	//v0
			glVertex3d(-1.80, x_y*0.2+x_offset, x_z*0.35);		//v1
			glVertex3d(-1.50, x_y*0.80+x_offset, x_z*wingspan);		//v2
			glVertex3d(-0.50, x_y*0.80+x_offset, x_z*wingspan);		//v3
			
			//connect parts
			glColor3f(0,0,0.8);
			glNormal3f(0,x_y*-0.6,x_z*-3.15);
			glVertex3d(-0.20, x_y*0.2+x_offset, x_z*0.35); 	//v0
			glVertex3d(-0.20, x_y*0.2, x_z*0.35); 	//v0
			glVertex3d(-1.80, x_y*0.2, x_z*0.35);		//v1
			glVertex3d(-1.80, x_y*0.2+x_offset, x_z*0.35);		//v1
			
			glNormal3f(-3.3,x_y*1.67,x_z*0.32);
			glVertex3d(-1.80, x_y*0.2+x_offset, x_z*0.35);		//v1
			glVertex3d(-1.80, x_y*0.2, x_z*0.35);		//v1			
			glVertex3d(-1.50, x_y*0.80, x_z*wingspan);		//v2			
			glVertex3d(-1.50, x_y*0.80+x_offset, x_z*wingspan);		//v2
			
			glNormal3f(0,x_y*0.6,x_z*3.15);	
			glVertex3d(-1.50, x_y*0.80+x_offset, x_z*wingspan);		//v2
			glVertex3d(-1.50, x_y*0.80, x_z*wingspan);		//v2
			glVertex3d(-0.50, x_y*0.80, x_z*wingspan);		//v3
			glVertex3d(-0.50, x_y*0.80+x_offset, x_z*wingspan);		//v3
			
			glNormal3f(1,0,0);
			glVertex3d(-0.50, x_y*0.80+x_offset, x_z*wingspan);		//v3			
			glVertex3d(-0.50, x_y*0.80, x_z*wingspan);		//v3			
			glVertex3d(-0.20, x_y*0.2, x_z*0.35); 	//v0	
			glVertex3d(-0.20, x_y*0.2+x_offset, x_z*0.35); 	//v0	
		glEnd();
	}else{
		glBegin(GL_QUADS);	
		glColor3f(0.8,0,0);
			//bottom part of wing (from front)
			glNormal3f(0,-x_y,0);
			glVertex3d(-0.20, x_y*0.20, x_z*0.35); 	//v0
			glVertex3d(-1.80, x_y*0.20, x_z*0.35);		//v1
			glVertex3d(-1.50, x_y*0.20, x_z*wingspan);		//v2
			glVertex3d(-0.50, x_y*0.20, x_z*wingspan);		//v3
			
			//top part of wing (from front)
			glNormal3f(0,x_y,0);
			glVertex3d(-0.20, x_y*0.20+x_offset, x_z*0.35); 	//v0
			glVertex3d(-1.80, x_y*0.20+x_offset, x_z*0.35);		//v1
			glVertex3d(-1.50, x_y*0.20+x_offset, x_z*wingspan);		//v2
			glVertex3d(-0.50, x_y*0.20+x_offset, x_z*wingspan);		//v3
			
			//Connect Parts
			glColor3f(0,0,0.8);
			glNormal3f(0,0,x_z*-3.15);
			glVertex3d(-0.20, x_y*0.20+x_offset, x_z*0.35); 	//v0
			glVertex3d(-0.20, x_y*0.20, x_z*0.35); 	//v0
			glVertex3d(-1.80, x_y*0.20, x_z*0.35);		//v1
			glVertex3d(-1.80, x_y*0.20+x_offset, x_z*0.35);		//v1
			
			glNormal3f(-3.3,0,x_z*0.32);
			glVertex3d(-1.80, x_y*0.20+x_offset, x_z*0.35);		//v1
			glVertex3d(-1.80, x_y*0.20, x_z*0.35);		//v1			
			glVertex3d(-1.50, x_y*0.20, x_z*wingspan);		//v2			
			glVertex3d(-1.50, x_y*0.20+x_offset, x_z*wingspan);		//v2
			
			glNormal3f(0,0,x_z*3.15);	
			glVertex3d(-1.50, x_y*0.20+x_offset, x_z*wingspan);		//v2
			glVertex3d(-1.50, x_y*0.20, x_z*wingspan);		//v2
			glVertex3d(-0.50, x_y*0.20, x_z*wingspan);		//v3
			glVertex3d(-0.50, x_y*0.20+x_offset, x_z*wingspan);		//v3
			
			glNormal3f(1,0,0);
			glVertex3d(-0.50, x_y*0.20+x_offset, x_z*wingspan);		//v3			
			glVertex3d(-0.50, x_y*0.20, x_z*wingspan);		//v3			
			glVertex3d(-0.20, x_y*0.20, x_z*0.35); 	//v0	
			glVertex3d(-0.20, x_y*0.20+x_offset, x_z*0.35); 	//v0	
		glEnd();	
	}
}

static void tieWings(double wingRad, 
					 double wingspan, double offset)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures.tieWing);
	
	//Wing(outside)
	glNormal3f(0, 0, wingspan);
	glBegin(GL_TRIANGLES);
		glColor3f(1,1,1);
		glTexCoord2f(1,0.5);	
		glVertex3d(wingRad, 0, wingspan+offset); //v1
		glTexCoord2f(0.75,1);
		glVertex3d(wingRad*0.5, wingRad, wingspan+offset); //v2
		glTexCoord2f(0.5,0.5);
		glVertex3d(0, 0, wingspan+offset); //v0
		
		glTexCoord2f(0.5,0.5);
		glVertex3d(0, 0, wingspan+offset); //v0
		glTexCoord2f(0.75,1);
		glVertex3d(wingRad*0.5, wingRad, wingspan+offset); //v2
		glTexCoord2f(0.25,1);
		glVertex3d(-wingRad*0.5, wingRad, wingspan+offset); //v3
		
		glTexCoord2f(0.5,0.5);
		glVertex3d(0, 0, wingspan+offset); //v0
		glTexCoord2f(0.25,1);
		glVertex3d(-wingRad*0.5, wingRad, wingspan+offset); //v3
		glTexCoord2f(0,0.5);
		glVertex3d(-wingRad, 0, wingspan+offset); //v4
	
		glTexCoord2f(0.5,0.5);
		glVertex3d(0, 0, wingspan+offset); //v0	
		glTexCoord2f(0,0.5);	
		glVertex3d(-wingRad, 0, wingspan+offset); //v4
		glTexCoord2f(0.25,0);
		glVertex3d(-wingRad*0.5, -wingRad, wingspan+offset); //v5
		
		glTexCoord2f(0.5,0.5);
		glVertex3d(0, 0, wingspan+offset); //v0
		glTexCoord2f(0.25,0);
		glVertex3d(-wingRad*0.5, -wingRad, wingspan+offset); //v5
		glTexCoord2f(0.75, 0);
		glVertex3d(wingRad*0.5, -wingRad, wingspan+offset); //v6	
			
		glTexCoord2f(0.5,0.5);
		glVertex3d(0, 0, wingspan+offset); //v0
		glTexCoord2f(0.75, 0);
		glVertex3d(wingRad*0.5, -wingRad, wingspan+offset); //v6
		glTexCoord2f(1,0.5);
		glVertex3d(wingRad, 0, wingspan+offset); //v1		
		
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures.tieWing);
	
	//Wing(Inside)
	glNormal3f(0, 0, -wingspan);
	glBegin(GL_TRIANGLES);
		glColor3f(1,1,1);
		glTexCoord2f(0.5,0.5);	
		glVertex3d(0, 0, wingspan); //v0
		glTexCoord2f(0.75, 0);			
		glVertex3d(wingRad*0.5, -wingRad, wingspan); //v6
		glTexCoord2f(0.25,0);	
		glVertex3d(-wingRad*0.5, -wingRad, wingspan); //v5

		glTexCoord2f(0.5,0.5);
		glVertex3d(0, 0, wingspan); //v0		
		glTexCoord2f(0.25,0);							
		glVertex3d(-wingRad*0.5, -wingRad, wingspan); //v5
		glTexCoord2f(0,0.5);	
		glVertex3d(-wingRad, 0, wingspan); //v4
		
		glTexCoord2f(0.5,0.5);
		glVertex3d(0, 0, wingspan); //v0
		glTexCoord2f(0,0.5);	
		glVertex3d(-wingRad, 0, wingspan); //v4
		glTexCoord2f(0.25,1);
		glVertex3d(-wingRad*0.5, wingRad, wingspan); //v3

		glTexCoord2f(0.5,0.5);
		glVertex3d(0, 0, wingspan); //v0
		glTexCoord2f(0.25,1);
		glVertex3d(-wingRad*0.5, wingRad, wingspan); //v3
		glTexCoord2f(0.75,1);
		glVertex3d(wingRad*0.5, wingRad, wingspan); //v2

		glTexCoord2f(0.5,0.5);
		glVertex3d(0, 0, wingspan); //v0
		glTexCoord2f(0.75,1);
		glVertex3d(wingRad*0.5, wingRad, wingspan); //v2
		glTexCoord2f(1,0.5);
		glVertex3d(wingRad, 0, wingspan); //v1

		glTexCoord2f(0.5,0.5);
		glVertex3d(0, 0, wingspan); //v0
		glTexCoord2f(1,0.5);
		glVertex3d(wingRad, 0, wingspan); //v1	
		glTexCoord2f(0.75, 0);	
		glVertex3d(wingRad*0.5, -wingRad, wingspan); //v6
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	
	//Connect parts
	glBegin(GL_QUADS);
		glColor3f(1, 1, 1);
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

	xWingFoils(x_foils, wingspan, 0);
	xWingFoils(x_foils, wingspan, 1);
	xWingFoils(x_foils, wingspan, 2);
	xWingFoils(x_foils, wingspan, 3);
	 
	// Draw Nose
	glColor3f(0.745,0.745,0.745);
	glBegin(GL_QUADS);		
		//Top
		glNormal3f(1/(len_tip), 1/(wid_tip+0.08),0);
		glVertex3d((len_tip + nose), 0.0, wid_tip);				//v1
		glVertex3d((len_tip + nose), 0.0, -wid_tip); 			//v0
		glVertex3d(nose, (wid_tip + 0.08), -(wid_tip+ 0.08)); 	//v3
		glVertex3d(nose, (wid_tip + 0.08), (wid_tip+ 0.08));	//v2
				
		//Bottom
		glNormal3f(1/(len_tip), -1/(wid_tip+0.08),0);
		glVertex3d((len_tip + nose), 0.0, -wid_tip); 			//v0		
		glVertex3d((len_tip + nose), 0.0, wid_tip);				//v1
		glVertex3d(nose, (-wid_tip-0.08), (wid_tip+ 0.08));		//v4
		glVertex3d(nose, (-wid_tip-0.08), -(wid_tip+ 0.08));	//v5

		//Left
		glNormal3f(1/(len_tip), 0, 1/0.08);
		glVertex3d((len_tip + nose), 0.0, wid_tip);				//v1
		glVertex3d((len_tip + nose), 0.0, wid_tip);				//v1
		glVertex3d(nose, (-wid_tip-0.08), (wid_tip+ 0.08));		//v4
		glVertex3d(nose, (wid_tip + 0.08), (wid_tip+ 0.08));	//v2
		
		//Right
		glNormal3f(1/(len_tip), 0, -1/0.08);
		glVertex3d((len_tip + nose), 0.0, -wid_tip); 			//v0
		glVertex3d((len_tip + nose), 0.0, -wid_tip); 			//v0
		glVertex3d(nose, (-wid_tip-0.08), -(wid_tip+ 0.08));	//v5		
		glVertex3d(nose, (wid_tip + 0.08), -(wid_tip+ 0.08)); 	//v3

		//Back
		glNormal3f(-1,0,0);
		glVertex3d(nose, (wid_tip + 0.08), (wid_tip+ 0.08));	//v2
		glVertex3d(nose, (wid_tip + 0.08), -(wid_tip+ 0.08)); 	//v3
		glVertex3d(nose, (-wid_tip-0.08), (wid_tip+ 0.08));		//v4
		glVertex3d(nose, (-wid_tip-0.08), -(wid_tip+ 0.08));	//v5
		glEnd();
		
	// Draw Fusolage
	glColor3f(0.70,0.70,0.70);
	glBegin(GL_QUADS);
		//Top
		glNormal3f(1/nose,-1/(wid_tip-wid_base),0);
		glVertex3d(nose, wid_tip, wid_tip); 	//v0
		glVertex3d(nose, wid_tip, -wid_tip);	//v1
		glVertex3d(0, wid_base, -wid_base);		//v2
		glVertex3d(0, wid_base, wid_base); 		//v3
		
		//Bottom
		glNormal3f(1/nose,1/(wid_tip-wid_base),0);
		glVertex3d(0, -wid_base, -wid_base);	//v4
		glVertex3d(0, -wid_base, wid_base); 	//v5
		glVertex3d(nose, -wid_tip, wid_tip);	//v6
		glVertex3d(nose, -wid_tip, -wid_tip);	//v7
		
		//Right
		glNormal3f(1/nose,0,1/(wid_tip-wid_base));
		glVertex3d(nose, wid_tip, -wid_tip);	//v1
		glVertex3d(nose, -wid_tip, -wid_tip);	//v7
		glVertex3d(0, -wid_base, -wid_base);	//v4
		glVertex3d(0, wid_base, -wid_base);		//v2

		//Left
		glNormal3f(1/nose,0,-1/(wid_tip-wid_base));
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
		glNormal3f(1/2.5, 1/0.5, 0);
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
		glNormal3f(1/2.5, 0, -1/(wid_base-0.225));
		glVertex3d(0, 0.5, -wid_base); 			//v0
		glVertex3d(2.5, 0, -0.225);				//v3
		glVertex3d(0, 0, -wid_base); 			//V5	
		glVertex3d(0, 0.5, -wid_base); 			//v0
		
		//Left
		glNormal3f(1/2.5, 0, 1/(wid_base-0.225));
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
	
	//  Undo transformations
	glPopMatrix();
}


static void tieFighter(	double x, double y, double z, 
					double dx, double dy, double dz,
					double ux, double uy, double uz, double scale)
{
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
	//Right
	tieWings(wingRad, wingspan, 0.1);
	//Left
	tieWings(wingRad, -wingspan, -0.1);
	
	//FIX THIS
	//Cockpit
	glScaled(0.9, 0.9, 0.9);
	sphere(textures.tieBody);

	
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
		glNormal3f(0, 0, 1);
		glVertex3d(armRad2,  armRad2, wingspan+0.5); //v4
		glVertex3d(-armRad2, armRad2, wingspan+0.5); //v5
		glVertex3d(-armRad2,-armRad2, wingspan+0.5); //v6
		glVertex3d(armRad2, -armRad2, wingspan+0.5); //v7
	//Front of Arm
		glNormal3f(2.0, 0, 0.2);				
		glVertex3d(armRad1,  armRad1,0.5); //v0
		glVertex3d(armRad2,  armRad2, wingspan+0.5); //v4
		glVertex3d(armRad2, -armRad2, wingspan+0.5); //v7
		glVertex3d(armRad1, -armRad1,0.5); //v3
	//Back of Arm
		glNormal3f(-2.0, 0, 0.2);				
		glVertex3d(-armRad2, armRad2, wingspan+0.5); //v5
		glVertex3d(-armRad1, armRad1,0.5); //v1
		glVertex3d(-armRad1,-armRad1,0.5); //v2
		glVertex3d(-armRad2,-armRad2, wingspan+0.5); //v6
	//Top
		glNormal3f(0, 2.0, 0.2);
		glVertex3d(armRad2,  armRad2, wingspan+0.5); //v4
		glVertex3d(armRad1,  armRad1,0.5); //v0
		glVertex3d(-armRad1, armRad1,0.5); //v1
		glVertex3d(-armRad2, armRad2, wingspan+0.5); //v5
	//Bottom
		glNormal3f(0, -2.0, 0.2);
		glVertex3d(-armRad1,-armRad1,0.5); //v2
		glVertex3d(armRad1, -armRad1,0.5); //v3
		glVertex3d(armRad2, -armRad2, wingspan+0.5); //v7
		glVertex3d(-armRad2,-armRad2, wingspan+0.5); //v6
		
	//Right Arm
	//Base of arm
		glNormal3f(0, 0, -1);
		glVertex3d(armRad1,  armRad1,-0.5); //v0
		glVertex3d(-armRad1, armRad1,-0.5); //v1
		glVertex3d(-armRad1,-armRad1,-0.5); //v2
		glVertex3d(armRad1, -armRad1,-0.5); //v3
	//End of Arm
		glVertex3d(armRad2,  armRad2, -wingspan-0.5); //v4
		glVertex3d(-armRad2, armRad2, -wingspan-0.5); //v5
		glVertex3d(-armRad2,-armRad2, -wingspan-0.5); //v6
		glVertex3d(armRad2, -armRad2, -wingspan-0.5); //v7
	//Front of Arm
		glNormal3f(2.0, 0, -0.2);				
		glVertex3d(armRad1,  armRad1,-0.5); //v0                        
		glVertex3d(armRad2,  armRad2, -wingspan-0.5); //v4
		glVertex3d(armRad2, -armRad2, -wingspan-0.5); //v7
		glVertex3d(armRad1, -armRad1,-0.5); //v3
	//Back of Arm
		glNormal3f(-2.0, 0, -0.2);				
		glVertex3d(-armRad2, armRad2, -wingspan-0.5); //v5
		glVertex3d(-armRad1, armRad1,-0.5); //v1
		glVertex3d(-armRad1,-armRad1,-0.5); //v2
		glVertex3d(-armRad2,-armRad2, -wingspan-0.5); //v6
	//Top
		glNormal3f(0, 2.0, -0.2);
		glVertex3d(armRad2,  armRad2, -wingspan-0.5); //v4
		glVertex3d(armRad1,  armRad1,-0.5); //v0
		glVertex3d(-armRad1, armRad1,-0.5); //v1
		glVertex3d(-armRad2, armRad2, -wingspan-0.5); //v5
	//Bottom
		glNormal3f(0, -2.0, -0.2);
		glVertex3d(-armRad1,-armRad1,-0.5); //v2
		glVertex3d(armRad1, -armRad1,-0.5); //v3
		glVertex3d(armRad2, -armRad2, -wingspan-0.5); //v7
		glVertex3d(-armRad2,-armRad2, -wingspan-0.5); //v6
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
        
        lightsOn(Ambient, Diffuse, Specular, Position);
	}
	else
		glDisable(GL_LIGHTING);
     
	xWing(-8,0,0, 1,0,0, 0,1,1, 1);
	tieFighter(8,0,0, 1,0.5,0, 0,0,1, 1);
	//tieFighter(0,0,0, 1,0,0, 0,1,0, 1);
	xWing(4,-10,-15, 0.1,0.1,0.1, 0,1,-1, 1);	
	tieFighter(-27,+5,-4, 1,0,0, (20/35), -0.25, 0.1, 1);
	
	asteroidField(N, SPREAD);
	//  White
	glColor3f(1,1,1);
	
	//  Draw axes
	if (axes == 1)
	{
		drawAxes(len);
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
		Print("Press 'i' or 'I' to toggle program info/control display");
	}

	//  Debug info
	ErrCheck("display");
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
	else if (ch=='i')
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
	//  Initialize GLUT
	glutInit(&argc,argv);
	//  Request double buffered, true color window with Z buffering at 600x600
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(600,600);
	glutCreateWindow("HW 5 - Chris Fichman - X-Wing vs. TIE Fighter");
	setTextures(&textures);
	initAField(&field);
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
