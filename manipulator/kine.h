#ifndef __KINE_H__
#define __KINE_H__

#ifdef __cplusplus
extern "C" {
#endif

void fkine(float j[5], float pose[6]);
int ikine(float pose[6], float js[8][5]);
int ikineSmart(float pose[6], float j[5]);

#ifdef __cplusplus
}
#endif

#endif //__KINE_H__
