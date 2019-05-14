#include "modify.h"

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
