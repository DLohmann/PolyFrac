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
#include <limits> // Has max float limits:
#include <cmath> // has abs() absolute value for calculating area of a polygon

//std::numeric_limits<float>::max();
//std::numeric_limits<float>::min();
//std::numeric_limits<float>::infinity();



//template<class T>



float randFloat (float rand_upper_bound, float rand_lower_bound) {
	// remember that shifting an integer left << or right >> is equivalent to multiplying it or dividing it by 2
	//int normalized_rand_int = (rand() - (RAND_MAX>>1))<<1; // when rand() is 0, normalized_rand_int is about -RAND_MAX. When rand() is RAND_MAX, normalized_rand_int is also about RAND_MAX
	//float normalized_rand_float = ((float)(normalized_rand_int))/((float)RAND_MAX); // a normalized random float between -1 and 1
	
	float rand_float = ((float)rand())/((float)RAND_MAX); // a positive random float between 0 and 1
	
	return (rand_upper_bound - rand_lower_bound) * rand_float + rand_lower_bound;
}

struct vertex {
	float x;
	float y;
	vertex (float x, float y) : x(x), y(y) {

	}
	vertex() : x(0), y(0) {
		
	}
};

class Polygon {
	public:
		// Constructors
		Polygon ();	// Generates a square
		// Polygon (int seed);	// Generates a randomized polygon using the seed
		Polygon (float); // Generates a randomized polygon with vertices that are averageSize away from each other (float averageSize). (the (n+1)th point is no more than 2*(averageSize) away from the nth point)  for all points
		Polygon (std::list <vertex> );
		
		// Destructor
		~Polygon();
		
		// Accessors
		
		// Mutators
		void poly_Color3f (float, float, float);
		
		// Other
		void draw ();
		bool contains (float, float); // returns whether the point with the given coordinates is within the polygon
		bool contains (vertex);
		//bool contains (vertex v);
		size_t size(); // Returns the number of points that the polygon is made of.
		float area (); // Returns the area of the polygon. Uses a clever algorithm: _____________________TODO___
		
		// Member variables
		
		// Colors
		float poly_red;
		float poly_green;
		float poly_blue;
		//static list<Polygon&> polygonList;
		//static std::list<Polygon*> allPolygons;
		 // global variable to keep track of all polygons created so far (constructors should add to this list)
		// TODO: Create drawAllPolygons variable that draws all polygons that have been created
	private:
		
		
		
		std::list<vertex> points;

		
};

// Generates a square
Polygon::Polygon () {
	this->points.push_back (vertex( 0.25f,  0.25f));
	this->points.push_back (vertex(-0.25f,  0.25f));
	this->points.push_back (vertex( 0.25f, -0.25f));
	this->points.push_back (vertex(-0.25f, -0.25f));
	poly_Color3f (0.5f, 0.5f, 0.5f);
	//Polygon::allPolygons.push_back(this);
}

Polygon::Polygon (std::list <vertex> points) {
	this->points = std::list <vertex> (points);
	//poly_Color3f (0.5f, 0.5f, 0.5f);
	poly_Color3f (randFloat(1.0f, 0.0f), randFloat(1.0f, 0.0f), randFloat(1.0f, 0.0f));
	//Polygon::allPolygons.push_back(this);
}

// Generates random polygons with vertices that are on average averageSize away from adjacent points
Polygon::Polygon (float averageSize) {
	int numSides = rand()%3 + 3;	// There should be 3-5 sides per polygon

	//float maxBound = 2*averageSize;	// max 
	std::list <vertex> poly_points;
	
	// The initial points can be anywhere on the canvas
	float x = randFloat(1.0, -1.0f);
	float y = randFloat(1.0, -1.0f);

	for (int i = 0; i < numSides; i++) {
		// Move the vertex slightly to get the next vertex
		x += randFloat(averageSize, -1*averageSize);
		y += randFloat(averageSize, -1*averageSize);

		poly_points.push_back(vertex(x, y));	// Add points to vertex list
	}

	//Polygon::Polygon(poly_points);	// Call other constructor
	this->points = std::list <vertex> (poly_points);
	poly_Color3f (randFloat(1.0f, 0.0f), randFloat(1.0f, 0.0f), randFloat(1.0f, 0.0f));
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
/*
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
	for (std::list<vertex>::iterator it = points.begin(); it != points.end(); it++) {
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

*/


size_t Polygon::size () {
	return points.size();
}

// The following was modified from GeeksForGeeks:
// https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/

// Given three colinear points p, q, r, the function checks if 
// point q lies on line segment 'pr' 
bool onSegment(vertex p, vertex q, vertex r) 
{ 
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && 
            q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y)) 
        return true; 
    return false; 
} 
  
// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are colinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int orientation(vertex p, vertex q, vertex r) 
{ 
    int val = (q.y - p.y) * (r.x - q.x) - 
              (q.x - p.x) * (r.y - q.y); 
  
    if (val == 0) return 0;  // colinear 
    return (val > 0)? 1: 2; // clock or counterclock wise 
} 
  
// The function that returns true if line segment 'p1q1' 
// and 'p2q2' intersect. 
bool doIntersect(vertex p1, vertex q1, vertex p2, vertex q2) 
{ 
    // Find the four orientations needed for general and 
    // special cases 
    int o1 = orientation(p1, q1, p2); 
    int o2 = orientation(p1, q1, q2); 
    int o3 = orientation(p2, q2, p1); 
    int o4 = orientation(p2, q2, q1); 
  
    // General case 
    if (o1 != o2 && o3 != o4) 
        return true; 
  
    // Special Cases 
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1 
    if (o1 == 0 && onSegment(p1, p2, q1)) return true; 
  
    // p1, q1 and p2 are colinear and q2 lies on segment p1q1 
    if (o2 == 0 && onSegment(p1, q2, q1)) return true; 
  
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2 
    if (o3 == 0 && onSegment(p2, p1, q2)) return true; 
  
     // p2, q2 and q1 are colinear and q1 lies on segment p2q2 
    if (o4 == 0 && onSegment(p2, q1, q2)) return true; 
  
    return false; // Doesn't fall in any of the above cases 
} 

bool Polygon::contains (float xp, float yp) {
	vertex p = {xp, yp};
	return contains (p);
}

// Returns true if the point p lies inside the polygon[] with n vertices 
bool Polygon::contains (vertex p) { 
    // There must be at least 3 vertices in polygon[] 
    if (this->size() < 3)  return false; 
  
    // Create a point for line segment from p to infinite 
    vertex extreme = {std::numeric_limits<float>::max(), p.y}; 
  
    // Count intersections of the above line with sides of polygon 
    int count = 0;
	std::list<vertex>::iterator it = points.end();
	std::list<vertex>::iterator next = points.begin();
	do {
		
		if (doIntersect(*it, *next, p, extreme)) {
			if (orientation(*it, p, *next) == 0) { 
               return onSegment(*it, p, *next); 
			}
			count++;
		}
		it = next;
		next++;
	} while (it != points.end());
	/*
	int count = 0, i = 0; 
    do
    { 
        int next = (i+1)%this->size(); 
  
        // Check if the line segment from 'p' to 'extreme' intersects 
        // with the line segment from 'polygon[i]' to 'polygon[next]' points
        if (doIntersect(points[i], points[next], p, extreme)) 
        { 
            // If the point 'p' is colinear with line segment 'i-next', 
            // then check if it lies on segment. If it lies, return true, 
            // otherwise false 
            if (orientation(points[i], p, points[next]) == 0) 
               return onSegment(points[i], p, points[next]); 
  
            count++; 
        } 
        i = next; 
    } while (i != 0); 
	*/

    // Return true if count is odd, false otherwise 
    return count&1;  // Same as (count%2 == 1) 
} 

// Copied from https://www.geeksforgeeks.org/area-of-a-polygon-with-given-n-ordered-vertices/
float Polygon::area () {
 
    // Initialze area 
    float area = 0.0; 
	
    // Calculate value of shoelace formula 
	std::list<vertex>::iterator it   = points.begin();
	std::list<vertex>::iterator last = points.end();
	do {
		area += (last->x + it->x) * (last->y - it->y);
		last = it;
	} while (it++ != points.end());
    /*
	int j = this->size - 1; 
    for (int i = 0; i < this->size; i++) 
    { 
        area += (X[j] + X[i]) * (Y[j] - Y[i]); 
        j = i;  // j is previous vertex to i 
    } 
	*/
    // Return absolute value 
    return std::abs(area / 2.0); 
}

#endif