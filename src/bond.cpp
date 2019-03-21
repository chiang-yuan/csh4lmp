#include "bond.h"

Atom * Bond::connected(Atom * atom_)
{
	if (*ij[0] == *atom_) return ij[1];
	else if (*ij[1] == *atom_) return ij[0];
	else return nullptr;
}

Bond::Bond()
{
	delete_flag = false;
	ij = std::vector<Atom*>(2);
}

Bond::~Bond()
{

}

bool operator==(const Bond & lhs, const Bond & rhs)
{
	if (lhs.id != rhs.id) return false;
	if (lhs.ij[0]->id != rhs.ij[0]->id) return false;
	if (lhs.ij[1]->id != rhs.ij[1]->id) return false;
	return true;
}

bool operator!=(const Bond & lhs, const Bond & rhs)
{
	return !(lhs == rhs);
}

Bond & operator&=(Bond & lhs, const Bond & rhs)
{
	lhs.id = rhs.id;
	lhs.type = rhs.type;
	*(lhs.ij[0]) = *(rhs.ij[0]);
	*(lhs.ij[1]) = *(rhs.ij[1]);
	strcpy(lhs.name, rhs.name);
	lhs.delete_flag = rhs.delete_flag;

	return lhs;
}
