#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<math.h>

#define XMAX 1200
#define YMAX 700
#define SPEED 20
#define TOP 0
#define RIGHT 1
#define BOTTOM 2
#define LEFT 3

GLint m_viewport[4];
bool mButtonPressed = false;
float mX, mY;   //mouse x and y
enum view { INTRO, MENU, INSTRUCTIONS, GAME, GAMEOVER };   
view viewPage = INTRO; // initial value
bool keyValue[256] = {false};
bool direction[4] = {false };
bool laser1Dir[2] = { false};
bool laser2Dir[2] = {false};

int alienLife1 = 100;
int alienLife2 = 100;
bool gameOver = false;
float xOne = 500, yOne = 0;
float xTwo = 500, yTwo = 0;
bool laser1 = false, laser2 = false;
GLint CI = 0;
GLfloat a[][2] = { 0,-50, 70,-50, 70,70, -70,70 };
GLfloat LightColor[][3] = { 1,1,0,   0,1,1,   0,1,0 };		//spaceship decorations

//there are 4 arguments in the raster function x,y,z(coordinates) and string variable
void displayRasterText(float x, float y, float z, const  char* string) {
	glRasterPos3f(x, y, z);		//It is used to position pixel and bitmap write operations. here we are writing implicitly i.e, default x,y,z because its coordinates will change below code for each raster display. 
	for (const char* c = string; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c); //arguments---void *font, int character
	}
}

void init()
{
	glClearColor(0.095, 0.005, 0.228, 0); // to set the background color of the window
	glColor3f(1.0, 0.0, 0.0); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();  //The glLoadIdentity function replaces the current matrix with the identity matrix
	gluOrtho2D(-1200, 1200, -700, 700);                  
	glMatrixMode(GL_MODELVIEW);	//Applies subsequent matrix operations to the modelview matrix stack.
}

void introScreen()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 1.0, 1.0);
	displayRasterText(-125, 225, 0.0, "COSMIC SHOOT");			//here x y z have been explicitly defined coordinates
	glColor3f(1.0, 1.0, 1.0);
	displayRasterText(-300, 100, 0.0, "Press ENTER to start the game");
	glFlush();
	glutSwapBuffers();	//Performs a buffer swap on the layer in use for the current window.
}

void startScreenDisplay()
{
	glBegin(GL_POLYGON);				//START GAME PLOYGON
	glColor3f(0.00095f, 0.0f, 1.0f); // purple
	glVertex2f(-200, 300);
	glColor3f(0.00095f, 0.0f, 1.0f); // purple
	glVertex2f(-200, 400);
	glColor3f(1, 0, 1);
	glVertex2f(200, 400);
	glColor3f(1, 0, 1);
	glVertex2f(200, 300);
	glEnd();

	glBegin(GL_POLYGON);				//INSTRUCTIONS POLYGON
	glColor3f(0.00095f, 0.0f, 1.0f); // purple
	glVertex2f(-200, 50);
	glColor3f(0.00095f, 0.0f, 1.0f); // purple
	glVertex2f(-200, 150);
	glColor3f(1, 0, 1);
	glVertex2f(200, 150);
	glColor3f(1, 0, 1);
	glVertex2f(200, 50);
	glEnd();

	glBegin(GL_POLYGON);				//QUIT POLYGON
	glColor3f(0.00095f, 0.0f, 1.0f); // purple
	glVertex2f(-200, -200);
	glColor3f(0.00095f, 0.0f, 1.0f); // purple
	glVertex2f(-200, -100);
	glColor3f(1, 0, 1);
	glVertex2f(200, -100);
	glColor3f(1, 0, 1);
	glVertex2f(200, -200);
	glEnd();

	if (mX >= -100 && mX <= 100 && mY >= 150 && mY <= 200) {
		glColor3f(1,1,1);
		if (mButtonPressed) {
			alienLife1 = alienLife2 = 100;
			viewPage = GAME;
			mButtonPressed = false;
		}
	}
	else
		glColor3f(0,0,0);
	displayRasterText(-100, 340, 0.4, "Start Game");

	if (mX >= -100 && mX <= 100 && mY >= 30 && mY <= 80) {
		glColor3f(1,1,1);
		if (mButtonPressed) {
			viewPage = INSTRUCTIONS;
			printf("button pressed \n");
			mButtonPressed = false;
		}
	}
	else
		glColor3f(0,0,0);
	displayRasterText(-120, 80, 0.4, "Instructions");

	if (mX >= -100 && mX <= 100 && mY >= -90 && mY <= -40) {
		glColor3f(1,1,1);
		if (mButtonPressed) {
			mButtonPressed = false;
			exit(0);
		}
	}
	else
		glColor3f(0,0,0);
	displayRasterText(-100, -170, 0.4, "    Quit");
	glutPostRedisplay();
}

void backButton() {
	if (mX <= -450 && mX >= -500 && mY >= -275 && mY <= -250) {
		glColor3f(0.0,1,0.0);
		if (mButtonPressed) {
			viewPage = MENU;
			mButtonPressed = false;
			//instructionsGame = false;
			glutPostRedisplay();
		}
	}
	else glColor3f(1, 0, 0);
	displayRasterText(-1000, -550, 0, "Back");
}

void instructionsScreenDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 0, 0);
	displayRasterText(-900, 550, 0.4, "INSTRUCTIONS");
	glColor3f(1, 0, 0);
	displayRasterText(-1000, 400, 0.4, "PLAYER 1");
	displayRasterText(200, 400, 0.4, "PLAYER 2");
	glColor3f(1, 1, 1);
	displayRasterText(-1100, 300, 0.4, "Key 'w' to move up.");
	displayRasterText(-1100, 200, 0.4, "Key 's' to move down.");
	displayRasterText(-1100, 100, 0.4, "Key 'd' to move right.");
	displayRasterText(-1100, 0, 0.4, "Key 'a' to move left.");
	displayRasterText(100, 300, 0.4, "Key 'i' to move up.");
	displayRasterText(100, 200, 0.4, "Key 'k' to move down.");
	displayRasterText(100, 100, 0.4, "Key 'j' to move right.");
	displayRasterText(100, 0, 0.4, "Key 'l' to move left.");
	displayRasterText(-1100, -100, 0.4, "Key 'c' to shoot, Use 'w' and 's' to change direction.");
	displayRasterText(100, -100, 0.4, "Key 'm' to shoot, Use 'i' and 'k' to change direction.");
	displayRasterText(-1100, -300, 0.4, "The Objective is to kill your opponent.");
	displayRasterText(-1100, -370, 0.4, "Each time a player gets shot, LIFE decreases by 20 points.");
	backButton();
}

void DrawSpaceshipBody(bool isPlayer1)
{
	if (isPlayer1)
		glColor3f(0.5, 0.5, 0.5);			//BASE - right side player
	else
		glColor3f(0.998, 0.25, 0.56);

	glPushMatrix();							//Use glPushMatrix and glPopMatrix to save and restore the untranslated coordinate system.
		glScalef(70, 20, 1);
		glutSolidSphere(1, 50, 50);
	glPopMatrix();

	//LIGHTS-spaceship decorations
	glPushMatrix();	
	glScalef(3, 3, 1);

	glTranslated(-20, 0, 0);				//1		The glTranslatef function multiplies the current matrix by a translation matrix.
	glColor3fv(LightColor[(CI + 0) % 3]);			//here LightColor is the array and we are doing modulus operation to fetch an index which represents color in the array. 
	glutSolidSphere(1, 1000, 1000);

	glTranslated(5, 0, 0);					//2
	glColor3fv(LightColor[(CI + 1) % 3]);
	glutSolidSphere(1, 1000, 1000);

	glTranslated(5, 0, 0);					//3
	glColor3fv(LightColor[(CI + 2) % 3]);
	glutSolidSphere(1, 1000, 1000);

	glTranslated(5, 0, 0);					//4
	glColor3fv(LightColor[(CI + 0) % 3]);
	glutSolidSphere(1, 1000, 1000);

	glTranslated(5, 0, 0);					//5
	glColor3fv(LightColor[(CI + 1) % 3]);
	glutSolidSphere(1, 1000, 1000);

	glTranslated(5, 0, 0);					//6
	glColor3fv(LightColor[(CI + 2) % 3]);
	glutSolidSphere(1, 1000, 1000);

	glTranslated(5, 0, 0);					//7
	glColor3fv(LightColor[(CI + 0) % 3]);
	glutSolidSphere(1, 1000, 1000);

	glTranslated(5, 0, 0);					//8
	glColor3fv(LightColor[(CI + 1) % 3]);
	glutSolidSphere(1, 1000, 1000);

	glTranslated(5, 0, 0);					//9
	glColor3fv(LightColor[(CI + 2) % 3]);
	glutSolidSphere(1, 1000, 1000);
	glPopMatrix();
}

void DrawSpaceshipDoom()
{
	glColor4f(0.7, 1, 1, 0.0011); 
	glPushMatrix();
		glTranslated(0, 30, 0); 
		glScalef(35, 50, 1);
		glutSolidSphere(1, 50, 50);
	glPopMatrix();
}

void DrawLaser(int x, int y, bool dir[]) {
	//glPushMatrix();
	int xend = -XMAX, yend = y;
	if (dir[0])
		yend = YMAX;
	else if (dir[1])
		yend = -YMAX;
	glLineWidth(5);								//the glLineWidth function specifies the rasterized width of both aliased and antialiased lines.
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(x, y);
	glVertex2f(xend, yend);
	glEnd();
	//glPopMatrix();
}

void SpaceshipCreate(int x, int y, bool isPlayer1) {
	glPushMatrix();
		glTranslated(x, y, 0);
		DrawSpaceshipDoom();
		glPushMatrix();
			glTranslated(4, 19, 0);
			glPopMatrix();
		DrawSpaceshipBody(isPlayer1);
	
		glEnd();
	glPopMatrix();
}

void displayLife1() {
	char temp1[40];
	glColor3f(1, 1, 1);
	sprintf_s(temp1, " LIFE 1 = %d", alienLife1);
	displayRasterText(-1100, 600, 0.4, temp1);
	glColor3f(1, 0, 0);
}

void displayLife2() {
	char temp2[40];
	glColor3f(1, 1, 1);
	sprintf_s(temp2, "  LIFE 2 = %d", alienLife2);
	displayRasterText(600, 600, 0.4, temp2);
	glColor3f(1, 0, 0);
}

void checkLaserContact(int x, int y, bool dir[], int xp, int yp, bool player1) {
	int xend = -XMAX, yend = y;
	xp += 8; yp += 8; // moving circle slightly up to fix laser issue
	if (dir[0])
		yend = YMAX;
	else if (dir[1])
		yend = -YMAX;

	// Here we find out if the laser(line) intersects with spaceship(circle)
	// by solving the equations for the same and finding the discriminant of the
	// quadratic equation obtained
	float m = (float)(yend - y) / (float)(xend - x);
	float k = y - m * x;
	int r = 50; // approx radius of the spaceship

	//calculating value of b, a, and c needed to find discriminant
	float b = 2 * xp - 2 * m * (k - yp);
	float a = 1 + m * m;
	float c = xp * xp + (k - yp) * (k - yp) - r * r;

	float d = (b * b - 4 * a * c); // discriminant for the equation
	printf("\nDisc: %f x: %d, y: %d, xp: %d, yp: %d", d, x, y, xp, yp);
	if (d >= 0) {
		if (player1)
			alienLife1 -= 20;
		else
			alienLife2 -= 20;

		printf("%d %d\n", alienLife1, alienLife2);
	}
}

void gameScreenDisplay()
{
	displayLife1();
	displayLife2();
	glScalef(2, 2, 0);

	if (alienLife1 > 0) {
		SpaceshipCreate(xOne, yOne, true);
		if (laser1) {
			DrawLaser(xOne, yOne, laser1Dir);
			checkLaserContact(xOne, yOne, laser1Dir, -xTwo, yTwo, true);
		}
	}
	else {
		viewPage = GAMEOVER;
	}

	if (alienLife2 > 0) {
		glPushMatrix();
		glScalef(-1, 1, 1);
		SpaceshipCreate(xTwo, yTwo, false);
		if (laser2) {
			DrawLaser(xTwo, yTwo, laser2Dir);
			checkLaserContact(xTwo, yTwo, laser2Dir, -xOne, yOne, false);
		}
		glPopMatrix();
	}
	else {
		viewPage = GAMEOVER;
	}

	if (viewPage == GAMEOVER) {
		xOne = xTwo = 500;
		yOne = yTwo = 0;
	}
}

void GameOverMessage() {
	glColor3f(0, 1, 0);
	const char* message;
	if (alienLife1 > 0)
		message = "Game Over! Player 1 won the game";
	else
		message = "Game Over! Player 2 won the game";

	displayRasterText(-350, 600, 0.4, message);
}

void keyFunction() {
	if (keyValue[13] == true && viewPage == INTRO) {
		viewPage = MENU;
		printf("view value changed to %d", viewPage);
		printf("enter key pressed\n");
	}
	if (viewPage == GAME) {
		laser1Dir[0] = laser1Dir[1] = false;
		laser2Dir[0] = laser2Dir[1] = false;
		if (keyValue['c'] == true) {
			laser2 = true;
			if (keyValue['w'] == true)
				laser2Dir[0] = true;
			if (keyValue['s'] == true)
				laser2Dir[1] = true;
		}
		else {
			laser2 = false;
			if (keyValue['d'] == true)
				xTwo -= SPEED;
			   if (keyValue['a'] == true)
				xTwo += SPEED;
			if (keyValue['w'] == true)
				yTwo += SPEED;
		  if (keyValue['s'] == true)
				yTwo -= SPEED;
		}

		if (keyValue['m'] == true) {
			laser1 = true;
			if (keyValue['i'] == true)
				laser1Dir[0] = true;
			  if (keyValue['k'] == true)
				laser1Dir[1] = true;
		}
		else {
			laser1 = false;
			if (keyValue['l'] == true)
				xOne += SPEED;
				if (keyValue['j'] == true)
				xOne -= SPEED;
			if (keyValue['i'] == true)
				yOne += SPEED;
		   if (keyValue['k'] == true)
				yOne -= SPEED;
		}
	}
}

void display()
{
	keyFunction();
	glClear(GL_COLOR_BUFFER_BIT);

	switch (viewPage)
	{
	case INTRO:
		introScreen();
		break;
	case MENU:
		startScreenDisplay();
		break;
	case INSTRUCTIONS:
		instructionsScreenDisplay();
		break;
	case GAME:
		gameScreenDisplay();
		glScalef(1 / 2, 1 / 2, 0);
		break;
	case GAMEOVER:
		GameOverMessage();
		startScreenDisplay();
		break;
	}

	glFlush();
	glLoadIdentity();
	glutSwapBuffers();
}

void passiveMotionFunc(int x, int y) {

	//when mouse not clicked
	mX = float(x) / (m_viewport[2] / 1200.0) - 600.0;  //converting screen resolution to ortho 2d spec
	mY = -(float(y) / (m_viewport[3] / 700.0) - 350.0);

	glutPostRedisplay();
}

void mouseClick(int buttonPressed, int state, int x, int y) {

	if (buttonPressed == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		mButtonPressed = true;
	else
		mButtonPressed = false;
	glutPostRedisplay();
}

void keyPressed(unsigned char key, int x, int y)
{
	keyValue[key] = true;
	glutPostRedisplay();
}

void refresh() {
	glutPostRedisplay();
}

void keyReleased(unsigned char key, int x, int y) {
	keyValue[key] = false;
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1200, 600);
	glutCreateWindow("COSMIC SHOOT");
	init();

	glutIdleFunc(refresh);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyReleased);
	glutMouseFunc(mouseClick);
	glutPassiveMotionFunc(passiveMotionFunc);
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	glutDisplayFunc(display);
	glutMainLoop();
}
