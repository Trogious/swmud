/*
 * test.cpp
 *
 *  Created on: 2010-08-21
 *      Author: Trog
 *
 * Ten test to tylko przyklad jak robic testy do tego, co sie zakodowalo.
 * Nie musicie tutaj dodawac zadnych, ale mozecie. Wazne, ze jest mechanizm,
 * ktory wspiera testy jednostkowe (ang. unit tests).
 */

#include <iostream>
#include <vector>
#include <iterator>
#include <cmath>
#include <SWInt64.h>
#include <SWDbArray.h>
#include <SWHome.h>
#include <SWItemHome.h>
#include <SWTimeStamp.h>
#include <SWPlayerCharacter.h>
#include <SWMatrix.h>
#include <SWSphere.h>
#include <math/constants.h>
//#include <algorithm>

extern FILE *fpReserve;
extern FILE *fpLOG;

void init()
{
	const char *NULL_FILE = "/dev/null";

	if( !(fpReserve = fopen( NULL_FILE, "r" )) )
	{
		perror( NULL_FILE );
		exit( 1 );
	}

	if( !(fpLOG = fopen( NULL_FILE, "r" )) )
	{
		perror( NULL_FILE );
		exit( 1 );
	}
}

void test001()
{
	SWString str;

	str = "    ala ma kota   ";
	cout << "'" << str.trimLeft() << "'" << endl;
	str = "    ala ma kota   ";
	cout << "'" << str.trimRight() << "'" << endl;
	str = "    ala ma kota   ";
	cout << "'" << str.trim() << "'" << endl;
	str = "    ala ma kota   ";
	cout << "'" << str.trimLeft().trimRight() << "'" << endl;

	cout << endl;
	cout << endl;

	vector<SWInt64> v(10);
	for (SWInt64 i = 0; i < (long long) 10; ++i)
	{
		v[i] = i;
		cout << v[i] << endl;
	}
	cout << SWDbArray::toQueryValue(v) << endl;

	str = "ala,ma,kota,w,dupie,x,x,x,x,,,,,x,,x,,x,";
	vector<SWString> splitted = str.split(",");
	::copy(splitted.begin(), splitted.end(), ostream_iterator<SWString> (cout, "|"));
	cout << endl;
	str = "aladupamadupakotadupawdupadupiedupaxdupaxdupaxdupaxdupadupadupadupadupaxdupadupaxdupadupaxdupa";
	splitted = str.split("dupa");
	::copy(splitted.begin(), splitted.end(), ostream_iterator<SWString> (cout, ","));
	cout << endl;
	str = "alamakota";
	splitted = str.split("");
	::copy(splitted.begin(), splitted.end(), ostream_iterator<SWString> (cout, "|"));
	cout << endl;
	str = " ";
	splitted = str.split("|");
	::copy(splitted.begin(), splitted.end(), ostream_iterator<SWString> (cout, ","));
	cout << endl;
	str = "|";
	splitted = str.split("|");
	::copy(splitted.begin(), splitted.end(), ostream_iterator<SWString> (cout, ","));
	cout << endl;

	cout << endl;
	cout << endl;

	str = "{0,1,2,3,4,5,6,7,8,9}";
	cout << str << endl;
	vector<SWInt64> v1 = SWDbArray::toArray<SWInt64, vector>(str);
	::copy(v1.begin(), v1.end(), ostream_iterator<SWInt64> (cout, ","));
	cout << endl;
	str = "{\"ala\",\"ma\",\"kota\",\"w\",\"dupie\"}";
	cout << str << endl;
	vector<SWString> v2 = SWDbArray::toArray<SWString, vector>(str, "\"");
	::copy(v2.begin(), v2.end(), ostream_iterator<SWString> (cout, ","));
	cout << endl;
}

void test002()
{
	using namespace player;
	SWHome h;

	h.addItem(new SWItemHome(2));
	h.addItem(new SWItemHome(3));
	h.addItem(new SWItemHome(6553));
	h.addItem(new SWItemHome(6600));
	h.addItem(new SWItemHome(4));
	for (list<SWItemHome *>::const_iterator it = h.getItems().begin(); it != h.getItems().end(); ++it)
	{
		cout << (*it)->getVnum() << " ";
	}
	cout << endl;
}

void test003()
{
	SWTime t;
	cout << static_cast<SWString> (t) << endl;

	SWTimeStamp ts;
	cout << ts.getDescriptive() << endl;

	SWDate *d = new SWDate;
	cout << d->getDescriptive() << endl;
	d = new SWTimeStamp;
	cout << d->getDescriptive() << endl;
	delete d;

	cout << endl;
}

void test004()
{
	list<SWString> t;
	t.push_back("pixel");
	t.push_back("nie");
	t.push_back("ma");
	t.push_back("mozgu");

	cout << SWString::join(t, ",").prepend("(").append(")") << endl;
}

void test005()
{
	using namespace player;

	SWPlayerCharacter c;
	c.setSpeaking(new SWLanguageSkill("basic", 100));
	cout << "insert query:" << endl;
	cout << c.getInsertQuery() << endl;
	cout << "update query:" << endl;
	cout << c.getUpdateQuery() << endl;
	//	cout << "select query:" << endl;
	//	cout << c.getSelectQuery() << endl;
	cout << "delete query:" << endl;

	class DeleteQuery: public SWPlayerCharacter
	{
	public:
		SWString getDeleteQuery() const
		{
			return SWPlayerCharacter::getDeleteQuery();
		}
	};
	cout << static_cast<DeleteQuery&> (c).getDeleteQuery() << endl;
}

void test006()
{
	SWDate now;

	SWDate later = SWDate().addDays(7);

	cout << "now: " << now.getTimeT() << ", later: " << later.getTimeT() << endl;
	cout << endl;
}

void test007()
{
	using namespace player;

	SWPlayerData *p = SWPlayerData::loadPlayer("Trog", "dupa.8");
	if (p)
	{
		cout << "Player: " << p->getName() << "(" << p->getId() << ", " << static_cast<SWString> (p->getLastPlayed())
				<< ") loaded." << endl;
		delete p;
	}
	else
	{
		cout << "Player NOT loaded!" << endl;
	}

	p = SWPlayerData::loadPlayer("Trog", "alamakota");
	if (p)
	{
		cout << "Player: " << p->getName() << "(" << p->getId() << ", " << static_cast<SWString> (p->getLastPlayed())
				<< ") loaded." << endl;
		delete p;
	}
	else
	{
		cout << "Player NOT loaded!" << endl;
	}

	p = SWPlayerData::loadPlayer("Jot", "alamakota");
	if (p)
	{
		cout << "Player: " << p->getName() << "(" << p->getId() << ", " << static_cast<SWString> (p->getLastPlayed())
				<< ") loaded." << endl;
		delete p;
	}
	else
	{
		cout << "Player NOT loaded!" << endl;
	}
	cout << endl;
}

void test008()
{
	SWTimeStamp ts = SWTimeStamp::fromString("2011-06-22 10:11:33");

	cout << "timestamp: " << static_cast<SWString> (ts) << endl;
	cout << "timestamp: " << ts.getDescriptive() << endl;
	cout << endl;
}

void test009()
{
	using namespace math;

	struct SWTestMatrix: public SWMatrix
	{
		void out()
		{
			for (unsigned char r = 0; r < 4; ++r)
			{
				cout << "[ ";
				for (unsigned char c = 0; c < 4; ++c)
				{
					cout << ((fabs(m[r][c]) < SWEpsilon) ? .0f : m[r][c]) << " ";
				}
				cout << "]" << endl;
			}
		}
	};

	SWTestMatrix *m1 = (SWTestMatrix*) new SWMatrix(1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,
			1.f, 1.f, 1.f);
	SWTestMatrix *m2 = (SWTestMatrix*) new SWMatrix(1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,
			1.f, 1.f, 2.f);
	SWTestMatrix *mm = (SWTestMatrix*) new SWMatrix(*m1 * *m2);

	cout << "m1: " << endl;
	m1->out();
	cout << "m2: " << endl;
	m2->out();
	cout << "m1 x m2: " << endl;
	mm->out();

	SWTestMatrix *mt = (SWTestMatrix*) new SWMatrix(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f,
			13.f, 14.f, 15.f);

	cout << "mt: " << endl;
	mt->out();
	mt->transpose();
	cout << "mT: " << endl;
	mt->out();
	mt->transpose();
	cout << "mTT: " << endl;
	mt->out();

	SWTestMatrix *mi = (SWTestMatrix*) new SWMatrix(1.f, 1.f, 2.f, 3.f, 4.f, 1.f, 1.f, 1.f, 1.f, 2.f, 3.f, 2.f, 3.f,
			4.f, 1.f, 2.f);
	/* proper inverse:
	 * -0.0333  0.3000 -0.0667 -0.0333
	 * -0.2000 -0.2000  0.1000  0.3000
	 * -0.2333  0.1000  0.5333 -0.2333
	 * 0.5667 -0.1000 -0.3667  0.0667
	 */

	SWTestMatrix *mi2 = (SWTestMatrix*) new SWMatrix(*mi);
	cout << "mi: " << endl;
	mi->out();
	cout << "mi2: " << endl;
	mi2->out();
	mi->inverse();
	cout << "mi2: " << endl;
	mi2->out();
	cout << "mi: " << endl;
	mi->out();
	cout << "mi x mi2: " << endl;
	SWTestMatrix *mmi = (SWTestMatrix*) new SWMatrix(*mi * *mi2);
	mmi->out();
	cout << "mi2 x mI: " << endl;
	SWTestMatrix *mI = (SWTestMatrix*) new SWMatrix(*mi2 * SWMatrix::getIdentity());
	mI->out();
	cout << "mi: " << endl;
	mi->inverse();
	mi->out();

	delete m1;
	delete m2;
	delete mm;
	delete mt;
	delete mi;
	delete mi2;
	delete mmi;
	delete mI;
	cout << endl;
}

void test010()
{
	using namespace math;

	SWSphere s(SWVector(0,0,100), 10);
	SWRay r(SWVector(0, 0, 0), SWVector(0,0,100));
	cout << "ray " << (s.intersect(r) ? "intersects" : "doesn't intersect") << " sphere" << endl;

	s = SWSphere(SWVector(0,0,100), 10);
	r = SWRay(SWVector(0, 0, 0), SWVector(0,10,100));
	cout << "ray " << (s.intersect(r) ? "intersects" : "doesn't intersect") << " sphere" << endl;

	r = SWRay(SWVector(0, 0, 0), SWVector(0,11,100));
	cout << "ray " << (s.intersect(r) ? "intersects" : "doesn't intersect") << " sphere" << endl;

	r = SWRay(SWVector(0, 0, 0), SWVector(0,10,100));
	std::list<SWPoint> *points = new std::list<SWPoint>;
	if (s.intersect(r, points))
	{
		cout << "ray intersects sphere at: ";
		for (std::list<SWPoint>::const_iterator it = points->begin(); it != points->end(); ++it)
		{
			cout << "(" << it->getX() << "," << it->getY() << "," << it->getZ() << "); ";
		}
		cout << endl;
	}
	else
	{
		cout << "ray doesn't intersect sphere" << endl;
	}
	delete points;

	r = SWRay(SWVector(0, 0, 0), SWVector(0,9,99));
	points = new std::list<SWPoint>;
	if (s.intersect(r, points))
	{
		cout << "ray intersects sphere at: ";
		for (std::list<SWPoint>::const_iterator it = points->begin(); it != points->end(); ++it)
		{
			cout << "(" << it->getX() << "," << it->getY() << "," << it->getZ() << "); ";
		}
		cout << endl;
	}
	else
	{
		cout << "ray doesn't intersect sphere" << endl;
	}
	delete points;

	r = SWRay(SWVector(0, 0, 0), SWVector(0,10,99.9f));
	points = new std::list<SWPoint>;
	if (s.intersect(r, points))
	{
		cout << "ray intersects sphere at: ";
		for (std::list<SWPoint>::const_iterator it = points->begin(); it != points->end(); ++it)
		{
			cout << "(" << it->getX() << "," << it->getY() << "," << it->getZ() << "); ";
		}
		cout << endl;
	}
	else
	{
		cout << "ray doesn't intersect sphere" << endl;
	}
	delete points;

	r = SWRay(SWVector(0, 0, 0), SWVector(0,20,200));
	points = new std::list<SWPoint>;
	if (s.intersect(r, points))
	{
		cout << "ray intersects sphere at: ";
		for (std::list<SWPoint>::const_iterator it = points->begin(); it != points->end(); ++it)
		{
			cout << "(" << it->getX() << "," << it->getY() << "," << it->getZ() << "); ";
		}
		cout << endl;
	}
	else
	{
		cout << "ray doesn't intersect sphere" << endl;
	}
	delete points;

	r = SWRay(SWVector(0, 0, 0), SWVector(0,21,200));
	points = new std::list<SWPoint>;
	if (s.intersect(r, points))
	{
		cout << "ray intersects sphere at: ";
		for (std::list<SWPoint>::const_iterator it = points->begin(); it != points->end(); ++it)
		{
			cout << "(" << it->getX() << "," << it->getY() << "," << it->getZ() << "); ";
		}
		cout << endl;
	}
	else
	{
		cout << "ray doesn't intersect sphere" << endl;
	}
	delete points;

	r = SWRay(SWVector(0, 0, 91), SWVector(0,0,91));
	cout << "ray " << (s.intersect(r) ? "intersects" : "doesn't intersect") << " sphere" << endl;

	points = new std::list<SWPoint>;
	if (s.intersect(r, points))
	{
		cout << "ray intersects sphere at: ";
		for (std::list<SWPoint>::const_iterator it = points->begin(); it != points->end(); ++it)
		{
			cout << "(" << it->getX() << "," << it->getY() << "," << it->getZ() << "); ";
		}
		cout << endl;
	}
	else
	{
		cout << "ray doesn't intersect sphere" << endl;
	}
	delete points;

	r = SWRay(SWVector(10, 0, 0), SWVector(10,0,100));
	points = new std::list<SWPoint>;
	if (s.intersect(r, points))
	{
		cout << "ray intersects sphere at: ";
		for (std::list<SWPoint>::const_iterator it = points->begin(); it != points->end(); ++it)
		{
			cout << "(" << it->getX() << "," << it->getY() << "," << it->getZ() << "); ";
		}
		cout << endl;
	}
	else
	{
		cout << "ray doesn't intersect sphere" << endl;
	}
	delete points;

	s = SWSphere(SWVector(0,0,10), 2);
	r = SWRay(SWVector(0, 2, 0), SWVector(0,0,10));
	points = new std::list<SWPoint>;
	if (s.intersect(r, points))
	{
		cout << "ray intersects sphere at: ";
		for (std::list<SWPoint>::const_iterator it = points->begin(); it != points->end(); ++it)
		{
			cout << "(" << it->getX() << "," << it->getY() << "," << it->getZ() << "); ";
		}
		cout << endl;
	}
	else
	{
		cout << "ray doesn't intersect sphere" << endl;
	}
	delete points;

	s = SWSphere(SWVector(0,0,10), 2);
	r = SWRay(SWVector(3, 0, 10), SWVector(-1,0,0));
	points = new std::list<SWPoint>;
	if (s.intersect(r, points))
	{
		cout << "ray intersects sphere at: ";
		for (std::list<SWPoint>::const_iterator it = points->begin(); it != points->end(); ++it)
		{
			cout << "(" << it->getX() << "," << it->getY() << "," << it->getZ() << "); ";
		}
		cout << endl;
	}
	else
	{
		cout << "ray doesn't intersect sphere" << endl;
	}
	delete points;


	cout << endl;
}

int main(int argc, char **argv)
{
	cout << endl << "==== init ====" << endl;
	init();
	cout << endl << "==== test001 ====" << endl;
	test001();
	cout << endl << "==== test002 ====" << endl;
	test002();
	cout << endl << "==== test003 ====" << endl;
	test003();
	cout << endl << "==== test004 ====" << endl;
	test004();
	cout << endl << "==== test005 ====" << endl;
	test005();
	cout << endl << "==== test006 ====" << endl;
	test006();
	cout << endl << "==== test007 ====" << endl;
	test007();
	cout << endl << "==== test008 ====" << endl;
	test008();
	cout << endl << "==== test009 ====" << endl;
	test009();
	cout << endl << "==== test010 ====" << endl;
	test010();
	cout << endl << "==== end of tests ====" << endl;

	return 0;
}

