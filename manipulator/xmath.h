#ifndef __XMATH_H__
#define __XMATH_H__

#ifdef __cplusplus
extern "C" {
#endif

float rad2deg(float rad);
float deg2rad(float deg);
void rads2degs(float *rads, float *out, size_t n);
void degs2rads(float *degs, float *out, size_t n);
float distance(float a[3], float b[3]);

void xabss(float *a, float *out, size_t n);
float xmax(float *a, size_t n);
float xmin(float *a, size_t n);
void xdiv(float *a, float b, float *out, size_t n);
void xmul(float *a, float b, float *out, size_t n);
void xadd(float *a, float *b, float *out, size_t n);
void xsub(float *a, float *b, float *out, size_t n);
void xcopy(float *src, float *dst, size_t n);

#ifdef __cplusplus
}
#endif

#endif //__XMATH_H__
