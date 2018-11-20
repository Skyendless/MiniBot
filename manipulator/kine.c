#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/*
 * 机械臂的D-H参数表，这里使用标准的D-H(SStandard Denavit–Hartenberg)对
 * 机械臂进行运动学建模。
 * +---+-----------+-----------+-----------+-----------+-----------+
 * | j |     theta |         d |         a |     alpha |    offset |
 * +---+-----------+-----------+-----------+-----------+-----------+
 * |  1|         q1|          0|          0|    -1.5708|          0|
 * |  2|         q2|          0|       75.7|          0|    -1.5708|
 * |  3|         q3|          0|          0|     1.5708|     1.5708|
 * |  4|         q4|      145.4|          0|    -1.5708|          0|
 * |  5|         q5|          0|          0|     1.5708|          0|
 * +---+-----------+-----------+-----------+-----------+-----------+
 *
 * 其中，offset是为了方便运动学推导，为关节的角度设置的偏移量，
 * offset会在进行运动学正解前被加到个各关节的角度上，再进行正解运算；
 * 同样地，在完成运动学逆解后，会从各关节角度减去offset，再进行输出。
 */

#define PI (3.141593f)

#define IN_RANGE(x, r1, r2) ( (x) >= ((r1)-0.0001f) && (x) < ((r2)+0.0001f) )
#define EQUAL(x1, x2) (fabsf((x1) - (x2)) < 0.0001f)

/******** 全局变量 ********/
float a2 = 75.7;
float d4 = 145.4;
float offsets[5] = { 0, -PI/2, PI/2, 0, 0};

/*
 * 各关节角度的运动范围限制，超过该范围的逆解会被直接过滤掉
 */
float jMinLims[5] = { -5*PI/6, -PI/2, -PI/2, -5*PI/6, -PI/2 };
float jMaxLims[5] = {  5*PI/6,  PI/2,  PI/2,  5*PI/6,  PI/2 };

/*
 * 上次解算的结果，用于ikineSmart中选择一个合适的解
 */
float lastJoints[5] = { 0, 0, 0, 0, 0 };
/******** 全局变量 ********/

/******** 内部函数 ********/
static void rotationMatrixToRpy(float rm[3][3], float rpy[3]);
static void rpyToRotationMatrix(float rpy[3], float rm[3][3]);
static int ikine_j1(float x, float y, float z, float j1[2]);
static int ikine_j2_j3(float x, float y, float z, float j1, float j2[2], float j3[2]);
static int ikine_j4_j5(float T[4][4], float j1, float j2, float j3, float j4[2], float j5[2]);
static int checkJointLim(float j, int jNum);
static float distance(float j1[5], float j2[5]);
/******** 内部函数 ********/

/* name       : fkine
 * decription : 运动学正解
 * arguments  : j      关节角度数组，用户需要确保长度至少为5，以防数组越界；
 *              pose   解算得到的位姿数组，含义为 [ x, y, z, roll, pitch, yaw ]；
 *                     用户需要确保长度至少为6，以防数组越界； 
 * return     : void
 */
void fkine(float j[5], float pose[6])
{
	float j1 = j[0] + offsets[0],
	      j2 = j[1] + offsets[1], 
	      j3 = j[2] + offsets[2],
	      j4 = j[3] + offsets[3],
	      j5 = j[4] + offsets[4];

	float c1 = cosf(j1), s1 = sinf(j1), c2 = cosf(j2), s2 = sinf(j2),
	      c3 = cosf(j3), s3 = sinf(j3), c4 = cosf(j4), s4 = sinf(j4),
	      c5 = cosf(j5), s5 = sinf(j5), c23 = cosf(j2+j3), s23 = sinf(j2+j3);
	/*
	 * t** 为连杆5相对于世界坐标系的位姿，用齐次变换矩阵表示为
	 *     T = [ t11 t12 t13 t14;
	 *           t21 t22 t23 t24;
	 *           t31 t32 t33 t34;
	 *             0   0   0   1; ]
	 */
	float t11 = - c5*(s1*s4 - c4*(c1*c2*c3 - c1*s2*s3)) - s5*(c1*c2*s3 + c1*c3*s2);
	float t12 = - c4*s1 - s4*(c1*c2*c3 - c1*s2*s3);
	float t13 = c5*(c1*c2*s3 + c1*c3*s2) - s5*(s1*s4 - c4*(c1*c2*c3 - c1*s2*s3));
	float t14 = c1*(d4*s23 + a2*c2);
	float t21 = c5*(c1*s4 - c4*(s1*s2*s3 - c2*c3*s1)) - s5*(c2*s1*s3 + c3*s1*s2);
	float t22 = c1*c4 + s4*(s1*s2*s3 - c2*c3*s1);
	float t23 = s5*(c1*s4 - c4*(s1*s2*s3 - c2*c3*s1)) + c5*(c2*s1*s3 + c3*s1*s2);
	float t24 = s1*(d4*s23 + a2*c2);
	float t31 = - c23*s5 - s23*c4*c5;
	float t32 = s23*s4;
	float t33 = c23*c5 - s23*c4*s5;
	float t34 = d4*c23 - a2*s2;
	/*
	printf("[ %10.6f %10.6f %10.6f %10.6f\n"
	       "  %10.6f %10.6f %10.6f %10.6f\n"
	       "  %10.6f %10.6f %10.6f %10.6f\n"
	       "  %10.6f %10.6f %10.6f %10.6f ]\n", 
	       t11, t12, t13, t14,
	       t21, t22, t23, t24,
	       t31, t32, t33, t34,
	       0.0f, 0.0f, 0.0f, 1.0f);
	*/

	/*
	 * R 为位姿齐次变换矩阵中的旋转矩阵，这里将旋转矩阵转换为RPY角，方便输出
	 *   R = [ t11 t12 t13;
	 *         t21 t22 t23;
	 *         t31 t32 t33; ]
	 */
	float R[3][3] = {t11, t12, t13, t21, t22, t23, t31, t32, t33};
	float rpy[3];
	rotationMatrixToRpy(R, rpy);

	pose[0] = t14;
	pose[1] = t24;
	pose[2] = t34;
	pose[3] = rpy[0];
	pose[4] = rpy[1];
	pose[5] = rpy[2];
}

/* name       : ikine
 * decription : 运动学逆解
 * arguments  : pose   待解算的位姿数组，用户需要确保长度至少为6，以防数组越界；
 *              js  解算结果，二维数组，每一行表示一组关节角度数据；
 *                     用户需要确保数组至少为4行，5列，以防数组越界；
 * return     : 得到的逆解个数，最多可能有4组解，无逆解时返回0；
 */
int ikine(float pose[6], float js[4][5])
{
	/*
	 * R, T 含义正解中相同
	 * 这里同样将用户输入的RPY角转换为旋转矩阵，方便计算
	 */
	float R[3][3];
	rpyToRotationMatrix(pose+3, R);
	float T[4][4];
	for (int r=0; r<3; r++) {
		for (int c=0; c<3; c++) {
			T[r][c] = R[r][c];
		}
	}
	for (int i=0; i<3; i++) {
		T[i][3] = pose[i];
	}
	T[3][0] = 0; T[3][1] = 0; T[3][2] = 0; T[3][3] = 1;

	float j1[2];
	int count = 0;
	int j1N = ikine_j1(pose[0], pose[1], pose[2], j1);
	for (int i=0; i<j1N; i++) {
		float j2[2];
		float j3[2];
		int j2N = ikine_j2_j3(pose[0], pose[1], pose[2], j1[i], j2, j3);
		for (int j=0; j<j2N; j++) {
			float j4[4], j5[4];
			int j4_j5N = ikine_j4_j5(T, j1[i], j2[j], j3[j], j4, j5);
			for (int k=0; k<j4_j5N; k++) {
				js[count][0] = j1[i] - offsets[0];
				js[count][1] = j2[j] - offsets[1];
				js[count][2] = j3[j] - offsets[2];
				js[count][3] = j4[k] - offsets[3];
				js[count][4] = j5[k] - offsets[4];
				count++;
			}
		}
	}

	return count;
}

/* name       : ikineSmart
 * decription : 运动学解算，与ikine的区别是，当存在多个逆解结果时，只选择合适的一组；
 * arguments  : pose   待解算的位姿数组，用户需要确保长度至少为6，以防数组越界；
 *              theta  解算结果，表示一组关节角度数据；
 *                     用户需要确保数组成都至少为5，以防数组越界；
 * return     : 得到的逆解个数，1或者0；
 */
int ikineSmart(float pose[6], float j[5])
{
	float t[4][5];
	int N = ikine(pose, t);
	if (N == 0) return 0;
	
	/*
	 * min distance 找出与上组逆解最接近的一个返回
	 * 这里用关节坐标下的欧氏距离来衡量与上一组解的接近程度
	 */
	float md = 200;
	int mi = 0;
	for (int i=0; i<N; i++) {
		float d = distance(lastJoints, t[i]);
		if (d < md) {
			md = d;
			mi = i;
		}
	}
	for (int i=0; i<5; i++) {
		j[i] = t[mi][i];
		lastJoints[i] = t[mi][i];
	}
	return 1;
}

/* name       : rotationMatrixToRpy
 * decription : 内部函数，将旋转矩阵转换为以roll，pitch，yaw表示的欧拉角，
 *              转换结果由rpy输出；
 * arguments  : rm    旋转矩阵，3x3的二维数组；
 *                    用户需要确保数组至少为3行3列，以防数组越界；
 *              rpy   表示roll，pitch，yaw的数组，
 *                    用户需要确保数组长度至少为3，以防数组越界；
 * return     : void
 */
void rotationMatrixToRpy(float rm[3][3], float rpy[3])
{
	rpy[0] = atan2f(rm[2][1], rm[2][2]);
	rpy[1] = atan2f(-rm[2][0], sqrtf(rm[2][1]*rm[2][1] + rm[2][2]*rm[2][2]));
	rpy[2] = atan2f(rm[1][0], rm[0][0]);
}

/* name       : rpyToRotationMatrix 
 * decription : 内部函数，将以roll，pitch，yaw表示的欧拉角转换为旋转矩阵，
 *              转换结果由rm输出；
 * arguments  : rpy   表示roll，pitch，yaw的数组，
 *                    用户需要确保数组长度至少为3，以防数组越界；
 *              rm    旋转矩阵，3x3的二维数组；
 *                    用户需要确保数组至少为3行3列，以防数组越界；
 * return     : 
 */
void rpyToRotationMatrix(float rpy[3], float rm[3][3])
{
	float cr = cosf(rpy[0]), cp = cosf(rpy[1]), cy = cosf(rpy[2]),
	      sr = sinf(rpy[0]), sp = sinf(rpy[1]), sy = sinf(rpy[2]);

	rm[0][0] = cp*cy;
	rm[0][1] = cy*sp*sr - cr*sy;
	rm[0][2] = sr*sy + cr*cy*sp;
	rm[1][0] = cp*sy;
	rm[1][1] = cr*cy + sp*sr*sy;
	rm[1][2] = cr*sp*sy - cy*sr;
	rm[2][0] = -sp;
	rm[2][1] = cp*sr;
	rm[2][2] = cp*cr;
}

/* name       : ikine_j1
 * decription : 内部函数，求第1关节逆解，结果由j1输出；
 * arguments  : x, y, z    目标位置
 *              j1         存放解算结果的数组，用户需要确保数组长度至少为2，以防数组越界；
 * return     : 解的个数，最多可能会有2个解，无解时返回0；
 */
int ikine_j1(float x, float y, float z, float j1[2])
{
	j1[0] = atan2(y, x);
	if (IN_RANGE(j1[0], 0, PI)) {
		j1[1] = j1[0] - PI;
	} else {
		j1[1] = j1[0] + PI;
	}
    
//	printf("j11 = %f, j12 = %f\n", j1[0], j1[1]);
	if (checkJointLim(j1[0], 1)) {
		if (checkJointLim(j1[1], 1)) {
			return 2;
		} else {
			return 1;
		}
	} else {
		if (checkJointLim(j1[1], 1)) {
			j1[0] = j1[1];
			return 1;
		} else {
			return 0;
		}	
	}
	return 2;
	
}

/* name       : ikine_j2_j3 
 * decription : 内部函数，求第2、3关节逆解，结果由j2，j3输出；
 * arguments  : x, y, z    目标位置
 *              j2         存放第2关节的解算结果，用户需要确保长度至少为2，以防数组越界；
 *              j3         存放第3关节的解算结果，用户需要确保长度至少为2，以防数组越界；
 * return     : 解的个数，最多可能会有2个解，无解时返回0；
 */
int ikine_j2_j3(float x, float y, float z, float j1, float j2[2], float j3[2])
{
	float c1 = cosf(j1);
	float m1 = x / c1, m2 = z;

	float sq2 = sqrtf(- a2*a2*a2*a2 + 2*a2*a2*d4*d4 + 2*a2*a2*m1*m1 + 2*a2*a2*m2*m2 - d4*d4*d4*d4 + 2*d4*d4*m1*m1 + 2*d4*d4*m2*m2 - m1*m1*m1*m1 - 2*m1*m1*m2*m2 - m2*m2*m2*m2);
	/*
	 * 当给定位置超出机械臂手臂运动空间时，会出现j2, j3轴无解的情况，
	 * 表现为这里对负数的开平方，会得到一个nan（Not A Number）的返回值，
	 * 这里通过判断isnan来判断有无逆解，isnan由math.h提供
	 */
	if (isnan(sq2)) {
		return 0;
	}
	j2[0] = -2*atanf((2*a2*m2 - sq2)/(a2*a2 + 2*a2*m1 - d4*d4 + m1*m1 + m2*m2));
	j2[1] = -2*atanf((2*a2*m2 + sq2)/(a2*a2 + 2*a2*m1 - d4*d4 + m1*m1 + m2*m2));

	float sq3 = sqrtf((- a2*a2 + 2*a2*d4 - d4*d4 + m1*m1 + m2*m2)*(a2*a2 + 2*a2*d4 + d4*d4 - m1*m1 - m2*m2));
	if (isnan(sq3)) {
		return 0;
	}

	j3[0] = -2*atanf((2*a2*d4 - sq3)/(a2*a2 + d4*d4 - m1*m1 - m2*m2));
	j3[1] = -2*atanf((2*a2*d4 + sq3)/(a2*a2 + d4*d4 - m1*m1 - m2*m2));
//	printf("j20 = %f, j31 = %f\n", j2[0] - offsets[1], j3[0] - offsets[2]);
//	printf("j21 = %f, j32 = %f\n", j2[1] - offsets[1], j3[1] - offsets[2]);
	if (checkJointLim(j2[0], 2) && checkJointLim(j3[0], 3)) {
		if (checkJointLim(j2[1], 2) && checkJointLim(j3[1], 3)) {
			return 2;
		} else {
			return 1;
		}
	} else {
		if (checkJointLim(j2[1], 2) && checkJointLim(j3[1], 3)) {
			j2[0] = j2[1];
			j3[0] = j3[1];
			return 1;
		} else {
			return 0;
		}
	}

	return 2;
}

/* name       : ikine_j4_j5 
 * decription : 内部函数，求第4、5关节逆解，结果由j4，j5输出；
 * arguments  : x, y, z    目标位置
 *              j4         存放第4关节的解算结果，用户需要确保长度至少为1，以防数组越界；
 *              j5         存放第5关节的解算结果，用户需要确保长度至少为1，以防数组越界；
 * return     : 解的个数，最多可能会有1个解，无解时返回0；
 */
int ikine_j4_j5(float T[4][4], float j1, float j2, float j3, float j4[2], float j5[2])
{
	float c1 = cosf(j1), s1 = sinf(j1), c2 = cosf(j2), s2 = sinf(j2),
	      c23 = cosf(j2+j3), s23 = sinf(j2+j3);
	j4[0] = atan2f(T[2][1]*s23 - T[0][1]*c23*c1 - T[1][1]*c23*s1, T[1][1]*c1 - T[0][1]*s1);
	j5[0] = atan2f(- T[2][0]*c23 - T[0][0]*s23*c1 - T[1][0]*s23*s1, T[2][2]*c23 + T[0][2]*s23*c1 + T[1][2]*s23*s1);

	float j41 = atan2f(- T[0][0]*s1 + T[1][0]*c1, T[0][0]*c23*c1 - T[2][0]*s23 + T[1][0]*c23*s1);
	float j42 = atan2f(- T[0][2]*s1 + T[1][2]*c1, T[0][2]*c23*c1 - T[2][2]*s23 + T[1][2]*c23*s1);
	
	/*
	 * 因为5自由度机械臂缺少一个自由度，不能达到任意位姿，在这种构型下，
	 * 表现为机械臂能在达到工作空间内的任意位置（在不限制关节运动范围时，
	 * j1, j2, j3一定有解），但不能以任意姿态达到。必须在一组约束条件下成立时，
	 * j4, j5才有解，这里表现为上面计算出的j4[0], j41, j42必须相等。
	 * 这里也可令j4[0], j41, j42在一定范围内相等，这样得到的解姿态与输入存在一定偏差，
	 * 偏差的大小取决于j4[0], j41, j42的接近程度。
	 */
	if (EQUAL(j4[0], PI) || EQUAL(j4[0], 0) || EQUAL(j4[0], -PI)) {
		// 处理边界点的情况，atan为以pi为周期的函数
		if (EQUAL(j41, PI) || EQUAL(j41, 0) || EQUAL(j41, -PI)) {
			j41 = j4[0];
		}
		if (EQUAL(j42, PI) || EQUAL(j42, 0) || EQUAL(j42, -PI)) {
			j42 = j4[0];
		}
	} else if (IN_RANGE(j4[0], 0, PI)) {
		// 将j41, j42转换到与j4[0]处于同一象限
		if (IN_RANGE(j41, -PI, 0)) {
			j41 += PI;
		}
		if (IN_RANGE(j42, -PI, 0)) {
			j42 += PI;
		}
	} else {
		if (IN_RANGE(j41, 0, PI)) {
			j41 -= PI;
		}
		if (IN_RANGE(j42, 0, PI)) {
			j42 -= PI;
		}

	}
	
//	if (!EQUAL(j4[0], j41) || !EQUAL(j4[0], j42)) {
//		return 0;
//	}
    j4[0] = (j4[0] + j41 + j42) / 3; // 取平均值作为j4
	if (!checkJointLim(j4[0], 4) || !checkJointLim(j5[0], 5)) {
		return 0;
	}
	return 1;
}

/* name       : checkJointLim
 * decription : 内部函数，检查角度j是否超出第jNum关节的运动范围；
 * arguments  : j          待检查角度，单位为弧度
 *              jNum       关节编号，取值范围为 1 ~ 5；
 * return     : 0          超出运动范围；
 *              1          没有超出运动范围；
 */
static int checkJointLim(float j, int jNum)
{
	j = j - offsets[jNum - 1];
	if (j > 2*PI) { j -= 2*PI; }
	if (j < -2*PI) { j += 2*PI; }
	return IN_RANGE(j, jMinLims[jNum - 1], jMaxLims[jNum - 1]);
}

/* name       : distance
 * decription : 内部函数，计算两组关节角度差值的平方和，即关节空间下的欧氏距离的平方，
 *              用来衡量两组关节角度的接近程度；
 * arguments  : j1         第1组关节角度，用户需要确保数组长度至少为5，以防数组越界；
 *              j2         第2组关节角度，用户需要确保数组长度至少为5，以防数组越界；
 * return     : 欧式距离的平方
 */
static float distance(float j1[5], float j2[5])
{
	float sum = 0;
	for (int i=0; i<5; i++) {
		sum += (j1[i] - j2[i]) * (j1[i] - j2[i]);
	}
	return sum;
}

