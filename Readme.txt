RA-Query - a tool for RA Query Execution 

Instructions to create executable file :
	1) Execte the following command in terminal : g++ -std=c++11 QProcessor.cpp  -o RAQuery
	2) Now execute the RAQuery executable so created.

This tool has been developed by Janvijay Singh [ 14075061 ], as a part of DBMS project for session 2016-17 under the guidence of Proff. Ravindranath Chowdary C.

This tool assumes input as space-seprated stream of tokens. So it is advised to seprate each token by atleast one space & Relation names / Attribute names / Attribute Values must be devoid of any spaces as well ( Use '_' for multi-word Relation names / Attribute names / Attribute Values ). This tool is Case-Sensitive. So it is advised to keep case in mind while executing queries. Operations available in this tool & their Syntax is as given below :


1)	Project Operation : 
		Purpose of this Operation is to carry out Projection of certain columns of a Relation. This operation can be carried out using 'P' operator provided with this tool. 'P' is a binary operator, which takes first argument as a Relation Name or a expression evaluating to Relation, & the second argument is a '|' (pipe) seprated list of attributes included in the Result, enclosed within parantheses.

		Syntax :
			[ RelationA P (Attribute1|Attribute2|Attribute3|....|AttributeN) ]

		Notes Regarding this Operator :
			Attribute list is required to be devoid of any spaces.
			RelationA can be name of some pre-existing relation or an expression evaluating to a Relation.


2)	Rename Operation :
		Purpose of this Operation is to Rename a Relation existing in Database or result of some expression which evaluates to a Relation. This operation can be carried out using 'R' operator provided with this tool. 'R' is a binary operator, which takes first argument as a Relation Name or a expression evaluating to Relation, & the second argument is a '|' (pipe) seprated list of newRelation name & newAttribute names, enclosed within parantheses. Second argument can be a single name, enclosed within parantheses, in this case attribute names of Relation are preserved, only Relation name is changed. 

		Syntax :
			[ RelationA R (newRelationA|newAttribute1|newAttribute2|newAttribute3|....|newAttributeN) ]
				OR
			[ RelationA R (newRelationA) ]       
		
		Notes Regarding this Operator :
			new Name list is required to be devoid of any spaces.
			RelationA can be name of some pre-existing relation or an expression evaluating to a Relation.		


3)	Union Operation :
		Purpose of this Operation is to carry out union of two Relations. This operation can be carried out using 'U' operator provided with this tool. 'U' is a binary operator, which takes two argument each being a Relation Name or a expression evaluating to Relation. Two relations passed as arguments must have same arity & domain (Data Type) of corressponding attributes must match as well.

		Syntax :
			[ RelationA U RelationB ]

		Notes Regarding this Operator :
			RelationA & RelationB can be name of some pre-existing relation or an expression evaluating to a Relation.


4)	Set Difference Operation :
		Purpose of this Operation is to carry out Set Differnce of two Relations. This operation can be carried out using '-' operator provided with this tool. '-' is a binary operator, which takes two argument each being a Relation Name or a expression evaluating to Relation. Two relations passed as arguments must have same arity & domain (Data Type) of corressponding attributes must match as well. Result is of this operator is a relation including all tuple including in first relation, not present in second relation.

		Syntax :
			[ RelationA - RelationB ]

		Notes Regarding this Operator :
			RelationA & RelationB can be name of some pre-existing relation or an expression evaluating to a Relation.				


5)	Cartesian Product Operation :
		Purpose of this Operation is to carry out Cartesian Product of two Relations. This operation can be carried out using 'X' operator provided with this tool. 'X' is a binary operator, which takes two argument each being a Relation Name or a expression evaluating to Relation. Result is of this operator is a relation including each tuple included in first relation glued with every tuple in second relation. 

		Syntax :
			[ RelationA X RelationB ]

		Notes Regarding this Operator :
			RelationA & RelationB can be name of some pre-existing relation or an expression evaluating to a Relation.
			Attributes if common in RelationA & RelationB are automatically renamed.


6)	Selection Operation :
		Purpose of this Operation is to carry out Selection of certain tuple of a Relation, based on if they satisfy the specified predicate or not. This operation can be carried out using 'S' operator provided with this tool. 'S' is a binary operator, which takes first argument as a Relation Name or a expression evaluating to Relation, & the second argument is a Predicate Expression enclosed within parantheses.

		Syntax :
			[ RelationA S PredicateExpression ]

		Notes Regarding this Operator :
			RelationA can be name of some pre-existing relation or an expression evaluating to a Relation.

		Syntax for Predicate Expression :
			1. Relational operators available with this tool are : (greater than) >> / (less than) << / (greater than or equal to) >= / (less than or equal to) <= /  (not equal to) <> / (equal to) == .
			2. Logical operators available with this tool are : & (logical and) / | (logical or) / ! (logical not) 
			3. Each constant if used is required to be appended with "@C".
			4. Syntax for Predicate Expression can be defined as recursively as follows :
				i. (Attribute1 RelOp Attribute2) & (Attribute1 RelOp Const) & (Const RelOp Attribute2) are Basic Predicate expression. (Spaces are included for undestanding purpose only, must be devoid of 		spaces)
				ii. Any predicate expression formed using combination of two / one predicate expression is in itself a valid predicate expression.
					((PredicateExpression1)&(PredicateExpression2)) and ((PredicateExpression1)|(PredicateExpression2)) and (!(PredicateExpression1))) are valid Predicate Expressions.


7)	Natural Join Operation :
		Purpose of this Operation is to carry out Natural Join of two Relations. This operation can be carried out using 'J' operator provided with this tool. 'J' is a binary operator, which takes two argument each being a Relation Name or a expression evaluating to Relation. Result is of this operator is a relation including each tuple included in first relation glued with every tuple in second relation, provided the tuples to glued have same Attributes Values for Attributes common in two Relations ( Common Attributes are kept only once in resulting tuple).

		Syntax :
			[ RelationA J RelationB ]

		Notes Regarding this Operator :
			RelationA & RelationB can be name of some pre-existing relation or an expression evaluating to a Relation.
			Attributes if common in RelationA & RelationB occur only once in the resulting relation.				


Some Operators provided to ease Table creation are as follows :


1)	Create Table Operation : 
		Purpose of this Operation is to create a empty Relation. This operation can be carried out using '*C' operator provided with this tool. 

		Syntax :
			*C [ RelationName ] [ Attribute1Name Attribute1DataType ] [ Attribute2Name Attribute2DataType ] [ Attribute3Name Attribute3DataType ] .... [ AttributeNName AttributeNDataType ];

		Notes Regarding this Operator :
			Attribute Name / Relation Name is required to be devoid of any spaces.
			RelationA must not be a pre-existing Relation.

2)	Insert into Table Operation : 
		Purpose of this Operation is to insert a tuple in pre-existing Relation. This operation can be carried out using '*I' operator provided with this tool. 

		Syntax :
			*I [ RelationA ] [ Attribute1Value Attribute2Value Attribute3Value ... AttributeNValue ];

		Notes Regarding this Operator :
			Attribute Name / Relation Name is required to be devoid of any spaces.
			RelationA must not be a pre-existing Relation.	


3)	Save Table Operation : 
		Purpose of this Operation is save a pre-existing Relation (in Temporary Database) into Permanent Database. This operation can be carried out using '*S' operator provided with this tool. 

		Syntax :
			*S [ RelationA ];

		Notes Regarding this Operator :
			RelationA can be name of some pre-existing relation or an expression evaluating to a Relation.	


4)	Exit Operation : 
		Purpose of this Operation is to exit the Tool. This operation can be carried out using '*E' operator provided with this tool. 

		Syntax :
			*E;

		Notes Regarding this Operator :
			Exiting using this operator allows you to save the relation in permanent database in "File.dat" file, from where they can be reloaded.

5)  Show Tables Operation :
		Purpose of this Operation is to Show Tables in Permanent Database. This operation can be carried out using '*V' operator provided with this tool. 

		Syntax :
			*V;

Nested Queries have been implemented by firstly carrying out Postfix expression for given RA expresion & then evaluating the Postfix expression.										



