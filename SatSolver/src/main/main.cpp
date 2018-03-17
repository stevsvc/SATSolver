// ECE 108 Lab 2 -- Sat Solver
//
// Read logical expressions in DIMACS format from files specified on the
// command line, and for each one print its name and one of "satisfiable",
// "unsatisfiable" or "tautology".

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

using namespace std;

extern "C" {
	#include "dimacs_reader.h"
}
	int numberVariables;
	int numberClauses;
	int *numberLiterals;
	int counter;
	int** a;
	
void init(int numvars, int numclauses) {
	// Callback function that gets passed to read_dimacs().
	// Indicates start of a new expression with up to numvars variables
	// and numclauses clauses.
	numberLiterals = new int[numclauses];
	numberVariables = numvars;
	numberClauses = numclauses;
	counter = 0;
	a = new int*[numberClauses];
	for(int i = 0; i < numberClauses; ++i)
	{
		a[i] = new int[numberVariables * 2];
	}
}

void add_clause(int numlits, int literals[]) {
	// Callback function that gets passed to read_dimacs().
	// The literals argument is an array of ints, numlits is its length.
	// Each literal is the index (starting from 1) of a variable.
	// Negative integers represent logical negations of variables.
	for(int i = 0; i < numlits; i++)
	{
		a[counter][i] = literals[i];
	}
	numberLiterals[counter] = numlits;
	counter++;
	
}

char *classify() {
	// Evaluate the current expression and return one of
	// "satisfiable", "unsatisfiable" or "tautology".
	int array[numberClauses];
	bool satisfied;
	bool tautology = true; 
	bool prevSatis = false;
	
	for(int c = 0; c < pow(2, numberVariables); c++)
	{
		satisfied = true;
		
		for(int i = 0; i < numberClauses; i++)
		{
			array[i] = 0;
			for(int j = 0; j < numberLiterals[i]; j++)
			{
				int varTemp1 = a[i][j];
				
				if(varTemp1 < 0)
					{
						varTemp1 *= -1;
					}
				int varTemp = (c >> (varTemp1-1)) & 1;
				if(a[i][j] < 0)
				{
					if(varTemp == 0)
						varTemp = 1;
					else
						varTemp = 0;
				}
				array[i] += varTemp;
			}
			if(array[i] == 0)
			{
				satisfied = false;
				tautology = false;
				break;
			}
			
		}
		if(satisfied == true)
		{
			prevSatis = true;
		}
		if(prevSatis == true && tautology == false)
		{
			break;
		}
	}
	if(tautology)
	{
		return (char *) "tautology";
	}
	if(prevSatis == true)
	{
		return (char *) "satisfiable";
	}

	return (char *) "unsatisfiable";
}

int main(int argc, char *argv[]) {
	int i;
	for (i = 1; i < argc; ++i) {
		read_dimacs(argv[i], init, add_clause);
		printf("%s: %s\n", argv[i], classify());
	}
	return 0;
}

