/* -*- C++ -*------------------------------------------------------------
 *
 *	bond.h
 *
 * ---------------------------------------------------------------------- */

#ifndef BOND_H
#define BOND_H

#include "atom.h"
#include "stl2lmp.h"

class Bond {
public:
	bigint id;			// bond id
	char name[MAX_NAME];	// bond name
	int type;		// bond type
	bool delete_flag;

	std::vector<Atom*> ij;
	
	Atom* connected(Atom* atom_);

	friend bool operator==(const Bond& lhs, const Bond& rhs);
	friend bool operator!=(const Bond& lhs, const Bond& rhs);
	friend Bond& operator&=(Bond& lhs, const Bond& rhs);
	
	Bond();
	~Bond();
protected:
private:


};

#endif // !BOND_H
