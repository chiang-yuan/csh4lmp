#include "angle.h"

Angle::Angle()
{
	delete_flag = false;
	ijk = std::vector<Atom*>(3);
}

Angle::~Angle()
{
}

Angle & operator&=(Angle & lhs, const Angle & rhs)
{
	lhs.id = rhs.id;
	*(lhs.ijk[0]) = *(rhs.ijk[0]);
	*(lhs.ijk[1]) = *(rhs.ijk[1]);
	*(lhs.ijk[2]) = *(rhs.ijk[2]);
	lhs.type = rhs.type;
	lhs.delete_flag = rhs.delete_flag;
	strcpy(lhs.name, rhs.name);	

	return lhs;
}
