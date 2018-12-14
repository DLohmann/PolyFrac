#include <iostream>
#include <deque>
#include <math.h> // has square root for drawing curves
#include <stdlib.h> // has rand() and RAND_MAX
#include "Polygon.h"
#include <stdio.h> // has perror
#include <thread>	// has threads
#include <chrono>	// used to convert to a chrono::duration format that threads needs to sleep for a certain amount of time

// Used to control access to the fractalPoints and attractorPoints deque's
//#include <mutex>
//#include <condition_variable>

#if defined WIN32
#include <freeglut.h>
#elif defined __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

//#define debug_graphics
#define debug_addFractalPoint

using namespace std;

// Some global variables to maintain state


// A point data structure
struct Point {
	// The coordinates of the point
	float x;
	float y;

	// The color of the point
	float r;
	float g;
	float b;

	// A constructor for point
	Point(float x, float y, float r, float g, float b) {
		this->x = x;
		this->y = y;
		this->r = r;
		this->g = g;
		this->b = b;
	}
};

// A "Double Ended QUEue" to store attractorPoints 
deque<Point> attractorPoints;
deque<Point> fractalPoints;
thread fractalPointThread;

// Ensures that fractal points and attractorPoints are only modified by one thread at a time
//condition_variable isAddingFractalPoints;
//condition_variable isAddingAttractorPoints;
//bool isAddingAttractorPoints;
//mutex fractalPointsModify;
//mutex attractorPointsModify;

Polygon* polyPtr = NULL;

// Variables to store current color, initialize to black
float red = 0.0, green = 0.0, blue = 0.0;

// Store the width and height of the window
int width = 640, height = 640;

float randFloat (float rand_upper_bound, float rand_lower_bound) {
	// remember that shifting an integer left << or right >> is equivalent to multiplying it or dividing it by 2
	//int normalized_rand_int = (rand() - (RAND_MAX>>1))<<1; // when rand() is 0, normalized_rand_int is about -RAND_MAX. When rand() is RAND_MAX, normalized_rand_int is also about RAND_MAX
	//float normalized_rand_float = ((float)(normalized_rand_int))/((float)RAND_MAX); // a normalized random float between -1 and 1
	
	float rand_float = ((float)rand())/((float)RAND_MAX); // a positive random float between 0 and 1
	
	return (rand_upper_bound - rand_lower_bound) * rand_float + rand_lower_bound;
}

void drawPoints (deque<Point>& pointsList) {
	// Draw all the pointsList stored in the double-ended queue
	#ifdef debug_graphics
		cout << "\tDrawing a points list" << endl;
	#endif
	for (int i = 0; i < pointsList.size(); i++) {

		// Set the vertex color to be whatever we stored in the point
		glColor3f(pointsList[i].r, pointsList[i].g, pointsList[i].b);

		glBegin(GL_POINTS);

		// Draw the vertex in the right position
		glVertex2f(pointsList[i].x, pointsList[i].y);

		glEnd();
		#ifdef debug_graphics
			cout << "\t\tDrew point " << i << endl;
		#endif
	}
	#ifdef debug_graphics
		cout << "\tDrew list" << endl;
	#endif
}

void drawRectangle (float x1, float y1, float x2, float y2) {
	float xLeft  = x1 < x2 ? x1 : x2;	// xLeft  = min (x1, x2)
	float yUpper = y1 > y2 ? y1 : y2;	// yUpper = max (y1, y2)
	float xRight = x1 > x2 ? x1 : x2;	// xRight = max (x1, x2)
	float yLower = y1 < y2 ? y1 : y2;	// yLower = min (y1, y2)
	
	glBegin (GL_POLYGON);
	glVertex2f (xLeft,  yLower);
	glVertex2f (xLeft,  yUpper);
	glVertex2f (xRight, yUpper);
	glVertex2f (xRight, yLower);
	glEnd ();
}

void drawText (float xPos, float yPos, char * textStr) {
	void* font = GLUT_BITMAP_TIMES_ROMAN_10;
	glRasterPos2f (xPos, yPos);
	int i = 0;
	while (textStr[i] != '\0' && i < 128) {
		glutBitmapCharacter (font, textStr[i]);
	}
	if (i == 128) {
		cout << "Error: drawText function encountered over 128 characters without a terminating null '\0' character.\n";
		return;
	}
}

//-------------------------------------------------------
// A function to draw a letter identified by "char shape" at coordinates (xPos, yPos)
//-------------------------------------------------------
// void drawLetter(char letter, int xPos, int yPos) { // Version with default parameters
	// drawLetter(letter, xPos, yPos, 10);
// }
void drawLetter(char letter, float xPos, float yPos, float size) {
	//cout << "Drawing \"" << letter << "\"!" << endl;
	glBegin (GL_POLYGON);
	//glColor3f (1, 1, 1);
	float deltaX;
	float radius;
	switch (letter) {
		case 'A':
			
			break;
		
		case 'D':
			
			// Left rectangular side (separate polygon)
			
			glVertex2f (xPos, yPos); // Bottom left corner
			glVertex2f (xPos, yPos + size); // top left corner
			glVertex2f (xPos + (size/6), yPos + size); // top, on right corner
			glVertex2f (xPos + (size/6), yPos); // bottom right
			glEnd();
			
			
			// Circular portion (separate polygon, to make the hole in the "D")
			glBegin (GL_POLYGON);
			
			radius = size/2;	// Outer radius is size/2
			
			// Outer circle:
			for (float deltaY = radius; deltaY > -1*radius; deltaY -= size/10) {
				deltaX = sqrt((radius*radius) - (deltaY*deltaY));
				glVertex2f (xPos + (size/6) + deltaX, yPos + (size/2) + deltaY);
			}
			
			glVertex2f (xPos + (size/6), yPos + (size/6)); // Bottom inner corner of "D"
			
			// Inner circle
			radius = size/3; //5*size/12; // Inner radius is size/2 - size/6 = 5*size/6
			for (float deltaY = -1*radius; deltaY < radius; deltaY += size/10) {
				deltaX = sqrt((radius*radius) - (deltaY*deltaY));
				glVertex2f (xPos + (size/6) + deltaX, yPos + (size/2) + deltaY);
			}
			
			
			//glVertex2f (xPos + (size/6), yPos + (size/6)); // Bottom inner corner of "D"
			
			
			
			
			// Left side. Same as "I" (fall through to the "I" case)
			break;
			
		case 'I':
			glVertex2f (xPos, yPos); // Bottom left corner
			glVertex2f (xPos, yPos + size); // top left corner
			glVertex2f (xPos + (size/6), yPos + size); // top, on right corner
			glVertex2f (xPos + (size/6), yPos); // bottom right
			break;
		
		case 'L':
			// points on "L" start at bottom left corner, and go clockwise around
			glVertex2f (xPos, yPos); //Bottom left corner of "L"
			glVertex2f (xPos, yPos + size); // top left corner of "L"
			glVertex2f (xPos + (size/6), yPos + size); // top, on right corner
			glVertex2f (xPos + (size/6), yPos + (size/6)); // inner corner
			glVertex2f (xPos + (size/2), yPos + (size/6)); // right side, tip on top of "heel" of L
			glVertex2f (xPos + (size/2), yPos); // bottom right corner
			
			
			
			break;
		
		default:
			
			break;
	}
	glEnd();
}

//chrono::duration<int, milli> fractalAddPeriod = chrono::duration<int, milli>(10);
chrono::duration <int, milli> fractalAddPeriod = chrono::milliseconds(1);

void initFractalPoints();

void setFractalPointAddPeriod (int milliseconds_amount) {
	if (milliseconds_amount < 0) {
		cout << "setFractalPointAddPeriod error: Must enter a positive value for the number of milliseconds to add a fractal point, but not: " << milliseconds_amount << endl;
		return;
	}
	if (milliseconds_amount > 60000) {
		cout << "setFractalPointAddPeriod error: Must enter a positive value for the number of milliseconds to add a fractal point, but not over 60,000 (10 minutes): " << milliseconds_amount << endl;
		return;
	}

	fractalAddPeriod = chrono::milliseconds(milliseconds_amount);
}

long lastFractalAdd = 0;

void addFractalPoint () {
	#ifdef debug_addFractalPoint
		cout << "addFractalPoint called" << endl;
	#endif

	while (true) {	// Keep adding points throughout the program
		// get both locks
		//unique_lock<std::mutex> fractalLock(fractalPointsModify);
		//unique_lock<std::mutex> attractorLock(attractorPointsModify);
		//isAddingAttractorPoints.wait(fractalLock);
		//isAddingFractalPoints.wait(attractorLock);
		if (attractorPoints.size() == 0 || fractalPoints.size() == 0) {
			

			//perror ("Cannot  add new attractor point if the first one has not been placed\n");
			cout << "Cannot add new fractal point if there are not any fractal points or attractor points\n" << endl;
			cout << "There most be at least 1 attractor point and at least one fractalpoint to add more fractal points. There are currently " << fractalPoints.size() << " fractal points and " << attractorPoints.size() << " attractor points." << endl;
			// release both locks
			//fractalLock.unlock();
			//attractorLock.unlock();
			// notify waiting threads that locks were released
			//isAddingAttractorPoints.notify_one();
			//isAddingFractalPoints.notify_one();
			
			this_thread::sleep_for(fractalAddPeriod);
			continue;
		}

		
		// Choose an attractor point to get the average of
		int chosenAttractor = rand() % attractorPoints.size();

		// TODO: This section of code should not be interrupted, or it can cause a segmentation fault of the attractor points are cleared in this section. Because Point& attractor would point to a deleted point
		Point attractor = attractorPoints[chosenAttractor];
		Point lastFractalPoint = fractalPoints.back();
		
		// release attractor lock
		
		//attractorLock.unlock();
		// notify waiting threads that attractor lock was released
		//isAddingAttractorPoints.notify_one();
		

		// Find midpoint between last fractal point and the chosen attractor point.
		float midX = (attractor.x + lastFractalPoint.x) / 2;
		float midY = (attractor.y + lastFractalPoint.y) / 2;

		// Create the new fractal point at the midpoint, and add it to the fractal points list
		fractalPoints.push_back(Point(midX, midY, red, green, blue));
		
		// unlock fractal points
		//fractalLock.unlock();
		// notify waiting threads that fractal lock was released
		//isAddingFractalPoints.notify_one();
		
		// TODO: Make the sleep amount variable

		// Helpful print statement
		/*
		auto currentTime = chrono::high_resolution_clock::now();
		chrono::duration<double, std::milli> millis = currentTime;
		cout << "Added fractal point #" << fractalPoints.size() << " at time " << millis.count() << "ms" << endl;
		*/
		//int elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(chrono::high_resolution_clock::now()).count();
		//int currentTime = chrono::high_resolution_clock::now().count();	// might not be in ms
		//int currentTime = chrono::milliseconds(chrono::system_clock::now());

		long currentTime = chrono::system_clock::now().time_since_epoch().count();
		//int currentTime = chrono::system_clock::duration
		#ifdef debug_addFractalPoint
			cout << "Fractal thread: Added fractal point #" << fractalPoints.size() << " at time " << currentTime - lastFractalAdd << " ms attracted to attractor #" << chosenAttractor << endl;
		#endif
		lastFractalAdd = currentTime;
		// Make this thread sleep until it is time to place the next point. The point placement rate is defined by variable __ 
		glutPostRedisplay();
		this_thread::sleep_for(fractalAddPeriod);
	}	
}


//-------------------------------------------------------
// A function to draw the scene
//-------------------------------------------------------
void appDrawScene() {
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set background color to white
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClearColor(0.0, 0.0, 0.0, 0.0);
	// Set up the transformations stack
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set the color to the current values of the global vars
	glColor3f(red, green, blue);

	// // Set point size
	glPointSize(6);

	//Draw my initials
	// glColor3f (0.0, 1.0, 0.0);
	// drawLetter ('D', 0.0, 0.0, 0.4);
	// glColor3f (0.0, 0.0, 1.0);
	// drawLetter ('L', 0.4, 0.0, 0.4);
	glColor3f(red, green, blue);
	
	// Draw the attractorPoints
	//unique_lock<std::mutex> attractorLock(attractorPointsModify);	// Get attractor points lock
	
	drawPoints(attractorPoints);
	//attractorLock.unlock();	// Unlock the attractor points
	//isAddingAttractorPoints.notify_one();	// Notify waiting threads

	// Draw the fractalPoints
	glColor3f (0.0, 1.0, 0.0);
	//unique_lock<std::mutex> fractalLock(fractalPointsModify);	// Get the fractal points lock
	drawPoints(fractalPoints);
	//fractalLock.unlock();	// Unclock the fractal points
	//isAddingFractalPoints.notify_one();	// Notify waiting threads
	
	//drawRectangle (-0.98, 0.98, -0.68, 0.88);
	//drawRectangle (-0.98, 0.83, -0.68, 0.73);
	//polyPtr->draw();
	
	// Draw a point at the bottom-right
	//glBegin(GL_POINTS);
	//glVertex2f(-0.8, -0.8);
	//glEnd();



	

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

	// Add a point with with coordinates matching the
	// current mouse position, and the current color values
	// TODO: Only add to attractor points at most once every 200 ms. Check if 200 ms has passed since last point was placed
	if (s == 0) {

		
		//unique_lock<std::mutex> attractorLock(attractorPointsModify);
		attractorPoints.push_front(Point(mx, my, red, green, blue));
		//attractorLock.unlock();
		// notify waiting threads that locks were released
		//isAddingAttractorPoints.notify_one();
	}

	// If polygon is clicked, set it's color to a random color
	if (polyPtr->contains(mx, my)) {
		polyPtr->poly_Color3f (randFloat (-1.0f, 1.0f), randFloat (-1.0f, 1.0f), randFloat (-1.0f, 1.0f));
	}
	
	// Redraw the scene by calling appDrawScene above
	// so that the point we added above will get painted
	glutPostRedisplay();
}

//-------------------------------------------------------
// A function to handle mouse dragging
// Called every time mouse moves while button held down
// Arguments: 	
//	x, y - current coordinates of the mouse
//-------------------------------------------------------
void appMotionFunc(int x, int y) {
	// Convert from Window to Scene coordinates
	float mx = (float)x;
	float my = (float)y;

	windowToScene(mx, my);

	// Similar behavior to click handler. This function
	// allows us to paint free hand with the mouse.
	//attractorPoints.push_front(Point(mx, my, red, green, blue));

	// If polygon is clicked, set it's color to a random color
	if (polyPtr->contains(mx, my)) {
		polyPtr->poly_Color3f (randFloat (-1.0f, 1.0f), randFloat (-1.0f, 1.0f), randFloat (-1.0f, 1.0f));
	}
	
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
	// Define what should happen for a given key press 
	switch (key) {
		// Space was pressed. Erase all attractorPoints
	case ' ':
		// Pause fractalPointThread until initFractalPoints is called again
		// TODO : Add a condition variable or lock to suspend the fractalPointThread until the attractorPoints size and fractalPoints size is greater than 0
		//fractalPointThread.terminate();
		//fractalPointThread.sleep_for(fractalAddPeriod);

		// Clear all points

		//Get both locks to add attractor points and fractal points

		attractorPoints.clear();
		fractalPoints.clear();
		
		//release both locks

		break;
		// "d" was pressed. Delete fractal points
	case 'd':	// TODO: Fix segmentation fault that this causes
		// Resume fractalPointThread, if it is sleeping

		initFractalPoints();
		break;

		// Escape was pressed. Quit the program
	case 27:
		exit(0);
		break;

		// The "r" key was pressed. Set global color to red
	case 'r':
		red = 1.0;
		green = 0.0;
		blue = 0.0;
		break;

		// The "g" key was pressed. Set global color to green
	case 'g':
		red = 0.0;
		green = 1.0;
		blue = 0.0;
		break;

		// The "b" key was pressed. Set global color to blue
	case 'b':
		red = 0.0;
		green = 0.0;
		blue = 1.0;
		break;

		// The "k" key was pressed. Set global color to black
	case 'k':
		red = 0.0;
		green = 0.0;
		blue = 0.0;
		break;

		// The "w" key was pressed. Set global color to white
	case 'w':
		red = 1.0;
		green = 1.0;
		blue = 1.0;
		break;
		
	}

	// After all the state changes, redraw the scene
	glutPostRedisplay();
}

void initPolygons () {
	list <vertex> vList;
	vertex upper;
	upper.x = 0.0f;
	upper.y = 0.5f;
	vList.push_back(upper);
	
	vertex left;
	left.x = -0.5f;
	left.y = 0.0f;
	vList.push_back(left);
	
	vertex right;
	right.x = 0.5f;
	right.y = 0.0f;
	vList.push_back(right);
	
	
	//Polygon poly (vList);
	polyPtr = new Polygon (vList);
	
}


void initFractalPoints() {
	// Acquire lock to add to fractalPoints

	// Randomly choose coordinates of 1st point to place
	fractalPoints.push_back(Point(randFloat (-1.0f, 1.0f), randFloat (-1.0f, 1.0f), 0.0f, 1.0f, 0.0f));

	// Create a thread that adds new random points asynchronously, for the duration of the program. This thread should call addFractalPoint()
	cout << "Creating a thread to add fractal points" << endl;
	fractalPointThread = thread(addFractalPoint);
	//fractalPointThread.star
	fractalPointThread.detach();
	
	//thread fractalPointThread2 (addFractalPoint);
	
	//fractalPointThread2.detach();
}

int main(int argc, char** argv) {
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

	// Setup window position, size, and title
	glutInitWindowPosition(20, 60);
	glutInitWindowSize(width, height);
	glutCreateWindow("Lohmann CSE165 Fractal");

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);

	initPolygons();
	//initFractalPoints();

	appDrawScene(); // Set initial screen

	// Tell the thread that attractorpoints can be added at startup 
	//isAddingAttractorPoints.notify_one();
	//isAddingAttractorPoints.notify_one();
	//isAddingAttractorPoints = true;

	// Set callback for drawing the scene
	glutDisplayFunc(appDrawScene);

	// Set callback for resizing th window
	glutReshapeFunc(appReshapeFunc);

	// Set callback to handle mouse clicks
	glutMouseFunc(appMouseFunc);

	// Set callback to handle mouse dragging
	glutMotionFunc(appMotionFunc);

	// Set callback to handle keyboard events
	glutKeyboardFunc(appKeyboardFunc);

	// Start the main loop
	glutMainLoop();
	
	delete polyPtr;
}