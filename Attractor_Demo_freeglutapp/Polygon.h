#ifndef Polygon_h
#define Polygon_h

#if defined WIN32
#include <freeglut.h>
#elif defined __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <list>

//template<class T>

//static std::list<Polygon&> allPolygons; // global variable to keep track of all polygons created so far (constructors should add to this list)
// TODO: Create drawAllPolygons variable that draws all polygons that have been created

struct vertex {
	float x;
	float y;
};

class Polygon {
	public:
		// Constructors
		// Polygon ();
		Polygon (std::list <vertex> );
		
		// Destructor
		~Polygon();
		
		// Accessors
		
		// Mutators
		void poly_Color3f (float, float, float);
		
		// Other
		void draw ();
		bool contains (float, float); // returns whether the point with the given coordinates is within the polygon
		//bool contains (vertex v);
		size_t size(); // Returns the number of points that the polygon is made of.
		//float area (); // Returns the area of the polygon. Uses a clever algorithm: _____________________TODO___
		
		// Member variables
		
		// Colors
		float poly_red;
		float poly_green;
		float poly_blue;
		
	private:
		
		
		
		std::list<vertex> points;
};

// Polygon::Polygon () {
	
// }

Polygon::Polygon (std::list <vertex> points) {
	this->points = std::list <vertex> (points);
	poly_Color3f (0.5f, 0.5f, 0.5f);
	//allPolygons.push_front(this);
}

Polygon::~Polygon () {
	//delete points;
}

void Polygon::poly_Color3f (float red, float green, float blue) {
	this->poly_red   = red;
	this->poly_green = green;
	this->poly_blue  = blue;
}

void Polygon::draw() {
	// Assign new color
	glColor3f (poly_red, poly_green, poly_blue);
	
	// Draw polygon
	glBegin (GL_POLYGON);
	for (std::list<vertex>::iterator it = points.begin(); it != points.end(); it++) {
		glVertex2f (it->x, it->y);
	}
	glEnd();
	
	// Restore old color
	//glColor3f (red, green, blue);
}

//Uses a clever algorithm: https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/

// TODO: Ensure that this function handles special cases, such as when some or all points are on the same line.
bool Polygon::contains (float xp, float yp) {
	// (x, y) is within the polygon if the line segment/ray/vector from the point (x, y) to a point at infinity, crosses the polygon an odd number of times
	// So there are 3 steps to check if the point is within the line:
	// 1) Draw a line segment/ray/vector from (xp, yp) in 1 direction to infinity (in any direction).
	// 2) Count how many times the line segment/ray/vector crosses the polygon
	// 3) If it crosses an even number of times, it's outside the polygon. If it crosses an odd number of times, the point is inside.
	
	// In this implementation, I'm using the horizontal line from (xp, yp) towards the right, to positive infinity ((xp, yp)----->).
	
	// Choosing a horizontal (or verical) line makes comparing the position of the point to the line easier???
	// For the line ax + by = 1, 
	// TODO: finish explanation
	
	int countIntersects = 0;
	for (std::list<vertex>::iterator it = points.begin(); it != points.end(); /*it++*/) {
		// For each pair of 2 adjacent points, get the equation of the line between them in the form ax + by = 1
		
		
		// For the 2 endpoints of the line, (x1, y1) & (x2, y2), we know a(x1) + b(y1) = 1 & a(x2) + b(y2) = 1
		
		// Calculate a and b. 
		
		float x1 = it->x;
		float y1 = it->y;
		it++;	// iterate "it" here, so do not need to iterate it in the "for" loop
		float x2 = it->x;
		float y2 = it->y;
		
		// TODO: Include behavior for when at end of list, and need to wrap around to end of list. Or maybe this should be done in the beginning from the tail of the list to the beginning of the list, before running the "for" loop? 
		// Need to ensure that the line segment from the last point in the list to the first point in the list is also accounted for
		
		// TODO: Do I also need to check if (xp, yp) are within a certain bound?
		// Such as ((x1 <= xp <= x2) or (x2 <= xp <= x1)) and ((y1 <= yp <= y2) or (y2 <= yp <= y1))???? Is it required???
		
		if ((y1 - y2)*xp + (x2 - x1)*yp < x2*y1 - x1*y2) { // This formula should tell whether the point is to the left of the line. If so,then the infinite line that goes from the point in the positive x direction, must cross this line
			countIntersects += 1;
		}
		
	}
	
	// if (countIntersects % 2 == 0) {
		// return false;
	// }
	// return true;
	
	return countIntersects % 2 != 0;
}

size_t Polygon::size () {
	return points.size();
}



#endif