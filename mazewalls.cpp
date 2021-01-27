#include "windows.h"
//#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>


int hereX;
int hereY;

int mazeH[9][9] = { {0, 1, 2, 3, 4, 0, 1, 2, 3 },
					{0, 1, 0, 1, 0, 0, 0, 1, 0 },
					{0, 0, 1, 0, 1, 2, 3, 4, 0 },
					{0, 0, 0, 0, 0, 1, 0, 0, 0 },
					{0, 1, 0, 1, 0, 0, 0, 0, 0 },
					{0, 1, 2, 3, 4, 0, 1, 2, 0 },
					{0, 1, 2, 3, 4, 5, 0, 1, 2 },
					{0, 0, 0, 1, 2, 3, 4, 5, 0 },
					{0, 0, 1, 2, 3, 0, 1, 2, 3 } };

int mazeV[9][9] = { {5, 1, 0, 0, 2, 2, 0, 0, 2 },
					{4, 0, 0, 3, 1, 1, 1, 4, 1 },
					{3, 2, 2, 2, 0, 0, 0, 3, 0 },
					{2, 1, 1, 1, 3, 3, 2, 2, 5 },
					{1, 0, 0, 0, 2, 2, 0, 1, 4 },
					{0, 0, 0, 0, 1, 1, 0, 0, 3 },
					{2, 2, 1, 0, 0, 0, 1, 0, 2 },
					{1, 1, 0, 0, 1, 0, 0, 0, 1 },
					{0, 0, 0, 0, 0, 0, 0, 0, 0 } };

void movenorth() {
	if (mazeV[hereX][hereY] < mazeV[hereX][hereY + 1]) {
		//go
	}
	else {
		//collision
	}
}

void movesouth() {
	if (mazeV[hereX][hereY] > mazeV[hereX][hereY - 1]) {
		//go
	}
	else {
		//collision
	}
}

void moveeast() {
	if (mazeH[hereX][hereY] < mazeH[hereX + 1][hereY]) {
		//go
	}
	else {
		//collision
	}
}

void movewest() {
	if (mazeH[hereX][hereY] > mazeH[hereX - 1][hereY]) {
		//go
	}
	else {
		//collision
	}
}