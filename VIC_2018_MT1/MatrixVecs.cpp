#include "MatrixVecs.h"
#include <math.h>

Vector3::Vector3()
{
}

Vector3::Vector3(fp x, fp y, fp z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

fp Vector3::Length()
{
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

Vector3 Vector3::cross(Vector3 vec)
{
	return Vector3(v[1]*vec(2) - v[2] * vec(1), v[2] * vec(0) - v[0] * vec(2), v[0] * vec(1) - v[1] * vec(0));
}

fp Vector3::dot(Vector3 vec)
{
	return v[0] * vec(0) + v[1] * vec(1) + v[2] * vec(2);
}

Vector3 Vector3::add(Vector3 vec)
{
	return Vector3(v[0] + vec(0), v[1] + vec(1), v[2] + vec(2));
}

Vector3 Vector3::sub(Vector3 vec)
{
	return Vector3(v[0] - vec(0), v[1] - vec(1), v[2] - vec(2));
}

Vector3 Vector3::scale(fp s)
{
	return Vector3(v[0] * s, v[1] * s, v[2] * s);
}

fp & Vector3::operator()(int arg)
{
	return v[arg];
}

Matrix33::Matrix33(fp angX, fp angY, fp angZ)
{
	Matrix33 m = (Matrix33(angZ, 'z').Multiply(Matrix33(angY, 'y').Multiply(Matrix33(angX, 'x'))));

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			Matrix[i][j] = m(i, j);
		}
	}
}

Matrix33::Matrix33(fp ang, char c)
{
	if (c == 'X' || c == 'x') {
		Matrix[1][1] = cos(ang);
		Matrix[2][2] = cos(ang);
		Matrix[2][1] = sin(ang);
		Matrix[1][2] = -sin(ang);
	}
	else if (c == 'Z' || c == 'z') {
		Matrix[0][0] = cos(ang);
		Matrix[1][1] = cos(ang);
		Matrix[1][0] = sin(ang);
		Matrix[0][1] = -sin(ang);
	}
	else {
		Matrix[0][0] = cos(ang);
		Matrix[2][2] = cos(ang);
		Matrix[0][2] = sin(ang);
		Matrix[2][0] = -sin(ang);
	}
}

void Matrix33::RotX(fp ang)
{
	Matrix[1][1] = cos(ang);
	Matrix[2][2] = cos(ang);
	Matrix[2][1] = sin(ang);
	Matrix[1][2] = -sin(ang);
}

void Matrix33::RotZ(fp ang)
{
	Matrix[0][0] = cos(ang);
	Matrix[1][1] = cos(ang);
	Matrix[1][0] = sin(ang);
	Matrix[0][1] = -sin(ang);
}

Matrix33 Matrix33::RotByX(fp ang)
{
	Matrix33 M = Matrix33();
	M.RotX(ang);

	Matrix33 nM = Multiply(M);

	return nM;
}

Matrix33 Matrix33::RotByZ(fp ang)
{
	Matrix33 M = Matrix33();
	M.RotZ(ang);

	Matrix33 nM = Multiply(M);
	return nM;
}

Vector3
Matrix33::Multiply(Vector3 b) {
	Vector3 vec = Vector3();
	for (int i = 0; i < 3; i++) {
		vec.v[i] = Matrix[i][0] * b.v[0] + Matrix[i][1] * b.v[1] + Matrix[i][2] * b.v[2];
	}

	return vec;
}
Vector3
Matrix33::Multiply(Vector3 b, fp s) {
	Vector3 vec = Vector3();
	for (int i = 0; i < 3; i++) {
		vec.v[i] = Matrix[i][0] * b.v[0] * s + Matrix[i][1] * b.v[1] * s + Matrix[i][2] * b.v[2] * s;
	}

	return vec;
}

Matrix33
Matrix33::Multiply(Matrix33 p) {

	Matrix33 m = Matrix33();
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			m.Matrix[i][j] = Matrix[i][0] * p(j,0) + Matrix[i][1] * p(j, 1) + Matrix[i][2] * p(j, 2);
		}
	}

	return m;
}

Matrix33 Matrix33::invert()
{
	double d = det();

	double invdet = 1.0 / d;

	Matrix33 minv; // inverse of matrix m
	minv.Matrix[0][0] = (Matrix[1][1] * Matrix[2][2] - Matrix[2][1] * Matrix[1][2]) * invdet;
	minv.Matrix[0][1] = (Matrix[0][2] * Matrix[2][1] - Matrix[0][1] * Matrix[2][2]) * invdet;
	minv.Matrix[0][2] = (Matrix[0][1] * Matrix[1][2] - Matrix[0][2] * Matrix[1][1]) * invdet;
	minv.Matrix[1][0] = (Matrix[1][2] * Matrix[2][0] - Matrix[1][0] * Matrix[2][2]) * invdet;
	minv.Matrix[1][1] = (Matrix[0][0] * Matrix[2][2] - Matrix[0][2] * Matrix[2][0]) * invdet;
	minv.Matrix[1][2] = (Matrix[1][0] * Matrix[0][2] - Matrix[0][0] * Matrix[1][2]) * invdet;
	minv.Matrix[2][0] = (Matrix[1][0] * Matrix[2][1] - Matrix[2][0] * Matrix[1][1]) * invdet;
	minv.Matrix[2][1] = (Matrix[2][0] * Matrix[0][1] - Matrix[0][0] * Matrix[2][1]) * invdet;
	minv.Matrix[2][2] = (Matrix[0][0] * Matrix[1][1] - Matrix[1][0] * Matrix[0][1]) * invdet;

	return minv;
}

Matrix33 Matrix33::Multiply(Matrix33 p, fp s)
{
	Matrix33 m = Matrix33();
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			m.Matrix[i][j] = Matrix[i][0] * s*p(j, 0) + Matrix[i][1] * s* p(j, 1) + Matrix[i][2] * s* p(j, 2);
		}
	}

	return m;
}

fp Matrix33::det()
{
	return Matrix[0][0]*(Matrix[1][1]* Matrix[2][2] - Matrix[1][2] * Matrix[2][1]) - 
		Matrix[0][1] * (Matrix[1][0] * Matrix[2][2] - Matrix[1][2] * Matrix[2][0]) +
		Matrix[0][2] * (Matrix[1][0] * Matrix[2][1] - Matrix[1][1] * Matrix[2][0]);
}

void Matrix33::scale(fp s)
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j< 3; j++) {
			Matrix[i][j] = s * Matrix[i][j];
		}
	}
}

fp & Matrix33::operator()(int arg, int arg1)
{
	return Matrix[arg][arg1];
}

void AffineMap::RotX(fp ang)
{
	Matrix33 *old = M;
	*M = M->RotByX(ang);
	//delete &old;
}

void AffineMap::RotZ(fp ang)
{
	Matrix33 *old = M;
	*M = M->RotByZ(ang);
	//delete &old;
}

//AffineMap AffineMap::invert()
//{
//	
//
//	
//}

AffineMap AffineMap::invert()
{
	AffineMap invertedAM =  AffineMap();
	Matrix33 iM = M->invert();
	*(invertedAM.M) = iM;
	*(invertedAM.Tr) = iM.Multiply(*Tr).scale(-1);
	return invertedAM;
}

Vector3 AffineMap::transform(Vector3 v)
{
	return Tr->add(M->Multiply(v));
}

Vector3 AffineMap::rotate(Vector3 v)
{
	return M->Multiply(v);
}

void AffineMap::update(AffineMap am, fp scale)
{
	*M = M->Multiply((*am.M), scale);
}

void AffineMap::updateTr(Vector3 vec)
{
	*Tr = (*Tr).add(vec);
}

const float * AffineMap::getMap()
{
	return new float[16]{ (float)(*M)(0,0),(float)(*M)(1,0),(float)(*M)(2,0),0,
		(float)(*M)(0,1),(float)(*M)(1,1),(float)(*M)(2,1),0,
		(float)(*M)(0,2),(float)(*M)(1,2),(float)(*M)(2,2),0,
		(float)(*Tr)(0) ,(float)(*Tr)(1) ,(float)(*Tr)(2) ,1 };
}
