// textures.h
#include "CSCIx229.h"

typedef struct Textures{
	int asteroid;
	int tieBody;
	int tieWing;
	//int xWingBody;
	//int xWingFoil;
} Textures;

void setTextures(Textures *textures){
	textures->asteroid = LoadTexBMP("asteroid.bmp");
	textures->tieBody = LoadTexBMP("TIETex.bmp");
	textures->tieWing = LoadTexBMP("TIEWing.bmp");
	//textures.xWingBody = ;
	//textures.xWingFoil = ;
}
