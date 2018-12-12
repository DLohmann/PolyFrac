# PolyFrac

## Game Description: This is a game to play with fractals and teach about the math behind attractor systems. The user places “attractor” points on the screen, which the program uses to create fractals. The more spread out the points are from each other, the more points the user gets. The fractals are generated when the computer randomly generates “fractal” points around the user’s “attractor” points. The program also randomly generates “pitfall” polygons around the screen. Every time a “fractal” points is placed in a pitfall, the user loses points. The longer the user lasts without any randomly generated fractal points landing within a polygon, the more points the user gets. The objective of the game is to place “attractor” points to build a fractal to avoid the “pitfall” polygons. If the score goes goes to 0, the player loses. The objective of the game is to survive as long as possible, by building fractals.

## Building fractals:
The user clicks the screen to place “attractor” points. The user can only place new attractor points at a certain rate (ie, no more than 10 new attractor points per minute). Old “attractor” points may be randomly deleted as time passes. 

## Fractal growth:
At the start of the game, a single “fractal” point will be randomly placed on the screen. Every 100-500 milliseconds, an “attractor” point will be randomly chosen from the set of attractor points that the user has created, and a new “fractal” point will be placed on the screen midway between the last “fractal” point and the selected “attractor” point. As more “fractal” points are created, the outline of all of these “fractal” points forms a fractal. The user has the option to choose the initial “seed” fractal point, if he or she desires.

## Gaining points:
The user gets score points over time. The amount of points that the user gains is proportional to the area of the polygon created by all the attractor points the user has placed on the screen. The score will be indicated in the corner of the screen. The user begins with 500-1000 points.

## Losing points:
“Pitfall” polygons are randomly generated on the screen. Initially, these are small, but they get larger over time. When a “fractal” point occurs inside a pitfall, the user loses points. Old pitfalls might be randomly deleted as time passes. In addition, the user will lose points if the polygon drawn out by the attractor points does not cover enough area. The user loses if her or his score goes to 0.

## Beast mode:
Once in a while, the rate at which fractal points are placed increases dramatically, along with the rate at which “pitfalls” are created and the user gains points. This is basically a speed up of the game. This will last for 10-15 seconds. 

## Future iterations:
- Maybe once in a while, the user could have some control over the movement pitfalls to a more desirable location.
- Maybe in some game modes, the “fractal” point positions could be calculated based on some weighted average, or some other function of the positions of the attractor points.
- Make the point generation, and score updating multithreaded. 
- There will also be a version without pitfalls, just to build fractals.
- Compile the program for WebAssembly, and host it on a website.
- During the CSE 165 final class demo, form a competition to see who can get the highest score on this game. Then give out a prize for this highest score (should log player names for the leaderboard).
- Make a 3D version of this game?



This readme file was taken from the project specifications at this link:
https://docs.google.com/document/d/1cf45kmz3oNQ7RqiqdlUyD_tw7w5lOnuN8sXz_nvTblA/edit?usp=sharing
