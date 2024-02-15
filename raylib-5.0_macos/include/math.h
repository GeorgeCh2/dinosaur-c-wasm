// Phony math.h. Since we are compiling with --no-standard-libraries raymath.h can't find math.h.
// But it only needs it for few function definitions. So we've put those definitions here.
#ifndef MATH_H_
#define MATH_H_
float sinf(float);
float cosf(float);
float acosf(float);
float asinf(float);
double tan(double);
float atan2f(float, float);
float sqrtf(float);
float floorf(float);
double floor(double);
float fminf(float, float);
float fmaxf(float, float);
float fabsf(float);
double fabs(double);
#endif // MATH_H_