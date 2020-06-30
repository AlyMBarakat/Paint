#include <GLUT/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

int Mojave = 1;

int phyWidth= 700;
int phyHeight= 700;
int logWidth=1000;
int logHeight=1000;
int centerX=0,	centerY=0;
int mouseX = 0,	mouseY = 0;

//Drawing Window Borders
int borderX1;
int borderX2;
int borderY1;
int borderY2;

//RED GREEN BLUE YELLOW ORANGE PURPLE WHITE BLACK
float colorArr[8][3] = {{1,0,0} , {0,1,0} , {0,0,1} , {1,1,0.3} , {1,0.6,0.3} , {0.8,0.1,0.9} , {1,1,1} , {0,0,0}};

// i: ID --- j: BCenterX, BCenterY, BState
int textButtonList[8][3];
int buttonWidth = 100;
int buttonHeight = 40;
char buttonText[8][10] = {"Line","Rect","Circle","Fill","Pen","Text","Undo","No fill"};
int colorButtonList[8][2]; //BCenterX, BCenterY

typedef struct {
	int id; 			// Line = 1 ... Rect, Circle, Pen, Text
	float color[3];
	int startPoint[2];	// X=0 , Y=1
	int endPoint[2];	// X=0 , Y=1
	int fill;
	int points[10000][2];
	char text[500];
} Object;

Object objects[1000];	// Objects Array
int fill = 1;
int currentObject = 0;	// Current State Holder
int currentState = 0;	// State Shape Holder
int currentColorIndex = 7; // Index From colorArr[][], Normally Black
int stringSize; //current String size

int click;

int buttonPressed;

void init2D() {
	glClearColor( 1.0, 1.0, 1.0, 0); // Background Color
	glMatrixMode( GL_PROJECTION); 
	gluOrtho2D( 0.0, logWidth, 0.0, logHeight);
	centerX = logWidth/2;
	centerY = logHeight/2;
}

/* Drawing Objects Handlers */

// Buttons GUI Actions and States  
void sendFeedBack(int mouseX, int mouseY, int state) {
	for(int i =0; i < 8; i++) {
		//Setting Text Box states
		if(mouseX>(textButtonList[i][0]-buttonWidth/2) && mouseX<(textButtonList[i][0]+buttonWidth/2) &&
		   mouseY>(textButtonList[i][1]-buttonWidth/2) && mouseY<(textButtonList[i][1]+buttonWidth/2)) {
	   		if(state == 1 && textButtonList[i][2] == 0) { // Button is Hovered
	   			textButtonList[i][2] = 1;
	   		}
	   		if(state == 2) {	// Button is clicked
	   			textButtonList[i][2] = 2;
	   			buttonPressed = 1;
	   			if(i == 0) { //line
		   			textButtonList[1][2] = 0;
		   			textButtonList[2][2] = 0;
		   			textButtonList[4][2] = 0;
		   			textButtonList[5][2] = 0;
		   			textButtonList[6][2] = 0;	
		   			currentState = 1;	
	   			}
	   			else if(i == 1) { //rect
	   				textButtonList[0][2] = 0;
		   			textButtonList[2][2] = 0;
		   			textButtonList[4][2] = 0;
		   			textButtonList[5][2] = 0;
		   			textButtonList[6][2] = 0;
		   			currentState = 2;
	   			}
	   			else if(i == 2) { //circle
	   				textButtonList[0][2] = 0;
		   			textButtonList[1][2] = 0;
		   			textButtonList[4][2] = 0;
		   			textButtonList[5][2] = 0;
		   			textButtonList[6][2] = 0;
		   			currentState = 3;
	   			}
	   			else if(i == 3) { //fill
	   				fill = 1;
	   				textButtonList[7][2] = 0;
	   			}
	   			else if(i == 4) { //pen
	   				textButtonList[0][2] = 0;
		   			textButtonList[1][2] = 0;
		   			textButtonList[2][2] = 0;
		   			textButtonList[5][2] = 0;
		   			textButtonList[6][2] = 0;
		   			currentState = 4;
	   			}
	   			else if(i == 5) { //text
	   				textButtonList[0][2] = 0;
		   			textButtonList[1][2] = 0;
		   			textButtonList[2][2] = 0;
		   			textButtonList[4][2] = 0;
		   			textButtonList[6][2] = 0;
		   			currentState = 5;
	   			}
	   			else if(i == 7) { //no fill
	   				fill = 0;
	   				textButtonList[3][2] = 0;
	   			}
	   			if( i == 6) { //undo
	   				if(currentObject > 0) {
	   					currentObject--;
	   				} 
	   			}
	   		}
		}
		else if(textButtonList[i][2] != 2) // UnClicked
			textButtonList[i][2] = 0;

		//Setting Color Boxes States
		if(mouseX>(colorButtonList[i][0] - 20) && mouseX<(colorButtonList[i][0] + 20) &&
		   mouseY>(colorButtonList[i][1] - 20) && mouseY<(colorButtonList[i][1] + 20)) { 
		   	if(state == 2) { // Color is clicked
		   		currentColorIndex = i;
		   	}
		}
	}
}
// Current Object Values Setter
void setCurrentObject() {
	printf("%d\n",currentObject);
	currentObject++;
	objects[currentObject].startPoint[0] = mouseX;
	objects[currentObject].startPoint[1] = mouseY;
	objects[currentObject].endPoint[0] = mouseX;
	objects[currentObject].endPoint[1] = mouseY;
	objects[currentObject].id = currentState;
	objects[currentObject].fill = fill;
	objects[currentObject].color[0] = colorArr[currentColorIndex][0]; // R
	objects[currentObject].color[1] = colorArr[currentColorIndex][1]; // G
	objects[currentObject].color[2] = colorArr[currentColorIndex][2]; // B
	memset(objects[currentObject].text, 0, sizeof(objects[currentObject].text));
	memset(objects[currentObject].points, 0, sizeof(objects[currentObject].points));
}


/* Call Back functions*/

// Keyboard Charachters Handler
void Keyboard(unsigned char key, int x, int y) {
	stringSize = strlen(objects[currentObject].text);
	if(key == 127) {
		objects[currentObject].text[--stringSize] = (char) 0;
	}
	else {
		strncat(objects[currentObject].text,&key,1);
	}
	glutPostRedisplay();
}
// Print Text
void printSome(char *str,int x,int y ,int colored) { 
	if(colored)
		glColor3f(objects[colored].color[0],objects[colored].color[1],objects[colored].color[2]);
	else
		glColor3f(0,0,0);
	glRasterPos2d(x,y);
	for(int i=0;i<strlen(str);i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
		glFlush();
	}
}
// On Hovering(Passive Motion)
void passiveMouse(int x, int y){
	mouseX = x;
	mouseX = 0.5+1.0*mouseX*logWidth/phyWidth;
	mouseY = phyHeight - y;
	mouseY = 0.5+1.0*mouseY*logHeight/phyHeight;
	for(int i =0; i < 8; i++) { 
		if(mouseX>(textButtonList[i][0]-buttonWidth/2) && mouseX<(textButtonList[i][0]+buttonWidth/2) &&
		   mouseY>(textButtonList[i][1]-buttonHeight/2) && mouseY<(textButtonList[i][1]+buttonHeight/2)) {
		   	sendFeedBack(mouseX,mouseY,1);
		}
	}
	glutPostRedisplay();
}
// On Mouse Click
void mouseClick(int btn, int state, int x, int y) {
	if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		mouseX = x;
		mouseX = 0.5 + 1.0 * mouseX * logWidth/phyWidth;
		mouseY = phyHeight - y;
		mouseY = 0.5 + 1.0 * mouseY * logHeight/phyHeight;
		sendFeedBack(mouseX, mouseY, 2);
		// On mouse click inside boarders
		if(mouseX > borderX1  && mouseX < borderX2 && mouseY > borderY1 && mouseY < borderY2) {
			setCurrentObject();
			click = 1;
		}
	}
	else{
		click = 0;
	}
	if(btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		exit(1); //to exit program
	}
	glutPostRedisplay();
}
int pointNumb = 0;
// On Drag and Drop
void dragMouse(int x, int y) {
	mouseX = x;
	mouseX = 0.5 + 1.0 * mouseX * logWidth/phyWidth;
	mouseY = phyHeight - y;
	mouseY = 0.5 + 1.0 * mouseY * logHeight/phyHeight;
	
	if(mouseX > borderX1  && mouseX < borderX2 && mouseY > borderY1 && mouseY < borderY2){
		if(click)
		{
			objects[currentObject].endPoint[0] = mouseX;
			objects[currentObject].endPoint[1] = mouseY;
		}
		if(objects[currentObject].id == 4) { // Handling pen points
			objects[currentObject].points[pointNumb][0] = mouseX;
			objects[currentObject].points[pointNumb][1] = mouseY;
			pointNumb++;
		}
	}
	glutPostRedisplay();
}


/* GUI */

// Drawing Area
void drawWindow(int sideBorderWidth, int upperBoarderFactor) {
	//Window Borders
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0,0,0);
	//Setting Borders Values
	borderX1 = centerX - logWidth/2 + sideBorderWidth ;
	borderX2 = centerX + logWidth/2 - sideBorderWidth ;
	borderY1 = centerY - logHeight/2 + sideBorderWidth;
	borderY2 = centerY + logHeight/2 - sideBorderWidth*upperBoarderFactor;
	//Drawing Borders
	glBegin(GL_LINE_STRIP);
	glVertex2i(borderX1, borderY1);
	glVertex2i(borderX1, borderY2);
	glVertex2i(borderX2, borderY2);
	glVertex2i(borderX2, borderY1);
	glVertex2i(borderX1, borderY1);
	glEnd();
}
// Selection Buttons
void drawTextButtons(int sideBorderWidth) {
	int buttonsCounter = 0;
	int colorState;
	float buttonColorState[3][3] = {{1,1,1} , {0.9,0.8,0.4} , {0.9,0.6,0.1}};	
	int spacerX = 0;
	int spacerY = 0;
	
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 4; j++) {
			//Calculating Button's Center
			textButtonList[buttonsCounter][0] = centerX - logWidth/2 + sideBorderWidth + buttonWidth/2 + spacerX;
			textButtonList[buttonsCounter][1] = centerY + logHeight/2 - 50 - spacerY;
			//Outer Box
			glColor3f(0,0,0);
			glBegin(GL_POLYGON);
			glVertex2i(textButtonList[buttonsCounter][0] - buttonWidth/2 , textButtonList[buttonsCounter][1] - buttonHeight/2);
			glVertex2i(textButtonList[buttonsCounter][0] - buttonWidth/2 , textButtonList[buttonsCounter][1] + buttonHeight/2);
			glVertex2i(textButtonList[buttonsCounter][0] + buttonWidth/2 , textButtonList[buttonsCounter][1] + buttonHeight/2);
			glVertex2i(textButtonList[buttonsCounter][0] + buttonWidth/2 , textButtonList[buttonsCounter][1] - buttonHeight/2);
			glEnd();
			if( (fill && i == 0 && j == 3) || (!fill && i == 1 && j == 3)) 
				colorState = 2;
			else 
				colorState = textButtonList[buttonsCounter][2];
			//Inner Box
			glColor3f(buttonColorState[colorState][0], buttonColorState[colorState][1], buttonColorState[colorState][2]);
			glBegin(GL_POLYGON);
			glVertex2i(textButtonList[buttonsCounter][0] - buttonWidth/2 + 5, textButtonList[buttonsCounter][1] - buttonHeight/2 + 5);
			glVertex2i(textButtonList[buttonsCounter][0] - buttonWidth/2 + 5, textButtonList[buttonsCounter][1] + buttonHeight/2 - 4);
			glVertex2i(textButtonList[buttonsCounter][0] + buttonWidth/2 - 4, textButtonList[buttonsCounter][1] + buttonHeight/2 - 4);
			glVertex2i(textButtonList[buttonsCounter][0] + buttonWidth/2 - 4, textButtonList[buttonsCounter][1] - buttonHeight/2 + 5);
			glEnd();
			//Text
			printSome(buttonText + buttonsCounter,textButtonList[buttonsCounter][0] - 30, textButtonList[buttonsCounter][1] - 10, 0);
			//Spacing
			spacerX += buttonWidth + sideBorderWidth;
			buttonsCounter++;
		}
		//Spacing
		spacerX = 0;
		spacerY += buttonHeight + sideBorderWidth;
	}
}
// Color Buttons
void drawColorButtons(int upperBorderWidth) {
	int colorButtonCenterX;
	int colorButtonCenterY;
	int buttonWidth = 20;
	int spacerX = 0;

	for(int i = 0; i < 8; i++) {
		colorButtonCenterX = centerX + 75 + spacerX;
		colorButtonCenterY = centerY + logHeight/2 - upperBorderWidth/2;
		colorButtonList[i][0] = colorButtonCenterX;
		colorButtonList[i][1] = colorButtonCenterY;	
		//Outer Color Button
		glColor3f(0,0,0);
		glBegin(GL_POLYGON);
		glVertex2i(colorButtonCenterX - buttonWidth  ,colorButtonCenterY - buttonWidth);
		glVertex2i(colorButtonCenterX - buttonWidth  ,colorButtonCenterY + buttonWidth);
		glVertex2i(colorButtonCenterX + buttonWidth  ,colorButtonCenterY + buttonWidth);
		glVertex2i(colorButtonCenterX + buttonWidth  ,colorButtonCenterY - buttonWidth);
		glEnd();
		//Inner Color Button
		glColor3f(colorArr[i][0],colorArr[i][1],colorArr[i][2]);
		glBegin(GL_POLYGON);
		glVertex2i(colorButtonCenterX - buttonWidth + 3  ,colorButtonCenterY - buttonWidth + 3);
		glVertex2i(colorButtonCenterX - buttonWidth + 3  ,colorButtonCenterY + buttonWidth - 3);
		glVertex2i(colorButtonCenterX + buttonWidth - 3  ,colorButtonCenterY + buttonWidth - 3);
		glVertex2i(colorButtonCenterX + buttonWidth - 3  ,colorButtonCenterY - buttonWidth + 3);
		glEnd();
		//Spacing
		spacerX += 55;
	}
}
// Setting GUI
void gui() {
	int sideBorderWidth = 20;
	int upperBoarderFactor = 8;

	drawWindow(sideBorderWidth, upperBoarderFactor);
	drawTextButtons(sideBorderWidth);
	drawColorButtons(sideBorderWidth * upperBoarderFactor);
}


/* Displaying Drawings */

// Line Object
void drawLine(int i) {
	glColor3f(objects[i].color[0], objects[i].color[1], objects[i].color[2]);
	glBegin(GL_LINE_STRIP);
	glVertex2i(objects[i].startPoint[0],objects[i].startPoint[1]);
	glVertex2i(objects[i].endPoint[0],objects[i].endPoint[1]);
	glEnd();
}
// Rectangle Object
void drawRect(int i) {
	glColor3f(objects[i].color[0], objects[i].color[1], objects[i].color[2]);
	if(objects[i].fill) {
		glBegin(GL_POLYGON);
		glVertex2i(objects[i].startPoint[0],objects[i].startPoint[1]);
		glVertex2i(objects[i].startPoint[0],objects[i].endPoint[1]);
		glVertex2i(objects[i].endPoint[0],objects[i].endPoint[1]);
		glVertex2i(objects[i].endPoint[0],objects[i].startPoint[1]);
		glEnd();	
	}
	else {
		glBegin(GL_LINE_LOOP);
		glVertex2i(objects[i].startPoint[0],objects[i].startPoint[1]);
		glVertex2i(objects[i].startPoint[0],objects[i].endPoint[1]);
		glVertex2i(objects[i].endPoint[0],objects[i].endPoint[1]);
		glVertex2i(objects[i].endPoint[0],objects[i].startPoint[1]);
		glEnd();
	}
}
// Circle Object
void drawCircle(int i) {
	glColor3f(objects[i].color[0], objects[i].color[1], objects[i].color[2]);
	int num_segments = 50;
	float r = (float)objects[i].startPoint[0] - (float)objects[i].endPoint[0];
	if(objects[i].fill) {
		glBegin(GL_POLYGON);
		for (int j = 0; j < num_segments; j++) {
			float theta = 2.0f * 3.1415926f * (float)j / (float)num_segments; //get the current angle in radians
			float x = r * cosf(theta);//calculate the x component
			float y = r * sinf(theta);//calculate the y component
			if ((x + (float)objects[i].startPoint[0] > borderX1)  && x + (float)objects[i].startPoint[0] < borderX2 && y + (float)objects[i].startPoint[1] > borderY1 && y + (float)objects[i].startPoint[1] < borderY2) {
				glVertex2f(x + (float)objects[i].startPoint[0] , y + (float)objects[i].startPoint[1]);//output vertex
			}
		}
		glEnd();
	}
	else {
		glBegin(GL_LINE_LOOP);
		for (int j = 0; j < num_segments; j++) {
			float theta = 2.0f * 3.1415926f * (float)j / (float)num_segments; //get the current angle in radians
			float x = r * cosf(theta);//calculate the x component
			float y = r * sinf(theta);//calculate the y component
			if ((x + (float)objects[i].startPoint[0] > borderX1)  && x + (float)objects[i].startPoint[0] < borderX2 && y + (float)objects[i].startPoint[1] > borderY1 && y + (float)objects[i].startPoint[1] < borderY2) {
				glVertex2f(x + (float)objects[i].startPoint[0] , y + (float)objects[i].startPoint[1]);//output vertex
			}
		}
		glEnd();
	}
}
// Pen Object
void drawPen(int i) {
	glColor3f(objects[i].color[0], objects[i].color[1], objects[i].color[2]);
	glBegin(GL_POINTS);
	glVertex2i(objects[i].startPoint[0] , objects[i].startPoint[1]);
	for(int j = 0; j < 10000 ; j ++) {
		glVertex2i(objects[i].points[j][0] , objects[i].points[j][1]);
	}
 	glEnd();
}
// Text Object
void drawText(int i) {
	printSome(objects[i].text, objects[i].startPoint[0], objects[i].startPoint[1], i);
}
// Drawing Objects Loop
void drawObjects() {
	if(currentObject < 0)
		return;
	for (int i = 0; i <= currentObject; i++)
	{
		if(objects[i].id == 1)
			drawLine(i);
		else if(objects[i].id == 2)
			drawRect(i);
		else if(objects[i].id == 3)
			drawCircle(i);
		else if(objects[i].id == 4)
			drawPen(i);
		else if(objects[i].id == 5)
			drawText(i);
	}
}


void Display() {
	if(Mojave){
		glutReshapeWindow(700, 701);
		Mojave = 0;
	}
	glClear(GL_COLOR_BUFFER_BIT);
	gui();
	drawObjects();
	glutSwapBuffers();
	glFlush();
}

int main( int argc, char ** argv) {
	glutInit( &argc, argv);
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition( 0, 100);
	glutInitWindowSize(phyWidth, phyHeight);
	glutCreateWindow( "Paint");
	init2D();
	glutMouseFunc(mouseClick);
	glutPassiveMotionFunc(passiveMouse); 
	glutMotionFunc(dragMouse);
	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(Display);
	glutMainLoop();
}