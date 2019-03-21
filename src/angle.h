/* -*- C++ -*------------------------------------------------------------
 *
 *	angle.h
 *
 * ---------------------------------------------------------------------- */

#ifndef ANGLE_H
#define ANGLE_H

#include "atom.h"
#include "stl2lmp.h"

class Angle {
public:
	bigint id;				// angle id
	char name[MAX_NAME];	// angle name
	int type;				// angle type
	bool delete_flag;

	std::vector<Atom*> ijk;

	friend Angle& operator&=(Angle& lhs, const Angle& rhs);

	Angle();
	~Angle();
protected:
private:


};

#endif // !ANGLE_H
