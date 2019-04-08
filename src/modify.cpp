#include "modify.h"

Modify::Modify(Error * error_)
{
	error = error_;
}

Modify::~Modify()
{
}

bool Modify::check_arg(char ** arg, const char * flag, int num, int argc)
{
	if (num >= argc) {
		printf("Missing argument for \"%s\" flag\n", flag);
		return false;
	}

	return true;
}

int Modify::triclinic(System & sys, double alpha_, double beta_, double gamma_)
{
	
	
	return 0;
}

int Modify::add_atomtype(System & sys, char ele_[2], double mass_, char pot_[MAX_NAME], double coeff_[2])
{
	sys.atomTypes.resize(++sys.no_atom_types);

	strcpy(sys.atomTypes[sys.no_atom_types - 1].element, ele_);
	sys.atomTypes[sys.no_atom_types - 1].mass = mass_;
	strcpy(sys.atomTypes[sys.no_atom_types - 1].potential, pot_);
	sys.atomTypes[sys.no_atom_types - 1].coeff[0] = coeff_[0];
	sys.atomTypes[sys.no_atom_types - 1].coeff[1] = coeff_[1];
	
	return sys.no_atom_types;
}

int Modify::add_bondtype(System & sys, int ij_[2], double coeff_[2], char style_[MAX_NAME], char name_[MAX_NAME])
{
	sys.bondTypes.resize(++sys.no_bond_types);

	sys.bondTypes[sys.no_bond_types - 1].IJType[0] = ij_[0];
	sys.bondTypes[sys.no_bond_types - 1].IJType[1] = ij_[1];
	sys.bondTypes[sys.no_bond_types - 1].coeff[0] = coeff_[0];
	sys.bondTypes[sys.no_bond_types - 1].coeff[1] = coeff_[1];
	strcpy(sys.bondTypes[sys.no_bond_types - 1].style, style_);
	strcpy(sys.bondTypes[sys.no_bond_types - 1].name, name_);
	
	return sys.no_bond_types;
}

int Modify::add_angletype(System & sys, int ijk_[3], double coeff_[2], char style_[MAX_NAME], char name_[MAX_NAME])
{
	sys.angleTypes.resize(++sys.no_angle_types);

	sys.angleTypes[sys.no_angle_types - 1].types[0] = ijk_[0];
	sys.angleTypes[sys.no_angle_types - 1].types[1] = ijk_[1];
	sys.angleTypes[sys.no_angle_types - 1].types[2] = ijk_[2];
	sys.angleTypes[sys.no_angle_types - 1].coeff[0] = coeff_[0];
	sys.angleTypes[sys.no_angle_types - 1].coeff[1] = coeff_[1];
	strcpy(sys.angleTypes[sys.no_angle_types - 1].style, style_);
	strcpy(sys.angleTypes[sys.no_angle_types - 1].name, name_);

	return sys.no_angle_types;
}

int Modify::add_atoms(System & sys, std::vector<Atom> atoms_)
{
	int num = 0;

	for (std::vector<Atom>::iterator a = atoms_.begin(); a != atoms_.end(); ++a) {
		//sys.atoms.push_back(*a);
		//sys.natoms++;
		if (sys.natoms + 1 > sys.atoms.capacity()) {
			error->warning("Total number of atoms exceeds the vector capacity %s", 1, std::to_string(sys.atoms.capacity()).c_str());
			break;
		}
			
		sys.atoms.resize(++sys.natoms);
		sys.atoms[sys.natoms - 1] = *a;
		num++;
	}
	
	return num;
}

int Modify::add_bonds(System & sys, std::vector<Bond> bonds_)
{
	int num = 0;

	for (std::vector<Bond>::iterator b = bonds_.begin(); b != bonds_.end(); ++b) {
		
		if (sys.nbonds + 1 > sys.bonds.capacity()) {
			error->warning("Total number of bonds exceeds the vector capacity %s", 1, std::to_string(sys.bonds.capacity()).c_str());
			break;
		}
			
		sys.bonds.resize(++sys.nbonds);
		sys.bonds[sys.nbonds - 1] = *b;
		num++;
	}

	return num;
}

int Modify::add_angles(System & sys, std::vector<Angle> angles_)
{
	int num = 0;

	for (std::vector<Angle>::iterator a = angles_.begin(); a != angles_.end(); ++a) {

		if (sys.nangles + 1 > sys.angles.capacity()) {
			error->warning("Total number of angles exceeds the vector capacity %s", 1, std::to_string(sys.angles.capacity()).c_str());
			break;
		}

		sys.angles.resize(++sys.nangles);
		sys.angles[sys.nangles - 1] = *a;
		num++;
	}

	return num;
}

int Modify::delete_atoms(System & sys)
{
	int cnt = 0;

	bigint iatom = 0;
	for (std::vector<Atom>::iterator a = sys.atoms.begin(); a != sys.atoms.end();) {
		
		if ((int)sys.natoms / BAR_BINS != 0) {
			if (cnt % ((int)sys.natoms / BAR_BINS) == 0) {
				printf("\tatom [");
				for (int i = 0; i < BAR_BINS; i++) {
					if (i < cnt / ((int)sys.natoms / BAR_BINS)) printf("=");
					else printf(".");
				}
				printf("] %2d/%2d\n", cnt / ((int)sys.natoms / BAR_BINS), BAR_BINS);
			}
		}
		
		
		if (a->delete_flag) {

			// update bonds

			for (std::vector<Bond>::iterator b = sys.bonds.begin(); b != sys.bonds.end(); ++b) {
				for (int j = 0; j < 2; j++) {
					if (b->ij[j]->id > a->id) {
						// offset pointer
						std::vector<Atom>::iterator pos;
						pos = std::find(sys.atoms.begin(), sys.atoms.end(), *(b->ij[j]));
						if (pos != sys.atoms.end()) b->ij[j] = &(*(pos - 1));
					}
					else if (b->ij[j]->id == a->id) b->delete_flag = true;
				}
			}

			// update angles

			for (std::vector<Angle>::iterator c = sys.angles.begin(); c != sys.angles.end(); ++c) {
				for (int i = 0; i < 3; i++) {
					if (c->ijk[i]->id > a->id) {
						std::vector<Atom>::iterator pos;
						pos = std::find(sys.atoms.begin(), sys.atoms.end(), *(c->ijk[i]));
						if (pos != sys.atoms.end()) c->ijk[i] = &(*(pos - 1));
					}
					else if (c->ijk[i]->id == a->id) c->delete_flag = true;
				}
			}
			
			a = sys.atoms.erase(a);
			cnt++;
		}
		else {
			a->id = ++iatom;
			++a;
			cnt++;
		}

	}
	sys.natoms = iatom;
	return 0;
}

int Modify::delete_bonds(System & sys)
{
	int cnt = 0;

	bigint ibond = 0;
	for (std::vector<Bond>::iterator iter = sys.bonds.begin(); iter != sys.bonds.end();) {

		if ((int)sys.nbonds / BAR_BINS != 0) {
			if (cnt % ((int)sys.nbonds / BAR_BINS) == 0) {
				printf("\tbond [");
				for (int i = 0; i < BAR_BINS; i++) {
					if (i < cnt / ((int)sys.nbonds / BAR_BINS)) printf("=");
					else printf(".");
				}
				printf("] %2d/%2d\n", cnt / ((int)sys.nbonds / 20), BAR_BINS);
			}
		}
		
		
		// if one of i,j atoms is deleted, delete this bond
		if (iter->ij[0]->delete_flag == true || iter->ij[1]->delete_flag == true) {
			iter->delete_flag = true;
		}
		
		if (iter->delete_flag) {

			for (int i = 0; i < 2; i++) {
				std::vector<Bond*>::iterator pos = std::find(iter->ij[i]->bonds.begin(), iter->ij[i]->bonds.end(), &(*iter));
				if (pos != iter->ij[i]->bonds.end()) {
					iter->ij[i]->bonds.erase(pos);
					iter->ij[i]->bonds.resize(MAX_BONDS_PER_ATOM);
					iter->ij[i]->bondNum--;
				}
			}

			for (std::vector<Bond>::iterator flw = iter + 1; flw != sys.bonds.end(); ++flw) {
				for (int i = 0; i < 2; i++) {

					std::vector<bigint>::iterator pos1 = std::find(flw->ij[i]->bondID.begin(), flw->ij[i]->bondID.end(), flw->id);
					if (pos1 != flw->ij[i]->bondID.end()) (*pos1)--;

					std::vector<Bond*>::iterator pos2 = std::find(flw->ij[i]->bonds.begin(), flw->ij[i]->bonds.end(), &*flw);
					if (pos2 != flw->ij[i]->bonds.end()) (*pos2) = &*(flw - 1);
				}
			}
			
			iter = sys.bonds.erase(iter);
			cnt++;
		}
		else {
			++ibond;
			for (int i = 0; i < 2; i++) {
				// update per bond id vector
				std::vector<bigint>::iterator pos = std::find(iter->ij[i]->bondID.begin(), iter->ij[i]->bondID.end(), iter->id);
				if (pos != iter->ij[i]->bondID.end()) *pos = ibond;
			}

			// update this bond

			iter->id = ibond;
			++iter;
			cnt++;
		}

		/*
		if(true){
			printf("2");
			for (int i = 0; i < 2; i++) {

				/*
				std::vector<bigint>::iterator pos;
				pos = std::find(iter->ij[i]->bondID.begin(), iter->ij[i]->bondID.end(), iter->id);
				if (pos != iter->ij[i]->bondID.end()) iter->ij[i]->bondID.erase(pos);
				// delete bond vector in i,j atoms
				std::vector<Bond*>::iterator pos2;
				pos2 = std::find(iter->ij[i]->bonds.begin(), iter->ij[i]->bonds.end(), &(*iter));
				if (pos2 != iter->ij[i]->bonds.end()) {
					iter->ij[i]->bonds.erase(pos2);
					iter->ij[i]->bonds.resize(MAX_BONDS_PER_ATOM);
					// downsize bond number in i,j atoms
					iter->ij[i]->bondNum--;
				}
				

				// update per bond id vector
				printf("3");
				

				printf("4");
				
				// delete or offset bonds pointer in bonded atoms
				
				for (std::vector<Bond*>::iterator j = iter->ij[i]->bonds.begin(); j != iter->ij[i]->bonds.end();) {
					if (*j == NULL) {
						++j;
					}
					else if (*j == &*iter) {
						j = iter->ij[i]->bonds.erase(j);
						//iter->ij[i]->bonds.resize(MAX_BONDS_PER_ATOM);
						iter->ij[i]->bondNum--;
					}
					else {
						//*j = &*(iter - 1);
						++j;
					}
				}
			}

			// delete this bond

			iter = sys.bonds.erase(iter);
		}
		else {
			bigint newibond = ++ibond;
			for (int i = 0; i < 2; i++) {

				// update per bond id vector

				std::vector<bigint>::iterator pos;
				pos = std::find(iter->ij[i]->bondID.begin(), iter->ij[i]->bondID.end(), iter->id);
				if (pos != iter->ij[i]->bondID.end()) *pos = newibond;
			}

			// update this bond

			iter->id = newibond;
			++iter;
		}
		*/
	}
	sys.nbonds = ibond;
	
	return 0;
}

int Modify::delete_angles(System & sys)
{
	bigint iangle = 0;
	for (std::vector<Angle>::iterator iter = sys.angles.begin(); iter != sys.angles.end();) {

		// if one of i,j,k atoms is deleted, delete this angle
		if (iter->ijk[0]->delete_flag == true ||
			iter->ijk[1]->delete_flag == true ||
			iter->ijk[2]->delete_flag == true) {
			iter->delete_flag = true;
		}

		if (iter->delete_flag) {
			for (int i = 0; i < 3; i++) {
				// delete angle vector in i,j atoms
				std::vector<Angle*>::iterator pos;
				pos = std::find(iter->ijk[i]->angles.begin(), iter->ijk[i]->angles.end(), &(*iter));
				if (pos != iter->ijk[i]->angles.end()) {
					iter->ijk[i]->angles.erase(pos);
					// downsize bond number in i,j atoms
					iter->ijk[i]->angleNum--;
				}
			}
			// delete this angle
			iter = sys.angles.erase(iter);
		}
		else {
			bigint newiangle = ++iangle;

			// update this angle
			iter->id = newiangle;
			++iter;
		}
	}
	sys.nangles = iangle;
	return 0;
}

int Modify::delete_type(System & sys, int type_)
{
	int ndelete = 0;
	for (int i = 0; i < sys.natoms; i++) {
		if (sys.atoms[i].type == type_) {
			sys.atoms[i].delete_flag = true;
			ndelete++;
		}
	}

	delete_atoms(sys);
	delete_bonds(sys);

	return ndelete;
}

int Modify::delete_bond_type(System & sys, int type_)
{
	int ndelete = 0;
	for (int i = 0; i < sys.nbonds; i++) {
		if (sys.bonds[i].type == type_) {
			sys.bonds[i].delete_flag = true;
			ndelete++;
		}
	}

	delete_atoms(sys);
	delete_bonds(sys);

	return ndelete;
}

int Modify::neutralize(System & sys, std::vector<int> types_)
{
	
	double qt = 0;
	int no_dis = 0;				
	std::vector<Atom*> dis_atom;
	for (std::vector<Atom>::iterator a = sys.atoms.begin(); a != sys.atoms.end(); ++a) {
		qt += a->q;
		if (std::find(types_.begin(), types_.end(), a->type) != types_.end()){
			dis_atom.push_back(&*a);
			no_dis++;
		}
	}
	printf("Total charge: %f\n", qt);
	printf("Charge distributed on per atom : %f\n", -qt / no_dis);

	for (std::vector<Atom*>::iterator a = dis_atom.begin(); a != dis_atom.end(); ++a)
		(*a)->q -= qt / no_dis;
	
	qt = 0;
	for (std::vector<Atom>::iterator a = sys.atoms.begin(); a != sys.atoms.end(); ++a)
		qt += a->q;
	printf("Total charge: %f\n", qt);

	return 0;
}

int Modify::command(int argc, char * argv[], System & sys)
{
	int n = 0;
	while (n < argc)
	{
		if (strncmp(argv[n], "delete", 6) == 0) {
			int ncomm = 3;
			char** commd = new char*[ncomm];

			for (int i = 0; i < ncomm; i++) {
				n++;
				if (!check_arg(argv, "delete", n, argc)) return error->message("", 1);
				commd[i] = argv[n];
			}

			if (strncmp(commd[0], "atom", 4) == 0) {
				if (strncmp(commd[1], "id", 2) == 0) {

				}
				else if (strncmp(commd[1], "type", 4) == 0) {
					printf("Modify::delete_type(): %d\n", delete_type(sys, atoi(commd[2])));
				}
				else return error->message("Invalid delete argument: %s", 2, commd[1]);
			}
			else if (strncmp(commd[0], "bond", 4) == 0) {
				if (strncmp(commd[1], "id", 2) == 0) {

				}
				else if (strncmp(commd[1], "type", 4) == 0) {
					printf("Modify::delete_bond_type(): %d\n", delete_bond_type(sys, atoi(commd[2])));
				}
				else return error->message("Invalid delete argument: %s", 2, commd[1]);
			}
			else return error->message("Invalid delete argument: %s", 2, commd[0]);
		}

		n++;
	}
	return 0;
}

ModifyH2O::ModifyH2O(Error * error_) : Topology(error_)
{
	error = error_;
}

ModifyH2O::~ModifyH2O()
{
}

int ModifyH2O::command(int argc, char * argv[], System & sys)
{
	int n = 0;
	while (n < argc)
	{
		if (strncmp(argv[n], "delete", 6) == 0) {
			int ncomm = 2;
			char** commd = new char*[ncomm];

			for (int i = 0; i < ncomm; i++) {
				n++;
				if (!check_arg(argv, "delete", n, argc)) return error->message("", 1);
				commd[i] = argv[n];
			}

			if (strncmp(commd[0], "id", 2) == 0) {

			}
			else if (strncmp(commd[0], "type", 4) == 0) {
				printf("ModifyH2O::delete_type(): %d\n", delete_type(sys, atoi(commd[0]), atoi(commd[1])));
			}
			else return error->message("Invalid delete argument: %s", 2, commd[0]);
		}

		n++;
	}
	return 0;
}

int ModifyH2O::delete_type(System & sys, int typeOw_, int typeHw_)
{
	int ndelete = 0;

	for (std::vector<Atom>::iterator a = sys.atoms.begin(); a != sys.atoms.end(); ++a) {
		if ((*a).type == typeOw_ && (*a).bondNum == 2) {
			
			bool h2o = true;

			for (int i = 0; i < (*a).bondNum; i++) {
				if ((*a).bonds[i]->connected(&*a)->type != typeHw_) {
					h2o = false;
					break;
				}				
			}

			if (h2o) {
				a->delete_flag = true;
				a->bonds[0]->connected(&*a)->delete_flag = true;
				a->bonds[1]->connected(&*a)->delete_flag = true;
				ndelete++;
			}
		}
	}
	
	delete_atoms(sys);
	delete_bonds(sys);
	delete_angles(sys);

	return ndelete;
}

ModifySiO2::ModifySiO2(Error * error_) : Topology(error_)
{
	error = error_;
}

ModifySiO2::~ModifySiO2()
{
}

int ModifySiO2::command(int argc, char * argv[], System & sys)
{
	int n = 0;
	while (n < argc)
	{
		if (strncmp(argv[n], "number", 6) == 0) {
			int ncomm = 3;
			char** commd = new char*[ncomm];

			for (int i = 0; i < ncomm; i++) {
				n++;
				if (!check_arg(argv, "number", n, argc)) return error->message("", 1);
				commd[i] = argv[n];
			}

			printf("ModifySiO2::randomly_remove_SiO2(): %d\n",
				randomly_remove_SiO2(sys, atoi(commd[0]), atoi(commd[1]), atof(commd[2])));
		}
		else if (strncmp(argv[n], "ratio", 5) == 0) {
			
		}
		else return error->message("Invalid randomly_remove_SiO2 argument: %s", 2, argv[n]);
		n++;
	}
	
	return 0;
}

int ModifySiO2::randomly_remove_SiO2(System & sys, int typeSi_, int num_, double rbp_)
{
	srand(time(NULL));

	std::vector<Atom*> group_BT_Si, group_PT_Si;
	for (std::vector<Atom>::iterator a = sys.atoms.begin(); a != sys.atoms.end(); ++a) {
		if ((*a).type == typeSi_ && strncmp((*a).name, "BT", 2) == 0)
			group_BT_Si.push_back(&*a);
		else if ((*a).type == typeSi_ && strncmp((*a).name, "PT", 2) == 0)
			group_PT_Si.push_back(&*a);
	}

	double cs = double(group_BT_Si.size() + group_PT_Si.size()) /
		double((group_BT_Si.size() + group_PT_Si.size() - num_));

	if (cs < 0 || num_ < 0) error->message("Enter incorrect removal number: %s", 1, std::to_string(num_).c_str());
	double bp = rbp_;

	FILE* nmr = fopen("NMR.txt", "a");
	printf("\n\tCa/Si = %4.2f | BT/PT = %4.2f | BT: %4.2f | PT: %4.2f\n\n", cs, bp, bp / (1.0 + bp), 1.0 / (1.0 + bp));
	fprintf(nmr,"%4.2f\t%4.2f", cs, bp);

	int num_del = 0;
	while (num_del < num_*bp / (1.0 + bp) && group_BT_Si.size() > 0) {
		int idel = rand() % group_BT_Si.size();
		Atom* delSi = &*group_BT_Si[idel];

		std::vector<Atom*> bridging;
		std::vector<Atom*> hydroxyl;
		for (int i = 0; i < delSi->bondNum; i++) {
			if (*delSi->bonds[i]->ij[0] != *delSi) {
				if (delSi->bonds[i]->ij[0]->bondNum == 1 && delSi->bonds[i]->ij[0]->delete_flag == false)
					hydroxyl.push_back(&*(delSi->bonds[i]->ij[0]));
				else if (delSi->bonds[i]->ij[0]->bondNum == 2 && delSi->bonds[i]->ij[0]->delete_flag == false)
					bridging.push_back(&*(delSi->bonds[i]->ij[0]));
			}
			else if (*delSi->bonds[i]->ij[1] != *delSi) {
				if (delSi->bonds[i]->ij[1]->bondNum == 1 && delSi->bonds[i]->ij[1]->delete_flag == false)
					hydroxyl.push_back(&*(delSi->bonds[i]->ij[1]));
				else if ((*delSi).bonds[i]->ij[1]->bondNum == 2 && delSi->bonds[i]->ij[1]->delete_flag == false)
					bridging.push_back(&*(delSi->bonds[i]->ij[1]));
			}
		}

		// Mode 1 : delete one bridging oxygen and one hydroxyl oxygen
		
		if (bridging.size() == 0 || hydroxyl.size() == 0) {
			group_BT_Si.erase(group_BT_Si.begin() + idel);
			continue;
		}

		bridging[rand() % bridging.size()]->delete_flag = true;
		hydroxyl[rand() % hydroxyl.size()]->delete_flag = true;
		
		// Mode 2 : delete two hydroxyl oxygen
		/*
		if (hydroxyl.size() != 2) {
			group_BT_Si.erase(group_BT_Si.begin() + idel);
			continue;
		}
		
		
		hydroxyl[0]->delete_flag = true;
		hydroxyl[1]->delete_flag = true;
		*/

		//

		bridging.clear();
		hydroxyl.clear();

		(*delSi).delete_flag = true;
		group_BT_Si.erase(group_BT_Si.begin() + idel);
		num_del++;
	}

	int num_del_BT = num_del;
	printf("\t\tRandomly remove %4d BT\n", num_del_BT);
	
	while (num_del < num_ && group_PT_Si.size() > 0) {
		int idel = rand() % group_PT_Si.size();
		Atom* delSi = &*group_PT_Si[idel];

		std::vector<Atom*> bridging;
		std::vector<Atom*> hydroxyl;
		for (int i = 0; i < delSi->bondNum; i++) {
			if (*delSi->bonds[i]->ij[0] != *delSi) {
				if (delSi->bonds[i]->ij[0]->bondNum == 1 && delSi->bonds[i]->ij[0]->delete_flag == false)
					hydroxyl.push_back(&*(delSi->bonds[i]->ij[0]));
				else if (delSi->bonds[i]->ij[0]->bondNum == 2 && delSi->bonds[i]->ij[0]->delete_flag == false)
					bridging.push_back(&*(delSi->bonds[i]->ij[0]));
			}
			else if (*delSi->bonds[i]->ij[1] != *delSi) {
				if (delSi->bonds[i]->ij[1]->bondNum == 1 && delSi->bonds[i]->ij[1]->delete_flag == false)
					hydroxyl.push_back(&*(delSi->bonds[i]->ij[1]));
				else if ((*delSi).bonds[i]->ij[1]->bondNum == 2 && delSi->bonds[i]->ij[1]->delete_flag == false)
					bridging.push_back(&*(delSi->bonds[i]->ij[1]));
			}
		}

		// Mode 1 : delete one bridging oxygen and one hydroxyl oxygen

		
		if (bridging.size() == 0 || hydroxyl.size() == 0) {
			group_PT_Si.erase(group_PT_Si.begin() + idel);
			continue;
		}

		bridging[rand() % bridging.size()]->delete_flag = true;
		hydroxyl[rand() % hydroxyl.size()]->delete_flag = true;
		

		// Mode 2 : delete two hydroxyl oxygen
		/*
		if (hydroxyl.size() != 2) {
			group_PT_Si.erase(group_PT_Si.begin() + idel);
			continue;
		}
		
		hydroxyl[0]->delete_flag = true;
		hydroxyl[1]->delete_flag = true;
		*/

		//

		bridging.clear();
		hydroxyl.clear();

		(*delSi).delete_flag = true;
		group_PT_Si.erase(group_PT_Si.begin() + idel);
		num_del++;
	}

	printf("\t\tRandomly remove %4d PT\n", num_ - num_del_BT);

	if (num_del < num_) error->warning("The cutting number of SiO2 exceeds the existent number", 2);

	delete_atoms(sys);
	delete_bonds(sys);
	
	// NMR Analysis
	NMR(sys, typeSi_);

	return 0;
}

int ModifySiO2::randomly_remove_type(System & sys, int type_, int num_)
{
	srand(time(NULL));

	int nSi = 0;
	std::vector<Atom*> group_Si;
	for (int i = 0; i < sys.natoms; i++) {
		if (sys.atoms[i].type == type_) {
			group_Si.push_back(&sys.atoms[i]);
			nSi++;
		}
	}
	
	int ndelete = 0;
	while (ndelete < num_ && nSi > 0)
	{
		int idelete = rand() % group_Si.size() + 1;
		if (group_Si[idelete - 1]->type == type_) {
			Atom* delSi = &sys.atoms[group_Si[idelete - 1]->id - 1];
			if (delSi->delete_flag) continue;
			else {
				delSi->delete_flag = true;
				nSi--;
				ndelete++;
			}
			for (int i = 0; i < delSi->bondNum; i++) {
				Atom* delO;
				if (delSi == sys.bonds[delSi->bondID[i] - 1].ij[0]) delO = sys.bonds[delSi->bondID[i] - 1].ij[1];
				else delO = sys.bonds[delSi->bondID[i] - 1].ij[0];
	
				if (delO->bondNum == 1) {
					delO->delete_flag = true;
				}
			}

		}
	}
	
	delete_atoms(sys);
	delete_bonds(sys);

	return 0;
}

int ModifySiO2::NMR(System & sys, int type_)
{
	// NMR analysis
	double Q[3]{ 0,0,0 };
	for (std::vector<Atom>::iterator a = sys.atoms.begin(); a != sys.atoms.end(); ++a) {
		if (a->type == type_) {
			int cntbrid = 0;
			for (int i = 0; i < a->bondNum; i++) {
				Atom* bondAtom = &*a;

				if (*(a->bonds[i]->ij[0]) != *a)
					bondAtom = &*(a->bonds[i]->ij[0]);
				else if (*(a->bonds[i]->ij[1]) != *a)
					bondAtom = &*(a->bonds[i]->ij[1]);

				if (bondAtom != NULL && bondAtom->bondNum == 2)cntbrid++;
			}
			Q[cntbrid] = Q[cntbrid] + 1;
		}
	}

	printf("-------------Theoretical NMR Spectrum-------------\n\tQ0: %5.2f | Q1: %5.2f | Q2: %5.2f\n--------------------------------------------------\n"
		, Q[0] / (Q[0] + Q[1] + Q[2])
		, Q[1] / (Q[0] + Q[1] + Q[2])
		, Q[2] / (Q[0] + Q[1] + Q[2]));

	FILE* nmr = fopen("NMR.txt", "a");
	fprintf(nmr, "\t%6.3f\t%6.3f\t%6.3f\t%4.2f\n",
		Q[0] / (Q[0] + Q[1] + Q[2]),
		Q[1] / (Q[0] + Q[1] + Q[2]),
		Q[2] / (Q[0] + Q[1] + Q[2]),
		2.0 / (Q[1] / (Q[1] + Q[2])));

	return 0;
}

Topology::Topology(Error * error_) : Modify(error_)
{
	error = error_;
}

Topology::~Topology()
{
}

int Topology::command(System & sys)
{
	
	strcpy(sys.atomTypes[typeO - 1].element, "O"); sys.atomTypes[typeO - 1].mass = 15.999;
	strcpy(sys.atomTypes[typeCa - 1].element, "Ca"); sys.atomTypes[typeCa - 1].mass = 40.078;
	strcpy(sys.atomTypes[typeSi - 1].element, "Si"); sys.atomTypes[typeSi - 1].mass = 28.085;
	strcpy(sys.atomTypes[typeH - 1].element, "H"); sys.atomTypes[typeH - 1].mass = 1.00794;
	strcpy(sys.atomTypes[typeOh - 1].element, "Oh"); sys.atomTypes[typeOh - 1].mass = 15.999;
	strcpy(sys.atomTypes[typeOw - 1].element, "Ow"); sys.atomTypes[typeOw - 1].mass = 15.999;
	strcpy(sys.atomTypes[typeCw - 1].element, "Cw"); sys.atomTypes[typeCw - 1].mass = 40.078;
	strcpy(sys.atomTypes[typeHw - 1].element, "Hw"); sys.atomTypes[typeHw - 1].mass = 1.00794;

	for (std::vector<Atom>::iterator a = sys.atoms.begin(); a != sys.atoms.end(); ++a) {
		
		/***** type 1 : O , type 5 : Oh and type 6 : Ow *****/
		if (a->type == typeO || a->type == typeOh || a->type == typeOw) {
			if (a->type == typeO) a->q = -1.14;
			if (a->type == typeOh) a->q = -1.00;
		}

		/***** type 2 : Ca and type 7 : Cw *****/
		if (a->type == typeCa || a->type == typeCw) {
			
		}

		/***** type 3 : Si *****/
		if (a->type == typeSi) {
			double r = 2.0;
			std::vector<Atom*> neigh;
			for (std::vector<Atom>::iterator j = sys.atoms.begin(); j != sys.atoms.end(); ++j) {
				if (pow((*j).x[2] - (*a).x[2], 2) < pow(r, 2)) {
					neigh.push_back(&*j);
				}
			}

			bool BT = false;
			for (std::vector<Atom*>::iterator n = neigh.begin(); n != neigh.end(); ++n) {
				if ((*n)->type == typeCw) {
					BT = true;
					break;
				}
			}
			neigh.clear();
			if (BT) strcpy((*a).name, "BT");
			else strcpy((*a).name, "PT");
			
		}

		/***** type 4 : H and type 8 : Hw *****/
		if (a->type == typeH || a->type == typeHw) {
			
		}

	}

	
	if (type_Ow_Hw > sys.no_bond_types)
		type_Ow_Hw = add_bondtype(sys, new int[2]{ 0,0 }, new double[2]{ 0,0 }, (char*)"harmonic", (char*)"Ow-Hw");
	sys.bondTypes[type_Ow_Hw - 1].IJType[0] = typeOw; sys.bondTypes[type_Ow_Hw - 1].IJType[1] = typeHw;
	strcpy(sys.bondTypes[type_Ow_Hw - 1].name, "Ow-Hw"); strcpy(sys.bondTypes[type_Ow_Hw - 1].style, "harmonic");

	
	if (type_Oh_H > sys.no_bond_types)
		type_Oh_H = add_bondtype(sys, new int[2]{ 0,0 }, new double[2]{ 0,0 }, (char*)"harmonic", (char*)"Oh-H");
	sys.bondTypes[type_Oh_H - 1].IJType[0] = typeOh; sys.bondTypes[type_Oh_H - 1].IJType[1] = typeH;
	strcpy(sys.bondTypes[type_Oh_H - 1].name, "Oh-H"); strcpy(sys.bondTypes[type_Oh_H - 1].style, "harmonic");

	
	if (type_O_Si > sys.no_bond_types)
		type_O_Si = add_bondtype(sys, new int[2]{ 0,0 }, new double[2]{ 0,0 }, (char*)"harmonic", (char*)"O-Si");
	sys.bondTypes[type_O_Si - 1].IJType[0] = typeO; sys.bondTypes[type_O_Si - 1].IJType[1] = typeSi;
	strcpy(sys.bondTypes[type_O_Si - 1].name, "O-Si"); strcpy(sys.bondTypes[type_O_Si - 1].style, "harmonic");

	
	if (type_Oh_Si > sys.no_bond_types)
		type_Oh_Si = add_bondtype(sys, new int[2]{ 0,0 }, new double[2]{ 0,0 }, (char*)"harmonic", (char*)"Oh-Si");
	sys.bondTypes[type_Oh_Si - 1].IJType[0] = typeOh; sys.bondTypes[type_Oh_Si - 1].IJType[1] = typeSi;
	strcpy(sys.bondTypes[type_Oh_Si - 1].name, "Oh-Si"); strcpy(sys.bondTypes[type_Oh_Si - 1].style, "harmonic");

	for (std::vector<Bond>::iterator b = sys.bonds.begin(); b != sys.bonds.end(); ++b) {
		
		// bond type 1 : Ow-Hw
		if (b->ij[0]->type == typeOw && b->ij[1]->type == typeHw ||
			b->ij[0]->type == typeHw && b->ij[1]->type == typeOw) 
			b->type = type_Ow_Hw;

		// bond type 2 : Oh-H
		if (b->ij[0]->type == typeOh && b->ij[1]->type == typeH || 
			b->ij[0]->type == typeH && b->ij[1]->type == typeOh) 
			b->type = type_Oh_H;

		// bond type 3 : O-Si
		if (b->ij[0]->type == typeSi && b->ij[1]->type == typeO ||
			b->ij[0]->type == typeO && b->ij[1]->type == typeSi) {
			b->type = type_O_Si;
		}

		// bond type 4 : Oh-Si
		if (b->ij[0]->type == typeSi && b->ij[1]->type == typeOh ||
			b->ij[0]->type == typeOh && b->ij[1]->type == typeSi) {
			b->type = type_Oh_Si;
		}
	}
	
	
	sys.angleTypes[type_Hw_Ow_Hw - 1].types[0] = typeHw;
	sys.angleTypes[type_Hw_Ow_Hw - 1].types[1] = typeOw;
	sys.angleTypes[type_Hw_Ow_Hw - 1].types[2] = typeHw;
	strcpy(sys.angleTypes[type_Hw_Ow_Hw - 1].name, "Hw-Ow-Hw");
	strcpy(sys.angleTypes[type_Hw_Ow_Hw - 1].style, "harmonic");

	for (std::vector<Angle>::iterator a = sys.angles.begin(); a != sys.angles.end(); ++a) {
		// angle type 1 : Hw-Ow-Hw
		if (a->ijk[0]->type == typeHw && a->ijk[1]->type == typeOw && a->ijk[2]->type == typeHw) {
			a->type = type_Hw_Ow_Hw;
		}
	}

	// Balance excess charge
	//std::vector<int> dis_atom{ typeO, typeOh };
	//printf("Modify::neutralize(): %d\n", neutralize(sys, dis_atom));

	std::cout << sys;

	return 0;
}

int Topology::NMR(System & sys, int type_)
{
	// NMR analysis
	double Q[3]{ 0,0,0 };
	for (std::vector<Atom>::iterator a = sys.atoms.begin(); a != sys.atoms.end(); ++a) {
		if (a->type == type_) {
			int cntbrid = 0;
			for (int i = 0; i < a->bondNum; i++) {
				Atom* bondAtom = &*a;

				if (*(a->bonds[i]->ij[0]) != *a)
					bondAtom = &*(a->bonds[i]->ij[0]);
				else if (*(a->bonds[i]->ij[1]) != *a)
					bondAtom = &*(a->bonds[i]->ij[1]);

				if (bondAtom != NULL && bondAtom->bondNum == 2)cntbrid++;
			}
			Q[cntbrid] = Q[cntbrid] + 1;
		}
	}

	printf("-------------Theoretical NMR Spectrum-------------\n\tQ0: %5.2f | Q1: %5.2f | Q2: %5.2f\n--------------------------------------------------\n"
		, Q[0] / (Q[0] + Q[1] + Q[2])
		, Q[1] / (Q[0] + Q[1] + Q[2])
		, Q[2] / (Q[0] + Q[1] + Q[2]));

	FILE* nmr = fopen("NMR.txt", "a");
	fprintf(nmr, "\t%6.3f\t%6.3f\t%6.3f\t%4.2f\n",
		Q[0] / (Q[0] + Q[1] + Q[2]),
		Q[1] / (Q[0] + Q[1] + Q[2]),
		Q[2] / (Q[0] + Q[1] + Q[2]),
		2.0 / (Q[1] / (Q[1] + Q[2])));

	return 0;
}

Initialize::Initialize(Error * error_) : Topology(error_)
{
	error = error_;
}

Initialize::~Initialize()
{
}

int Initialize::command(System & sys)
{

	strcpy(sys.atomTypes[typeO - 1].element, "O"); sys.atomTypes[typeO - 1].mass = 15.999;
	strcpy(sys.atomTypes[typeCa - 1].element, "Ca"); sys.atomTypes[typeCa - 1].mass = 40.078;
	strcpy(sys.atomTypes[typeSi - 1].element, "Si"); sys.atomTypes[typeSi - 1].mass = 28.085;
	
	if (typeH > sys.no_atom_types)
		typeH = add_atomtype(sys, (char*)"H", 1.00794, (char*)"lj/cut/coul/long", new double[2]{ 0, 0 });
	else {
		strcpy(sys.atomTypes[typeH - 1].element, "H");
		sys.atomTypes[typeH - 1].mass = 1.00794;
	}

	if (typeOh > sys.no_atom_types)
		typeOh = add_atomtype(sys, (char*)"Oh", 15.999, (char*)"lj/cut/coul/long", new double[2]{ 0, 0 });
	else {
		strcpy(sys.atomTypes[typeOh - 1].element, "Oh");
		sys.atomTypes[typeOh - 1].mass = 15.999;
	}

	if (typeOw > sys.no_atom_types)
		typeOw = add_atomtype(sys, (char*)"Ow", 15.999, (char*)"lj/cut/coul/long", new double[2]{ 0, 0 });
	else {
		strcpy(sys.atomTypes[typeOw - 1].element, "Ow");
		sys.atomTypes[typeOw - 1].mass = 15.999;
	}

	if (typeCw > sys.no_atom_types)
		typeCw = add_atomtype(sys, (char*)"Cw", 40.078, (char*)"lj/cut/coul/long", new double[2]{ 0, 0 });
	else {
		strcpy(sys.atomTypes[typeCw - 1].element, "Cw");
		sys.atomTypes[typeCw - 1].mass = 15.999;
	}
	
	if (typeHw > sys.no_atom_types)
		typeHw = add_atomtype(sys, (char*)"Hw", 1.00794, (char*)"lj/cut/coul/long", new double[2]{ 0, 0 });
	else {
		strcpy(sys.atomTypes[typeHw - 1].element, "Hw");
		sys.atomTypes[typeHw - 1].mass = 1.00794;
	}
	
	for (std::vector<Atom>::iterator a = sys.atoms.begin(); a != sys.atoms.end(); ++a) {

		/***** type 1 : O , type 5 : Oh and type 6 : Ow *****/
		if (a->type == typeO || a->type == typeOh || a->type == typeOw) {
			if (a->bondNum == 1) {
				a->type = typeOh;
				a->q = -1.00;
			}
			else
			{
				for (int i = 0; i < a->bondNum; i++) {
					if (a->bonds[i]->ij[0]->type == typeH || a->bonds[i]->ij[1]->type == typeH) {
						a->type = typeOw;
						a->q = -0.82;
						break;
					}
					else if (a->bonds[i]->ij[0]->type == typeSi || a->bonds[i]->ij[1]->type == typeSi) {
						a->type = typeO;
						a->q = -1.14;
					}
				}
			}

		}

		/***** type 2 : Ca and type 7 : Cw *****/
		if (a->type == typeCa || a->type == typeCw) {
			double r1 = 4;
			double rs = 2;
			std::vector<Atom*> neighbor;
			for (std::vector<Atom>::iterator j = sys.atoms.begin(); j != sys.atoms.end(); ++j) {
				if (pow(j->x[2] - a->x[2], 2) < pow(r1, 2) &&
					pow(j->x[2] - a->x[2], 2) > 1) {
					neighbor.push_back(&*j);
				}
			}
			bool intra = false;
			for (std::vector<Atom*>::iterator n = neighbor.begin(); n != neighbor.end(); ++n) {
				if ((*n)->type == typeCa || (*n)->type == typeCw) {
					intra = true;
					break;
				}
			}
			neighbor.clear();

			if (intra) {
				a->type = typeCa;
				a->q = 1.43;
			}
			else {
				a->type = typeCw;
				a->q = 1.70;
			}
			/*
			if (!intra || abs(a->x[2] - sys.box[2][0]) < rs || abs(a->x[2] - sys.box[2][1]) < rs)
			{
				a->type = typeCw;
				a->q = 1.70;
			}
			else
				a->q = 1.43;
			*/
		}

		/***** type 3 : Si *****/
		if (a->type == typeSi) {
			a->q = 1.72;
		}

		/***** type 4 : H and type 8 : Hw *****/
		if (a->type == typeH || a->type == typeHw) {
			for (int i = 0; i < a->bondNum; i++) {
				if (a->bonds[i]->ij[0]->type == typeOw || a->bonds[i]->ij[1]->type == typeOw) {
					a->q = 0.41;
					a->type = typeHw;
				}
				else {
					a->q = 0.29;
					a->type = typeH;
				}

			}
		}

	}

	int type_Ow_Hw = 1;
	if (type_Ow_Hw > sys.no_bond_types)
		type_Ow_Hw = add_bondtype(sys, new int[2]{ 0,0 }, new double[2]{ 0,0 }, (char*)"harmonic", (char*)"Ow-Hw");
	sys.bondTypes[type_Ow_Hw - 1].IJType[0] = typeOw; sys.bondTypes[type_Ow_Hw - 1].IJType[1] = typeHw;
	strcpy(sys.bondTypes[type_Ow_Hw - 1].name, "Ow-Hw"); strcpy(sys.bondTypes[type_Ow_Hw - 1].style, "harmonic");

	int type_Oh_H = 2;
	if (type_Oh_H > sys.no_bond_types)
		type_Oh_H = add_bondtype(sys, new int[2]{ 0,0 }, new double[2]{ 0,0 }, (char*)"harmonic", (char*)"Oh-H");
	sys.bondTypes[type_Oh_H - 1].IJType[0] = typeOh; sys.bondTypes[type_Oh_H - 1].IJType[1] = typeH;
	strcpy(sys.bondTypes[type_Oh_H - 1].name, "Oh-H"); strcpy(sys.bondTypes[type_Oh_H - 1].style, "harmonic");

	int type_O_Si = 3;
	if (type_O_Si > sys.no_bond_types)
		type_O_Si = add_bondtype(sys, new int[2]{ 0,0 }, new double[2]{ 0,0 }, (char*)"harmonic", (char*)"O-Si");
	sys.bondTypes[type_O_Si - 1].IJType[0] = typeO; sys.bondTypes[type_O_Si - 1].IJType[1] = typeSi;
	strcpy(sys.bondTypes[type_O_Si - 1].name, "O-Si"); strcpy(sys.bondTypes[type_O_Si - 1].style, "harmonic");

	int type_Oh_Si = 4;
	if (type_Oh_Si > sys.no_bond_types)
		type_Oh_Si = add_bondtype(sys, new int[2]{ 0,0 }, new double[2]{ 0,0 }, (char*)"harmonic", (char*)"Oh-Si");
	sys.bondTypes[type_Oh_Si - 1].IJType[0] = typeOh; sys.bondTypes[type_Oh_Si - 1].IJType[1] = typeSi;
	strcpy(sys.bondTypes[type_Oh_Si - 1].name, "Oh-Si"); strcpy(sys.bondTypes[type_Oh_Si - 1].style, "harmonic");

	for (std::vector<Bond>::iterator b = sys.bonds.begin(); b != sys.bonds.end(); ++b) {

		// bond type 1 : Ow-Hw
		if (b->ij[0]->type == typeOw && b->ij[1]->type == typeHw ||
			b->ij[0]->type == typeHw && b->ij[1]->type == typeOw) 
			b->type = type_Ow_Hw;

		// bond type 2 : Oh-H
		if (b->ij[0]->type == typeOh && b->ij[1]->type == typeH || 
			b->ij[0]->type == typeH && b->ij[1]->type == typeOh) 
			b->type = type_Oh_H;

		// bond type 3 : O-Si
		if (b->ij[0]->type == typeSi && b->ij[1]->type == typeO ||
			b->ij[0]->type == typeO && b->ij[1]->type == typeSi) {
			b->type = type_O_Si;
		}

		// bond type 4 : Oh-Si
		if (b->ij[0]->type == typeSi && b->ij[1]->type == typeOh ||
			b->ij[0]->type == typeOh && b->ij[1]->type == typeSi) {
			b->type = type_Oh_Si;
		}
	}

	int type_Hw_Ow_Hw = 1;
	if (type_Hw_Ow_Hw > sys.no_angle_types)
		type_Hw_Ow_Hw = add_angletype(sys, new int[3]{ typeHw,typeOw,typeHw }, new double[2]{ 0,0 }, (char*)"harmonic", (char*)"Hw-Ow-Hw");
	else {
		sys.angleTypes[type_Hw_Ow_Hw - 1].types[0] = typeHw;
		sys.angleTypes[type_Hw_Ow_Hw - 1].types[1] = typeOw;
		sys.angleTypes[type_Hw_Ow_Hw - 1].types[2] = typeHw;
		strcpy(sys.angleTypes[type_Hw_Ow_Hw - 1].name, "Hw-Ow-Hw");
		strcpy(sys.angleTypes[type_Hw_Ow_Hw - 1].style, "harmonic");
	}
	

	/*
	int type_O_Si_O = 2;
	sys.angleTypes[type_O_Si_O - 1].types[0] = typeO;
	sys.angleTypes[type_O_Si_O - 1].types[1] = typeSi;
	sys.angleTypes[type_O_Si_O - 1].types[2] = typeO;
	strcpy(sys.angleTypes[type_O_Si_O - 1].name, "O-Si-O");
	strcpy(sys.angleTypes[type_O_Si_O - 1].style, "harmonic");

	int type_Si_O_Si = 3;
	sys.angleTypes[type_Si_O_Si - 1].types[0] = typeSi;
	sys.angleTypes[type_Si_O_Si - 1].types[1] = typeO;
	sys.angleTypes[type_Si_O_Si - 1].types[2] = typeSi;
	strcpy(sys.angleTypes[type_Si_O_Si - 1].name, "Si-O-Si");
	strcpy(sys.angleTypes[type_Si_O_Si - 1].style, "harmonic");
	*/
	

	for (std::vector<Angle>::iterator a = sys.angles.begin(); a != sys.angles.end(); ++a) {
		// angle type 1 : Hw-Ow-Hw
		if (a->ijk[0]->type == typeHw && a->ijk[1]->type == typeOw && a->ijk[2]->type == typeHw) {
			a->type = type_Hw_Ow_Hw;
		}
		// angle type 2 : O-Si-O
		else if (a->ijk[0]->type == typeO && a->ijk[1]->type == typeSi && a->ijk[2]->type == typeO) {
			//a->type = type_O_Si_O;
			a->delete_flag = true;
		}
		else if (a->ijk[0]->type == typeOh && a->ijk[1]->type == typeSi && a->ijk[2]->type == typeO ||
			a->ijk[0]->type == typeO && a->ijk[1]->type == typeSi && a->ijk[2]->type == typeOh ||
			a->ijk[0]->type == typeOh && a->ijk[1]->type == typeSi && a->ijk[2]->type == typeOh) {
			//a->type = type_O_Si_O;
			a->delete_flag = true;
		}
		// angle type 3 : Si-O-Si
		else if (a->ijk[0]->type == typeSi && a->ijk[1]->type == typeO && a->ijk[2]->type == typeSi) {
			//a->type = type_Si_O_Si;
			a->delete_flag = true;
		}
	}

	// Balance excess charge
	//std::vector<int> dis_atom{ typeO, typeOh };
	//printf("Modify::neutralize(): %d\n", neutralize(sys, dis_atom));

	// delete angle O-Si-O and angle Si-O-Si
	delete_angles(sys);
	sys.no_angle_types = 1;
	sys.angleTypes.resize(sys.no_angle_types);

	std::cout << sys;

	return 0;
}

ModifyH::ModifyH(Error * error_) : Topology(error_)
{
	error = error_;
}

ModifyH::~ModifyH()
{
}

int ModifyH::command(int argc, char * argv[], System & sys)
{
	int n = 0;
	while (n < argc)
	{
		if (strncmp(argv[n], "valence", 7) == 0) {
			int ncomm = 4;
			char** commd = new char*[ncomm];

			for (int i = 0; i < ncomm; i++) {
				n++;
				if (!check_arg(argv, "valence", n, argc)) return error->message("", 1);
				commd[i] = argv[n];
			}
		
			if (strncmp(commd[0], "n", 2) == 0) {
				printf("ModifyH::add_to_nuetral(): %d\n", add_to_neutral(sys, atoi(commd[1]), atoi(commd[2]), atoi(commd[3])));
			}
			else if (isdigit(commd[0][0])) {
				printf("ModifyH::add_to_number(): %d\n", add_to_number(sys, atoi(commd[0]), atoi(commd[1]), atoi(commd[2]), atoi(commd[3])));
			}
			else return error->message("Invalid add argument: %s", 2, commd[0]);
		}

		n++;
	}
	return 0;

}

double ModifyH::charge(System & sys)
{
	double qt = 0;

	std::vector<Atom*> dis_atom;
	for (std::vector<Atom>::iterator a = sys.atoms.begin(); a != sys.atoms.end(); ++a) {
		qt += a->q;
	}

	return qt;
}

int ModifyH::add_to_neutral(System & sys, int typeO_, int typeH_, int type_O_H)
{
	srand(time(NULL));

	double qt = charge(sys);
	double q_H = 0.29;
	double bl = 1.00;			// bond length

	// count the number of H atoms need to be added to reach neutral
	if (qt > 0) error->message("Cannot neutralize chage since lumped charge > 0", 1);
	int num_ = - qt / q_H;

	printf("\n\tTotal Excessive Charge: %6.3E | Target Number of Addition: %3d\n\n", qt, num_);

	return add_to_number(sys, num_, typeO_, typeH_, type_O_H);
}

int ModifyH::add_to_number(System & sys, int num_, int typeO_, int typeH_, int type_O_H)
{
	srand(time(NULL));

	double q_H = 0.29;
	double bl = 1.00;			// bond length

	printf("\n\tTry to append %d type %d atoms to type %d atoms\n\n", num_, typeH_, typeO_);

	std::vector<Atom*> group_O;

	if (typeO_ == typeOh) {
		for (std::vector<Atom>::iterator a = sys.atoms.begin(); a != sys.atoms.end(); ++a) {

			// Mode 1 : add at oxygen with coordination number equal to 0

			if ((*a).type == typeO_ && (*a).bondNum == 0) group_O.push_back(&*a);

			// Mode 2 : add at bridging oxygen with coordination number less than 2

			//if ((*a).type == typeO_ && (*a).bondNum < 2) group_O.push_back(&*a);

		}
	}
	else if (typeO_ == typeO) {
		for (std::vector<Atom>::iterator a = sys.atoms.begin(); a != sys.atoms.end(); ++a) {

			// Mode 1 : add at oxygen with coordination number equal to 0

			//if ((*a).type == typeO_ && (*a).bondNum == 0) group_O.push_back(&*a);

			// Mode 2 : add at bridging oxygen with coordination number less than 2

			if ((*a).type == typeO_ && (*a).bondNum < 2) {
				a->type = typeOh;
				a->q = -1.00;
				group_O.push_back(&*a);
			}

		}
	}
	
	
	std::vector<Atom> added_H;
	std::vector<Bond> added_O_H;
	int num_add = 0;

	while (num_add < num_ && group_O.size() > 0) {

		int iadd = rand() % group_O.size();
		Atom* target = &*group_O[iadd];

		// H atom

		bigint iatom = sys.natoms + 1;

		added_H.clear();
		added_H.resize(1);
		added_H[0].id = iatom;
		added_H[0].molecule = target->molecule;
		added_H[0].type = typeH_;
		added_H[0].q = q_H;

		double r[3];

		// Mode 1 : add outward from the residue
		
		if (target->bondNum != 0) {
			if (target->bonds[0]->connected(target) == nullptr)
				error->message("Incompleted ij atoms in bond data: bond %s", 1, std::to_string(target->bonds[0]->id).c_str());

			r[0] = target->x[0] - target->bonds[0]->connected(target)->x[0];
			r[1] = target->x[1] - target->bonds[0]->connected(target)->x[1];
			r[2] = target->x[2] - target->bonds[0]->connected(target)->x[2];
		}

		// Mode 2 : add randomly around the target
		
		else if (target->bondNum == 0) {
			double theta = (double)(rand()) / ((double)(RAND_MAX)+1.0) * 360.0;
			double phi = (double)(rand()) / ((double)(RAND_MAX)+1.0) * 180.0;

			r[0] = cos(theta * M_PI / 180.0);
			r[1] = sin(theta * M_PI / 180.0);
			r[2] = cos(phi * M_PI / 180.0);
		}
		
		added_H[0].x[0] = target->x[0] + bl * r[0] / sqrt(pow(r[0], 2) + pow(r[1], 2) + pow(r[2], 2));
		added_H[0].x[1] = target->x[1] + bl * r[1] / sqrt(pow(r[0], 2) + pow(r[1], 2) + pow(r[2], 2));
		added_H[0].x[2] = target->x[2] + bl * r[2] / sqrt(pow(r[0], 2) + pow(r[1], 2) + pow(r[2], 2));

		added_H[0].n[0] = 0; added_H[0].n[1] = 0; added_H[0].n[2] = 0;

		if (add_atoms(sys, added_H) == 0) break;

		// O-H bond

		bigint ibond = sys.nbonds + 1;

		added_O_H.clear();
		added_O_H.resize(1);
		added_O_H[0].id = ibond;
		added_O_H[0].type = type_O_H;

		if (add_bonds(sys, added_O_H) == 0) break;

		// correlate atom and bond

		sys.atoms[iatom - 1].bondID[sys.atoms[iatom - 1].bondNum] = sys.bonds[ibond - 1].id;
		sys.atoms[iatom - 1].bonds[sys.atoms[iatom - 1].bondNum] = &sys.bonds[ibond - 1];
		sys.atoms[iatom - 1].bondNum++;

		
		target->bondID[target->bondNum] = sys.bonds[ibond - 1].id;
		target->bonds[target->bondNum] = &sys.bonds[ibond - 1];
		target->bondNum++;

		sys.bonds[ibond - 1].ij[0] = &*target;
		sys.bonds[ibond - 1].ij[1] = &sys.atoms[iatom - 1];

		// purge O from group

		group_O.erase(group_O.begin() + iadd);

		//std::vector<Atom*>::iterator pos = std::find(group_O.begin(), group_O.end(), &*target);
		//if (pos != group_O.end()) group_O.erase(pos);

		num_add++;
	}

	printf("\tAdd %d type %d atoms bonded with type %d atoms\n", num_add, typeH_, typeO_);

	if (num_add < num_) error->warning("Not all of type %s atoms being added", 2, std::to_string(typeH_).c_str());

	// Balance excess charge
	std::vector<int> dis_atom{ typeO, typeOh };
	printf("Modify::neutralize(): %d\n", neutralize(sys, dis_atom));

	return 0;
}

AddH2O::AddH2O(Error * error_) : Topology(error_)
{
	error = error_;
}

AddH2O::~AddH2O()
{
}

int AddH2O::command(int argc, char * argv[], System & sys)
{
	int n = 0;
	while (n < argc)
	{
		if (strncmp(argv[n], "add", 3) == 0) {
			int ncomm = 3;
			char** commd = new char*[ncomm];

			for (int i = 0; i < ncomm; i++) {
				n++;
				if (!check_arg(argv, "add", n, argc)) return error->message("", 2);
				commd[i] = argv[n];
			}

			printf("ModifyH2O::add_type(): %d\n", add_type(sys, atoi(commd[0]), atoi(commd[1]), atoi(commd[2])));
		}

		n++;
	}
	return 0;
}

int AddH2O::add_type(System & sys, int num_, int typeOw_, int typeHw_)
{
	srand(time(NULL));

	int num_add = 0;

	// assign group

	std::vector<Atom*> group_Ca;
	std::vector<Atom*> group_Ca_Cw;
	std::vector<Atom*> group_Si;

	for (std::vector<Atom>::iterator a = sys.atoms.begin(); a != sys.atoms.end(); ++a) {

		if (a->type == typeCa) group_Ca.push_back(&*a);

		if (a->type == typeCa || a->type == typeCw) group_Ca_Cw.push_back(&*a);

		else if (a->type == typeSi) group_Si.push_back(&*a);

	}

	// water template

	double temp[3][3];
	temp[0][0] = 0; temp[0][1] = 0; temp[0][2] = 0;
	temp[1][0] = 1; temp[1][1] = 0; temp[1][2] = 0;
	temp[2][0] = cos(109.47*M_PI / 180.0); temp[2][1] = sin(109.47*M_PI / 180.0); temp[2][2] = 0;

	// z gap water distributed away from the intralayer Ca
	double zgap = 3.3; 

	// tolerant radius water distributed away from the other atoms except for Cw
	double r = 1.50;

	// tolerant radius water distributed away from Cw
	double rcw = 0.01;

	// triclinic coordinate

	double lx = sys.box[0][1] - sys.box[0][0];
	double ly = sys.box[1][1] - sys.box[1][0];
	double lz = sys.box[2][1] - sys.box[2][0];
	double yz = sys.box[0][2];
	double xz = sys.box[1][2];
	double xy = sys.box[2][2];

	double a = lx;
	double b = sqrt(pow(ly, 2) + pow(xy, 2));
	double c = sqrt(pow(lz, 2) + pow(xz, 2) + pow(yz, 2));
	double alpha = acos((xy*xz + ly * yz) / (b*c));
	double beta = acos(xz / c);
	double gamma = acos(xy / b);

	double omega = a * b * c *
		sqrt(1 - pow(cos(alpha), 2) - pow(cos(beta), 2) - pow(cos(gamma), 2) + 2 * cos(alpha) * cos(beta) *cos(gamma));

	//printf("a: %6.3f b: %6.3f c: %6.3f \nalpha: %6.3f beta: %6.3f gamma: %6.3f\n", a, b, c, alpha, beta, gamma);
	while (num_add < num_)
	{
		double ra = (double)(rand()) / ((double)(RAND_MAX)+1.0);
		double rb = (double)(rand()) / ((double)(RAND_MAX)+1.0);
		double rc = (double)(rand()) / ((double)(RAND_MAX)+1.0);
		double trans[3]{ sys.box[0][0] + a*ra + b * cos(gamma)*rb + c * cos(beta)*rc,
			sys.box[1][0] + b*sin(gamma)*rb + c * (cos(alpha) - cos(beta)*cos(gamma)) / sin(gamma)*rc,
			sys.box[2][0] + omega / (a*b*sin(gamma))*rc };

		
		bool add = true;

		if (abs(trans[2] - sys.box[2][0]) < zgap ||
			abs(trans[2] - sys.box[2][1]) < zgap) {
			add = false;
			continue;
		}

		for (std::vector<Atom>::iterator a = sys.atoms.begin(); a != sys.atoms.end(); ++a) {
			
			if (a->type == typeCa) {
				if (abs(trans[2] - a->x[2]) < zgap) {
					add = false;
					break;
				}
			}
			else if (a->type == typeCw) {
				if ((pow(trans[0] - a->x[0], 2)
					+ pow(trans[1] - a->x[1], 2)
					+ pow(trans[2] - a->x[2], 2)) < pow(rcw, 2)) {
					add = false;
					break;
				}
			}
			else if ((pow(trans[0] - a->x[0], 2)
				+ pow(trans[1] - a->x[1], 2)
				+ pow(trans[2] - a->x[2], 2)) < pow(r, 2)) {
				add = false;
				break;
			}
			
		}

		if (add) {
			if ((int)num_ / BAR_BINS != 0) {
				if (num_add % ((int)num_ / BAR_BINS) == 0) {
					printf("\tadd water [");
					for (int i = 0; i < BAR_BINS; i++) {
						if (i < num_add / ((int)num_ / BAR_BINS)) printf("=");
						else printf(".");
					}
					printf("] %2d/%2d\n", num_add / ((int)num_ / BAR_BINS), BAR_BINS);
				}
			}

			double theta = (double)(rand()) / ((double)(RAND_MAX)+1.0) * 360.0;
			double phi = (double)(rand()) / ((double)(RAND_MAX)+1.0) * 180.0;

			// rotation matrix
			double rot1[3][3];
			rot1[0][0] = cos(theta * M_PI / 180.0);
			rot1[0][1] = cos((90.00 + theta) * M_PI / 180.0);
			rot1[0][2] = cos(90.00 * M_PI / 180.0);
			rot1[1][0] = cos((90.00 - theta) * M_PI / 180.0);
			rot1[1][1] = cos(theta * M_PI / 180.0);
			rot1[1][2] = cos(90.00 * M_PI / 180.0);
			rot1[2][0] = cos(90.00 * M_PI / 180.0);
			rot1[2][1] = cos(90.00 * M_PI / 180.0);
			rot1[2][2] = cos(0.00 * M_PI / 180.0);

			double rot2[3][3];
			rot2[0][0] = cos(phi * M_PI / 180.0);
			rot2[0][1] = cos(90.00 * M_PI / 180.0);
			rot2[0][2] = cos((90.00 - phi) * M_PI / 180.0);
			rot2[1][0] = cos(90.00 * M_PI / 180.0);
			rot2[1][1] = cos(0.00 * M_PI / 180.0);
			rot2[1][2] = cos(90.00 * M_PI / 180.0);
			rot2[2][0] = cos((90.00 + phi) * M_PI / 180.0);
			rot2[2][1] = cos(90.00 * M_PI / 180.0);
			rot2[2][2] = cos(phi * M_PI / 180.0);

			double rot[3][3];
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					rot[i][j] = rot1[i][0] * rot2[0][j] + rot1[i][1] * rot2[1][j] + rot1[i][2] * rot2[2][j];
				}
			}

			// translate and rotate water molecular

			double coord[3][3];
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {				
					coord[i][j] = trans[j] + rot[j][0] * temp[i][0] +
						rot[j][1] * temp[i][1] +
						rot[j][2] * temp[i][2];
						
				}
			}
			
			std::vector<Atom> water;
			water.resize(3);

			bigint iO = sys.natoms + 1;
			water[0].id = iO;
			water[0].molecule = ++sys.no_molecular;
			water[0].type = typeOw_;
			water[0].q = -0.82;

			bigint iH1 = sys.natoms + 2;
			water[1].id = iH1;
			water[1].molecule = sys.no_molecular;
			water[1].type = typeHw_;
			water[1].q = 0.41;

			bigint iH2 = sys.natoms + 3;
			water[2].id = iH2;
			water[2].molecule = sys.no_molecular;
			water[2].type = typeHw_;
			water[2].q = 0.41;

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++) {
					water[i].x[j] = coord[i][j];
					water[i].n[j] = 0;
				}
			}

			if (add_atoms(sys, water) == 0) error->warning("Cannot add water atoms to system", 1);

			std::vector<Bond> waterbond;
			waterbond.resize(2);

			bigint iOH1 = sys.nbonds + 1;
			waterbond[0].id = iOH1;
			waterbond[0].type = type_Ow_Hw;
			
			bigint iOH2 = sys.nbonds + 2;
			waterbond[1].id = iOH2;
			waterbond[1].type = type_Ow_Hw;

			if (add_bonds(sys, waterbond) == 0) error->warning("Cannot add water bonds to system", 2);

			std::vector<Angle> waterangle;
			waterangle.resize(1);

			bigint iH1OH2 = sys.nangles + 1;
			waterangle[0].id = iH1OH2;
			waterangle[0].type = type_Hw_Ow_Hw;

			if (add_angles(sys, waterangle) == 0) error->warning("Cannot add water angles to system", 3);

			// correlate the atoms, bonds and angle

			Atom* O = &sys.atoms[iO - 1];
			Atom* H1 = &sys.atoms[iH1 - 1];
			Atom* H2 = &sys.atoms[iH2 - 1];

			Bond* OH1 = &sys.bonds[iOH1 - 1];
			Bond* OH2 = &sys.bonds[iOH2 - 1];

			Angle* H1OH2 = &sys.angles[iH1OH2 - 1];

			O->bondID[O->bondNum] = OH1->id;
			O->bonds[O->bondNum] = &*OH1;
			O->bondNum++;

			H1->bondID[H1->bondNum] = OH1->id;
			H1->bonds[H1->bondNum] = &*OH1;
			H1->bondNum++;

			O->bondID[O->bondNum] = OH2->id;
			O->bonds[O->bondNum] = &*OH2;
			O->bondNum++;

			H2->bondID[H2->bondNum] = OH2->id;
			H2->bonds[H2->bondNum] = &*OH2;
			H2->bondNum++;

			//sys.atoms[iO - 1].bondID[sys.atoms[iO - 1].bondNum] = sys.bonds[iOH1 - 1].id;
			//sys.atoms[iO - 1].bonds[sys.atoms[iO - 1].bondNum] = &sys.bonds[iOH1 - 1];
			//sys.atoms[iO - 1].bondNum++;

			OH1->ij[0] = &*O; 
			OH1->ij[1] = &*H1;

			OH2->ij[0] = &*O;
			OH2->ij[1] = &*H2;

			H1OH2->ijk[0] = &*H1;
			H1OH2->ijk[1] = &*O;
			H1OH2->ijk[2] = &*H2;

			num_add++;

		}
	}

	printf("\n\tCa/Si = %4.2f | H2O/Si = %4.2f\n\n", 
		group_Ca_Cw.size() / (double)group_Si.size(),
		num_add / (double)group_Si.size());

	/*
	FILE* nmr = fopen("NMR.txt", "a");
	fprintf(nmr, "\t%6.3f\t%6.3f\t%6.3f\t%4.2f\n",
		Q[0] / (Q[0] + Q[1] + Q[2]),
		Q[1] / (Q[0] + Q[1] + Q[2]),
		Q[2] / (Q[0] + Q[1] + Q[2]),
		2.0 / (Q[1] / (Q[1] + Q[2])));
	*/

	return num_add;
}
