#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#define numOfShapes 7
// Derived from trinangles.c

struct piece {
	int coordinates[4][2];
	int size;
	int currentTranslation[2];
	int currentRotaion;
	int center[2];
	int isSelected;
} typedef PuzzlePiece;
PuzzlePiece* puzzle;
PuzzlePiece* selectedShape;
int shapeCoordinates[7][4][2] = { { { -100, 100 },{ 0, 100 },{ -100, 0 },{ 0, 0 } }, //for triangles last coordinate will not draw
{ { 0, 100 },{ 50, 50 },{ 0, 0 },{ -50, 50 } }, 
{ { 0, 100 },{ 100, 100 },{ 50, 50 },{ 0, 0 } },
{ { 100, 100 },{ 100, -100 },{ 0, 0 },{ 0, 0 } },
{ { -50, 50 },{ -50, -50 },{ -100, -100 },{ -100, 0 } },
{ { -50, 50 },{ 0, 0 },{ -50, -50 },{ 0, 0 } },
{ { 0,0 },{ 100, -100 },{ -100, -100 },{ 0, 0 } } };

int vertexArray[7] = { 3, 4, 3, 3, 4, 3, 3 };

void calculateCenter(PuzzlePiece* piece) {
	int i = 0;
	piece->center[0] = 0;
	piece->center[1] = 0;

	for (i = 0; i < piece->size; i++) {
		piece->center[0] += piece->coordinates[i][0];
		piece->center[1] += piece->coordinates[i][1];
	}
	piece->center[0] /= piece->size;
	piece->center[1] /= piece->size;
	3;
}

void shapify() {
	puzzle = (PuzzlePiece *)malloc(numOfShapes * sizeof(PuzzlePiece));
	int i, j;
	for (i = 0; i < numOfShapes; i++) {
		puzzle[i].size = vertexArray[i];
		for (j = 0; j < puzzle[i].size; j++) {
			puzzle[i].coordinates[j][0] = shapeCoordinates[i][j][0];	
			puzzle[i].coordinates[j][1] = shapeCoordinates[i][j][1];
		}
		calculateCenter(&puzzle[i]);
		puzzle[i].currentRotaion = 0;
		puzzle[i].currentTranslation[0] = i + 1 % 2 * 10;
		puzzle[i].currentTranslation[1] = i % 2* 10;
		puzzle[i].isSelected = 0;
	}
}

void drawShapes(PuzzlePiece puzzlePiece)
{
	if (puzzlePiece.isSelected) {
		glColor3f(0, 1, 0);
	}
	if (puzzlePiece.size == 3) {
		glTranslatef( puzzlePiece.center[0] + puzzlePiece.currentTranslation[0],
			 puzzlePiece.center[1] + puzzlePiece.currentTranslation[1],
			0);
		glRotatef(puzzlePiece.currentRotaion, 0, 0, 1);
		glTranslatef(- ( puzzlePiece.center[0] + puzzlePiece.currentTranslation[0]),
			-(puzzlePiece.center[1] + puzzlePiece.currentTranslation[1]),
			0);
		glTranslatef(puzzlePiece.currentTranslation[0], puzzlePiece.currentTranslation[1], 0);
		glBegin(GL_TRIANGLES);
		glVertex2i(puzzlePiece.coordinates[0][0], puzzlePiece.coordinates[0][1]);
		glVertex2i(puzzlePiece.coordinates[1][0], puzzlePiece.coordinates[1][1]);
		glVertex2i(puzzlePiece.coordinates[2][0], puzzlePiece.coordinates[2][1]);
		glEnd();
	}
	else {
		glTranslatef(puzzlePiece.center[0] + puzzlePiece.currentTranslation[0],
			puzzlePiece.center[1] + puzzlePiece.currentTranslation[1],
			0);
		glRotatef(puzzlePiece.currentRotaion, 0, 0, 1);
		glTranslatef(-(puzzlePiece.center[0] + puzzlePiece.currentTranslation[0]),
			-(puzzlePiece.center[1] + puzzlePiece.currentTranslation[1]),
			0);
		glTranslatef(puzzlePiece.currentTranslation[0], puzzlePiece.currentTranslation[1], 0);
		glBegin(GL_QUADS);
		glVertex2i(puzzlePiece.coordinates[0][0], puzzlePiece.coordinates[0][1]);
		glVertex2i(puzzlePiece.coordinates[1][0], puzzlePiece.coordinates[1][1]);
		glVertex2i(puzzlePiece.coordinates[2][0], puzzlePiece.coordinates[2][1]);
		glVertex2i(puzzlePiece.coordinates[3][0], puzzlePiece.coordinates[3][1]);
		glEnd();
	}
}

void clearSelections() {
	int i;
	for (i = 0; i < numOfShapes; i++) {
		puzzle[i].isSelected = 0;
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	int i;
	for (i = 0; i<numOfShapes; i++) {
		glColor3f(1, 0, 0);
		glPushMatrix();
		drawShapes(puzzle[i]);
		glPopMatrix();

	}
	glutSwapBuffers();

}

void processNormalKeys(unsigned char key, int x, int y) {

	switch (key) {
		case 27:
			exit(0);
			break;
		case 'w':
			if (selectedShape)
				selectedShape->currentTranslation[1]+=3;
			break;
		case 'a':
			if (selectedShape)
				selectedShape->currentTranslation[0] -= 3;
			break;
		case 's':
			if (selectedShape)
				selectedShape->currentTranslation[1] -= 3;
			break;
		case 'd':
			if (selectedShape)
				selectedShape->currentTranslation[0] += 3;
			break;
		case 'r':
			if (selectedShape)
				selectedShape->currentRotaion += 3;
			break;
		default:
			if (key > 47 && key < 55) {
				selectedShape = &puzzle[key - 48];
				clearSelections();
				selectedShape->isSelected = 1;
			}
			else {
				clearSelections();
				selectedShape = NULL;
			}
	}
	glutPostRedisplay();
}

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-300.0, 300.0, -300.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	shapify();
	selectedShape = NULL;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(600, 600);
	glutCreateWindow("HOMEWORK 3 | Tangram");
	glutKeyboardFunc(processNormalKeys);
	init();
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
