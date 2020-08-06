#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <emscripten.h>

//#define NUM_CIRCLES 20
const int NUM_CIRCLES = 20;

//Circle struct
struct Circle {
  int x;
  int y;
  int r;//radius
  //colors
  int cr;
  int cg;
  int cb;
};

//Circle animation data sctruct
struct CircleAnimationData {
  int x;
  int y;
  int r;//radius
  int xv;//x axis velocity
  int yv;//y axis velocity
  int xd;//x axis direction
  int yd;//y axis direction
};

// Circles variable
struct Circle circles[NUM_CIRCLES];
struct CircleAnimationData animationData[NUM_CIRCLES];

// Random number generator
int getRand(max) {
  return (rand() % max);
}

// Init circle data and start render - JS
int main() {

  srand(time(NULL));

  for (int i = 0; i < NUM_CIRCLES; i++) {
    int radius = getRand(50);
    int x = getRand(1000) + radius;
    int y = getRand(1000) + radius;

    //animation struct
    animationData[i].x = x;
    animationData[i].y = y;
    animationData[i].r = radius;
    animationData[i].xv = getRand(10);
    animationData[i].yv = getRand(10);
    animationData[i].xd = 1;
    animationData[i].yv = 1;

    //circle struct
    circles[i].x = x;
    circles[i].y = y;
    circles[i].r = radius;
    circles[i].cr = getRand(255);
    circles[i].cg = getRand(255);
    circles[i].cb = getRand(255);
  }
//    emscripten_run_script("render()");

  EM_ASM({render($0, $1);}, NUM_CIRCLES * 6, 6);

}


struct Circle *getCircles(int canvasWidth, int canvasHeight) {
  //Collision detection
  for (int i = 0; i < NUM_CIRCLES; i++) {
    //X axis
    if ((animationData[i].x + animationData[i].r) >= canvasWidth) animationData[i].xd = 0;
    if ((animationData[i].x - animationData[i].r) <= 0) animationData[i].xd = 1;
    //Y axis
    if ((animationData[i].y + animationData[i].r) >= canvasHeight) animationData[i].yd = 0;
    if ((animationData[i].y - animationData[i].r) <= 0) animationData[i].yd = 1;
    //move circle
    if (animationData[i].xd == 1) {//x axis
      animationData[i].x += animationData[i].xv;
    } else {
      animationData[i].x -= animationData[i].xv;
    }

    if (animationData[i].yd == 1) {//y axis
      animationData[i].y += animationData[i].yv;
    } else {
      animationData[i].y -= animationData[i].yv;
    }

    //Update matching circle
    circles[i].x = animationData[i].x;
    circles[i].y = animationData[i].y;

  }

  return circles;
}