#include <bits/stdc++.h>
using namespace std;

typedef long long LL;

bool isInteger(string S);

class Relation;
class Database;

class Relation
{
	private:

	string Name; // Unique Identifier of Relation	
	vector<string> AttributeName;  
	vector<string> AttributeType; // INT or STRING
	set< vector<string> > Instance; // Set of Tuples(Vectors) to store Relation Instance		

	public:	

	Relation()
	{

	}

	Relation(int N)
	{
		Name = "ERROR";
	}

	void writeToFile(fstream &file) //writes this relation to file	
	{
		file << Name << endl;
		file << to_string(AttributeName.size()) << endl;

		LL i,N = AttributeName.size(),M = Instance.size();

		for(i=0;i<N;i++)
		{
			file << AttributeName[i] << endl;
		}

		for(i=0;i<N;i++)
		{
			file << AttributeType[i] << endl;
		}

		file << to_string(M) << endl;

		set< vector<string> >::iterator itr;

		for(itr = (*this).InstBegin();itr!=(*this).InstEnd();++itr) // Insert tuples from RelA
		{
			vector<string> T = (*itr);

			for(i=0;i<N;i++)
			{
				file << T[i] << endl;
			}
		} 
	}

	void readFromFile(fstream &file)
	{
		string S;
		file >> S ;
		// cout << S <<endl;
		Name = S;
		LL i,j,N,M;
		file >> S;
		N = stoll(S);
		// cout <<"N = "<< N <<endl;
		vector<string> T;

		for(i=0;i<N;i++)
		{
			file >> S ;
			T.push_back(S);
		}
		AttributeName = T; T.clear();

		for(i=0;i<N;i++)
		{
			file >> S ;
			T.push_back(S);
		}
		AttributeType = T; T.clear();

		file >> S;
		M = stoll(S);
		// cout <<"M = "<< M <<endl;

		for(j=0;j<M;j++)
		{
			for(i=0;i<N;i++)
			{
				file >> S ;
				// cout <<"S = "<< S <<endl;
				T.push_back(S);
			}
			insertTuple(T); T.clear();
		}		

	}

	void renameRelation(string newName) // assigns a new name to some relation
	{
		Name = newName ; 
	}

	void renameAttributes(vector<string> &AttributeName) 
	{
		(*this).AttributeName = AttributeName;
	}

	set< vector<string> >::iterator InstBegin()
	{
		return Instance.begin();
	}

	set< vector<string> >::iterator InstEnd()
	{
		return Instance.end();
	}

	set< vector<string> >& InstanceSet()
	{
		return Instance;
	}

	vector<string> AttributeNameVector()
	{
		return AttributeName;
	}

	vector<string> AttributeTypeVector()
	{
		return AttributeType;
	}	

	Relation(string Name,vector<string> &AttributeName,vector<string> &AttributeType) //Creates Relation object	
	{
		(*this).Name = Name;

		LL N = AttributeName.size();

		for(LL i=0;i<N;i++)
		{
			(*this).AttributeName.push_back(AttributeName[i]); // Must be Valid
			(*this).AttributeType.push_back(AttributeType[i]); // Must be Valid
		}
	}

	bool insertTuple(vector<string> &Tuple) // Accepts a Tuple (Checks for its Validity) (If Valid Inserts it)
	{
		LL N = AttributeName.size();

		if(Tuple.size() != N) // Arity of Tuple to be inserted & Relation should match
			return false;

		for(LL i=0;i<N;i++) // Checking for Validity of Integer Data-type
		{
			if(AttributeType[i]=="INT")
			{
				if(!isInteger(Tuple[i]))
					return false;
			}	
		}	

		Instance.insert(Tuple);
		return true;
	}

	bool eraseTuple(vector<string> &Tuple) // Accepts a Tuple (Checks for its Validity) (If Valid Inserts it)
	{
		LL N = AttributeName.size();

		if(Tuple.size() != N) // Arity of Tuple to be inserted & Relation should match
			return false;

		for(LL i=0;i<N;i++) // Checking for Validity of Integer Data-type
		{
			if(AttributeType[i]=="INT")
			{
				if(!isInteger(Tuple[i]))
					return false;
			}	
		}	

		Instance.erase(Tuple);
		return true;
	}	

	LL NoOfAttributes()
	{
		return AttributeName.size();
	}

	string AttrTypeAtInd(LL i) // return Attribute type at certain index
	{
		return AttributeType[i];
	}

	LL AttributeExists(string S)
	{
		LL i,N = AttributeName.size();
		for(i=0;i<N;i++)
		{
			if(AttributeName[i]==S)
				return i;
		}
		return -1;	
	}

	string RelationName()
	{
		return Name;
	}

	void printRelation() // prints a relation in specific format
	{
		if(Instance.empty())
		{
			printf("Null Set !\n");
			return;
		}	

		set< vector<string> >::iterator i;
		LL j,k,N = AttributeName.size();

		LL *Sizes = new LL [N];

		for(j=0;j<N;j++)
		{
			Sizes[j] = 1 + AttributeName[j].length() ;
		}	

		for(i = Instance.begin();i != Instance.end();++i)
		{
			vector<string> T = *i;
			for(j=0;j<N;j++)
			{
				Sizes[j] = Sizes[j]>(1 + T[j].length()) ? Sizes[j] : (1 + T[j].length()) ;
			}	
		}

		printf("+");
		for(j=0;j<N;j++)
		{
			for(k=0;k<Sizes[j];k++)
				printf("-");
			printf("-+");
		}
		printf("\n");

		printf("|");
		for(j=0;j<N;j++)
		{
			cout << setw(Sizes[j]) << AttributeName[j] << " |" ;
		}
		printf("\n");

		printf("+");
		for(j=0;j<N;j++)
		{
			for(k=0;k<Sizes[j];k++)
				printf("-");
			printf("-+");
		}
		printf("\n");			

		for(i = Instance.begin();i != Instance.end();++i)
		{
			vector<string> T = *i;
			printf("|");
			for(j=0;j<N;j++)
			{
				cout << setw(Sizes[j]) << T[j] << " |" ;
			}
			printf("\n");	
		}	

		printf("+");
		for(j=0;j<N;j++)
		{
			for(k=0;k<Sizes[j];k++)
				printf("-");
			printf("-+");
		}
		printf("\n");			

		delete Sizes;	
	}
};

class Database // Collection of relations
{
	private :

	vector<Relation> RelationList; // Vector of Relation Objects

	public :

	Database()
	{

	}

	void PrintRelationNames()
	{
		LL i,N = RelationList.size();
		cout << "Relations in Permanent Database are :\n";
		for(i=0;i<N;i++)
		{
			cout << i+1 << ") "<<RelationList[i].RelationName() << endl;
		}	
	}

	LL NoOfRelation()
	{
		return RelationList.size();
	}


	bool RelationExists(string RelName) // Accepts Relation Name & determines if it exists
	{
		LL N = RelationList.size();

		for(LL i=0;i<N;i++)
		{
			if( RelationList[i].RelationName() == RelName ) 
				return true;
		}

		return false;	
	}

	void writeToFile(fstream &file)
	{
		// cout << Name << " is the name.\n";
		file << to_string(RelationList.size()) << endl;
		// cout << RelationList.size() << " is the N.\n";
		LL i,N = RelationList.size();

		for(i=0;i<N;i++)
		{
			RelationList[i].writeToFile(file);
		}	

	}

	void readFromFile(fstream &file)
	{
		string S;
		LL i,N;
		file >> S;
		N = stoll(S);
		for(i=0;i<N;i++)
		{
			Relation R;
			R.readFromFile(file);
			RelationList.push_back(R);
		} 
	}

	Relation& RelationRef(string RelName) // Accepts Relation Name & returns its reference (Check its existence first)
	{
		LL N = RelationList.size();

		for(LL i=0;i<N;i++)
		{
			if( RelationList[i].RelationName() == RelName ) 
				return RelationList[i];
		}		
	}

	bool insertRelation(Relation &R) // Appends Relation Object in Database Object
	{
		RelationList.push_back(R);
	}

};



Database Permanent; // Stores Permanent Relation Objects
Database Temporary; // Stores Temporary Relation Objects
LL QueriesProcesed = 0;

bool isInteger(string S)
{
	LL N = strlen( S.c_str() );
	if( !( (S.c_str())[0]=='+' || (S.c_str())[0]=='-' || (S.c_str())[0]<='9' || (S.c_str())[0]>='0') )
	{
		return false;
	}
	for(LL i=0;i < N;i++)
	{
		if(!('0'<=(S.c_str())[i] && (S.c_str())[i]<='9')) // checks if there is some non-numeric character in string
			return false;
	}

	return true;	
}







