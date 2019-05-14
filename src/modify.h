/* -*- C++ -*------------------------------------------------------------
 *
 *	modify.h
 *
 * ---------------------------------------------------------------------- */

#ifndef MODIFY_H
#define MODIFY_H

#include "error.h"
#include "system.h"
#include "stl2lmp.h"

#define BAR_BINS	20

class Modify
{
public:
	Modify(Error *error_);
	~Modify();

	virtual int command(int argc, char * argv[], System &sys);
protected:
	Error *error;
	
	bool check_arg(char **arg, const char *flag, int num, int argc);

	virtual int triclinic(System & sys, double alpha_, double beta_, double gamma_);

	virtual int add_atomtype(System & sys, char ele_[2], double mass_, char pot_[MAX_NAME], double coeff_[2]);
	virtual int add_bondtype(System & sys, int ij_[2], double coeff_[2], char style_[MAX_NAME], char name_[MAX_NAME]);
	virtual int add_angletype(System & sys, int ijk_[3], double coeff_[2], char style_[MAX_NAME], char name_[MAX_NAME]);
	virtual int add_atoms(System & sys, std::vector<Atom> atoms_);
	virtual int add_bonds(System & sys, std::vector<Bond> bonds_);
	virtual int add_angles(System & sys, std::vector<Angle> angles_);
	virtual int delete_atoms(System &sys);
	virtual int delete_bonds(System &sys);
	virtual int delete_angles(System &sys);
	virtual int delete_type(System &sys, int type_);
	virtual int delete_bond_type(System &sys, int type_);
	
	
	virtual int neutralize(System &sys, std::vector<int> types_);
private:

};

class Topology : public Modify
{
public:
	Topology(Error *error_);
	~Topology();

	virtual int command(System &sys);
	virtual int NMR(System &sys, int type_);

	int typeO = 1;
	int typeCa = 2;
	int typeSi = 3; 
	int typeH = 4;
	int typeOh = 5; 
	int typeOw = 6;
	int typeCw = 7; 
	int typeHw = 8;
	int typeOb = 9;
	
	int type_Ow_Hw = 1;
	int type_Oh_H = 2;
	int type_O_Si = 3;
	int type_Oh_Si = 4;
	int type_Hw_Ow_Hw = 1;
private:
};

class Initialize : public Topology
{
public:
	Initialize(Error *error_);
	~Initialize();

	int command(System &sys);
private:
};

class ModifyH : public Topology
{
public:
	ModifyH(Error *error_);
	~ModifyH();

	int command(int argc, char * argv[], System &sys);
private:
	double charge(System &sys);
	int add_to_neutral(System & sys, int typeO_, int typeH_, int type_O_H);
	int add_to_number(System & sys, int num_, int typeO_, int typeH_, int type_O_H);
};

class ModifyH2O : public Topology
{
public:
	ModifyH2O(Error *error_);
	~ModifyH2O();

	int command(int argc, char * argv[], System &sys);
private:
	int delete_type(System &sys, int typeOw_, int typeHw_);
};

class AddH2O : public Topology
{
public:
	AddH2O(Error *error_);
	~AddH2O();

	int command(int argc, char * argv[], System &sys);
private:
	int add_type(System &sys, int num_, int typeOw_, int typeHw_);
};

class ModifySiO2 : public Topology
{
public:
	ModifySiO2(Error *error_);
	~ModifySiO2();

	int command(int argc, char * argv[], System &sys);
private:
	int randomly_remove_SiO2(System &sys, int typeSi_, int num_, double rbp_);
	int randomly_remove_type(System &sys, int type_, int num_);
	int NMR(System & sys, int type_);
};

#endif // !MODIFY_H
