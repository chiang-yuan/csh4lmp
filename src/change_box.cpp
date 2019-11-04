#include "change_box.h"

ChangeBox::ChangeBox(Error * error_)
{
	error = error_;
}

ChangeBox::~ChangeBox()
{
}

int ChangeBox::command(int argc, char * argv[], System & sys)
{
	int n = 0;
	while (n < argc)
	{
		if (strncmp(argv[n], "boundary", 8) == 0) {
			int ncomm = 3;
			char** commd = new char*[ncomm];

			for (int i = 0; i < ncomm; i++) {
				n++;
				if (!check_arg(argv, "boundary", n, argc)) return error->message("", 1);
				
				commd[i] = argv[n];
			}

			int xyz[3];

			for (int i = 0; i < ncomm; i++) {

				if (strncmp(commd[i], "p", 1) == 0) {
					xyz[i] = 1;
				}
				else if (strncmp(commd[i], "f", 1) == 0) {
					xyz[i] = 2;
				}
				else return error->message("Unknown periodic boundary condition: %s", 2, commd[i]);
			}

			printf("ChangeBox::periodic_to_shrinkwrapped(): %d\n", periodic_to_shrinkwarpped(sys, xyz));

		}

		n++;
	}
	return 0;
}

bool ChangeBox::check_arg(char ** arg, const char * flag, int num, int argc)
{
	if (num >= argc) {
		printf("Missing argument for \"%s\" flag\n", flag);
		return false;
	}

	return true;
}

int ChangeBox::periodic_to_shrinkwarpped(System & sys, int xyz[3])
{
	double cellvectors[3][3] = { {sys.box[0][1] - sys.box[0][0], 0 , 0},
							 {sys.box[2][2], sys.box[1][1] - sys.box[1][0] ,0},
							 {sys.box[1][2], sys.box[0][2] , sys.box[2][1] - sys.box[2][0]} };

	// make image consistent
	/*
	for (std::vector<Atom>::iterator a = sys.atoms.begin(); a != sys.atoms.end(); ++a) {
		for (int i = 0; i < 3; i++) {
			a->x[i] += (cellvectors[0][i] * a->n[0] + cellvectors[1][i] * a->n[1] + cellvectors[2][i] * a->n[2]);
		}
		a->n[0] = 0;
		a->n[1] = 0;
		a->n[2] = 0;
	}
	*/
	// check bonds not cross boundary

	for (std::vector<Bond>::iterator b = sys.bonds.begin(); b != sys.bonds.end(); ++b) {
		
		double vij[3] = { b->ij[1]->x[0] - b->ij[0]->x[0],
			b->ij[1]->x[1] - b->ij[0]->x[1],
			b->ij[1]->x[2] - b->ij[0]->x[2] };

		for (int i = 0; i < 3; i++) {
			if (xyz[i] == 2) {
				for (int j = 0; j < 3; j++) {
					b->ij[1]->x[j] -= round((vij[0] * cellvectors[i][0] + vij[1] * cellvectors[i][1] + vij[2] * cellvectors[i][2]) /
						(powl(cellvectors[i][0], 2) + powl(cellvectors[i][1], 2) + powl(cellvectors[i][2], 2))) * cellvectors[i][j];
				}
				

				/*
				double proj = (vij[0] * cellvectors[i][0] + vij[1] * cellvectors[i][1] + vij[2] * cellvectors[i][2]) /
					sqrtl(powl(cellvectors[i][0], 2) + powl(cellvectors[i][1], 2) + powl(cellvectors[i][2], 2));

				if (abs(proj) > sqrtl(powl(cellvectors[i][0], 2) + powl(cellvectors[i][1], 2) + powl(cellvectors[i][2], 2)) / 2.0) {
					b->ij[1]->x[0] -= proj / abs(proj)*cellvectors[i][0];
					b->ij[1]->x[1] -= proj / abs(proj)*cellvectors[i][1];
					b->ij[1]->x[2] -= proj / abs(proj)*cellvectors[i][2];
				}
				*/
			}
		}

	}

	// adjust box dimension


	

	return 0;
}
