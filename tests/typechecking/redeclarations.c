// Feature tests of typechecking redeclarations of functions
// and variables
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note -fcheckedc-extension %s

#include "../../include/stdchecked.h"

//---------------------------------------------------------------------------//
// Declarations of functions with unchecked parameters are compatible with   //
// declarations that add bounds-safe interfaces.                             //
//---------------------------------------------------------------------------//

void f1(int *p, int len);
void f1(int *p : count(len), int len);

void f2(int *p, int len);
void f2(int *p : bounds(p, p + len), int len);

void f3(int *p, int len);
void f3(int *p : byte_count(len * sizeof(int)), int len);

void f4(int *p, int len);
void f4(int *p : itype(ptr<int>), int len);

// Order doesn't matter for declarations
void f5(int *p : count(len), int len);
void f5(int *p, int len);

void f6(int *p : bounds(p, p + len), int len);
void f6(int *p, int len);

void f7(int *p : byte_count(len * sizeof(int)), int len);
void f7(int *p, int len);

void f8(int *p : itype(ptr<int>), int len);
void f8(int *p, int len);

//---------------------------------------------------------------------------//
// Declarations of functions that return unchecked types are compatible      //
// with declarations that add bounds-safe interfaces.                        //
//---------------------------------------------------------------------------//

int *f20(int len);
int *f20(int len) : count(len);

int *f21(int len);
int *f21(int len) : byte_count(len * sizeof(int));

int *f22(int len);
int *f22(int len) : itype(ptr<int>);

// Order doesn't matter
int *f23(int len) : count(len);
int *f23(int len);

int *f24(int len) : byte_count(len * sizeof(int));
int *f24(int len);

int *f25(int len) : itype(ptr<int>);
int *f25(int len);

//---------------------------------------------------------------------------//
// Redeclarations of functions that have parameters that have bounds-safe    //
// interfaces must have matching interfaces.                                 //
//---------------------------------------------------------------------------//

void f30(int *p : count(len), int len);
void f30(int *p : count(len), int len);
void f30(int *p : count(len + 1), int len);  // expected-error {{conflicting types for 'f30'}}

void f31(int *p : count(len), int len);
// A redeclaration without bounds-safe interface is compatible with the
// original declaration, but the function retains its original bounds-safe
// interface.
void f31(int *p, int len);
void f31(int *p : count(len + 1), int len);  // expected-error {{conflicting types for 'f31'}}

void f32(int *p : bounds(p, p + len), int len);
void f32(int *p : bounds(p, p + len), int len);
void f32(int *p : bounds(p, p + len + 1), int len);  // expected-error {{conflicting types for 'f32'}}

void f33(int *p : bounds(p, p + len), int len);
void f33(int *p, int len);
// A redeclaration without a bounds-safe interface is compatible with the
// original declaration, but the function retains its original bounds-safe
// interface
void f33(int *p : bounds(p, p + len + 1), int len);  // expected-error {{conflicting types for 'f33'}}

void f34(int *p : itype(ptr<int>), int len);
void f34(int *p : count(len), int len);  // expected-error {{conflicting types for 'f34'}}

//---------------------------------------------------------------------------//
// Redeclarations of functions that have parameters that have bounds         //
// declarations must have matching declarations.                             //
//---------------------------------------------------------------------------//
void f40(array_ptr<int> p : count(len), int len);
void f40(array_ptr<int> p : count(len), int len);
void f40(array_ptr<int> p : count(len + 1), int len); // expected-error {{conflicting types for 'f40'}}

void f41(array_ptr<int> p : bounds(p, p + len), int len);
void f41(array_ptr<int> p : bounds(p, p + len), int len);
void f41(array_ptr<int> p : bounds(p, p + len + 1), int len); // expected-error {{conflicting types for 'f41'}}

// Add a parameter bounds declaration.
void f42(array_ptr<int> p, int len);
void f42(array_ptr<int> p : count(len), int len);  // expected-error {{conflicting types for 'f42'}}

// Drop a parameter bounds declaration.
void f43(array_ptr<int> p : count(len), int len);
void f43(array_ptr<int> p, int len);               // expected-error {{conflicting types for 'f43'}}

//---------------------------------------------------------------------------//
// Redeclarations of functions that have bounds-safe interfaces for returns  //
// must have matching interfaces.                                            //
//---------------------------------------------------------------------------//
int *f50(int len) : count(len);
int *f50(int len) : count(len);
int *f50(int len) : count(len + 1);  // expected-error {{conflicting types for 'f50'}}

int *f51(int len) : count(len);
// A redeclaration without bounds-safe interface is compatible with the
// original declaration, but the function retains its original bounds-safe
// interface.
int *f51(int len);
int *f51(int len) : count(len + 1);  // expected-error {{conflicting types for 'f51'}}

int *f52(int *p : bounds(p, p + len), int len) : bounds(p, p + len);
int *f52(int *p : bounds(p, p + len), int len) : bounds(p, p + len);
int *f52(int *p : bounds(p, p + len), int len) : bounds(p, p + len + 1);  // expected-error {{conflicting types for 'f52'}}

int *f53(int *p : bounds(p, p + len), int len) : bounds(p, p + len);
int *f53(int *p, int len);
// A redeclaration without a bounds-safe interface is compatible with the
// original declaration, but the function retains its original bounds-safe
// interface.
int *f53(int *p : bounds(p, p + len), int len) : bounds(p, p + len + 1);  // expected-error {{conflicting types for 'f53'}}

int *f54(int len) : itype(ptr<int>);
int *f54(int len) : count(len);  // expected-error {{conflicting types for 'f54'}}

//---------------------------------------------------------------------------//
// Redeclarations of functions that have bounds declarations for returns     //
// must have matching declarations.                                          //
//---------------------------------------------------------------------------//
array_ptr<int> f60(int len) : count(len);
array_ptr<int> f60(int len) : count(len);
array_ptr<int> f60(int len) : count(len + 1); // expected-error {{conflicting types for 'f60'}}

array_ptr<int> f61(array_ptr<int> p : count(len), int len) : bounds(p, p + len);
array_ptr<int> f61(array_ptr<int> p : count(len), int len) : bounds(p, p + len);
array_ptr<int> f61(array_ptr<int> p : count(len), int len) : bounds(p, p + len - 1); // expected-error {{conflicting types for 'f61'}}

// Add a bounds declaration.
array_ptr<int> f62(int len);
array_ptr<int> f62(int len) : count(len);   // expected-error {{conflicting types for 'f62'}}

// Drop a bounds declaration.
array_ptr<int> f63(int len) : count(len);
array_ptr<int> f63(int len);                // expected-error {{conflicting types for 'f63'}}


//---------------------------------------------------------------------------//
// Redeclarations of functions that have parameters with function pointer    //
// types that have bounds-safe interfaces must have matching bounds-safe     //
// interfaces.                                                               //
//---------------------------------------------------------------------------//

// Interface type on parameter with function pointer type.
void f70(int * fn(int *, int *));
void f70(int * (fn(int *, int *)) :
           itype(array_ptr<int> (ptr<int>, ptr<int>)));
// identical redeclaration.
void f70(int * (fn(int *, int *)) :
           itype(array_ptr<int> (ptr<int>, ptr<int>)));
// Add interface types for parameters, still valid
void f70(int * (fn(int * : itype(ptr<int>), int * : itype(ptr<int>))) :
  itype(array_ptr<int>(ptr<int>, ptr<int>)));
// return type of itype differs.
void f70(int (*fn(int *, int *)) : itype(ptr<int> (ptr<int>, ptr<int>))); // expected-error {{conflicting types for 'f70'}}
// changed interface types for parameters of function pointer
void f70(int * (fn(int * : itype(array_ptr<int>), int * : itype(array_ptr<int>)))); // expected-error {{conflicting types for 'f70'}}

// Interface type on parameters of a function pointer type
void f71(int * fn(int *, int *));
void f71(int * fn(int * : count(5), int *: count(5)));
void f71(int * fn(int * : count(6), int * : count(6))); // expected-error {{conflicting types for 'f71'}}

// Interface type on return value of a function pointer type
void f72(int * fn(int *, int *));
void f72(int * fn(int *, int *) : itype(ptr<int>));

//---------------------------------------------------------------------------//
// Redeclarations of functions that have parameters with function pointer    //
// types that have bounds declarations must have matching bounds declarations//
//---------------------------------------------------------------------------//

void fn80(void (*fnptr)(array_ptr<int> p1 : count(5)));
void fn80(void (*fnptr)(array_ptr<int> p2 : count(5)));
void fn80(void (*fnptr)(array_ptr<int> p1 : count(6)));  // expected-error {{conflicting types for 'fn80'}}

void fn81(ptr<int(array_ptr<int> mid : bounds(p1, p1 + 5), array_ptr<int> p1)> fnptr);
void fn81(ptr<int(array_ptr<int> mid : bounds(p1, p1 + 5), array_ptr<int> p1)> fnptr);
void fn81(ptr<int(array_ptr<int> mid : bounds(p1, p1 + 6), array_ptr<int> p1)> fnptr);  // expected-error {{conflicting types for 'fn81'}}

void fn82(array_ptr<int>(*fnptr)(int i, int k) : count(i));
void fn82(array_ptr<int>(*fnptr)(int j, int k) : count(j));
void fn82(array_ptr<int>(*fnptr)(int j, int k) : count(k)); // expected-error {{conflicting types for 'fn82'}}

void fn83(array_ptr<int>(*fnptr)(void) : count(5));
void fn83(array_ptr<int>(*f)(void) : count(5));
void fn83(array_ptr<int>(*f)(void) : count(6));          // expected-error {{conflicting types for 'fn83'}}



//---------------------------------------------------------------------------//
// The bounds declarations must be syntactically identical for now, modulo   //
// parameter names.                                                          //
//---------------------------------------------------------------------------//

void f80(int *p : count(len), int len);
// Rename parameters
void f80(int *p : count(mylen), int mylen);
void f80(int *r : count(i), int i);
void f80(int *p : count((len)), int len); // expected-error {{conflicting types for 'f80'}}

void f81(int *p : count(len), int len);
void f81(int *p : bounds(p, p + len), int len);  // expected-error {{conflicting types for 'f81'}}
