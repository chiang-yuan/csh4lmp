/* -*- C++ -*------------------------------------------------------------
 *
 *	atom.h
 *
 * ---------------------------------------------------------------------- */

#ifndef ATOM_H
#define ATOM_H

#define MAX_BONDS_PER_ATOM	16
#define MAX_ANGLES_PER_ATOM	16

#include "stl2lmp.h"

class Bond;
class Angle;

class Atom {
public:
	bigint id;			// atom id
	int molecule;	// molecule tag
	int type;		// atom type
	double q;		// charge
	double x[3];	// position
	int n[3];	// image flag

	char name[MAX_NAME];	// custom name
	bool delete_flag;		//

	
	int bondNum;						// number of bonds
	int angleNum;
	std::vector<bigint> bondID;			// bond id
	std::vector<Bond*> bonds;
	std::vector<Angle*> angles;


	Atom();
	~Atom();

	friend bool operator==(const Atom& lhs, const Atom& rhs);
	friend bool operator!=(const Atom& lhs, const Atom& rhs);
	friend Atom& operator&=(Atom& lhs, const Atom& rhs);
protected:
private:


};

bool operator==(const Atom& lhs, const Atom& rhs);
bool operator!=(const Atom& lhs, const Atom& rhs);

Atom& operator&=(Atom& lhs, const Atom& rhs);

#endif // !ATOM_H
