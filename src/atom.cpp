#include "atom.h"

Atom::Atom()
{
	delete_flag = false;
	bondNum = 0;
	bondID = std::vector<bigint>(MAX_BONDS_PER_ATOM);
	bonds = std::vector<Bond*>(MAX_BONDS_PER_ATOM);
	angleNum = 0;
}

Atom::~Atom()
{
}

bool operator==(const Atom& lhs, const Atom& rhs) {
	return (lhs.id == rhs.id);
}

bool operator!=(const Atom & lhs, const Atom & rhs)
{
	return !(lhs == rhs);
}


Atom & operator&=(Atom & lhs, const Atom & rhs)
{
	lhs.id = rhs.id;
	lhs.molecule = rhs.molecule;
	lhs.type = rhs.type;
	lhs.q = rhs.q;
	lhs.x[0] = rhs.x[0]; lhs.x[1] = rhs.x[1]; lhs.x[2] = rhs.x[2];
	lhs.n[0] = rhs.n[0]; lhs.n[1] = rhs.n[1]; lhs.n[2] = rhs.n[2];
	strcpy(lhs.name, rhs.name);
	lhs.delete_flag = rhs.delete_flag;
	lhs.bondNum = rhs.bondNum;
	lhs.bondID = rhs.bondID;
	lhs.bonds = rhs.bonds;
	lhs.angleNum = rhs.angleNum;
	lhs.angles = rhs.angles;

	return lhs;
}
