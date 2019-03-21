/* -*- C++ -*------------------------------------------------------------
 *
 *	stl2lmp.h
 *
 * ---------------------------------------------------------------------- */

#ifndef STL2LMP_H
#define STL2LMP_H

#define STL2LMP_VERSION "v5.3 / 12 Mar 2019"

#define WHITESPACE " \t\r\n\f"

#define MAX_NAME	64
#define MAX_LINE	128
#define MAX_STRING	1024
#define MAX_CHUNK	1024

#define BIGINT_FORMAT "%lld"

#define NSECTIONS	25

typedef long long bigint;

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define _USE_MATH_DEFINES

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cctype>
#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>


#endif // !STL2LMP_H
