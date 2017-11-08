#pragma once

#define DISALLOW_COPY_AND_ASSIGN(TypeName)    \
	TypeName(const TypeName&);                \
	TypeName& operator=(const TypeName&)

///////////////////////////////////////////////
#define SAFE_DELETE(x)                        \
	do {                                      \
		if (x) { delete x; x = NULL; }        \
	}while(0)

///////////////////////////////////////////////
#define SAFE_DELETE_ARR(x)                    \
	do {                                      \
		if (x) { delete[] x; x = NULL; }      \
	}while(0)

///////////////////////////////////////////////
#define SAFE_RELEASE(x)                       \
	do {                                      \
		if (x) { x->Release(); x = NULL; }    \
	}while(0)