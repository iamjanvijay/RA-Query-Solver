#include <bits/stdc++.h>
#include <string>
using namespace std;

typedef long long LL;

char Query[10000007];

// Prototypes of all the functions defined in this header file
string PostfixEval(vector<string> &PostfixExpr);
vector<string> Tokeniser(char Query[]);
bool isoperator(string S);
vector<string> InfixToPostfix(vector<string> &Tokens);
bool isValidQuery(vector<string> Tokens);
bool isRAQuery(vector<string> Tokens);
bool NRAExecutor(vector<string> Tokens);
bool isUnionValid(string RelA,string RelB); 
string Union(string RelA,string RelB,string Name);
string SetDiff(string RelA,string RelB,string Name);
bool isProjectionValid(string RelName,string AttrList);
string Project(string RelName,string AttrList,string Name);
bool isCPValid(string RelA,string RelB);
string CartesianProd(string RelA,string RelB,string Name);
string Rename(string RelA,string newName);
bool isRenameValid(string RelA,string newName);
bool isSelectValid(string RelA,string Predicate);
string Select(string RelA,string Predicate,string Name);
bool PredicateProcessor(string RelA,string Predicate,vector<string> &PostfixExpr);

vector<string> Tokeniser(char Query[]) // splits the tokens out of a character string places them in String Vector
{
	vector<string> Tokens;

	LL i,j,N = strlen(Query);

	for(i=0;i<N;i++)
	{
		if(Query[i] == ' ' || Query[i] == '\t' || Query[i] == '\n')
			continue;

		string S = "";
		for(j=i;j<N;j++)
		{
			if(Query[j] == ' ' || Query[j] == '\t' || Query[j] == '\n')
				break;
			S.push_back(Query[j]); // "Appended "<<Query[j] << " in Token "<< Tokens.size() << endl;
		}
		i = j;
		Tokens.push_back(S); // "Pushing " << S << " in Tokens.\n";
	}

	return Tokens;
}

bool isoperator(string S) // return true if S is a operator
{
	return (S=="S") || (S=="P") || (S=="U") || (S=="-") || (S=="X") || (S=="R") || (S=="J") ;
	// Select / Project / Rename / Union / Set Difference / Cartesian Product / Rename  
}

vector<string> InfixToPostfix(vector<string> &Tokens)
{
	stack<string> StrStack; // A stack to carry out conversion from Infix to Postfix

	vector<string> PostfixExpr; // stores postfix expression

	LL i,N = Tokens.size();

	for(i=0;i<N;i++)
	{
		if(Tokens[i] == "]")
		{
			while(StrStack.size() != 0 && StrStack.top() != "[")
			{
				PostfixExpr.push_back( StrStack.top() );
				StrStack.pop();
			}
			if(StrStack.size() == 0) // if expression Passed in an invalid one
			{
				PostfixExpr.clear();
				PostfixExpr.push_back("**ERROR**");
				return PostfixExpr; // Return a Invalid vector as mark of error
			}
			StrStack.pop();	
		}	

		else if( Tokens[i] == "[" || isoperator(Tokens[i]) ) 
			StrStack.push(Tokens[i]);

		else // is a operand occurs (A relation/predicate/attributelist/NewName)
		{
			if( (Tokens[i].c_str())[0]=='(' && (Tokens[i].c_str())[Tokens[i].length() - 1]=='(' && ! Temporary.RelationExists(Tokens[i]) )
			{

				PostfixExpr.clear();
				PostfixExpr.push_back("**ERROR**");
				cout << "HERE !\n";
				return PostfixExpr; // Return a Invalid vector as mark of error
			}	
			PostfixExpr.push_back( Tokens[i] );
		}	
	}

	// for(i=0;i<PostfixExpr.size();i++)
	// 	cout << PostfixExpr[i] <<endl;
	return PostfixExpr;
}

bool isValidQuery(vector<string> Tokens) // checks if the queries to be processed are Valid
{
	LL i,j,N = Tokens.size();

	if(Tokens[0]=="*C") // check Validity for CREATE Query
	{
		
		if( N<8 || N%4!=0 || Tokens[1]!="[")
		{
			if(N<8)
				cout << "Too few tokens for Creation query.";
			cout << "Spaces are not allowed in Relation/Attribute name. Use proper spaces & braces[].\n";
			return false;
		}
		for(i=3;i<N;i+=4)
		{
			if(Tokens[i]!="]")
			{
				cout << "Spaces are not allowed in Relation/Attribute name. Use proper spaces & braces.\n";
				return false;
			}
		}

		for(i=4;i<N;i+=4)
		{
			if(Tokens[i]!="[")
			{
				cout << "Spaces are not allowed in Relation/Attribute name. Use proper spaces & braces.\n";
				return false;
			}
		}

		for(i=6 ;i<N;i+=4)
		{
			if(Tokens[i]!="INT" && Tokens[i]!="STRING")
			{
				cout << "Use proper Datatypes.[INT/STRING].\n";
				return false;
			}	
		}	

		if(	Temporary.RelationExists(Tokens[2]) )
		{
			cout << "Relation with this name already exists in Temporary DB.\n";
			return false;
		}

		for(i=5;i<N;i+=4)
		{
			for(j=i+4;j<N;j+=4)
			{
				if(Tokens[i]==Tokens[j])
				{
					cout << "Relations can't have Attributes with same name.\n";
				}
			}	
		}	

		return true;
	}
	else if(Tokens[0]=="*I") // check Validity for Insert Query
	{
		if(N<7 || Tokens[1]!="[" || Tokens[3]!="]" || Tokens[4]!="[" || Tokens[N-1]!="]" )
		{
			if(N<7)
				cout << "Too few tokens for Insertion query.";
			cout << "Spaces are not allowed in Attribute Values. Use proper spaces & braces[].\n";
		}
		string RelName = Tokens[2];

		if(!Temporary.RelationExists(Tokens[2]))
		{
			cout << "Relation with name \"" << Tokens[2] << "\" doesn't exist in Temporary Database.\n";
			return false;
		}
		Relation R = Temporary.RelationRef(Tokens[2]);

		if(R.NoOfAttributes()!=N-6)
		{
			cout << "Relation \"" << Tokens[2] << "\" requires "<< R.NoOfAttributes() <<" no. of attributes. Use proper spaces & braces[].\n";
			return false;
		}
		return true;
	}
	else if(Tokens[0]=="*S") // To Save This Relation 
	{
		if( N<4 || Tokens[1]!="[" || Tokens[N-1]!="]" )
		{
			if(N<4)
				cout << "Too few tokens for Save Query.";
			cout <<  "Use proper spaces & braces[].\n";
			return false;
		}
		return true;
	}
	else
	{
		if( Tokens[0] != "[" )
		{
			cout << "RA Queries must begin with \"[\".";
			return false;
		}
	}
	return true;	
}

bool isRAQuery(vector<string> Tokens) // return true if its a RA query / false is its create|insert Query
{
	if( Tokens[0]=="*C" || Tokens[0]=="*I" || Tokens[0]=="*S")
		return false;
	return true;
} 
 
bool NRAExecutor(vector<string> Tokens) // Non RA Query Executor Function
{
	if( Tokens[0]=="*C" ) // Creation Of new Relation
	{
		string Name = Tokens[2];
		vector<string> AttributeName;
		vector<string> AttributeType;

		LL i,j; LL N = Tokens.size();

		for(i=5;i<N;i+=4)
		{
			AttributeName.push_back(Tokens[i]);
		}

		for(i=6;i<N;i+=4)
		{
			AttributeType.push_back(Tokens[i]);
		}		

		Relation R(Name,AttributeName,AttributeType);

		Temporary.insertRelation(R);
		
	}	
	else if( Tokens[0]=="*I" ) // Insertion of a tuple in New Relation
	{
		Relation &R = Temporary.RelationRef(Tokens[2]);
		vector<string> Tuple;

		LL i,N = Tokens.size();

		for(i=5;i<N-1;i++)
		{
			Tuple.push_back(Tokens[i]);
		}	

		if( !R.insertTuple(Tuple) )
		{
			cout<< "Use Attribute Values with proper datatypes only.\n";
			return false;
		}
		//R.printRelation();
	}
	else if( Tokens[0]=="*S" ) // Saving the current relation in permanent database
	{
			Tokens.erase(Tokens.begin());
			QueriesProcesed++;
		
			vector<string> PostfixExpr = InfixToPostfix(Tokens); // evaluate the following expression
			string Name = PostfixEval( PostfixExpr );

			if(Name!= "**ERROR**")  // if query was executed successfully
			{
				Relation RRelA = Temporary.RelationRef(Name); // returns you the refrence of relation from Temprorary DB
				if( Permanent.RelationExists(Name) ) // check if relation already exists in permanent DB
				{
					cout <<"Relation with name \""<<Name<< "\" already exists in database.\nOverwrite ? [1/0]\n";
					int Overwrite = -1;
					scanf("%d",&Overwrite);
					while(Overwrite!=0 && Overwrite!=1)
					{
						cout << "Enter [1/0] only. '1' to overwite. '0' to Cancel\n";
						string S;
						cin >> S;
						if(isInteger(S))
							Overwrite = stoll(S);
					}
					if( Overwrite == 0)
						return true;
					else
					{
						Relation &R = Permanent.RelationRef(Name);
						R = Temporary.RelationRef(Name);
						cout << "Overwritten Successfully!\n";
						return true;
					}			
				}
				else
				{
					Relation RRel = Temporary.RelationRef(Name);
					Permanent.insertRelation(RRel);
					cout << "Saved Successfully!\n";
					return true;
				}	
			}
			else
			{
				printf("Unable to Save. Query Execution Unsuccessful.\n");
				return false;
			}	
	
	}			
		

}



bool isUnionValid(string RelA,string RelB) // Function to Check if two relations are Union-compatible
{
	Relation RRelA,RRelB; // Assumes two relations RelA & RelB 

	if( Temporary.RelationExists(RelA) ) // Get relation RelA object from Temporary DB
	{
		RRelA = Temporary.RelationRef(RelA);
	}
	else
	{
		cout << "Relation with \""<<RelA<<"\" doesn't exist in Temporary Database.\n";
		return false;
	}

	if( Temporary.RelationExists(RelB) ) // Get relation RelB object from Temprorary DB
	{
		RRelB = Temporary.RelationRef(RelB);
	}
	else
	{
		cout << "Relation with \""<<RelB<<"\" doesn't exist in Temporary Database.\n";		
		return false;
	}	

	LL N1 = RRelA.NoOfAttributes(), N2 = RRelB.NoOfAttributes(); 
	
	if( N1 != N2 ) // if arity doesn't match
	{
		cout << "Union operation requires relations with same arity.\n";
		return false;
	}

	LL i;

	for(i=0;i<N1;i++) //checking for domain-match of Relations
	{
		if( RRelA.AttrTypeAtInd(i) != RRelB.AttrTypeAtInd(i))
		{
			cout << "Union operation requires relations with same DomainType for corresponding Attributes.\n";
			return false;
		}
	}

	return true;	
}


string SetDiff(string RelA,string RelB,string Name) // Function to perform set difference
{
	Relation RRelA,RRelB; // Assumes two relations RelA & RelB 

	RRelA = Temporary.RelationRef(RelA);  RRelB = Temporary.RelationRef(RelB);
	LL N1 = RRelA.NoOfAttributes(), N2 = RRelB.NoOfAttributes(); 

	vector<string> AttributeName = RRelA.AttributeNameVector();
	vector<string> AttributeType = RRelA.AttributeTypeVector();

	Relation SetDiffRel(Name,AttributeName,AttributeType);

	set< vector<string> >::iterator itr;
	
	for(itr = RRelA.InstBegin();itr!=RRelA.InstEnd();++itr) // Insert tuples from RelA
	{
		vector<string> T = (*itr);

		SetDiffRel.insertTuple( T );
	}

	for(itr = RRelB.InstBegin();itr!=RRelB.InstEnd();++itr) // Erase tuples of RelB
	{
		vector<string> T = (*itr);

		SetDiffRel.eraseTuple( T );
	}

	Temporary.insertRelation(SetDiffRel); // Save the refrence of this relation in Temporary Database

	return 	SetDiffRel.RelationName();	// return the SetDiff Relation Name
}

bool isProjectionValid(string RelName,string AttrList) // checks if prjection Of Relname on Attribute list is a valid one
{
	LL i,j,N = strlen( AttrList.c_str() );

	vector<string> AttrVector; // Vector of Attribute Name

	if(  (AttrList.c_str())[0]!= '(' || (AttrList.c_str())[N-1]!= ')' )
	{
		cout << "Attribute list must begin with '(' & end with ')' with no spaces & '|' as seprator.\n";
		return false;
	}	

	for(i=1;i<N-1;i++)
	{
		if((AttrList.c_str())[i] == '|')
			continue;

		string S = "";
		for(j=i;j<N-1;j++)
		{
			if((AttrList.c_str())[j] == '|')
				break;
			S.push_back((AttrList.c_str())[j]); // "Appended "<<Query[j] << " in Token "<< Tokens.size() << endl;
		}
		i = j;
		if( S!= "")
			AttrVector.push_back(S); // "Pushing " << S << " in Tokens.\n";
		else 
		{
			cout << "Use non-empty proper Attribute names in Attribute list.\n";
			return false;
		}	
	}

	if(AttrVector.size() == 0)
	{
		cout << "Atleast one argument must be specified in list for projection.\n";
		return false;
	}

	if( Temporary.RelationExists(RelName) )
	{
		// cout << "Here\n";
		Relation &R = Temporary.RelationRef(RelName);
		LL N2 = AttrVector.size();
		bool flag = true;
		for(i = 0; i<N2 ; i++)
		{
			if(R.AttributeExists(AttrVector[i]) == -1)
			{
				cout << "Attribute with name \""<<AttrVector[i]<<"\" doesn't exist in \""<<RelName<<"\" Relation.\n";
				flag = false;
				break;
			}
		}	

		return flag;		
	}
	else
	{	
		cout << "Relation with \""<<RelName<<"\" doesn't exist in Temporary Database.\n";
		return false;
	}
}

string Project(string RelName,string AttrList,string Name) // if projection is valid carry out projection and return its name
{
	LL i,j,N = strlen( AttrList.c_str() );

	vector<string> AttrVector; // Vector of Attribute Name

	for(i=1;i<N-1;i++) // for getting the attributes (to be projected) out
	{
		if((AttrList.c_str())[i] == '|')
			continue;

		string S = "";
		for(j=i;j<N-1;j++)
		{
			if((AttrList.c_str())[j] == '|')
				break;
			S.push_back((AttrList.c_str())[j]); // "Appended "<<Query[j] << " in Token "<< Tokens.size() << endl;
		}
		i = j;
		AttrVector.push_back(S); // "Pushing " << S << " in Tokens.\n";
	}

	Relation &R = Temporary.RelationRef(RelName);
	LL N2 = AttrVector.size();

	LL *Indices = new LL [ N2 ]; // To store the Indices order of Attributes to be projected as in Original Relation

	for(i=0;i<N2;i++)
		Indices[i] = R.AttributeExists(AttrVector[i]); // storing the indices order

	vector<string> AttributeType;

	for(i=0;i<N2;i++)
		AttributeType.push_back( R.AttrTypeAtInd( Indices[i] ) );

	Relation ProjRel(Name,AttrVector,AttributeType); // creating new relation as Projected relation

	set< vector<string> >::iterator itr;
	
	for(itr = R.InstBegin();itr!=R.InstEnd();++itr) // Adding new Tuple in ProjRel corresponding to each tuple in Parent Relation
	{
		vector<string> T = (*itr);
		vector<string> newTuple;
		for(i=0;i<N2;i++)
		{
			newTuple.push_back( T[Indices[i]]  );
		}
		ProjRel.insertTuple( newTuple );
	}			
	delete Indices; // deallocate the space for indices

	Temporary.insertRelation(ProjRel); // Saving the refrence of projected relation in Temporary Database

	return ProjRel.RelationName();	// return the name of projected relation
}

bool isCPValid(string RelA,string RelB)
{
	if( Temporary.RelationExists(RelA) && Temporary.RelationExists(RelB) ) // Return true if both relations exist in Temporary Database
	{
		return true;
	}

	cout << "Relation with \""<< ((!Temporary.RelationExists(RelA))?RelA:RelB) <<"\" doesn't exist in Temporary Database.\n";
	return false;	// else retun false
}

string CartesianProd(string RelA,string RelB,string Name) // Function to perform Cartesian Product
{
	Relation RRelA,RRelB; // Assumes two relations RelA & RelB 

	RRelA = Temporary.RelationRef(RelA);  RRelB = Temporary.RelationRef(RelB);
	LL i,j,N1 = RRelA.NoOfAttributes(), N2 = RRelB.NoOfAttributes(); 

	vector<string> AttributeNameA = RRelA.AttributeNameVector(); // RelA attribute Name
	vector<string> AttributeTypeA = RRelA.AttributeTypeVector(); // RelA attribute Type

	vector<string> AttributeNameB = RRelB.AttributeNameVector(); // RelB attribute Name
	vector<string> AttributeTypeB = RRelB.AttributeTypeVector(); // RelB attribute Type

	for(i=0;i<N1;i++) // Rename Attributes if any have same name
	{
		for(j=0;j<N2;j++)
		{
			if( AttributeNameA[i] == AttributeNameB[i]) // if Attribute names in two relations are same
			{
				AttributeNameA[i] = RelA + "." + AttributeNameA[i]; // giving appropriate name to Attributes of relation A
				AttributeNameB[i] = RelB + "." + AttributeNameB[i]; // giving appropriate name to Attributes of relation B
			}	
		}	
	}	

	for(i=0;i<N2;i++)
	{
		AttributeNameA.push_back( AttributeNameB[i] ); // Append Attribute Name from RelB
		AttributeTypeA.push_back( AttributeTypeB[i] ); // Append Attribute Type from RelA
	}		

	Relation CartProdRel(Name,AttributeNameA,AttributeTypeA); // create a new relation

	set< vector<string> >::iterator itrA;
	set< vector<string> >::iterator itrB;
	
	for(itrA = RRelA.InstBegin();itrA!=RRelA.InstEnd();++itrA) // Insert tuples from RelA
	{
		vector<string> TA = (*itrA);

		for(itrB = RRelB.InstBegin();itrB!=RRelB.InstEnd();++itrB) // Erase tuples of RelB
		{
			vector<string> TB = (*itrB);
			vector<string> TCP = TA;

			for(i=0;i<N2;i++) // glue each tuple of RelA with every tuple of RelB
				TCP.push_back(TB[i]);

			CartProdRel.insertTuple(TCP);
		}
	}

	Temporary.insertRelation(CartProdRel); // Save the refrence of this relation in Temporary Database

	return 	CartProdRel.RelationName();	// return the Cartesian Product Relation Name
}

string NatJoin(string RelA,string RelB,string Name) // Function to perform Natural Join
{
	Relation RRelA,RRelB; // Assumes two relations RelA & RelB 

	RRelA = Temporary.RelationRef(RelA);  RRelB = Temporary.RelationRef(RelB);
	LL i,j,N1 = RRelA.NoOfAttributes(), N2 = RRelB.NoOfAttributes(); 

	vector<string> AttributeNameA = RRelA.AttributeNameVector(); // RelA attribute Name
	vector<string> AttributeTypeA = RRelA.AttributeTypeVector(); // RelA attribute Type

	vector<string> AttributeNameB = RRelB.AttributeNameVector(); // RelB attribute Name
	vector<string> AttributeTypeB = RRelB.AttributeTypeVector(); // RelB attribute Type

	vector<LL> IndCommonA;
	vector<LL> IndCommonB;

	for(i=0;i<N1;i++) // Rename Attributes if any have same name
	{
		for(j=0;j<N2;j++)
		{
			if( (AttributeNameA[i] == AttributeNameB[j]) && (AttributeTypeA[i] == AttributeTypeB[j])) // if Attribute names in two relations are same
			{
				// cout << AttributeNameA[i] << " " << AttributeNameB[j]<<endl;
				IndCommonA.push_back(i);
				IndCommonB.push_back(j);
			}	
		}	
	}

	if( IndCommonA.size() == 0 ) // if there are no common attrbutes
	{
		return CartesianProd(RelA,RelB,Name); 
	}	
	else
	{
		LL NCAttr = IndCommonB.size();
		// cout << NCAttr;
		for(i=0;i<N2;i++)
		{
			bool flag = true;
			for(j=0;j<NCAttr;j++)
			{
				if(IndCommonB[j]==i)
				{
					flag = false;
					break;
				}
			}
			if(flag)	
			{
				// cout << AttributeNameB[i] << endl;
				AttributeNameA.push_back( AttributeNameB[i] ); // Append Attribute Name from RelB
				AttributeTypeA.push_back( AttributeTypeB[i] ); // Append Attribute Type from RelB
			}
		}		

		Relation NatJoinRel(Name,AttributeNameA,AttributeTypeA); // create a new relation

		set< vector<string> >::iterator itrA;
		set< vector<string> >::iterator itrB;
	
		for(itrA = RRelA.InstBegin();itrA!=RRelA.InstEnd();++itrA) // Insert tuples from RelA
		{
			vector<string> TA = (*itrA);

			for(itrB = RRelB.InstBegin();itrB!=RRelB.InstEnd();++itrB) // Erase tuples of RelB
			{
				vector<string> TB = (*itrB);
				vector<string> TNT = TA;

				bool flag = true;

				for(i=0;i<NCAttr;i++)
				{
					if(TA[IndCommonA[i]] != TB[IndCommonB[i]])
					{
						flag = false;
						break;
					}	
				}	
				
				if(flag)
				{
					for(i=0;i<N2;i++) // glue each tuple of RelA with every tuple of RelB
					{
						bool flag2 = true;
						for(j=0;j<NCAttr;j++)
						{
							if(IndCommonB[j]==i)
							{
								flag2 = false;
								break;
							}
						}
						if(flag2)	
						{						
							TNT.push_back(TB[i]);
						}
					}

					NatJoinRel.insertTuple(TNT);
				}
			}
		}

		Temporary.insertRelation(NatJoinRel); // Save the refrence of this relation in Temporary Database

		return 	NatJoinRel.RelationName();	// return the Cartesian Product Relation Name
	}
}


bool isRenameValid(string RelA,string newName)
{
	if( !Temporary.RelationExists(RelA) ) //  Return false if relation doesn't exist in Temporary Database
	{
		cout << "Relation with \""<< RelA<<"\" doesn't exist in Temporary Database.\n";
		return false;
	}

	LL i,j,N = strlen( newName.c_str() );

	Relation RRelA = Temporary.RelationRef(RelA); 

	vector<string> newNameVector; // Vector of new Name

	if(  (newName.c_str())[0]!= '(' || (newName.c_str())[N-1]!= ')' )
	{
		cout << "new Name list must begin with '(' & end with ')' with no spaces & '|' as seprator.\n";
		return false;
	}		

	for(i=1;i<N-1;i++) // for getting the attributes (to be the new attribute names) out
	{
		if((newName.c_str())[i] == '|')
			continue;

		string S = "";
		for(j=i;j<N-1;j++)
		{
			if((newName.c_str())[j] == '|')
				break;
			S.push_back((newName.c_str())[j]); // "Appended "<<Query[j] << " in Token "<< Tokens.size() << endl;
		}
		i = j;
		newNameVector.push_back(S); // "Pushing " << S << " in Tokens.\n";
	}

	if( newNameVector.size()==1 || newNameVector.size() == 1 + RRelA.NoOfAttributes() )
		return true;
	else
	{
		cout << "Specify either only Relation name or Relation name along with all attribute names.\n";
		cout << "1 or "<< 1 + RRelA.NoOfAttributes() <<" must be specified.\n";
		return false;
	}	
}

string Rename(string RelA,string newName)
{
	Relation &RRelA = Temporary.RelationRef(RelA); // get the refrence of relation A

	LL i,j,N = strlen( newName.c_str() );

	vector<string> newNameVector; // Vector of new Name	

	for(i=1;i<N-1;i++) // for getting the attributes (to be the new attribute names) out
	{
		if((newName.c_str())[i] == '|')
			continue;

		string S = "";
		for(j=i;j<N-1;j++)
		{
			if((newName.c_str())[j] == '|')
				break;
			S.push_back((newName.c_str())[j]); // "Appended "<<Query[j] << " in Token "<< Tokens.size() << endl;
		}
		i = j;
		newNameVector.push_back(S); // "Pushing " << S << " in Tokens.\n";
	}

	if( newNameVector.size() == 1 ) // if only relation name is to be changed
	{
		RRelA.renameRelation(newNameVector[0]);
	}
	else // if both attribute names & relation name is to be updated
	{
		RRelA.renameRelation(newNameVector[0]);
		newNameVector.erase(newNameVector.begin());
		RRelA.renameAttributes(newNameVector);
	}

	return 	RRelA.RelationName();	

}


string Union(string RelA,string RelB,string Name) // Function to perform Union of two Relations
{
	Relation RRelA,RRelB; // Assumes two relations RelA & RelB 
	RRelA = Temporary.RelationRef(RelA);  RRelB = Temporary.RelationRef(RelB);
	LL N1 = RRelA.NoOfAttributes(), N2 = RRelB.NoOfAttributes(); 

	vector<string> AttributeName = RRelA.AttributeNameVector();
	vector<string> AttributeType = RRelA.AttributeTypeVector();

	Relation UnionRel(Name,AttributeName,AttributeType); // Create a new relation to perform Union

	set< vector<string> >::iterator itr;
	
	for(itr = RRelA.InstBegin();itr!=RRelA.InstEnd();++itr) // Insert tuples from RelA
	{
		vector<string> T = (*itr);

		UnionRel.insertTuple( T );
	}

	for(itr = RRelB.InstBegin();itr!=RRelB.InstEnd();++itr) // Insert tuples from RelB
	{
		vector<string> T = (*itr);

		UnionRel.insertTuple( T );
	}

	Temporary.insertRelation(UnionRel); // Save the refrence of this relation in Temporary Database

	return 	UnionRel.RelationName();	// return the Union Relation Name

}

// Functions for select operation

bool isSelectValid(string RelA,string Predicate)
{
	if( !Temporary.RelationExists(RelA) ) // Return false if relation doesn't exist in Temporary Database
	{
		cout << "Relation with \""<< RelA<<"\" doesn't exist in Temporary Database.\n";
		return false;
	}

	vector<string> PostfixExpr;

	if(!PredicateProcessor(RelA,Predicate,PostfixExpr))
	{
		return false;
	}

	// check if attribute names exist in relation
	LL i,N = PostfixExpr.size();
	
	Relation &RRelA = Temporary.RelationRef(RelA);

	for(i=0;i<N;i++)
	{
		if( PostfixExpr[i]!="!" && PostfixExpr[i]!="&" && PostfixExpr[i]!="|" && PostfixExpr[i]!=">=" && PostfixExpr[i]!=">>" && PostfixExpr[i]!="<=" && PostfixExpr[i]!="<<" && PostfixExpr[i]!="==" && PostfixExpr[i]!="<>")
		{
			if( (PostfixExpr[i].length() <= 2) || (! ( ((PostfixExpr[i]).c_str())[PostfixExpr[i].length() -1] == 'C' && ((PostfixExpr[i]).c_str())[PostfixExpr[i].length() -2] == '@' ) ) ) // if not a const must be an attribute // must be a attribute
			{
				if(RRelA.AttributeExists( PostfixExpr[i] ) == -1) // a attribute doesn't exists
				{
					//cout << "* " <<PostfixExpr[i] << endl;
					cout << "Attribute with name \""<< PostfixExpr[i] <<"\" doesn't exist in relation \""<< RelA << "\".\n";
					return false;
				}
			}	
		}	
	} 	

	return true;
}

bool isBinaryString(string S) // if a string stores a binary value
{
	return (S=="1@b") || (S=="0@b");
}

bool stringToBool(string S) // return bool value stored in string
{
	return (S.c_str())[0]=='1';
}

string boolToString(bool flag) // returns string for a boolean Value
{
	if(flag)
		return "1@b";
	return "0@b";
}

bool isAttributeName(string S)
{
	if( (S.length() <= 2) || (! ( ((S).c_str())[S.length() -1] == 'C' && ((S).c_str())[S.length() -2] == '@' ) ) ) // if not a const must be an attribute // must be a attribute
	{
		return true;
	}
	return false;
}

string BinaryOperator(string A,string B,string Op) // performs the binary operations & | 
{
	if(Op == "&")
	{
		return boolToString( stringToBool(A) && stringToBool(B) );
	}
	else
	{
		return boolToString( stringToBool(A) || stringToBool(B) );
	}	
}

string RelationalOperator(string A,string B,string Operator,bool isInteger)
{
	if(isInteger) // if the operands are integers Apply Integral comparisions
	{
		LL IntA = stoll(A); LL IntB = stoll(B);
		if(Operator == ">>")
			return boolToString(IntA > IntB);
		else if(Operator == ">=")
			return boolToString(IntA >= IntB);
		else if(Operator == "==")
			return boolToString(IntA == IntB);
		else if(Operator == "<<")
			return boolToString(IntA < IntB);
		else if(Operator == "<=")
			return boolToString(IntA <= IntB);
		else if(Operator == "<>")
			return boolToString(IntA != IntB); 			 			 			 			 
	}
	else // else if they are of string type Apply String comparisions as per Lexiographical Ordering
	{
		if(Operator == ">>")
			return boolToString(A > B);
		else if(Operator == ">=")
			return boolToString(A >= B);
		else if(Operator == "==")
			return boolToString(A == B);
		else if(Operator == "<<")
			return boolToString(A < B);
		else if(Operator == "<=")
			return boolToString(A <= B);
		else if(Operator == "<>")
			return boolToString(A != B); 		
	}	
}

string AttrValTuple(string RelA,vector<string> Tuple,string AttrName,char &type) // retruns the value of an attribute for particular tuple
{
	if(!isAttributeName(AttrName)) // if it is a constant
	{
		type = 'C'; // type of operand is constant
		return AttrName.substr(0,AttrName.length() - 2);
	}
	else // if its an attribute Name
	{
		Relation &RRelA = Temporary.RelationRef(RelA);
		if( "INT" == RRelA.AttrTypeAtInd( RRelA.AttributeExists(AttrName) ))
			type = 'I'; // type of operand is Integer
		else
			type = 'S'; // type of operand is String
		return Tuple[RRelA.AttributeExists(AttrName)];
	}	
}

bool checkCompatibility(string A,char typeA,string B,char typeB) // to check if integer attibutes are being compared with integers only
{
	if(typeA=='I' && typeB=='C')
	{
		if(!isInteger(B))
			return false;
	}	
	if(typeB=='I' && typeA=='C')
	{
		if(!isInteger(A))
			return false;		
	}
	if(typeB=='I' && typeA=='S')
	{
		return false;
	}
	if(typeB=='S' && typeA=='I')
	{
		return false;
	}	
	return true;
}

bool DataTypeValidation(string RelA,vector<string> Tuple,vector<string> PostfixExpr) // checks the validity of predicate expression to be evaluated assumes arbitary tuple to do so
{
	stack<string> StrStack;
	LL i,N = PostfixExpr.size();

	string A,B,C;

	Relation &RRelA = Temporary.RelationRef(RelA); // get refrence of parent relation	

	for(i=0;i<N;i++)
	{
		// if not a operator
		if(!(PostfixExpr[i]==">>" || PostfixExpr[i]=="<>" || PostfixExpr[i]==">=" || PostfixExpr[i]=="<<" || PostfixExpr[i]=="<=" || PostfixExpr[i]=="==" || PostfixExpr[i]=="|" || PostfixExpr[i]=="&" || PostfixExpr[i]=="!") )
		{
			StrStack.push(PostfixExpr[i]);
		}
		else if(PostfixExpr[i]=="!") // if a unary operator
		{
			if(StrStack.size() == 0)
			{
				printf("Improper Selection Predicate to be Evaluated.\n");
				return false;
			}			
			string A = StrStack.top(); StrStack.pop();

			if(isBinaryString(A)==false)
			{
				printf("Improper Selection Predicate to be Evaluated.\n");	
				return false; // invalid expression
			}	
			else
			{
				string C;

				if((A.c_str())[0]=='1')
					C.push_back('0');
				else
					C.push_back('1');

				C.push_back('@');C.push_back('b');

				StrStack.push(C);
			}	
		}	
		else // if a binary operator
		{
			if(StrStack.size() == 0)
			{
				printf("Improper Selection Predicate to be Evaluated.\n");
				return false;
			}			
			string B = StrStack.top(); StrStack.pop();
			
			if(StrStack.size() == 0)
			{
				printf("Improper Selection Predicate to be Evaluated.\n");
				return false;
			}			
			string A = StrStack.top(); StrStack.pop();

			if(PostfixExpr[i]=="&" || PostfixExpr[i]=="|" ) // if logical operator
			{
				if(isBinaryString(A)==false || isBinaryString(B)==false)
				{
					printf("Improper Selection Predicate to be Evaluated.\n");
					return false; // invalid expression
				}	
				else
				{
					string C = BinaryOperator(A,B,PostfixExpr[i]);
					StrStack.push(C);
				}

			}
			else // if a comparision operator
			{
				char typeA; char typeB;
				string AOperd = AttrValTuple(RelA,Tuple,A,typeA);
				string BOperd = AttrValTuple(RelA,Tuple,B,typeB);

				if( !checkCompatibility(AOperd,typeA,BOperd,typeB) )
				{
					cout << "Integer must be compared with an Integer only.\n";
					return false;
				}

				string C = RelationalOperator(AOperd,BOperd,PostfixExpr[i],typeA=='I'||typeB=='I');
				StrStack.push(C);
			}		
		}	
	}
	if(StrStack.size()!=1 || !isBinaryString(StrStack.top()) )
	{
		printf("Improper Selection Predicate to be Evaluated.\n");
		return false;
	}
	return true;		
}

bool isTruePred(string RelA,vector<string> Tuple,vector<string> PostfixExpr) // return true if the Tuple satisfies the predicate
{
	stack<string> StrStack;
	LL i,N = PostfixExpr.size();

	string A,B,C;

	Relation &RRelA = Temporary.RelationRef(RelA); // get refrence of parent relation	

	for(i=0;i<N;i++)
	{
		// if not a operator
		if(!(PostfixExpr[i]==">>" || PostfixExpr[i]=="<>" || PostfixExpr[i]==">=" || PostfixExpr[i]=="<<" || PostfixExpr[i]=="<=" || PostfixExpr[i]=="==" || PostfixExpr[i]=="|" || PostfixExpr[i]=="&" || PostfixExpr[i]=="!") )
		{
			StrStack.push(PostfixExpr[i]);
		}
		else if(PostfixExpr[i]=="!") // if a unary operator
		{
		
			string A = StrStack.top(); StrStack.pop();

			string C;

			if((A.c_str())[0]=='1')
				C.push_back('0');
			else
				C.push_back('1');

			C.push_back('@'); C.push_back('b');

			StrStack.push(C);
		}	
		else // if a binary operator
		{		
			string B = StrStack.top(); StrStack.pop();
		
			string A = StrStack.top(); StrStack.pop();

			if(PostfixExpr[i]=="&" || PostfixExpr[i]=="|" ) // if logical operator
			{
				string C = BinaryOperator(A,B,PostfixExpr[i]);
				StrStack.push(C);
			}
			else // if a comparision operator
			{
				char typeA; char typeB;
				string AOperd = AttrValTuple(RelA,Tuple,A,typeA);
				string BOperd = AttrValTuple(RelA,Tuple,B,typeB);

				string C = RelationalOperator(AOperd,BOperd,PostfixExpr[i],typeA=='I'||typeB=='I');
				StrStack.push(C);
			}		
		}	
	}

	return stringToBool(StrStack.top());		
}


string Select(string RelA,string Predicate,string Name)
{
	vector<string> PostfixExpr;
	PredicateProcessor(RelA,Predicate,PostfixExpr);

	Relation &RRelA = Temporary.RelationRef(RelA); // get refrence of parent relation

	vector<string> AttributeName = RRelA.AttributeNameVector();
	vector<string> AttributeType = RRelA.AttributeTypeVector();

	Relation SelectRel(Name,AttributeName,AttributeType); // Create a new relation to perform Selection

	set< vector<string> >::iterator itr;

	itr = RRelA.InstBegin();

	if( DataTypeValidation(RelA,(*itr),PostfixExpr) == false )
	{
		return "**ERROR**";
	}
	
	for(itr = RRelA.InstBegin();itr!=RRelA.InstEnd();++itr) // Insert tuples from RelA
	{
		vector<string> T = (*itr);

		if( isTruePred(RelA,T,PostfixExpr) )
			SelectRel.insertTuple( T );
	}

	Temporary.insertRelation(SelectRel); // Save the refrence of this relation in Temporary Database

	return 	SelectRel.RelationName();	// return the Union Relation Name		

}

bool PredicateProcessor(string RelA,string Predicate,vector<string> &PostfixExpr)
{
	vector<string> Tokens;

	LL i,j,N = Predicate.length();

	char *CPredicate = new char [N+2];
	strcpy(CPredicate ,Predicate.c_str());

	// tokenisation
	for(i=0;i<N;i++) // performing tokenisation of predicate
	{

		if( (CPredicate)[i] == '>'  || (CPredicate)[i] == '<' || (CPredicate)[i] == '=' )
		{
			string S = "";
			S.push_back((CPredicate)[i]);	
			S.push_back((CPredicate)[i+1]);

			if( S!="<<" && S!="<=" && S!=">>" && S!=">=" && S!="==" && S!="<>" )
			{
				cout << "Only '<<' '>>' '<=' '>=' '<>' '==' are allowed relational operators.\n";
				return false;
			}

			Tokens.push_back(S);
			i = i+1;
		}	
		else if( (CPredicate)[i] == '(' || (CPredicate)[i] == ')' || (CPredicate)[i] == '|' || (CPredicate)[i] == '&' || (CPredicate)[i] == '!')
		{
			string S = "";
			S.push_back(CPredicate[i]);
			Tokens.push_back(S);
		}
		else
		{
			string S = "";
			for(j=i;j<N;j++)
			{
				if((CPredicate)[j] == '(' || (CPredicate)[j] == ')' || (CPredicate)[j] == '|' || (CPredicate)[j] == '&' || (CPredicate)[j] == '!' || (CPredicate)[j] == '>'  || (CPredicate)[j] == '<' || (CPredicate)[j] == '=' )
					break;
				S.push_back(CPredicate[j]); // "Appended "<<Query[j] << " in Token "<< Tokens.size() << endl;
			}
			i = j-1;
			Tokens.push_back(S); // "Pushing " << S << " in Tokens.\n";
		}
	}

	delete CPredicate; // Predicate has been tokenised till now 

	// conversion to POSTFIX Predicate for one pass evaluation of each tuple
	stack<string> StrStack; // A stack to carry out conversion from Infix to Postfix

	N = Tokens.size();

	bool invalidExpr = false;

	for(i=0;i<N;i++)
	{
		if(Tokens[i] == ")")
		{
			while(StrStack.size() != 0 && StrStack.top() != "(")
			{
				PostfixExpr.push_back( StrStack.top() );
				StrStack.pop();
			}
			if(StrStack.size() == 0) // if expression Passed in an invalid one
			{
				PostfixExpr.clear();
				invalidExpr = true;
				break;
			}
			StrStack.pop();	
		}	

		else if( Tokens[i] == "(" || Tokens[i] == "!" || Tokens[i] == "|" || Tokens[i] == "&" ) 
			StrStack.push(Tokens[i]);
		else if( (Tokens)[i] == ">>" || (Tokens)[i] == "<<" || (Tokens)[i] == "==" || (Tokens)[i] == ">=" || (Tokens)[i] == "<=" || (Tokens)[i] == "<>" )
			StrStack.push(Tokens[i]);
		else // is a operand occurs (A test condition)
		{
			PostfixExpr.push_back( Tokens[i] );
		}	
	}

	if(invalidExpr == true)
	{
		cout << "Pridicate Expression is Invallid for 'S' operation. Refer to Readme Mannual.\n";
		return false;
	}

	return true; // predicate was processed successfully
}

string PostfixEval(vector<string> &PostfixExpr) // Evaluatioes the Postfix Expression
{
	stack<string> StrStack; // A stack to carry out evaluation

	LL i,N = PostfixExpr.size();
	LL  currOperation = 0;
	string A,B,C;

	for(i=0;i<N;i++)
	{
		if( !isoperator( PostfixExpr[i] ) )
		{
			StrStack.push( PostfixExpr[i] );
		}
		else
		{
			currOperation++;

			if( PostfixExpr[i]=="P" ) // if Projection is to be performed
			{
				if(StrStack.size() == 0)
				{
					printf("Improper Expression to be Evaluated.\n");
					return "**ERROR**";
				}
				B = StrStack.top(); StrStack.pop();
				if(StrStack.size() == 0)
				{
					printf("Improper Expression to be Evaluated.\n");
					return "**ERROR**";	
				}			
				A = StrStack.top(); StrStack.pop();

				if(isProjectionValid(A,B))
				{
					C = Project(A,B, to_string(QueriesProcesed) + "." + to_string(currOperation) );
				} 
				else 
				{
					return "**ERROR**";	
				}
				if(C!="**ERROR**")
					StrStack.push( C );	
				else
					return "**ERROR**";	
			}	
			else if( PostfixExpr[i]=="U" ) // if Union is to be performed
			{
				if(StrStack.size() == 0)
				{
					printf("Improper Expression to be Evaluated.\n");
					return "**ERROR**";
				}
				B = StrStack.top(); StrStack.pop();
				if(StrStack.size() == 0)
				{
					printf("Improper Expression to be Evaluated.\n");
					return "**ERROR**";	
				}			
				A = StrStack.top(); StrStack.pop();

				if(isUnionValid(A,B))
				{
					C = Union(A,B, to_string(QueriesProcesed) + "." + to_string(currOperation) );
				}
				else 
				{
					return "**ERROR**";	
				}	
				if(C!="**ERROR**")
					StrStack.push( C );	
				else
					return "**ERROR**";	
			}
			else if( PostfixExpr[i]=="-" ) // if Set Difference is to be performed
			{
				if(StrStack.size() == 0)
				{
					printf("Improper Expression to be Evaluated.\n");
					return "**ERROR**";
				}
				B = StrStack.top(); StrStack.pop();
				if(StrStack.size() == 0)
				{
					printf("Improper Expression to be Evaluated.\n");
					return "**ERROR**";	
				}			
				A = StrStack.top(); StrStack.pop();

				if(isUnionValid(A,B))
				{
					C = SetDiff(A,B, to_string(QueriesProcesed) + "." + to_string(currOperation) );
				} 
				else 
				{
					return "**ERROR**";	
				}
				if(C!="**ERROR**")
					StrStack.push( C );				
				else
					return "**ERROR**";	
			}
			else if( PostfixExpr[i]=="X" ) // if Cartesian Product is to be performed
			{
				if(StrStack.size() == 0)
				{
					printf("Improper Expression to be Evaluated.\n");
					return "**ERROR**";
				}
				B = StrStack.top(); StrStack.pop();
				if(StrStack.size() == 0)
				{
					printf("Improper Expression to be Evaluated.\n");
					return "**ERROR**";	
				}			
				A = StrStack.top(); StrStack.pop();

				if(isCPValid(A,B))
				{
					C = CartesianProd(A,B, to_string(QueriesProcesed) + "." + to_string(currOperation) );
				} 
				else 
				{
					return "**ERROR**";	
				}
				if(C!="**ERROR**")
					StrStack.push( C );	
				else
					return "**ERROR**";	
			}
			else if( PostfixExpr[i]=="R" )
			{
				if(StrStack.size() == 0)
				{
					printf("Improper Expression to be Evaluated.\n");
					return "**ERROR**";
				}
				B = StrStack.top(); StrStack.pop();
				if(StrStack.size() == 0)
				{
					printf("Improper Expression to be Evaluated.\n");
					return "**ERROR**";	
				}			
				A = StrStack.top(); StrStack.pop();
				if(isRenameValid(A,B))
				{
					C = Rename(A,B);
				} 
				else 
				{
					return "**ERROR**";	
				}				
				if(C!="**ERROR**")
					StrStack.push( C );				
				else
					return "**ERROR**";	
			}
			else if( PostfixExpr[i]=="S" )
			{

				if(StrStack.size() == 0)
				{
					printf("Improper Expression to be Evaluated.\n");
					return "**ERROR**";
				}
				B = StrStack.top(); StrStack.pop();
				if(StrStack.size() == 0)
				{
					printf("Improper Expression to be Evaluated.\n");
					return "**ERROR**";	
				}			
				A = StrStack.top(); StrStack.pop();

				//cout << "Here\n" << A << endl << B << endl;

				if(isSelectValid(A,B))
				{
					C = Select(A,B, to_string(QueriesProcesed) + "." + to_string(currOperation) );
				}
				else 
				{
					return "**ERROR**";	
				}				 
				if(C!="**ERROR**")
					StrStack.push( C );
				else
					return "**ERROR**";							
			}
			else if( PostfixExpr[i]=="J" )
			{
				
				if(StrStack.size() == 0)
				{
					printf("Improper Expression to be Evaluated.\n");
					return "**ERROR**";
				}
				B = StrStack.top(); StrStack.pop();
				if(StrStack.size() == 0)
				{
					printf("Improper Expression to be Evaluated.\n");
					return "**ERROR**";	
				}			
				A = StrStack.top(); StrStack.pop();

				if(isCPValid(A,B))
				{	
					C = NatJoin(A,B, to_string(QueriesProcesed) + "." + to_string(currOperation) );
				} 
				else 
				{
					return "**ERROR**";	
				}
				if(C!="**ERROR**")
					StrStack.push( C );	
				else
					return "**ERROR**";	
			}				
		}	
	}
	if(StrStack.size() != 1)
	{
		printf("Improper Expression to be Evaluated.\n");
		return "**ERROR**";
	}	
	if( Temporary.RelationExists( StrStack.top() ) )
	{
		Relation &R = Temporary.RelationRef( StrStack.top() ); StrStack.pop();
		R.printRelation();
		return R.RelationName();
	}
	else
	{
		cout << "Relation with name \"" << StrStack.top() << "\" doesn't exists in Temp DB.\n";
		return "**ERROR**";		
	}
}








