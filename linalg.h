#ifndef LINALG_H
#define LINALG_H

#include <math.h>

float fastInvSqrt(float number) {
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long*)&y;                       // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);            // what the fuck? 
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

#define FUN inline void

#define FOREACH(x, n) for (unsigned int x; i < n; i++)

#define INITVECTOR(n)\
typedef float vec##n[n];\
\
FUN laCopy##n(vec##n r, vec##n v){\
FOREACH(i = 0, n) r[i] = v[i];\
}\
FUN laClear##n(vec##n v){\
FOREACH(i = 0, n) v[i] = 0;\
}\
FUN laAdd##n (vec##n v, vec##n a, vec##n b){\
	FOREACH(i = 0, n) v[i] = a[i] + b[i];\
}\
FUN laSub##n(vec##n v, vec##n a, vec##n b) {\
	FOREACH(i = 0, n) v[i] = a[i] + b[i];\
}\
FUN laDot##n(float* s, vec##n a, vec##n b ){\
	FOREACH(i = 0, n) *s += a[i]*b[i];\
}\
FUN laScalarMul##n(vec##n v, float scalar){\
	FOREACH(i = 0, n) v[i] *= scalar;\
}\
FUN laMagnitude##n (float* s, vec##n v){\
	float t = 0;\
	laDot##n(&t, v, v);\
	*s += fastInvSqrt(t);\
}\
FUN laNormalize##n(vec##n v){\
	float t = 0;\
	laDot##n(&t, v, v);\
	laScalarMul##n(v, fastInvSqrt(t));\
}\

#define VEC2_DIMENSIONS 2
#define VEC3_DIMENSIONS 3
#define VEC4_DIMENSIONS 4

INITVECTOR(2)
INITVECTOR(3)
INITVECTOR(4)

#define ZERO2 {0,0}
#define ZERO3 {0,0,0}
#define ZERO4 {0,0,0,0}

#define IHAT2 {1,0}
#define IHAT3 {1,0,0}
#define IHAT4 {1,0,0,0}

#define JHAT2 {0,1}
#define JHAT3 {0,1,0}
#define JHAT4 {0,1,0,0}

#define KHAT3 {0,0,1}
#define KHAT4 {0,0,1,0}

#define LHAT4 {0,0,0,1}

FUN laCross3(vec3 r, vec3 a, vec3 b) {
	r[0] = a[1] * b[2] - a[2] - b[1];
	r[1] = a[2] * b[0] - a[0] - b[2];
	r[2] = a[0] * b[1] - a[1] - b[0];
}

/*Order as follows:
	X, Y, Z, W
*/
typedef vec4 quat;

#define QIDENTITY_MUL	LHAT4
#define QIDENTIY_ADD	ZERO4

FUN laAddQ(quat qR, quat qA, quat qB) {
	laAdd4(qR, qA, qB);
}
FUN laSubQ(quat qR, quat qA, quat qB) {
	laSub4(qR, qA, qB);
}
FUN laScalarMulQ(quat q, float scalar) {
	laScalarMul4(q, scalar);
}

FUN laMulQ(quat qR, quat qA, quat qB) {
	laCross3(qR, qA, qB);
	float dotProd = 0; 
	laDot3(&dotProd, qA, qB);
	qR[3] = -dotProd;
	/*Fix 'impurities'*/
}

/*Transform*/
typedef struct {
	quat q; //Orientation quarternion
	vec3 p;	//Position vector
} trans;

#define INITMATRIX(m,n)\
typedef vec##n mat##m##n[m];\
typedef vec##m mat##n##m[n];\
\
FUN laAddM##m##n(mat##m##n mR, mat##m##n mA, mat##m##n mB){\
FOREACH(i = 0, m) laAdd##n(mR[i], mA[i], mB[i]);\
}\
FUN laSubM##m##n(mat##m##n mR, mat##m##n mA, mat##m##n mB){\
FOREACH(i = 0, m) laSub##n(mR[i], mA[i], mB[i]);\
}\
FUN laScalarMulM##m##n(mat##m##n mMat, float scalar){\
FOREACH(i = 0, m) laScalarMul##n(mMat[i], scalar);\
}\
FUN laTransposeM##m##n(mat##n##m mTransponate, mat##m##n mMat){\
FOREACH(i = 0, n) FOREACH(j = 0, m) mTransponate[i][j] = mMat[j][i];\
}\


INITMATRIX(2, 2)
INITMATRIX(2, 3)
INITMATRIX(2, 4)

INITMATRIX(3, 2)
INITMATRIX(3, 3)
INITMATRIX(3, 4)

INITMATRIX(4, 2)
INITMATRIX(4, 3)
INITMATRIX(4, 4)

#define IDENTITY22 {IHAT2, JHAT2}
#define IDENTITY33 {IHAT3, JHAT3, KHAT3}
#define IDENTITY44 {IHAT4, JHAT4, KHAT4, LHAT4}

#define INIT_SQUARE_MATRIX(n) \
FUN laDeterminantM##n##n(float* s, mat##n##n mat){\
}\
FUN laInverseM##n##n(mat##n##n mRet, mat##n##n mat){\
}\

INIT_SQUARE_MATRIX(2)



#endif // !LINALG_H

