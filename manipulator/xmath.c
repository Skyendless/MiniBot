#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "xmath.h"

/* name       : rad2deg
 * decription : 内部函数，弧度转角度
 * arguments  : rad 弧度
 * return     : 角度
 */
float rad2deg(float rad)
{
    return rad * 57.3f;
}

/* name       : deg2rad
 * decription : 内部函数，角度转弧度
 * arguments  : deg 角度
 * return     : 弧度
 */
float deg2rad(float deg)
{
    return deg / 57.3f;
}

/* name       : rads2degs
 * decription : 内部函数，将一组弧度转为角度
 * arguments  : rads 弧度数组
 *              out  用于保存角度的数组
 *              n    数组长度
 * return     : void
 */
void rads2degs(float *rads, float *out, size_t n)
{
    for (int i=0; i<n; i++) {
        out[i] = rads[i] * 57.3f;
    }
}

/* name       : degs2rads
 * decription : 内部函数，将一组角度转为弧度
 * arguments  : degs 角度数组
 *              out  用于保存弧度的数组
 *              n    数组长度
 * return     : void
 */
void degs2rads(float *degs, float *out, size_t n)
{
    for (int i=0; i<n; i++) {
        out[i] = degs[i] / 57.3f;
    }
}

/* name       : distance
 * decription : 内部函数，计算三维空间点的距离
 * arguments  : a 点1
 *              b 点2
 * return     : 距离
 */
float distance(float a[3], float b[3])
{
    float diff[3];
    xsub(a, b, diff, 3);
    return sqrtf(diff[0]*diff[0] + diff[1]*diff[1] + diff[2]*diff[2]);
}

/* name       : xabss
 * decription : 内部函数，计算一组数据的绝对值
 * arguments  : a   输入数组
 *              out 输出数组
 *              n   数组长度
 * return     : void
 */
void xabss(float *a, float *out, size_t n)
{
    for (int i=0; i<n; i++) {
        out[i] = fabsf(a[i]);
    }
}

/* name       : xmax
 * decription : 内部函数，计算一组数据的最大值
 * arguments  : a   输入数组
 *              n   数组长度
 * return     : 最大值
 */
float xmax(float *a, size_t n)
{
    float max = 0;
    for (int i=0; i<n; i++) {
        if (a[i] > max) {
            max = a[i];
        }
    }
    return max;
}

/* name       : xmin
 * decription : 内部函数，计算一组数据的最小值
 * arguments  : a   输入数组
 *              n   数组长度
 * return     : 最小值
 */
float xmin(float *a, size_t n)
{
    float min = FLT_MAX;
    for (int i=0; i<n; i++) {
        if (a[i] < min) {
            min = a[i];
        }
    }
    return min;
}

/* name       : xdiv
 * decription : 内部函数，除法，将一组数据除以一个实数
 * arguments  : a   输入数组
 *              b   除数
 *              out 输出数组
 *              n   数组长度
 * return     : void
 */
void xdiv(float *a, float b, float *out, size_t n)
{
    for (int i=0; i<n; i++) {
        out[i] =  a[i] / b;
    }
}

/* name       : xmul
 * decription : 内部函数，数乘，将一组数据乘以一个实数
 * arguments  : a   输入数组
 *              b   乘数
 *              out 输出数组
 *              n   数组长度
 * return     : void
 */
void xmul(float *a, float b, float *out, size_t n)
{
    for (int i=0; i<n; i++) {
        out[i] = a[i] * b;
    }
}

/* name       : xadd
 * decription : 内部函数，加法，将两组数据对应相加
 * arguments  : a   输入数组1
 *              b   输入数组2
 *              out 输出数组
 *              n   数组长度
 * return     : void
 */
void xadd(float *a, float *b, float *out, size_t n)
{
    for (int i=0; i<n; i++) {
        out[i] = a[i] + b[i];
    }
}

/* name       : xsub
 * decription : 内部函数，减法，将两组数据对应相减
 * arguments  : a   输入数组1
 *              b   输入数组2
 *              out 输出数组
 *              n   数组长度
 * return     : void
 */
void xsub(float *a, float *b, float *out, size_t n)
{
    for (int i=0; i<n; i++) {
        out[i] = a[i] - b[i];
    }
}

/* name       : xcopy
 * decription : 内部函数，数组复制
 * arguments  : src   源数组
 *              dst   目标数组
 *              n     数组长度
 * return     : void
 */
void xcopy(float *src, float *dst, size_t n)
{
    for (int i=0; i<n; i++) {
        dst[i] = src[i];
    }
}
