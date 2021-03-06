/*
 * Matrix44.cpp
 *
 *  Created on: Dec 8, 2011
 *      Author: ashleygwinnell
 */

#include "Matrix44.h"

namespace ARK {
	namespace Util {

		Matrix44::Matrix44():
			col() {
			m_x = &col[0];
			m_y = &col[1];
			m_z = &col[2];
			m_w = &col[3];

			identity();
		}
		Matrix44::Matrix44(float vals[16]): col() {
			m_x = &col[0];
			m_y = &col[1];
			m_z = &col[2];
			m_w = &col[3];

			col[0][0] = vals[0];
			col[0][1] = vals[1];
			col[0][2] = vals[2];
			col[0][3] = vals[3];
			col[1][0] = vals[4];
			col[1][1] = vals[5];
			col[1][2] = vals[6];
			col[1][3] = vals[7];
			col[2][0] = vals[8];
			col[2][1] = vals[9];
			col[2][2] = vals[10];
			col[2][3] = vals[11];
			col[3][0] = vals[12];
			col[3][1] = vals[13];
			col[3][2] = vals[14];
			col[3][3] = vals[15];
		}

		const Vector4& Matrix44::getX() { return *m_x; }
		const Vector4& Matrix44::getY() { return *m_y; }
		const Vector4& Matrix44::getZ() { return *m_z; }
		const Vector4& Matrix44::getW() { return *m_w; }

		void Matrix44::toValue(float v) {
			m_x->toValue(v);
			m_y->toValue(v);
			m_z->toValue(v);
			m_w->toValue(v);
		}
		void Matrix44::toZero() {
			toValue(0);
		}
		void Matrix44::toOne() {
			toValue(1);
		}


		void Matrix44::identity() {
			col[0][0] = 1.0f;
			col[0][1] = 0.0f;
			col[0][2] = 0.0f;
			col[0][3] = 0.0f;

			col[1][0] = 0.0f;
			col[1][1] = 1.0f;
			col[1][2] = 0.0f;
			col[1][3] = 0.0f;

			col[2][0] = 0.0f;
			col[2][1] = 0.0f;
			col[2][2] = 1.0f;
			col[2][3] = 0.0f;

			col[3][0] = 0.0f;
			col[3][1] = 0.0f;
			col[3][2] = 0.0f;
			col[3][3] = 1.0f;

		}

		void Matrix44::ortho2d(float l, float r, float b, float t, float n, float f) {
			//http://www.gruzzlymug.com/projects/viseng/doc/d9/d7b/matrix44_8cpp-source.html
			glm::mat4 m = glm::ortho(l, r, b, t, n, f);
			set(m);
			return;

			float width = r - l;
			float height = t - b;
			float depth = f - n;

			col[0][0] = 2.0f / width;
			col[0][1] = 0.0f;
			col[0][2] = 0.0f;
			col[0][3] = 0.0f;

			col[1][0] = 0.0f;
			col[1][1] = 2.0f / height;
			col[1][2] = 0.0f;
			col[1][3] = 0.0f;

			col[2][0] = 0.0f;
			col[2][1] = 0.0f;
			col[2][2] = 2.0f / depth;
			col[2][3] = 0.0f;

			col[3][0] = -(r + l) / width;
			col[3][1] = -(t + b) / height;
			col[3][2] = -(f + n) / depth;
			col[3][3] = 1.0f;
		}
		void Matrix44::frustum(float l, float r, float b, float t, float n, float f) {
			col[0][0] = 2 * n / (r - l);
			col[0][1] = 0;
			col[0][2] = 0;
			col[0][3] = 0;

			col[1][0] = 0;
			col[1][1] = 2 * n / (t - b);
			col[1][2] = 0;
			col[1][3] = 0;

			col[2][0] = (r + l) / (r - l);
			col[2][1] = (t + b) / (t - b);
			col[2][2] = -(f + n) / (f - n);
			col[2][3] = -1;

			col[3][0] = 0;
			col[3][1] = 0;
			col[3][2] = -2 * f * n / (f - n);
			col[3][3] = 0;
		}
		void Matrix44::perspective(float fovDegrees, float aspectRatio, float znear, float zfar)
		{
			glm::mat4 m = glm::perspective(fovDegrees, aspectRatio, znear, zfar);
			set(m);
			return;
			/*float scale = tan(MathUtil::toRadians(fovDegrees * 0.5)) * znear;
		    float r = aspectRatio * scale;
		    float l = -r;
		    float t = scale;
		    float b = -t;
		    frustum(l, r, b, t, znear, zfar);*/

			/*float ymax = znear * tan(fov * PI_OVER_360);
			float ymin = -ymax;
			float xmax = ymax * aspect;
			float xmin = ymin * aspect;

			//float width = xymax - xmin;
			//float height = xymax - ymin;
			float width = xmax - xmin;
			float height = ymax - ymin;

			float depth = zfar - znear;
			float q = -(zfar + znear) / depth;
			float qn = -2 * (zfar * znear) / depth;

			float w = 2 * znear / width;
			w = w / aspect;
			float h = 2 * znear / height;

			col[0][0] = w;
			col[0][1] = 0;
			col[0][2] = 0;
			col[0][3] = 0;

			col[1][0] = 0;
			col[1][1] = h;
			col[1][2] = 0;
			col[1][3] = 0;

			col[2][0] = 0;
			col[2][1] = 0;
			col[2][2] = q;
			col[2][3] = -1;

			col[3][0] = 0;
			col[3][1] = 0;
			col[3][2] = qn;
			col[3][3] = 0;*/
		}

		void Matrix44::lookAt(float eyeX, float eyeY, float eyeZ, float lookAtX, float lookAtY, float lookAtZ, float upX, float upY, float upZ) {

			glm::mat4 m = glm::lookAt(
				glm::vec3(eyeX, eyeY, eyeZ),
				glm::vec3(lookAtX, lookAtY, lookAtZ),
				glm::vec3(upX, upY, upZ)
			);
			set(m);
			return;

			/*
			//detail::tvec3<T, P> f(normalize(center - eye));
			//detail::tvec3<T, P> s(normalize(cross(f, up)));
			//detail::tvec3<T, P> u(cross(s, f));
			//eyeZ *= -1.0f;
			//lookAtZ *= -1.0f;

			float fx, fy, fz; // forward
			MathUtil::normalizevec3(&fx, &fy, &fz, lookAtX - eyeX, lookAtY - eyeY, lookAtZ - eyeZ);
			//MathUtil::normalizevec3(&fx, &fy, &fz, eyeX - lookAtX, eyeY - lookAtY, eyeZ - lookAtZ);

			float sx, sy, sz; // side
			MathUtil::crossvec3(&sx, &sy, &sz, fx, fy, fz, upX, upY, upZ);
			MathUtil::normalizevec3(&sx, &sy, &sz, sx, sy, sz);

			float ux, uy, uz; // up
			MathUtil::crossvec3(&ux, &uy, &uz, sx, sy, sz, fx, fy, fz);

			Matrix44<float> lookAtMatrix;
			//lookAtMatrix.identity(); // this is done in constructor

			lookAtMatrix[0][0] = sx;
			lookAtMatrix[1][0] = sy;
			lookAtMatrix[2][0] = sz;

			lookAtMatrix[0][1] = ux;
			lookAtMatrix[1][1] = uy;
			lookAtMatrix[2][1] = uz;

			lookAtMatrix[0][2] = -fx;
			lookAtMatrix[1][2] = -fy;
			lookAtMatrix[2][2] = -fz;

			lookAtMatrix[3][0] = MathUtil::dotproductvec3(sx, sy, sz, eyeX, eyeY, eyeZ) * -1.0f;// -dot(s, eye);
			lookAtMatrix[3][1] = MathUtil::dotproductvec3(ux, uy, uz, eyeX, eyeY, eyeZ) * -1.0f; // -dot(u, eye);
			lookAtMatrix[3][2] = MathUtil::dotproductvec3(fx, fy, fz, eyeX, eyeY, eyeZ); // dot(f, eye);

			//lookAtMatrix[0][3] = 0.0f;
			//lookAtMatrix[1][3] = 0.0f;
			//lookAtMatrix[2][3] = 0.0f;
			//lookAtMatrix[3][3] = 1.0f;

			*this *= lookAtMatrix;

			//return Result;*/
		}









        void Matrix44::inverse() {
            glm::mat4 mat;
            mat[0][0] = col[0][0];
            mat[0][1] = col[0][1];
            mat[0][2] = col[0][2];
            mat[0][3] = col[0][3];
            mat[1][0] = col[1][0];
            mat[1][1] = col[1][1];
            mat[1][2] = col[1][2];
            mat[1][3] = col[1][3];
            mat[2][0] = col[2][0];
            mat[2][1] = col[2][1];
            mat[2][2] = col[2][2];
            mat[2][3] = col[2][3];
            mat[3][0] = col[3][0];
            mat[3][1] = col[3][1];
            mat[3][2] = col[3][2];
            mat[3][3] = col[3][3];

            mat = glm::inverse(mat);
            set(mat);
        }



		string Matrix44::toString() {
			string s = "{";
			s += Cast::toString<float>(col[0][0]); s += string(",");
			s += Cast::toString<float>(col[0][1]); s += string(",");
			s += Cast::toString<float>(col[0][2]); s += string(",");
			s += Cast::toString<float>(col[0][3]); s += string(",");

			s += Cast::toString<float>(col[1][0]); s += string(",");
			s += Cast::toString<float>(col[1][1]); s += string(",");
			s += Cast::toString<float>(col[1][2]); s += string(",");
			s += Cast::toString<float>(col[1][3]); s += string(",");

			s += Cast::toString<float>(col[2][0]); s += string(",");
			s += Cast::toString<float>(col[2][1]); s += string(",");
			s += Cast::toString<float>(col[2][2]); s += string(",");
			s += Cast::toString<float>(col[2][3]); s += string(",");

			s += Cast::toString<float>(col[3][0]); s += string(",");
			s += Cast::toString<float>(col[3][1]); s += string(",");
			s += Cast::toString<float>(col[3][2]); s += string(",");
			s += Cast::toString<float>(col[3][3]);
			s += string("}");
			return s;
		}


		void Matrix44::set(const glm::mat4& m) {
			col[0][0] = m[0][0];
			col[0][1] = m[0][1];
			col[0][2] = m[0][2];
			col[0][3] = m[0][3];

			col[1][0] = m[1][0];
			col[1][1] = m[1][1];
			col[1][2] = m[1][2];
			col[1][3] = m[1][3];

			col[2][0] = m[2][0];
			col[2][1] = m[2][1];
			col[2][2] = m[2][2];
			col[2][3] = m[2][3];

			col[3][0] = m[3][0];
			col[3][1] = m[3][1];
			col[3][2] = m[3][2];
			col[3][3] = m[3][3];
		}

		float* Matrix44::pointer() {
			return &col[0][0];
		}
		const float* Matrix44::pointer() const {
			return &col[0][0];
		}


		Matrix44::~Matrix44() {

		}

		void Matrix44::scale(float x, float y, float z)
		{
			*this *= createScaleMatrix(x, y, z);
		}
		void Matrix44::translate(float x, float y, float z)
		{
		    *this *= createTranslateMatrix(x, y, z);
		}
		void Matrix44::rotate(float angle, float x, float y, float z) {
			// http://www.gruzzlymug.com/projects/viseng/doc/d9/d7b/matrix44_8cpp-source.html
			// normalise vector
			float len = (float) sqrt(x*x + y*y + z*z);
			if (len > 0.0f) {
				len = 1.0f / len;
			} else {
				len = 0.0f;
			}
			x *= len;
			y *= len;
			z *= len;

			// create rotation matrix
			Matrix44 rotMatrix = createRotationMatrix(angle, x, y, z);


			// do rotation
			*this *= rotMatrix;
		}

		Matrix44 Matrix44::createScaleMatrix(float x, float y, float z) {
			// http://en.wikipedia.org/wiki/Scaling_(geometry)
			Matrix44 scaleMatrix;
			//scaleMatrix.identity(); // this is done in constructor
			scaleMatrix[0][0] = x;
			scaleMatrix[1][1] = y;
			scaleMatrix[2][2] = z;
			scaleMatrix[3][3] = 1;
			return scaleMatrix;
		}

		Matrix44 Matrix44::createTranslateMatrix(float x, float y, float z) {
			// http://en.wikipedia.org/wiki/Translation_(geometry)
			Matrix44 translateMatrix;
			//translateMatrix.identity(); // this is done in constructor
			translateMatrix[3][0] = x;
			translateMatrix[3][1] = y;
			translateMatrix[3][2] = z;
			return translateMatrix;
		}


		Matrix44 Matrix44::createRotationMatrix(float angleDegrees, float x, float y, float z) {
			float angle = MathUtil::toRadians(angleDegrees);
			Matrix44 rotMatrix;

			float sinA, cosA;
			float invCosA;
			float xSq, ySq, zSq;

			sinA = (float) sin(angle);
			cosA = (float) cos(angle);
			invCosA = 1.0f - cosA;

			xSq = x * x;
			ySq = y * y;
			zSq = z * z;

			rotMatrix[0][0] = (invCosA * xSq) + (cosA);
			rotMatrix[1][0] = (invCosA * x * y) - (sinA * z );
			rotMatrix[2][0] = (invCosA * x * z) + (sinA * y );
			rotMatrix[3][0] = 0.0f;

			rotMatrix[0][1] = (invCosA * x * y) + (sinA * z);
			rotMatrix[1][1] = (invCosA * ySq) + (cosA);
			rotMatrix[2][1] = (invCosA * y * z) - (sinA * x);
			rotMatrix[3][1] = 0.0f;

			rotMatrix[0][2] = (invCosA * x * z) - (sinA * y);
			rotMatrix[1][2] = (invCosA * y * z) + (sinA * x);
			rotMatrix[2][2] = (invCosA * zSq) + (cosA);
			rotMatrix[3][2] = 0.0f;

			rotMatrix[0][3] = 0.0f;
			rotMatrix[1][3] = 0.0f;
			rotMatrix[2][3] = 0.0f;
			rotMatrix[3][3] = 1.0f;

			return rotMatrix;
		}


		Matrix44& Matrix44::operator*=(const Matrix44& m) {
			*this = *this * m;
			return *this;
		}

		Vector4& Matrix44::operator[](unsigned int i) {
			//assert (i<4);
			//if (i >= 4) { ErrorDialog::createAndShow("Invalid matrix44[] index."); exit(0); }
			return col[i];
		}
		const Vector4& Matrix44::operator[](unsigned int i) const {
			//assert (i<4);
			//if (i >= 4) { ErrorDialog::createAndShow("Invalid matrix44[] index."); exit(0); }
			return col[i];
		}


		Vector4 Matrix44::operator*(const Vector4& other) const {
		    Vector4 ret(other);
		    ret *= *this;
		    return ret;
		}

        Matrix44 Matrix44::operator*(const Matrix44& m) const {

			glm::mat4 lhs;
			lhs[0][0] = col[0][0];
			lhs[0][1] = col[0][1];
			lhs[0][2] = col[0][2];
			lhs[0][3] = col[0][3];
			lhs[1][0] = col[1][0];
			lhs[1][1] = col[1][1];
			lhs[1][2] = col[1][2];
			lhs[1][3] = col[1][3];
			lhs[2][0] = col[2][0];
			lhs[2][1] = col[2][1];
			lhs[2][2] = col[2][2];
			lhs[2][3] = col[2][3];
			lhs[3][0] = col[3][0];
			lhs[3][1] = col[3][1];
			lhs[3][2] = col[3][2];
			lhs[3][3] = col[3][3];
			glm::mat4 rhs;
			rhs[0][0] = m[0][0];
			rhs[0][1] = m[0][1];
			rhs[0][2] = m[0][2];
			rhs[0][3] = m[0][3];
			rhs[1][0] = m[1][0];
			rhs[1][1] = m[1][1];
			rhs[1][2] = m[1][2];
			rhs[1][3] = m[1][3];
			rhs[2][0] = m[2][0];
			rhs[2][1] = m[2][1];
			rhs[2][2] = m[2][2];
			rhs[2][3] = m[2][3];
			rhs[3][0] = m[3][0];
			rhs[3][1] = m[3][1];
			rhs[3][2] = m[3][2];
			rhs[3][3] = m[3][3];
			glm::mat4 rs = lhs * rhs;
			Matrix44 ret;
			ret.set(rs);
			return ret;

		}

		void Matrix44::set(const Matrix44& m) {
			col[0][0] = m[0][0];
			col[0][1] = m[0][1];
			col[0][2] = m[0][2];
			col[0][3] = m[0][3];

			col[1][0] = m[1][0];
			col[1][1] = m[1][1];
			col[1][2] = m[1][2];
			col[1][3] = m[1][3];

			col[2][0] = m[2][0];
			col[2][1] = m[2][1];
			col[2][2] = m[2][2];
			col[2][3] = m[2][3];

			col[3][0] = m[3][0];
			col[3][1] = m[3][1];
			col[3][2] = m[3][2];
			col[3][3] = m[3][3];
		}

		Matrix44 Matrix44::copy() {
			Matrix44 cpy;
			cpy.col[0][0] = col[0][0];
			cpy.col[0][1] = col[0][1];
			cpy.col[0][2] = col[0][2];
			cpy.col[0][3] = col[0][3];

			cpy.col[1][0] = col[1][0];
			cpy.col[1][1] = col[1][1];
			cpy.col[1][2] = col[1][2];
			cpy.col[1][3] = col[1][3];

			cpy.col[2][0] = col[2][0];
			cpy.col[2][1] = col[2][1];
			cpy.col[2][2] = col[2][2];
			cpy.col[2][3] = col[2][3];

			cpy.col[3][0] = col[3][0];
			cpy.col[3][1] = col[3][1];
			cpy.col[3][2] = col[3][2];
			cpy.col[3][3] = col[3][3];
			return cpy;
		}

	}
}
