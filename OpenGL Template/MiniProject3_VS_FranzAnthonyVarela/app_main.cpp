#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <string.h>
#include <cmath>
#include <math.h>
#if defined WIN32
#include <freeglut.h>
#elif defined __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif


using namespace std;

class Square {
	float x, y, s;
public:
	float r, g, b;
	bool clicked, xTurn, oTurn;
	Square(): x(0), y(0), s(0.05) {}

	//main constructor for squares
	Square(float x, float y, float r, float g, float b) : s(0.15), clicked(false), xTurn(false), oTurn(false){
		this->x = x * .2;
		this->y = y * .2;
		this->r = r;
		this->g = g;
		this->b = b;	

	}

	//Print the coordinates (for my own testing)
	void printCoordinates() {
		cout << x << endl;
		cout << y << endl;
		cout << x + s << endl;
		cout << y - s << endl;
	}

	//How each square will be drawn in the appDrawScene method
	void draw() {

		if (xTurn) {
			//r = 0;
			//draws the x
			glColor3f(1, 0, 0);

			glBegin(GL_LINES);
			glVertex2f(x, y);
			glVertex2f(x+s, y-s);
			glVertex2f(x + s, y);
			glVertex2f(x, y - s);
			glEnd();

		}
		else if (oTurn) { 
			//b = 0;
			//draws the o
			float xCenter = (x + (x + s)) / 2;
			float yCenter = (y + (y - s)) / 2;

			glColor3f(0, 1, 0);
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i <= 300; i++) {
				float angle = 2 * M_PI * i / 300;
				float x = xCenter + s * .5* cos(angle);
				float y = yCenter + s * .5* sin(angle);
				glVertex2f(x, y); 
			}
			glEnd();
		}

		//backdrop square
		glColor3f(r, g, b);

		glBegin(GL_POLYGON);

		glVertex2f(x, y);
		glVertex2f(x, y - s);
		glVertex2f(x + s, y - s);	
		glVertex2f(x + s, y);

		glEnd();
	}

	bool contains(float mx, float my) {
		return (mx >= x) && (mx <= (x + s)) &&( my <= y) && (my >= (y - s));
	}
};

// Store the width and height of the window
int width = 640, height = 640;

vector< vector<Square*> > grid;

Square PVP, PVAI;

bool turn;
bool PvAI, PvP;
bool menu;
bool p1W, p2W;
bool gameOver;

int moves;

//checks the end condition (whether it's a win or a tie)
void checkEnd() {
	//vertical check
	for (int i = 0; i < grid.size(); i++) {
		if (grid[i][0]->clicked && grid[i][1]->clicked && grid[i][2]->clicked) {
			if (grid[i][0]->xTurn && grid[i][1]->xTurn && grid[i][2]->xTurn) {
				cout << "Player One Wins!" << endl;
				p1W = true;
				gameOver = true;
			}
			else if (grid[i][0]->oTurn && grid[i][1]->oTurn && grid[i][2]->oTurn) {
				cout << "Player Two Wins!" << endl;
				p2W = true;
				gameOver = true;
			}
		}
	}

	//horizontal check
	for (int i = 0; i < grid.size(); i++) {
		if (grid[0][i]->clicked && grid[1][i]->clicked && grid[2][i]->clicked) {
			if (grid[0][i]->xTurn && grid[1][i]->xTurn && grid[2][i]->xTurn) {
				cout << "Player One Wins!" << endl;
				p1W = true;
				gameOver = true;
			}
			else if (grid[0][i]->oTurn && grid[1][i]->oTurn && grid[2][i]->oTurn) {
				cout << "Player Two Wins!" << endl;
				p2W = true;
				gameOver = true;
			}
		}
	}

	//diagonal check
	if ((grid[0][0]->xTurn && grid[1][1]->xTurn && grid[2][2]->xTurn) || (grid[0][2]->xTurn && grid[1][1]->xTurn && grid[2][0]->xTurn)) {
		cout << "Player One Wins!" << endl;
		p1W = true;
		gameOver = true;
	}
	else if ((grid[0][0]->oTurn && grid[1][1]->oTurn && grid[2][2]->oTurn) || (grid[0][2]->oTurn && grid[1][1]->oTurn && grid[2][0]->oTurn)) {
		cout << "Player Two Wins!" << endl;
		p2W = true;
		gameOver = true;
	}

	//if maximum amount of moves with no valid wins, game is over
	if (moves == 9 || (PvAI && moves == 5)) {
		cout << "Game Over, nobody wins!" << endl;
		gameOver = true;
	}
}

//draws text (courtesy of alyakhtar's method @github)
void drawString(void * font, char *s, float x, float y) {
	glRasterPos2f(x, y);
	for (unsigned int i = 0; i < strlen(s); i++) {
		glutBitmapCharacter(font, s[i]);
	}
}
//-------------------------------------------------------
// A function to draw the scene
//-------------------------------------------------------
void appDrawScene() {
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set background color to black
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Set up the transformations stack
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    // Draw stuff here
	if (menu) {
		PVP.draw();
		PVAI.draw();

		glColor3f(1.0, 1.0, 1.0);
		drawString(GLUT_BITMAP_HELVETICA_18, "Choose a mode: Green for PVP, Red for PVAI", -0.9, 0.9);
	}
	else {
		//Grid of Squares 
		if (turn && !gameOver) {
			glColor3f(1.0, 1.0, 1.0);
			drawString(GLUT_BITMAP_HELVETICA_18, "Player 1's Turn", -0.9, 0.9);
		}
		else if (!turn && !gameOver) {
			glColor3f(1.0, 1.0, 1.0);
			drawString(GLUT_BITMAP_HELVETICA_18, "Player 2's Turn", -0.9, 0.9);
		}
		for (int i = 0; i < grid.size(); i++) {
			for (int j = 0; j < grid[i].size(); j++) {
				grid[i][j]->draw();
			}
		}
	}

	//draws the end text when the game is over

	if (p1W) {
		glColor3f(1.0, 0, 0);
		drawString(GLUT_BITMAP_HELVETICA_18, "Player 1 Wins!", -0.9, 0.9);
	}
	else if (p2W) {
		glColor3f(0, 1.0, 0);
		drawString(GLUT_BITMAP_HELVETICA_18, "Player 2 Wins!", -0.9, 0.9);
	}
	else if (!p1W && !p2W && gameOver) {
		glColor3f(1 ,1 , 1);
		drawString(GLUT_BITMAP_HELVETICA_18, "Nobody Wins!", -0.9, 0.9);
	}
	// We have been drawing everything to the back buffer
	// Swap the buffers to see the result of what we drew
	glFlush();
	glutSwapBuffers();
}

//-------------------------------------------------------
// A function to convert window coordinates to scene
// We use it when a mouse event is handled
// Arguments: 	
//	x, y - the coordinates to be updated
//-------------------------------------------------------
void windowToScene(float& x, float& y) {
	x = (2.0f*(x / float(width))) - 1.0f;
	y = 1.0f - (2.0f*(y / float(height)));
}

//-------------------------------------------------------
// A function to handle window resizing
// Called every time the window is resized
// Arguments: 	
//	b    - mouse button that was clicked, left or right
//	s 	 - state, either mouse-up or mouse-down
//	x, y - coordinates of the mouse when click occured
//-------------------------------------------------------
void appReshapeFunc(int w, int h) {
	// Window size has changed
	width = w;
	height = h;

	double scale, center;
	double winXmin, winXmax, winYmin, winYmax;

	// Define x-axis and y-axis range
	const double appXmin = -1.0;
	const double appXmax = 1.0;
	const double appYmin = -1.0;
	const double appYmax = 1.0;

	// Define that OpenGL should use the whole window for rendering
	glViewport(0, 0, w, h);

	// Set up the projection matrix using a orthographic projection that will
	// maintain the aspect ratio of the scene no matter the aspect ratio of
	// the window, and also set the min/max coordinates to be the disered ones
	w = (w == 0) ? 1 : w;
	h = (h == 0) ? 1 : h;

	if ((appXmax - appXmin) / w < (appYmax - appYmin) / h) {
		scale = ((appYmax - appYmin) / h) / ((appXmax - appXmin) / w);
		center = (appXmax + appXmin) / 2;
		winXmin = center - (center - appXmin)*scale;
		winXmax = center + (appXmax - center)*scale;
		winYmin = appYmin;
		winYmax = appYmax;
	}
	else {
		scale = ((appXmax - appXmin) / w) / ((appYmax - appYmin) / h);
		center = (appYmax + appYmin) / 2;
		winYmin = center - (center - appYmin)*scale;
		winYmax = center + (appYmax - center)*scale;
		winXmin = appXmin;
		winXmax = appXmax;
	}

	// Now we use glOrtho to set up our viewing frustum
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(winXmin, winXmax, winYmin, winYmax, -1, 1);
}


//-------------------------------------------------------
// A function to handle mouse clicks
// Called every time the mouse button goes up or down
// Arguments: 	
//	b    - mouse button that was clicked, left or right
//	s 	 - state, either mouse-up or mouse-down
//	x, y - coordinates of the mouse when click occured
//-------------------------------------------------------
void appMouseFunc(int b, int s, int x, int y) {
	// Convert from Window to Scene coordinates
	float mx = (float)x;
	float my = (float)y;

	windowToScene(mx, my);

	if (!gameOver) {
		if (s == 1) {
			if (menu) {
				//if PVP
				if (PVP.contains(mx, my) && !PVP.clicked) {
					PvP = true;
					menu = false;
					cout << "Game starting... PvP" << endl;
				}
				//if PVAI
				else if (PVAI.contains(mx, my) && !PVAI.clicked) {
					PvAI = true;
					menu = false;
					cout << "Game starting... PvAI" << endl;
				}
			}
			//After mode select, goes to this else to start the game
			else {
				//the next two for loops iterates through the entire grid
				for (int i = 0; i < grid.size(); i++) {
					for (int j = 0; j < grid[i].size(); j++) {
						//this if checks if the mouse clicks over a square THAT HAS NOT BEEN CLICKED BEFORE
						if (grid[i][j]->contains(mx, my) && !grid[i][j]->clicked) {
							//On player one's turn, make X's
							if (turn) {
								grid[i][j]->xTurn = true;
								grid[i][j]->clicked = true;
								//if PvAI is enabled, then it will automatically fill in the next available square
								if (PvAI) {
									//checks if the game has ended with the previous move first
									checkEnd();
									if (!gameOver) {
										for (int k = 0; k < grid.size(); k++) {
											bool found = false;
											for (int l = 0; l < grid[k].size(); l++) {
												if (!grid[k][l]->clicked && !grid[k][l]->xTurn) {
													found = true;
													grid[k][l]->oTurn = true;
													cout << "AI moves @: " << k << " " << l << endl;
													grid[k][l]->clicked = true;
													break;
												}
											}
											if (found) {
												break;
											}
										}
									}		
								}
								else if (PvP) {
									turn = false;
								}
								cout << "xTurn: " << grid[i][j]->xTurn << endl;
								cout << "oTurn: " << grid[i][j]->oTurn << endl;

							}
							else {
								//If you chose PvP, self explanatory 
								if (PvP) {
									grid[i][j]->oTurn = true;
									grid[i][j]->clicked = true;
									turn = true;
								}
								cout << "oTurn: " << grid[i][j]->oTurn << endl;
								cout << "xTurn: " << grid[i][j]->xTurn << endl;
							}
							cout << "CLICKED: " << i << " " << j << endl;
							//grid[i][j]->printCoordinates();
							moves++;
							cout << moves << endl;
							//whenever you click, and youre in game, at the end of everything, it'll check if it's the game's end 
							checkEnd();
						}
					}
				}
			}

		}
	}

	

	// Redraw the scene by calling appDrawScene above
	glutPostRedisplay();
}

//-------------------------------------------------------
// A function to handle mouse dragging
// Called every time mouse moves while button held down
// Arguments: 	
//	x, y - current coordinates of the mouse
//-------------------------------------------------------
void appMotionFunc(int x, int y) {

	// Again, we redraw the scene
	glutPostRedisplay();
}

//-------------------------------------------------------
// A function to handle keyboard events
// Called every time a key is pressed on the keyboard
// Arguments: 	
//	key  - the key that was pressed
//	x, y - coordinates of the mouse when key is pressed
//-------------------------------------------------------
void appKeyboardFunc(unsigned char key, int x, int y) {

	if (key == 27) {
		for (int i = 0; i < grid.size(); i++) {
			for (int j = 0; j < grid[i].size(); j++) {
				delete grid[i][j];
				cout << "Deleted Square @" << i << " " << j << endl;
			}
		}
		exit(0);
	}
	

	// After all the state changes, redraw the scene
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

	//initial state of some variables 
	turn = true;
	menu = true;
	PvP = false;
	PvAI = false;
	gameOver = false;

	//menu squares
	PVP = Square(-2, -1, 0, 1, 0);
	PVAI = Square(-1, -1, 1, 0, 0);

	//keeps track of the total amount of moves
	moves = 0;

	cout << "Menu: Mode Select" << endl;
	cout << "Green square is PVP, Red square is PVAI" << endl;
	// Initialize the grid
	grid.resize(3);
	for (int i = 0; i < 3; i++) {
		grid[i].resize(3);
		for (int j = 0; j < 3; j++) {
			grid[i][j] = new Square(i, j, 1, 1, 1);
			//cout << "Square @ " << i << " " << j << endl;
			//grid[i][j]->printCoordinates();
		}
	}

	// Setup window position, size, and title
	glutInitWindowPosition(20, 60);
	glutInitWindowSize(width, height);
	glutCreateWindow("Tic Tac Toe");

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);


	// Set callback for drawing the scene
	glutDisplayFunc(appDrawScene);

	// Set callback for resizing th window
	glutReshapeFunc(appReshapeFunc);

	// Set callback to handle mouse clicks
	glutMouseFunc(appMouseFunc);

	// Set callback to handle mouse dragging
	glutMotionFunc(appMotionFunc);

	// Set callback to handle keyboad events
	glutKeyboardFunc(appKeyboardFunc);

	// Start the main loop
	glutMainLoop();
}
