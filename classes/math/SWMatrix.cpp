/*
 * SWMatrix.cpp
 *
 *  Created on: 17-07-2011
 *      Author: Trog
 */

#include "SWMatrix.h"
#include "constants.h"
#include <algorithm>

namespace math
{

SWMatrix::SWMatrix(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20,
		float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
	m[0][0] = m00;
	m[0][1] = m01;
	m[0][2] = m02;
	m[0][3] = m03;
	m[1][0] = m10;
	m[1][1] = m11;
	m[1][2] = m12;
	m[1][3] = m13;
	m[2][0] = m20;
	m[2][1] = m21;
	m[2][2] = m22;
	m[2][3] = m23;
	m[3][0] = m30;
	m[3][1] = m31;
	m[3][2] = m32;
	m[3][3] = m33;
}

SWMatrix SWMatrix::operator+(const SWMatrix &m) const
{
	SWMatrix result;

	for (unsigned char r = 0; r < 4; ++r)
	{
		for (unsigned char c = 0; c < 4; ++c)
		{
			result.m[r][c] = this->m[r][c] + m.m[r][c];
		}
	}

	return result;
}

SWMatrix SWMatrix::operator-(const SWMatrix &m) const
{
	SWMatrix result;

	for (unsigned char r = 0; r < 4; ++r)
	{
		for (unsigned char c = 0; c < 4; ++c)
		{
			result.m[r][c] = this->m[r][c] - m.m[r][c];
		}
	}

	return result;
}

SWMatrix SWMatrix::operator*(const SWMatrix &m) const
{
	SWMatrix result;

	for (unsigned char r = 0; r < 4; ++r)
	{
		for (unsigned char c = 0; c < 4; ++c)
		{
			for (unsigned char i = 0; i < 4; ++i)
			{
				result.m[r][c] += this->m[r][i] * m.m[i][c];
			}
		}
	}

	return result;
}

SWMatrix SWMatrix::operator*(float s) const
{
	SWMatrix result;

	for (unsigned char r = 0; r < 4; ++r)
	{
		for (unsigned char c = 0; c < 4; ++c)
		{
			result.m[r][c] *= s;
		}
	}

	return result;
}

SWMatrix &SWMatrix::transpose()
{
	using std::swap;

	swap(m[0][1], m[1][0]);
	swap(m[0][2], m[2][0]);
	swap(m[0][3], m[3][0]);
	swap(m[1][2], m[2][1]);
	swap(m[1][3], m[3][1]);
	swap(m[2][3], m[3][2]);

	return *this;
}

SWMatrix &SWMatrix::inverse()
{
	// eliminacja Gaussa z wyborem elementu podstawowego

	const unsigned char rowSize = 8 * sizeof(float);
	float tm[4][8];
	float tmCopy[8];

	bzero(tm, 4 * rowSize);

	// kopiujemy macierz do tablicy tymczasowej
	for (unsigned char r = 0; r < 4; ++r)
	{
		for (unsigned char c = 0; c < 4; ++c)
		{
			tm[r][c] = m[r][c];
		}
	}

	// doklejamy macierz jednostkowa z prawej strony
	for (unsigned char r = 0; r < 4; ++r)
	{
		tm[r][r + 4] = 1.f;
	}

	// sprowadzenie do macierzy gornotrojkatnej
	for (unsigned char i = 0; i < 4; ++i)
	{
		// wyznaczamy wiersz z maksymalnym (co do wartosci bezwzglednej) wspolczynnikiem
		float maxCoefficient = tm[i][i];
		unsigned char rowWithMax = i;
		for (unsigned char r = i; r < 4; ++r)
		{
			for (unsigned char c = i; c < 4; ++c)
			{
				if (fabs(tm[r][c]) > maxCoefficient)
				{
					maxCoefficient = fabs(tm[r][c]);
					rowWithMax = r;
				}
			}
		}

		// zamieniamy wiersze kolejnoscia, tak aby i-ty byl zawsze tym z najwiekszym wspolczynnikiem
		if (rowWithMax != i)
		{
			memcpy(tmCopy, tm + rowWithMax, rowSize);
			memcpy(tm + rowWithMax, tm + i, rowSize);
			memcpy(tm + i, tmCopy, rowSize);
		}

		// eleminacja zmiennych
		for (unsigned char r = i + 1; r < 4; ++r)
		{
			float mult = tm[r][i] / tm[i][i];
			for (unsigned char c = i; c < 8; ++c)
			{
				tm[r][c] -= tm[i][c] * mult;
			}
		}
	}

	// postepowanie odwrotne
	for (short c = 3; c >= 0; --c)
	{
		for (short r = c - 1; r >= 0; --r)
		{
			float mult = tm[r][c] / tm[c][c];
			for (unsigned char i = 0; i < 8; ++i)
			{
				tm[r][i] -= tm[c][i] * mult;
			}
		}

		float div = tm[c][c];
		for (unsigned char i = c; i < 8; ++i)
		{
			tm[c][i] /= div;
		}

	}

	// przepisujemy spowrotem z tablicy tymczasowej do wlasciwej (atrybutu klasy)
	for (unsigned char r = 0; r < 4; ++r)
	{
		for (unsigned char c = 0; c < 4; ++c)
		{
			m[r][c] = tm[r][c + 4];
		}
	}

	return *this;
}

}
