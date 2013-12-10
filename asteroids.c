// asteroids.h
#include "CSCIx229.h"
#include "util.h"
#define N 200
#define SPREAD 50

typedef struct Asteroid{
	double r, x, y, z;
	double new_x, new_y, new_z;
	double v_x, v_y, v_z, v_rot;
	double rad[19][38];
	double rot;
} Asteroid;

typedef struct AField{
	Asteroid asteroids[N];
}AField;

void initAPoint(Asteroid *asteroid){
	int i;
	int j;
	double current;
	
	for(i = 0; i<10; i++){
		asteroid->rad[i][0] = randRange(0.9, 2.0);
		asteroid->rad[i][1] = randRange(0.9, 2.0);
		
		for(j = 2; j<18; j++){
			current = randRange(0.9, 2.0);
			asteroid->rad[i][j] = (current+asteroid->rad[i][j-1]+asteroid->rad[i][j-2])/3;
		}
		asteroid->rad[i][18] = asteroid->rad[i][0];
	}
}
void initAField(AField *field){
	int i;
	for(i = 0 ; i < N; i++){
		field->asteroids[i].r = randRange(1.0, 3.0);
		field->asteroids[i].x = randRange(-SPREAD, SPREAD);
		field->asteroids[i].y = randRange(-SPREAD, SPREAD);
		field->asteroids[i].z = randRange(-SPREAD, SPREAD);
		field->asteroids[i].rot = randRange(-90, 90);
		field->asteroids[i].v_rot = randRange(-3.0, 3.0);
		field->asteroids[i].v_x = randRange(-0.2, 0.2);
		field->asteroids[i].v_y = randRange(-0.2, 0.2);
		field->asteroids[i].v_z = randRange(-0.2, 0.2);
		field->asteroids[i].new_x = field->asteroids[i].x;
		field->asteroids[i].new_y = field->asteroids[i].y;
		field->asteroids[i].new_z = field->asteroids[i].z;
		initAPoint(&(field->asteroids[i]));
	}
}

void drawAsteroid(AField *field, int texture, int i){
	int th,ph,j,k;
	double x,y,z;
	//  Set texture 
	if(texture != 0){
	   glEnable(GL_TEXTURE_2D);
	   glBindTexture(GL_TEXTURE_2D, texture);
	}
	field->asteroids[i].new_x = field->asteroids[i].new_x+field->asteroids[i].v_x;
	field->asteroids[i].new_y = field->asteroids[i].new_y+field->asteroids[i].v_y;
	field->asteroids[i].new_z = field->asteroids[i].new_z+field->asteroids[i].v_z;
	field->asteroids[i].rot = field->asteroids[i].rot+field->asteroids[i].v_rot;
	
	
	glPushMatrix();
	glTranslated(field->asteroids[i].new_x, 
				 field->asteroids[i].new_y, 
				 field->asteroids[i].new_z);
	glRotated(field->asteroids[i].rot, 0.3,0.4,0.2);
	glScaled(field->asteroids[i].r, field->asteroids[i].r, field->asteroids[i].r);
	//  Latitude bands
	glColor3f(1,1,1);
	
	j = 0;
	for (ph=-90;ph<90;ph+=20)
	{
		k = 0;
		glBegin(GL_QUAD_STRIP);
			for (th=0;th<=360;th+=20)
			{
				x = -Sin(th)*Cos(ph)*(field->asteroids[i].rad[j][k]);
				y =  Cos(th)*Cos(ph)*(field->asteroids[i].rad[j][k]);
				z =          Sin(ph)*(field->asteroids[i].rad[j][k]);
				
				glNormal3f(x,y,z);
				glTexCoord2d(th/360.0,ph/180.0+0.5);
				glVertex3f(x,y,z);
								
				x = -Sin(th)*Cos(ph+20)*(field->asteroids[i].rad[j+1][k]);
				y =  Cos(th)*Cos(ph+20)*(field->asteroids[i].rad[j+1][k]);
				z =          Sin(ph+20)*(field->asteroids[i].rad[j+1][k]);
				
				glNormal3f(x,y,z);
				glTexCoord2d(th/360.0,(ph+20)/180.0+0.5);
				glVertex3f(x,y,z);
				
				k++;
			}
			
			j++;
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}

