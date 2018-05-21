// 8086tiny: a tiny, highly functional, highly portable PC emulator/VM
// Copyright 2013-14, Adrian Cable (adrian.cable@gmail.com) - http://www.megalith.co.uk/8086tiny
//
// Revision 1.25
//
// Changed by Dmitry 'MatrixS_Master' Soloviov, 2015-2016
//
// This work is licensed under the MIT License. See included LICENSE.TXT.

#include <iostream>
#include "VM86.h"

using namespace std;

#define PRNSIZEMACRO(a) cout << "\t" #a " = " << sizeof(a) << endl;

int main(int argc, char* argv[])
{
	VM86 vm;

	cout << "Type sizes:" << endl;
	PRNSIZEMACRO(unsigned);
	PRNSIZEMACRO(unsigned short);
	PRNSIZEMACRO(unsigned char);

	cout << "Running..." << endl;
	vm.Run();
	cout << endl << "End of execution. Ret = ";
	cout << vm.GetState() << endl;

	return 0;
}
