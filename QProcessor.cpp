#include <bits/stdc++.h>
#include "Wrapper.h"
using namespace std;

typedef long long LL;

int main()
{
	fstream ifile;
	ifile.open("Data/file.dat",ios::in );
	Permanent.readFromFile(ifile); // reading Permanent relations from "file.dat"
	ifile.close();
	// cout << Permanent.NoOfRelation() << endl;
	
	Temporary = Permanent; // Assigning to Temporary Object

	printf("\t\t\t\t***RA-Query***\n");
	while(true)
	{
		printf("> ");
		cin.getline(Query,10000000,';'); // get input terinating with spaces

		vector<string> Tokens = Tokeniser(Query);

		if(Tokens.size()==0)
		{
			cout << "Enter a valid Expression to be evaluated.\n";
			continue;
		}	
		else if(Tokens.size()==1 && Tokens[0]=="*E") // *E is exit coomand
		{
			fstream ofile;
			ofile.open("Data/file.dat", ios::out);
			Permanent.writeToFile(ofile); // writing Permanent relations to File Before Exiting
			ofile.close();
			// cout << Permanent.NoOfRelation() << endl;
			return 0;
		}
		else if(Tokens.size()==1 && Tokens[0]=="*V")
		{
			Permanent.PrintRelationNames();
			continue;
		}	
		else if(!isValidQuery(Tokens))
		{
			printf("Please refer to Readme Mannual.\n");
			continue;
		}

		if(!isRAQuery(Tokens))	
		{
			if(NRAExecutor(Tokens))
				cout << "Execution Successful.\n";
			else
			{
				printf("Please refer to Readme Mannual.\n");
				continue;
			}
		}
		else
		{
			QueriesProcesed++;

			vector<string> PostfixExpr = InfixToPostfix(Tokens);

			if(PostfixExpr[0]=="**ERROR**")
			{
				cout << "Here\n";
				printf("Invalid Syntax for Query. Improper relation name/bracket sequence.\n");
				continue;
			}	
			string S = PostfixEval( PostfixExpr );	
		}	

	}
	return 0;
}

