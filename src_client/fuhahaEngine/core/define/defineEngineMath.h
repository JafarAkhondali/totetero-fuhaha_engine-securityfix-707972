#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// -------- 行列演算

// ベクトル行列構造体
struct engineMathVector3{union{struct{GLfloat x, y, z;}; GLfloat v[3];};};
struct engineMathVector4{union{struct{GLfloat x, y, z, w;}; struct{GLfloat r, g, b, a;}; GLfloat v[4];};};
struct engineMathMatrix33{union{struct{GLfloat m00, m01, m02, m10, m11, m12, m20, m21, m22;}; GLfloat m[9];};};
struct engineMathMatrix44{union{struct{GLfloat m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33;}; GLfloat m[16];};};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

