#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <Windows.h>
#include <math.h>


/*--------------------------------------------------------------Team Name: Wo7osh-Elcollision--------------------------------------------------*/
/*Bassel Amgad 43-6927----------Arwa Tawfik 43-12603-------------Donia Ali 43-8354---------------------Salma El-Sayed 43-6801----------------*/

/*------------------------------------------------------------The Lost Bear------------------------------------------------------------------*/



#define DEG2RAD(a) (a * 0.0174532925)

//donia
bool Inthigh = true;
bool Intlow = false;
float Positionx = 1.0;
float Intensity = 0.1;
float Lightangle = 10.0;
//

int WIDTH = 1280;
int HEIGHT = 720;

GLuint tex;
GLuint bushtex;
GLuint spiketex;

char title[] = "The Lost Bear";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 200;
int playerX = -18;
int playerY = 0;
int playerZ = 17;
int playerR = 270;
int step = 1;
int cameramode = 1;
BOOLEAN dead = false;
BOOLEAN lvl1_passed = false;
BOOLEAN lvl2_passed = false;
int score = 0;
int timeout = 800;
int celebrate_time = 10;
bool coinpresent[6] = { true, true, true, true, true, true };

bool moveright = true;
bool moveleft = true;
bool moveup = true;
bool movedown = true;


//---------------------------------------------------------------Bassel------------------------------------------------
class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};


//---------------------------------------------------------------------------------------------------------------------------






//Note on camera:
//--eye is where the camera itself is
//--AT is the point where the camera is looking at

// so to move it forward i translate the eye and at at the same time

Vector3f eye(-19, 2,17 );
Vector3f center(-18.9, 2,17);
Vector3f firstpersoncenter(0, 2, 17);

Vector3f up(0, 1, 0);

int cameraZoom = 0;
int cameraZoomx = 0;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;
Model_3DS model_annie;
//--Bassel
Model_3DS model_grassWall;
Model_3DS model_brickWall;
//--Arwa
Model_3DS model_bear;
Model_3DS model_apple;
//salma
Model_3DS model_coin;

//---
// Textures
GLTexture tex_ground;
GLTexture tex_brickground;
GLTexture tex_grasswall;
GLTexture tex_bush;
int t = 1;
int v = 0;

//----------------------------------------Camera Functions---------------------------------------------

void moveZ(float d) {
	
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
		//first center
		//3rd center
	
}

void rotateY(float a) {
	Vector3f view = (center - eye).unit();
	Vector3f right = up.cross(view).unit();
	view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
	right = view.cross(up);
	center = eye + view;
}

void look() {
	gluLookAt(
		eye.x, eye.y, eye.z,
		center.x, center.y, center.z,
		up.x, up.y, up.z
	);
}
//-----------------------------------------------------------------------------------------------------------------------

//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	//GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	//// Define Light source 0 diffuse light
	//GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	//// Define Light source 0 Specular light
	//GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	//// Finally, define light source 0 position in World Space
	//GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 90.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}


//takes filename and plays the sound
void PS(char* sound) {
	PlaySound(TEXT(sound), NULL, SND_ASYNC);
}


void print(int x, int y,int z,  char* string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos3f(x, y,z);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

//int wallsZ[12] = { 30,23,16,9,2,-4.5,0,-7,-14,-21,-28,-35 };
int wallsZ[11] = { 15,8,1,-6,-13,-19.5,-25,-32,-39,-46,-53 };
int found(int z) {
	int n = sizeof(wallsZ);
	for (int i = 0;i < 11;i++) {
		if (z == wallsZ[i])
			return i;
	}
	return -1;
}
int openings[11] = { 6,1,4,6,3,1,4,6,1,5,2 };
boolean canPass(int x,int z) {
	int currwall = found(z);
	if (currwall > -1) {
		int openspace = openings[currwall];
		if (openspace * 5 + -20+5 > x && openspace * 5 -20 < x) {
			return true;
		}
		return false;
	}
	return true;
}


int back = -2.5;

void moveUp() {
	playerR = 0;
	if (playerZ>-59 && (found(playerZ-step)==-1 || canPass(playerX, playerZ-step))) {
		playerZ -= step;
		moveZ(1.0);

		if (t != 2) {
			if (cameramode == 1) {
				int rotation = 90 - v;
				rotateY(rotation);
				v = 90;
				t = 2;
			}

			if (cameramode == 3) {
				moveZ(-back);
				int rotation = 90 - v;
				rotateY(rotation);
				v = 90;
				t = 2;
				moveZ(back);
			}
		}

	}
	else {
		PS("sfx/slam.wav");
	}

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);	//Setup Camera with modified paramters

	//GLfloat l0Direction[] = { 0.0, Positiony, 0.0 };
	GLfloat l0Direction[] = { Positionx, 6, 0.0 };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, l0Direction);
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();
}


void moveDown() {
		playerR = 180;
	if (playerZ < 19 && (found(playerZ+step)==-1 || canPass(playerX, playerZ + step))) {
		playerZ += step;
		moveZ(1.0);

		if (t != 4) {

			if (cameramode == 1) {
				int rotation = 270 - v;
				rotateY(rotation);
				v = 270;
				t = 4;
			}
			if (cameramode == 3) {
				moveZ(-back);
				int rotation = 270 - v;
				rotateY(rotation);
				v = 270;
				t = 4;
				moveZ(back);
			}
		}

		
	}
	else {
		PS("sfx/slam.wav");
	}

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);	//Setup Camera with modified paramters

	GLfloat l0Direction[] = { 0.0, Positionx, 0.0 };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, l0Direction);
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();
}


void moveLeft() {
		playerR = 90;
	if (playerX > -19 && found(playerZ) == -1) {
		playerX -= step;
		moveZ(1.0);

		if (t != 3) {
			if (cameramode == 1) {
				int rotation = 180 - v;
				rotateY(rotation);
				v = 180;
				t = 3;
			}
			if (cameramode == 3) {
				moveZ(-back);
				int rotation = 180 - v;
				rotateY(rotation);
				v = 180;
				t = 3;
				moveZ(back);
			}
		}
	}
	else {
		PS("sfx/slam.wav");
	}

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);	//Setup Camera with modified paramters

	GLfloat l0Direction[] = { 0.0, Positionx, 0.0 };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, l0Direction);
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw scene 
}

void moveRight() {
		playerR = 270;
	if (playerX < 19 && found(playerZ) == -1) {
		playerX += step;
		moveZ(1.0);

		if (t != 1) {
			if(cameramode==1){

			int rotation = 0 - v;
			rotateY(rotation);
			v =0;
			t = 1;
			}
			if (cameramode == 3) {
				moveZ(-back);
				int rotation = 0 - v;
				rotateY(rotation);
				v = 0;
				t = 1;
				moveZ(back);
			}

		}
	}
	else {
		PS("sfx/slam.wav");
	}

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);	//Setup Camera with modified paramters

	GLfloat l0Direction[] = { 0.0, Positionx, 0.0 };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, l0Direction);
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw scene 
}

void spe(int k, int x0, int y0) // keyboard special key function takes 3 parameters								// int k: is the special key pressed such as the keyboard arrows the f1,2,3 and so on
{
	if (k == GLUT_KEY_RIGHT && moveright) { //if the right arrow is pressed, then the object will be translated in the x axis by 10. (moving right)
		//moveRight();
	}
	if (k == GLUT_KEY_LEFT && moveleft)
	{ //if the left arrow is pressed, then the object will be translated in the x axis by -10. (moving left)
		//moveLeft();
	}
	if (k == GLUT_KEY_UP && moveup) { //if the up arrow is pressed, then the object will be translated in the y axis by 10. (moving upwords)
		moveUp();
	}
	if (k == GLUT_KEY_DOWN && movedown) { //if the down arrow is pressed, then the object will be translated in the y axis by -10. (moving downwords)
		moveDown();
	}
	glutPostRedisplay(); //redisplay to update the screen with the changes
}
//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	//render level1 Ground
	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();
	

	//----render Level2 Ground
	glBindTexture(GL_TEXTURE_2D, tex_brickground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, -60);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, -60);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

//=======================================================================
// Display Function
//=======================================================================

void skyBox() {
	glPushMatrix();

	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 100, 100, 100);
	gluDeleteQuadric(qobj);
	glPopMatrix();

}

void bushes() {
	glPushMatrix();
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(9, 1, 3);
	//glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, bushtex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 2, 100, 100);
	gluDeleteQuadric(qobj);
	glPopMatrix();

	glPushMatrix();
	//GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(-12, 1, -10);
	//glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, bushtex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 2, 100, 100);
	gluDeleteQuadric(qobj);
	glPopMatrix();

}


void spike() {
	glPushMatrix();
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glRotated(-90, 1, 0, 0);
	glTranslatef(0,-3.0f,0);
	glBindTexture(GL_TEXTURE_2D, spiketex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	// glu.gluCylinder(quadric, bottomRadius, topRadius, height, slices, rings)
	gluCylinder(qobj, 0.3, 0, 2, 50, 50);
	gluDeleteQuadric(qobj);
	glPopMatrix();
}
void spikes() {


	glPushMatrix();
	glTranslated(5, 1, 3);
	spike();
	glPopMatrix();

	glPushMatrix();
	glTranslated(5, 1, 4);
	spike();
	glPopMatrix();
	glPushMatrix();
	glTranslated(5, 1, 5);
	spike();
	glPopMatrix();
	glPushMatrix();
	glTranslated(5, 1, 6);
	spike();
	glPopMatrix();

}

void brickWall(int g,int z) {
	//draw brickwall
	glPushMatrix();
	glTranslatef(g, 3, -25+z);
	glScalef(0.4, 0.5, 1);
	model_brickWall.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(g, 3, -24.7+z);
	glScalef(0.4, 0.5, 1);
	model_brickWall.Draw();
	glPopMatrix();

}

void grassWall(int g, int z) {
	//draw brickwall
	glPushMatrix();
	glTranslatef(g, 3, -15 + z);
	glScalef(0.4, 0.5, 1);
	model_grassWall.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(g, 3, -15.7 + z);
	glScalef(0.4, 0.5, 1);
	model_grassWall.Draw();
	glPopMatrix();

}

void brickBorders() {
	int g = -20;
	for (int i = 0;i < 8;i++) {
		glPushMatrix();
		glTranslatef(-20, 3, g);
		glRotatef(90, 0, 1, 0);
		glScalef(0.4, 0.5, 1);
		model_brickWall.Draw();
		glPopMatrix();
		g = g - 5;
	}
	g = -20;
	for (int i = 0;i < 8;i++) {
		glPushMatrix();
		glTranslatef(20, 3, g);
		glRotatef(90, 0, 1, 0);
		glScalef(0.4, 0.5, 1);
		model_brickWall.Draw();
		glPopMatrix();
		g = g - 5;
	}
}

void grassBorders() {
	int g = 20;
	for (int i = 0;i < 8;i++) {
		glPushMatrix();
		glTranslatef(-20, 3, g);
		glRotatef(90, 0, 1, 0);
		glScalef(0.4, 0.5, 1);
		model_grassWall.Draw();
		glPopMatrix();
		g = g - 5;
	}
	g = 20;
	for (int i = 0;i < 8;i++) {
		glPushMatrix();
		glTranslatef(20, 3, g);
		glRotatef(90, 0, 1, 0);
		glScalef(0.4, 0.5, 1);
		model_grassWall.Draw();
		glPopMatrix();
		g = g - 5;
	}
}

void coin(int x, int z) {

	glPushMatrix();
	glTranslated(x, 1, z);
	glScaled(0.3, 0.3, 0.3);
	glRotated(45, 0, 0, 1);
	model_coin.Draw();
	glPopMatrix();
	
}
void drawCoins() {
	//level1 coins
	if (coinpresent[0])
	coin(3,4);
	if (coinpresent[1])
	coin(-8,17);
	if (coinpresent[2])
	coin(4,-11);
	//level2 coins
	if (coinpresent[3])
	coin(3, -36);
	if (coinpresent[4])
	coin(-12, -50);
	if (coinpresent[5])
	coin(4, -43);
	
}

void drawBrickWalls(int noWall,int z) {
	//noWall is where I dont want a wall to block path

	int g = -20;
	for (int i = 0;i < 8;i++) {
		if (i != noWall) {
			brickWall(g, z);
		}	
		g = g + 5;
	}


}



void drawGrassWalls(int noWall, int z) {
	//noWall is where I dont want a wall to block path

	int g = -20;
	for (int i = 0;i < 8;i++) {
		if (i != noWall) {
			grassWall(g, z);
		}
		g = g + 5;
	}


}

void drawSpikes() {

	glPushMatrix();
	glTranslatef(10,-1,-58);
	spikes();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-11, -1, -37);
	spikes();
	glPopMatrix();
}

void drawLevel1Walls() {

	drawGrassWalls(-1, 35);

	drawGrassWalls(6,30);

	drawGrassWalls(1, 23);

	drawGrassWalls(4, 16);

	drawGrassWalls(6, 9);

	drawGrassWalls(3, 2);

	drawGrassWalls(1, -4.5);

	grassBorders();

}

void drawLevel2Walls() {

	//you can pick where the missing wall is
	//by changeing fhte first parameter
	//from 0 => 7
	drawBrickWalls(4,0);

	drawBrickWalls(6,-7);

	drawBrickWalls(1,-14);

	drawBrickWalls(5, -21);

	drawBrickWalls(2, -28);

	drawBrickWalls(-1, -35);

	
	brickBorders();

}



void renderLevel1() {

	drawLevel1Walls();

	bushes();

}

void renderLevel2() {

	drawLevel2Walls();
	drawSpikes();
	
}
void drawPlayer() {
	   glPushMatrix();
	   glTranslated(playerX,playerY,playerZ);
		glScalef(0.1, 0.1, 0.1);
		glRotatef(playerR, 0,1, 0);
		model_bear.Draw();
		glPopMatrix();
}

void drawMamaBear() {
	glPushMatrix();
	glTranslated(18, 0, -55);
	glScalef(0.4, 0.4, 0.4);
	glRotatef(playerR, 0, 1, 0);
	glRotatef(180, 0, 1, 0);
	model_bear.Draw();
	glPopMatrix();
}

void drawApple() {
	glPushMatrix();
	glTranslated(0,5,0);
	glScalef(1, 1, 1);
	model_apple.Draw();
	glPopMatrix();
}
boolean withinRange(int x,int z, int r) {
	if ((playerX <= x + r && playerX >= x - r) && (playerZ <= z + r && playerZ >= z - r)) {
		return true;
	}
	return false;
}
void checkCoin() {
	//boolean alreadythere = false;
	if (withinRange(3, 4, 2)) {
		score += 25;
		if (coinpresent[0]) {
			score += 25;
			PS("sfx//coin.wav");
		}
		coinpresent[0] = false;
	}
	if (withinRange(-8, 17, 2)) {
		score += 25;
		if (coinpresent[1]) {
			score += 25;
			PS("sfx//coin.wav");
		}
		coinpresent[1] = false;
	}
	if (withinRange(4, -11, 2)) {
		score += 25;
		if (coinpresent[2]) {
			score += 25;
			PS("sfx//coin.wav");
		}
		coinpresent[2] = false;
	}
	if (withinRange(3, -36, 2)) {
		score += 25;
		if (coinpresent[3]) {
			score += 25;
			PS("sfx//coin.wav");
		}
		coinpresent[3] = false;
	}
	if (withinRange(-12, -50, 2)) {
		
		if (coinpresent[4]) {
			score += 25;
			PS("sfx//coin.wav");
		}
		coinpresent[4] = false;
	}
	if (withinRange(4, -43, 2)) {
		
		if (coinpresent[5]) {
			score += 25;
			PS("sfx//coin.wav");
		}
		coinpresent[5] = false;
	}
	
}
boolean bushalreadythere = false;
void checkBush() {
	if (withinRange(9, 3, 2) || withinRange(-12, -10, 2)) {
		if (!bushalreadythere) {
			PS("sfx//bush.wav");
			timeout -= 50;
		}
		bushalreadythere = true;
	}
	else {
		bushalreadythere = false;
	}
}
boolean spikealreadythere = false;
void checkSpike() {
	if (withinRange(15, -50, 2) || withinRange(-6, -30, 2)) {
		if (!spikealreadythere) {
			PS("sfx//tomhurt.wav");
			dead = true;
		}
		spikealreadythere = true;
	}
	else {
		spikealreadythere = false;
	}
}
void checkLvl1() {
	if (withinRange(-10, -18, 2)) {
		lvl1_passed = true;
	}
}
void checkLvl2() {
	if (withinRange(18, -58, 2)) {
		lvl2_passed = true;
	}
}

bool l = true;
bool w1 = true;
bool w2 = true;
int currz=center.z;;

void myDisplay(void)
{
	checkBush();
	checkCoin();
	checkSpike();
	checkLvl1();
	checkLvl2();
	if (!dead && timeout > 0 && !lvl2_passed && (celebrate_time==10 || celebrate_time<1)) {
		currz = center.z;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLfloat lightIntensity[] = { 0.1, 0.1, 0.1, 1.0f };
		GLfloat lightIntensity1[] = { Intensity, Intensity, Intensity, 1.0f };

		GLfloat lightPosition[] = { 0.0,1.0, 0.0, 1 };
		GLfloat l0Direction[] = { 0.0, Positionx, 0.0 };
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, Lightangle);
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, l0Direction);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity1);

		// Draw Ground
		RenderGround();

		drawPlayer();
		drawMamaBear();

		//drawApple

		renderLevel1();
		renderLevel2();
		drawCoins();
		skyBox();
	}
	else
	{ 
		glEnable(GL_LIGHT0);
		GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
		GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (timeout>0) {
			if ((lvl1_passed||lvl2_passed) && dead==false) {

				if (lvl1_passed) {
					if (w1) {
						PS("sfx//mission1.wav");
						w1 = false;
					}
				}
				if (lvl2_passed) {
					if (w2) {
						PS("sfx//dabdoba.wav");
						w2 = false;
					}
				}

				char* p0s[40];
				sprintf((char*)p0s, "YOU WON!!!   Score: %d", score);
				print(center.x, center.y,center.z, (char*)p0s);
			}
			else {

				
				char* p0s[40];
				sprintf((char*)p0s, "You Lost!   Score: %d", score );
				print(center.x, center.y, center.z, (char*)p0s);
			}

		}
		else {
			if (l&&w2) {
				PS("sfx//lose.wav");
				l = false;
			}
			char* p0s[40];
			sprintf((char*)p0s, "You ran out of time!   Score: %d",score );
			print(center.x, center.y,center.z, (char*)p0s);
		}
	
	}

	glutSwapBuffers();
}

//=======================================================================
// Keyboard Function
//=======================================================================
void myKeyboard(unsigned char button, int x, int y)
{
	
	switch (button)
	{
	case'a':
		eye.x -= 0.2;
		center.x -= 0.2;
		break;
	case'd':
		eye.x += 0.2;
		center.x += 0.2;
		break;
		
	case'w':
		eye.z -= 0.2;
		center.z -= 0.2;
		rotateY(10);
		break;
	case's':
		eye.z += 0.2;
		center.z += 0.2;
		break;


	case GLUT_KEY_F1:
		exit(0);
		break;
	default:
		break;
	}

	if (button == '1'&&cameramode==3)
	{
			moveZ(-back);
			
			cameramode = 1;
	}
	if (button == '3'&&cameramode==1) {
		moveZ(back);
		cameramode = 3;
	}

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(eye.x,eye.y,eye.z, center.x, center.y, center.z, up.x, up.y, up.z);	//Setup Camera with modified paramters

	GLfloat l0Direction[] = { 0.0, Positionx, 0.0 };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, l0Direction);
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw s

	glutPostRedisplay();
}

//=======================================================================
// Motion Function
//=======================================================================
void myMotion(int x, int y)
{
	y = HEIGHT - y;
	x = WIDTH - x;

	if (cameraZoom - y > 0)
	{
		//rotateX(0.1);
	}
	else
	{
		//rotateX(-0.1);
	}
	if (cameraZoomx - x > 0)
	{
		//rotateY(0.5);
	}
	else
	{
		//rotateY(-0.5);
	}

	cameraZoom = y;
	cameraZoomx = x;
	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);	//Setup Camera with modified paramters

	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw scene 
}

//=======================================================================
// Mouse Function
//=======================================================================
int pasty = 0;
int pastx = 0;


//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
}

//=======================================================================
// Assets Loading Function
//=======================================================================

void myMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		moveRight();
	}
	if (button == GLUT_RIGHT_BUTTON) {
		moveLeft();
	}
	


}
void LoadAssets()
{
	// Loading Model files


	model_brickWall.Load("models/brickWall/wall.3DS");

	model_grassWall.Load("models/grassWall/wall.3DS");
	model_bear.Load("models/ted-bear/ted.3ds");
	model_coin.Load("models/coin/Coin.3ds");

	tex_brickground.Load("Textures/brickFloor.bmp");
	tex_ground.Load("Textures/grass.bmp");
	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);
	loadBMP(&bushtex, "Textures/bush.bmp", true);
	loadBMP(&spiketex, "Textures/spikes.bmp", true);
}

bool j = false;
void timer2(int val) {
	if (w2) {
		if (!w1&&!j) {
			Intensity = 0.9;
			Inthigh = false;
			Intlow = true;
			j = true;
		}else
		if (Inthigh) {
			Intensity = 0.9;
			Inthigh = false;
			Intlow = true;
			PS("sfx//thunder.wav");


		}
		else if (Intlow) {
			//play thunder sound
			Intensity = 0.0;

			Intlow = false;
			Inthigh = true;
		}
	}
	else {
		Intensity = 0.9;
		Inthigh = false;
		Intlow = true;
	}


	glutPostRedisplay();
	glutTimerFunc(5000,timer2, 0);

}

void timer(int val) {
	
	if (lvl1_passed && !lvl2_passed) {
		celebrate_time--;
	}
	if (!lvl2_passed && !dead) {
		timeout--;
	}
	Positionx = -1 * Positionx;
	Lightangle = ((int)rand() % 181) / 2;
	glutPostRedisplay();
	glutTimerFunc(100, timer, 0);

}
//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(0, 0);

	glutCreateWindow(title);
	glutTimerFunc(0, timer, 0);
	glutTimerFunc(5000, timer2, 0);

	//PS("sfx//win.wav");

	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(spe);

	glutMotionFunc(myMotion);

	glutMouseFunc(myMouse);

	glutReshapeFunc(myReshape);

	myInit();

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}