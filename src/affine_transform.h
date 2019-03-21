/* -*- C++ -*------------------------------------------------------------
 *
 *	affine_transform.h
 *
 * ---------------------------------------------------------------------- */

#ifndef AFFINE_TRANSFORM_H
#define AFFINE_TRANSFORM_H

#include "error.h"
#include "system.h"
#include "stl2lmp.h"

class AffineTransform
{
public:
	AffineTransform(Error *error_);
	~AffineTransform();

	virtual int command(System &sys);
protected:
	Error *error;

	
private:

};

#endif
