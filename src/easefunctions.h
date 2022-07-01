#include <math.h>
/* List of functions based of Robert Penner's easing equations and initially made for JavaScript ported to C++
Parameters to know:
    elapsed - Time elapsed between the starting time (or tick) and current time (or tick).
    initialValue - Initial value the function starts with.
    amountOfChange - Number that decides the amount to change the number from start to end. The difference between the end to start.
    duration - the duration of the entire animation
*/

double linearLerp(double elapsed, double initialValue, double amountOfChange, double duration) {
    return amountOfChange * (elapsed /= duration) + initialValue;
}

double bouncyWave(double amountOfChange, int angle) {
    return amountOfChange*sin(angle*PI/180);
}