#include "read_data.h"

ReadData::ReadData(Error *error_)
{
	error = error_;
}

ReadData::~ReadData()
{
}

bool ReadData::check_arg(char **arg, const char *flag, int num, int argc) {

	if (num >= argc) {
		printf("Missing argument for \"%s\" flag\n", flag);
		return false;
	}

	return true;
}

int ReadData::read_section(FILE * file_, int nrows_, int ncolumns_, char * buffer_)
{
	int m = 0;
	for (int i = 0; i < nrows_; i++) {
		if (!fgets(&buffer_[m], ncolumns_, file_)) {
			m = 0;
			break;
		}
		m += strlen(&buffer_[m]);
	}
	
	// EOL
	if (m) {
		if (buffer_[m - 1] != '\n') strcpy(&buffer_[m++], "\n");
		
		m = m + 1;

	}

	if (m == 0) return 1;
	return 0;
}

int ReadData::count_words(const char * line)
{
	int n = strlen(line) + 1;
	char *copy = new char[n];
	strcpy(copy, line);

	char *ptr;
	if ((ptr = strchr(copy, '#'))) *ptr = ' ';

	if (strtok(copy, WHITESPACE) == NULL) {
		delete[] copy;
		return 0;
	}

	n = 1;
	while (strtok(NULL, WHITESPACE)) n++;
	delete[] copy;
	return n;
}

int ReadData::command(int argc, char * argv[], System & sys)
{
	int n = 0;
	while (n < argc) {
		if (!check_arg(argv, "input file name", n, argc)) return error->message("",1);
		printf("ReadData::over(): %d\n", over(argv[n], sys));
		n++;
	}
	return 0;
}

int ReadData::over(char* filename_, System & sys)
{
	file = fopen(filename_, "r");
	if (file == NULL) return error->message("Cannot open file \"%s\"", 2, filename_);

	printf("\tReading from data file: %s ...\n", filename_);

	/***** Scan 0 : Read Header *****/

	if (fgets(header, MAX_STRING, file) == NULL) {
		return error->message("No header line included in the .data file",1);
	}

	/***** Scan 1 : Identify Data Structure *****/

	while (fgets(buffer, MAX_STRING, file) != NULL) {

		// skip the blank buffer line
		if (strspn(buffer, WHITESPACE) == strlen(buffer)) continue;

		//

		if (strstr(buffer, "atoms") != NULL) {
			sscanf(buffer, BIGINT_FORMAT, &sys.natoms);
		}
		else if (strstr(buffer, "bonds") != NULL) {
			sscanf(buffer, BIGINT_FORMAT, &sys.nbonds);
		}
		else if (strstr(buffer, "angles") != NULL) {
			sscanf(buffer, BIGINT_FORMAT, &sys.nangles);
		}
		else if (strstr(buffer, "dihedrals") != NULL) {
			sscanf(buffer, BIGINT_FORMAT, &sys.ndihedrals);
		}
		else if (strstr(buffer, "impropers") != NULL) {
			sscanf(buffer, BIGINT_FORMAT, &sys.nimpropers);
		}

		// 

		else if (strstr(buffer, "atom types") != NULL) {
			sscanf(buffer, "%d", &sys.no_atom_types);
		}
		else if (strstr(buffer, "bond types") != NULL) {
			sscanf(buffer, "%d", &sys.no_bond_types);
		}
		else if (strstr(buffer, "angle types") != NULL) {
			sscanf(buffer, "%d", &sys.no_angle_types);
		}
		else if (strstr(buffer, "dihedral types") != NULL) {
			sscanf(buffer, "%d", &sys.no_dihedral_types);
		}
		else if (strstr(buffer, "improper types") != NULL) {
			sscanf(buffer, "%d", &sys.no_improper_types);
		}

		// 

		else if (strstr(buffer, "xlo xhi") != NULL) {
			sscanf(buffer, "%lg %lg", &sys.box[0][0], &sys.box[0][1]);
		}
		else if (strstr(buffer, "ylo yhi") != NULL) {
			sscanf(buffer, "%lg %lg", &sys.box[1][0], &sys.box[1][1]);
		}
		else if (strstr(buffer, "zlo zhi") != NULL) {
			sscanf(buffer, "%lg %lg", &sys.box[2][0], &sys.box[2][1]);
		}
		else if (strstr(buffer, "xy xz yz")) {
			sys.triclinic_flag = 1;
			sscanf(buffer, "%lg %lg %lg", &sys.box[2][2], &sys.box[1][2], &sys.box[0][2]);
		}
		else break;
	}

	// Call for memory allocation

	//sys.atoms = std::vector<Atom>(sys.natoms);
	//sys.bonds = std::vector<Bond>(sys.nbonds);
	//sys.angles = std::vector<Angle>(sys.nangles);

	sys.atoms = std::vector<Atom>(MAX_ATOMS);
	sys.bonds = std::vector<Bond>(MAX_BONDS);
	sys.angles = std::vector<Angle>(MAX_ANGLES);

	sys.atoms.resize(sys.natoms);
	sys.bonds.resize(sys.nbonds);
	sys.angles.resize(sys.nangles);

	sys.atomTypes = std::vector<AtomType>(sys.no_atom_types);
	sys.bondTypes = std::vector<BondType>(sys.no_bond_types);
	sys.angleTypes = std::vector<AngleType>(sys.no_angle_types);

	/***** Scan 2 : Read Data Information *****/

	rewind(file);

	while (fgets(buffer, MAX_STRING, file) != NULL) {

		// skip the blank buffer line
		if (strspn(buffer, WHITESPACE) == strlen(buffer)) continue;

		// Section : Masses
		if (strstr(buffer, "Masses") != NULL) {
			char* multibuffer = new char[sys.no_atom_types*MAX_STRING];
			char* iter;

			//skip first blank line
			fgets(new char[MAX_STRING], MAX_STRING, file);

			int eof = read_section(file, sys.no_atom_types, MAX_STRING, multibuffer);
			if (eof) {
				return error->message("Unexpected format of data file",2);
			}

			for (int i = 0; i < sys.no_atom_types; i++) {
				iter = strchr(multibuffer, '\n');
				*iter = '\0';

				int itype;
				double imass;
				char ielement[MAX_NAME];

				int n = sscanf(multibuffer, "%d %lg # %s", &itype, &imass, ielement);

				if (n < 2 || n > 3) {
					return error->message("Invalid mass data in data file",3);
				}
				
				if (itype < 1 || itype > sys.no_atom_types) {
					return error->message("Invalid atom type for setting mass",4);
				}

				sys.atomTypes[itype - 1].mass = imass;
				strcpy(sys.atomTypes[itype - 1].element, ielement);

				multibuffer = iter + 1;
			}

		}

		// Section : Pair Coeffs
		if (strstr(buffer, "Pair Coeffs") != NULL) {
			char potential[MAX_NAME];
			sscanf(buffer, "%*s %*s # %s", potential);

			char* multibuffer = new char[sys.no_atom_types*MAX_STRING];
			char* iter;

			//skip first blank line
			fgets(new char[MAX_STRING], MAX_STRING, file);

			int eof = read_section(file, sys.no_atom_types, MAX_STRING, multibuffer);
			if (eof) {
				return error->message("Unexpected format of data file",5);
			}

			for (int i = 0; i < sys.no_atom_types; i++) {
				iter = strchr(multibuffer, '\n');
				*iter = '\0';

				int itype;
				double iepsilon;
				double isigma;
				char iname[MAX_NAME];

				int n = sscanf(multibuffer, "%d %lg %lg # %s", &itype, &iepsilon, &isigma, iname);
				if (n < 3) {
					return error->message("Invalid pair coeffs data in data file",6);
				}

				if (itype < 1 || itype > sys.no_atom_types) {
					return error->message("Invalid atom type for setting pair coeffs",7);
				}

				strcpy(sys.atomTypes[itype - 1].potential, potential);
				sys.atomTypes[itype - 1].coeff[0] = iepsilon;
				sys.atomTypes[itype - 1].coeff[1] = isigma;
				strcpy(sys.atomTypes[itype - 1].element, iname);

				multibuffer = iter + 1;
			}


		}

		// Section : Bond Coeffs
		if (strstr(buffer, "Bond Coeffs") != NULL) {
			char style[MAX_NAME];
			sscanf(buffer, "%*s %*s # %s", style);

			char* multibuffer = new char[sys.no_bond_types*MAX_STRING];
			char* iter;

			//skip first blank line
			fgets(new char[MAX_STRING], MAX_STRING, file);

			int eof = read_section(file, sys.no_bond_types, MAX_STRING, multibuffer);
			if (eof) {
				return error->message("Unexpected format of data file",8);
			}

			for (int i = 0; i < sys.no_bond_types; i++) {
				iter = strchr(multibuffer, '\n');
				*iter = '\0';

				int itype;
				double ik;
				double ia0;
				char iname[MAX_NAME];

				int n = sscanf(multibuffer, "%d %lg %lg # %s", &itype, &ik, &ia0, iname);
				if (n < 3) {
					return error->message("Invalid bond data in data file",9);
				}

				if (itype < 1 || itype > sys.no_bond_types) {
					return error->message("Invalid bond type for setting bond coeffs",10);
				}

				sys.bondTypes[itype - 1].coeff[0] = ik;
				sys.bondTypes[itype - 1].coeff[1] = ia0;
				strcpy(sys.bondTypes[itype - 1].name, iname);
				strcpy(sys.bondTypes[itype - 1].style, style);

				multibuffer = iter + 1;
			}

		}

		// Section : Angle Coeffs
		if (strstr(buffer, "Angle Coeffs") != NULL) {
			char style[MAX_NAME];
			sscanf(buffer, "%*s %*s # %s", style);

			char* multibuffer = new char[sys.no_angle_types*MAX_STRING];
			char* iter;

			//skip first blank line
			fgets(new char[MAX_STRING], MAX_STRING, file);

			int eof = read_section(file, sys.no_angle_types, MAX_STRING, multibuffer);
			if (eof) {
				return error->message("Unexpected format of data file", 8);
			}

			for (int i = 0; i < sys.no_angle_types; i++) {
				iter = strchr(multibuffer, '\n');
				*iter = '\0';

				int itype;
				double iK;
				double ia0;
				char iname[MAX_NAME];

				int n = sscanf(multibuffer, "%d %lg %lg # %s", &itype, &iK, &ia0, iname);
				if (n < 3) {
					return error->message("Invalid angle data in data file", 9);
				}

				if (itype < 1 || itype > sys.no_angle_types) {
					return error->message("Invalid angle type for setting angle coeffs", 10);
				}

				sys.angleTypes[itype - 1].coeff[0] = iK;
				sys.angleTypes[itype - 1].coeff[1] = ia0;
				strcpy(sys.angleTypes[itype - 1].name, iname);
				strcpy(sys.angleTypes[itype - 1].style, style);

				multibuffer = iter + 1;
			}

		}

		// Section : Atoms
		if (strstr(buffer, "Atoms") != NULL) {
			char style[MAX_NAME];
			sscanf(buffer, "%*s # %s", style);

			//skip first blank line
			fgets(new char[MAX_STRING], MAX_STRING, file);

			int ntruncate;
			bigint nread = 0;

			while (nread < sys.natoms) {
				ntruncate = std::min((int)(sys.natoms - nread), MAX_CHUNK);

				char* multibuffer = new char[ntruncate*MAX_STRING];
				char* iter;

				int eof = read_section(file, ntruncate, MAX_STRING, multibuffer);
				if (eof) {
					return error->message("Unexpected end of data file",11);
				}

				//

				iter = strchr(multibuffer, '\n');
				*iter = '\0';
				int nwords = count_words(multibuffer);
				*iter = '\n';

				char **values = new char*[nwords];

				for (int i = 0; i < ntruncate; i++) {
					iter = strchr(multibuffer, '\n');

					values[0] = strtok(multibuffer, WHITESPACE);


					if (values[0] == NULL) {
						return error->message("Incorrect atom format in data file", 12);
					}

					for (int j = 1; j < nwords; j++) {
						values[j] = strtok(NULL, "# \t\r\n\f");
						if (values[j] == NULL) {
							return error->message("Incorrect atom format in data file",13);
						}
					}

					int n = 0;

					bigint iid = atoi(values[n++]);
					int imolecule = atoi(values[n++]);
					int itype = atoi(values[n++]);
					double iq = atof(values[n++]);
					double ix = atof(values[n++]);
					double iy = atof(values[n++]);
					double iz = atof(values[n++]);
					double inx = atof(values[n++]);
					double iny = atof(values[n++]);
					double inz = atof(values[n++]);
					char iname[MAX_NAME]; 
					if (n < nwords - 1) strcpy(iname, values[n++]);

					sys.atoms[iid - 1].id = iid;
					sys.atoms[iid - 1].molecule = imolecule;
					sys.atoms[iid - 1].type = itype;
					sys.atoms[iid - 1].q = iq;
					sys.atoms[iid - 1].x[0] = ix; sys.atoms[iid - 1].x[1] = iy; sys.atoms[iid - 1].x[2] = iz;
					sys.atoms[iid - 1].n[0] = inx; sys.atoms[iid - 1].n[1] = iny; sys.atoms[iid - 1].n[2] = inz;
					strcpy(sys.atoms[iid - 1].name, iname);

					sys.atoms[iid - 1].bondNum = 0;

					if (imolecule > sys.no_molecular) sys.no_molecular++;

					multibuffer = iter + 1;
				}

				nread += ntruncate;
			}

		}

		// Section : Bonds
		if (strstr(buffer, "Bonds") != NULL) {
			char style[MAX_NAME];
			sscanf(buffer, "%*s # %s", style);

			//skip first blank line
			fgets(new char[MAX_STRING], MAX_STRING, file);

			int ntruncate;
			bigint nread = 0;

			while (nread < sys.nbonds) {
				ntruncate = std::min((int)(sys.nbonds - nread), MAX_CHUNK);

				char* multibuffer = new char[ntruncate*MAX_STRING];
				char* iter;

				int eof = read_section(file, ntruncate, MAX_STRING, multibuffer);
				if (eof) {
					return error->message("Unexpected end of data file",14);
				}

				//

				iter = strchr(multibuffer, '\n');
				*iter = '\0';
				int nwords = count_words(multibuffer);
				*iter = '\n';

				char **values = new char*[nwords];

				for (int i = 0; i < ntruncate; i++) {
					bigint iid;
					int itype;

					bigint iI;
					bigint iJ;

					iter = strchr(multibuffer, '\n');
					*iter = '\0';

					sscanf(multibuffer, "%lld %d %lld %lld", &iid, &itype, &iI, &iJ);

					if ((iI <= 0) || (iI > sys.natoms) ||
						(iJ <= 0) || (iJ > sys.natoms) || (iI == iJ)) {
						return error->message("Invalid atom ID in Bonds section of data file", 15);
					}
					if (itype <= 0 || itype > sys.nbonds) {
						return error->message("Invalid bond type in Bonds section of data file", 16);
					}


					sys.bonds[iid - 1].id = iid;
					sys.bonds[iid - 1].type = itype;

					sys.bonds[iid - 1].ij[0] = &sys.atoms[iI - 1];
					sys.atoms[iI - 1].bondID[sys.atoms[iI - 1].bondNum] = iid;
					//sys.atoms[iI - 1].bondatoms[sys.atoms[iI - 1].bondNum] = &sys.atoms[iJ - 1];
					sys.atoms[iI - 1].bonds[sys.atoms[iI - 1].bondNum] = &sys.bonds[iid - 1];
					sys.atoms[iI - 1].bondNum++;
					

					sys.bonds[iid - 1].ij[1] = &sys.atoms[iJ - 1];
					sys.atoms[iJ - 1].bondID[sys.atoms[iJ - 1].bondNum] = iid;
					//sys.atoms[iJ - 1].bondatoms[sys.atoms[iJ - 1].bondNum] = &sys.atoms[iI - 1];
					sys.atoms[iJ - 1].bonds[sys.atoms[iJ - 1].bondNum] = &sys.bonds[iid - 1];
					sys.atoms[iJ - 1].bondNum++;
					
					/*
					bonds[iid - 1].IJID[0] = iI;
					atoms[iI - 1].bondID[atoms[iI - 1].bondNum] = iid;
					atoms[iI - 1].bondAtomID[atoms[iI - 1].bondNum] = iJ;
					atoms[iI - 1].bondNum++;

					bonds[iid - 1].IJID[1] = iJ;
					atoms[iJ - 1].bondID[atoms[iJ - 1].bondNum] = iid;
					atoms[iJ - 1].bondAtomID[atoms[iJ - 1].bondNum] = iI;
					atoms[iJ - 1].bondNum++;
					*/

					
					multibuffer = iter + 1;
				}

				nread += ntruncate;
			}

		}

		// Section : Angles
		if (strstr(buffer, "Angles") != NULL) {
			char style[MAX_NAME];
			sscanf(buffer, "%*s # %s", style);

			//skip first blank line
			fgets(new char[MAX_STRING], MAX_STRING, file);

			int ntruncate;
			bigint nread = 0;

			while (nread < sys.nangles) {
				ntruncate = std::min((int)(sys.nangles - nread), MAX_CHUNK);

				char* multibuffer = new char[ntruncate*MAX_STRING];
				char* iter;

				int eof = read_section(file, ntruncate, MAX_STRING, multibuffer);
				if (eof) {
					return error->message("Unexpected end of data file", 14);
				}

				//

				iter = strchr(multibuffer, '\n');
				*iter = '\0';
				int nwords = count_words(multibuffer);
				*iter = '\n';

				char **values = new char*[nwords];

				for (int i = 0; i < ntruncate; i++) {
					bigint iid;
					int itype;

					bigint iI, iJ, iK;

					iter = strchr(multibuffer, '\n');
					*iter = '\0';

					sscanf(multibuffer, "%lld %d %lld %lld %lld", &iid, &itype, &iI, &iJ, &iK);

					if ((iI <= 0) || (iI > sys.natoms) ||
						(iJ <= 0) || (iJ > sys.natoms) ||
						(iK <= 0) || (iK > sys.natoms) ||
						(iI == iJ) || (iI == iK) || (iJ == iK)) {
						return error->message("Invalid atom ID in Angles section of data file", 15);
					}
					if (itype <= 0 || itype > sys.nbonds) {
						return error->message("Invalid angle type in Angles section of data file", 16);
					}

					sys.angles[iid - 1].id = iid;
					sys.angles[iid - 1].type = itype;

					sys.angles[iid - 1].ijk[0] = &sys.atoms[iI - 1];
					sys.atoms[iI - 1].angles.push_back(&sys.angles[iid - 1]);
					sys.atoms[iI - 1].angleNum++;

					sys.angles[iid - 1].ijk[1] = &sys.atoms[iJ - 1];
					sys.atoms[iJ - 1].angles.push_back(&sys.angles[iid - 1]);
					sys.atoms[iJ - 1].angleNum++;

					sys.angles[iid - 1].ijk[2] = &sys.atoms[iK - 1];
					sys.atoms[iK - 1].angles.push_back(&sys.angles[iid - 1]);
					sys.atoms[iK - 1].angleNum++;

					multibuffer = iter + 1;
				}

				nread += ntruncate;
			}

		}
	}

	fclose(file);
	std::cout << sys;	
	return 0;
}


