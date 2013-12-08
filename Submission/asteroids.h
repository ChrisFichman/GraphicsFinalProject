// asteroids.h
#include "CSCIx229.h"
#define N 20
#define SPREAD 40

double randRange(double min, double max) {
   return rand() * (max - min) / RAND_MAX + min;
}

typedef struct  Asteroid{
	double r;
	double x;
	double y;
	double z;
} Asteroid;
typedef struct AField{
	Asteroid asteroids[N];
}AField;

void initAField(AField *field){
	int i;
	for(i = 0 ; i < N; i++){
		field->asteroids[i].r = randRange(0, 3);
		field->asteroids[i].x = randRange(-SPREAD, SPREAD);
		field->asteroids[i].y = randRange(-SPREAD, SPREAD);
		field->asteroids[i].z = randRange(-SPREAD, SPREAD);
	}
}
