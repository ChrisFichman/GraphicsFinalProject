// asteroids.h
#include "CSCIx229.h"
#include "util.h"
#define N 20
#define SPREAD 40

typedef struct APoints{
	double x;
	double y;
	double z;
}APoints;

typedef struct  Asteroid{
	double r;
	double x;
	double y;
	double z;
	APoints points[2592];
} Asteroid;

typedef struct AField{
	Asteroid asteroids[N];
}AField;

void initAPoint(Asteroid asteroid){
	int i_0 = 0;
	int i_1 = 1;
	int i;
	
	asteroid.points[i_0].x = randRange(0.2, 0.6);
	asteroid.points[i_0].y = randRange(0.2, 0.6);
	asteroid.points[i_0].z = randRange(0.2, 0.6);
	
	asteroid.points[i_1].x = randRange(0.2, 0.6);
	asteroid.points[i_1].y = randRange(0.2, 0.6);
	asteroid.points[i_1].z = randRange(0.2, 0.6);
	
	for(i = 2; i<2592; i++){
		asteroid.points[i].x = (randRange(0.2, 0.6)+asteroid.points[i_0].x+asteroid.points[i_1].x)/3;
		asteroid.points[i].y = (randRange(0.2, 0.6)+asteroid.points[i_0].y+asteroid.points[i_1].y)/3;
		asteroid.points[i].z = (randRange(0.2, 0.6)+asteroid.points[i_0].z+asteroid.points[i_1].z)/3;
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
				x = -Sin(th)*Cos(ph)+(field->asteroids[i].points[j].x);
				y =  Cos(th)*Cos(ph)+(field->asteroids[i].points[j].y);
				z =          Sin(ph)+(field->asteroids[i].points[j].z);
				
				glNormal3f(x,y,z);
				glTexCoord2d(th/360.0,ph/180.0+0.5);
				glVertex3f(x,y,z);
				
				j++;
		  }
	  glEnd();
	}
	glDisable(GL_TEXTURE_2D);
		

}
