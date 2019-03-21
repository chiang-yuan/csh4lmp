#include "affine_transform.h"

AffineTransform::AffineTransform(Error * error_)
{
	error = error_;
}

AffineTransform::~AffineTransform()
{
}

int AffineTransform::command(System & sys)
{
	// rotation matrix
	double rotation[3][3];
	rotation[0][0] = cos(56.51 * M_PI / 180.0);
	rotation[0][1] = cos(146.51 * M_PI / 180.0);
	rotation[0][2] = cos(90.00 * M_PI / 180.0);
	rotation[1][0] = cos(33.49 * M_PI / 180.0);
	rotation[1][1] = cos(56.51 * M_PI / 180.0);
	rotation[1][2] = cos(90.00 * M_PI / 180.0);
	rotation[2][0] = cos(90.00 * M_PI / 180.0);
	rotation[2][1] = cos(90.00 * M_PI / 180.0);
	rotation[2][2] = cos(0.00 * M_PI / 180.0);


	// rotate atoms
	for (std::vector<Atom>::iterator a = sys.atoms.begin(); a != sys.atoms.end(); ++a) {
		double x[3]{ a->x[0],a->x[1],a->x[2] };
		for (int i = 0; i < 3; i++) {
			a->x[i] = rotation[i][0] * x[0]
					+ rotation[i][1] * x[1]
					+ rotation[i][2] * x[2];
		}
	}

	// rotate box
	double origin[3]{ sys.box[0][0],sys.box[1][0],sys.box[2][0] };

	for (int i = 0; i < 3; i++) {
		sys.box[i][0] = rotation[i][0] * origin[0] +
			rotation[i][1] * origin[1] +
			rotation[i][2] * origin[2];
	}

	sys.box[0][0] -= 51.7300;
	sys.box[0][1] = sys.box[0][0] + 51.7300; sys.box[0][2] = 0;
	sys.box[1][1] = sys.box[1][0] + 11.1587*2; sys.box[1][2] = 0;
	sys.box[2][1] = sys.box[2][0] + 113.8500*3.0/5.0; sys.box[2][2] = 7.3830*2;

	return 0;
}
