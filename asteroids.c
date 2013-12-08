// asteroids.h
#include "CSCIx229.h"
#include "util.h"
#define N 20
#define SPREAD 30


typedef struct  Asteroid{
	double r;
	double x;
	double y;
	double z;
	double rad[2592];
} Asteroid;

typedef struct AField{
	Asteroid asteroids[N];
}AField;

void initAPoint(Asteroid asteroid){
	int i_0 = 0;
	int i_1 = 1;
	int i;
	
	asteroid.rad[i_0] = randRange(0.8, 1);
	
	asteroid.rad[i_1] = randRange(0.8, 1);
	
	for(i = 2; i<2592; i++){
		asteroid.rad[i] = (randRange(0.8, 1)+asteroid.rad[i_1]+asteroid.rad[i_0])/3;
		i_0++;
		i_1++;
	}
}
void initAField(AField *field){
	int i;
	for(i = 0 ; i < N; i++){
		field->asteroids[i].r = randRange(1, 3);
		field->asteroids[i].x = randRange(-SPREAD, SPREAD);
		field->asteroids[i].y = randRange(-SPREAD, SPREAD);
		field->asteroids[i].z = randRange(-SPREAD, SPREAD);
		initAPoint(field->asteroids[i]);
	}
}

void drawAsteroids(AField *field, int texture, int i){
	int th,ph,j;
	double x,y,z;
	//  Set texture 
	if(texture != 0){
	   glEnable(GL_TEXTURE_2D);
	   glBindTexture(GL_TEXTURE_2D, texture);
	}
	//  Latitude bands
	glColor3f(1,1,1);
	j = 0;
	for (ph=-90;ph<90;ph+=5)
	{
	  glBegin(GL_QUAD_STRIP);
		  for (th=0;th<=360;th+=5)
		  {
				x = -Sin(th)*Cos(ph)*(field->asteroids[i].rad[j]);
				y =  Cos(th)*Cos(ph)*(field->asteroids[i].rad[j]);
				z =          Sin(ph)*(field->asteroids[i].rad[j]);
				
				glNormal3f(x,y,z);
				glTexCoord2d(th/360.0,ph/180.0+0.5);
				glVertex3f(x,y,z);
								
				x = -Sin(th)*Cos(ph+5)*(field->asteroids[i].rad[j+1]);
				y =  Cos(th)*Cos(ph+5)*(field->asteroids[i].rad[j+1]);
				z =          Sin(ph+5)*(field->asteroids[i].rad[j+1]);
				
				glNormal3f(x,y,z);
				glTexCoord2d(th/360.0,(ph+5)/180.0+0.5);
				glVertex3f(x,y,z);

				
				j+=2;
		  }
	  glEnd();
	}
	glDisable(GL_TEXTURE_2D);
		

}
