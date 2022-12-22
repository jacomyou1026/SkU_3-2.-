/* Basic Interpreter by H?eyin Uslu raistlinthewiz@hotmail.com */
/* Code licenced under GPL */


#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct node{
	int type;
/* system stack-> 1 for variable, 2 for function , 3 for function call
4 for begin 5 for end */

	char exp_data;
	int val;
	int line;
	struct node * next;

};

typedef struct node Node;

struct stack{
	Node * top;
};

typedef  struct stack Stack;


struct opnode{
	char op;
	struct opnode * next;
};
typedef struct opnode opNode;

struct opstack{
	opNode * top;
};

typedef struct opstack OpStack;

struct postfixnode{
	int val;
	struct postfixnode * next;
};

typedef struct postfixnode Postfixnode;

struct postfixstack{
	Postfixnode * top;
	};

typedef struct postfixstack PostfixStack;





int GetVal(char,int *,Stack *);
int GetLastFunctionCall(Stack*);

Stack* FreeAll(Stack *);

//
Stack * Push(Node sNode,Stack *stck)
{
Node *newnode;

if ((newnode=(Node*)malloc(sizeof(Node)))==NULL) {
printf("ERROR, Couldn't allocate memory...");
return NULL;
}
else
{
newnode->type=sNode.type;
newnode->val=sNode.val;
newnode->exp_data=sNode.exp_data;
newnode->line=sNode.line;
newnode->next=stck->top;
stck->top=newnode;
return stck;
}
}

OpStack * PushOp(char op,OpStack *opstck)
{
opNode *newnode;
if ((newnode=(opNode*)malloc(sizeof(opNode)))==NULL) {
printf("ERROR, Couldn't allocate memory...");
return NULL;
}
else
{
newnode->op=op;
newnode->next=opstck->top;
opstck->top=newnode;
return opstck;
}
}

char PopOp(OpStack *opstck)
{
opNode *temp;
char op;
if (opstck->top == NULL )
{
printf("ERROR, empty stack...");
}
else
{
op=opstck->top->op;
temp=opstck->top;
opstck->top=opstck->top->next;
free(temp);
return op;
}
return NULL;
}


PostfixStack * PushPostfix(int val,PostfixStack *poststck)
{
Postfixnode *newnode;
if ((newnode=(Postfixnode*)malloc(sizeof(Postfixnode)))==NULL) {
printf("ERROR, Couldn't allocate memory...");
return NULL;
}
else
{
newnode->val=val;
newnode->next=poststck->top;
poststck->top=newnode;
return poststck;
}
}

char PopPostfix(PostfixStack *poststck)
{
Postfixnode *temp;
int val;
if (poststck->top == NULL )
{
printf("ERROR, empty stack...");
}
else
{
val=poststck->top->val;
temp=poststck->top;
poststck->top=poststck->top->next;
free(temp);
return val;
}
return NULL;
}





void Pop(Node * sNode,Stack *stck)
{
Node *temp;

if (stck->top == NULL )
{
printf("ERROR, empty stack...");
}
//stck->top : newNode
else
{
sNode->exp_data=stck->top->exp_data;
sNode->type=stck->top->type;
sNode->line=stck->top->line;
sNode->val=stck->top->val;
temp=stck->top;
stck->top=stck->top->next;
//메모리 해제
free(temp);
}
}

int isStackEmpty(OpStack *stck)
{
	if (stck->top==0)
		return 1;
return 0;
}

/*void printAllStack(Stack * stck)
{
	Node tempNode;
	printf("\n------------------\n");
	printf("dumping the stack...\n");
	Pop(&tempNode,stck);
	printf("exp=%c type=%d val=%d 	line=%d\n",tempNode.exp_data,tempNode.type,tempNode.val,tempNode.line);
	while( (stck->top!=NULL) ) {
		Pop(&tempNode,stck);
		printf("exp=%c type=%d val=%d 				line=%d\n",tempNode.exp_data,tempNode.type,tempNode.val,tempNode.line);
	}
} */

int Priotry(char operator)
{
	if ((operator=='+') | (operator=='-'))
		return 1;
	else if ((operator=='/') | (operator=='*'))
		return 2;
return 0;
}

int main(int argc,char ** argv)
{
	char line[4096];
	char dummy[4096];
	char lineyedek[4096];

	char postfix[4096];

	char * firstword;



//	int i;
	int val1;
	int val2;


	int LastExpReturn;
	int LastFunctionReturn=-999;
	int CalingFunctionArgVal;

	Node tempNode;


	OpStack * MathStack;

	FILE *filePtr;

	PostfixStack * CalcStack;

	int resultVal;

	Stack * STACK;

	int curLine=0;
	int foundMain=0;
        int WillBreak=0;


	MathStack->top=NULL;
	CalcStack->top=NULL;
	STACK->top=NULL;
	//clrscr();

	if (argc!=2)
	{
		/* if argument count is =1 */
		printf("Incorrect arguments!\n");
		printf("Usage: %s <inputfile.spl>",argv[0]);
		return 1;
	}





	/* open the file */

	if ( ( filePtr=fopen(argv[1],"r") ) == NULL )
	{
		printf("Can't open %s. Check the file please",argv[1]);
		return 2;
	}


	//파일 스트림의 끝을 만날 때까지 반복
	while (!feof(filePtr))
	{

		int k=0;

		//입력 스트림에서 문자열 읽기

		fgets(line,4096,filePtr); /* read the file by Line by Line */
		/* scan for /t characters. get rid of them! */
		while(line[k]!='\0')
		{
		 if (line[k]=='\t')
		 {
			line[k]=' ';
		 }

		 k++;
		}

		strcpy(lineyedek,line);

		curLine++;
		tempNode.val=-999;
		tempNode.exp_data=' ';
		tempNode.line=-999;
		tempNode.type=-999;



		if (!strcmpi("begin\n",line) | !strcmpi("begin",line))
		{
			if (foundMain)
			{
				tempNode.type=4;
				STACK=Push(tempNode,STACK);
			}
		}
		else if (!strcmpi("end\n",line) | !strcmpi("end",line) )
		{
			if (foundMain)
			{
				int sline;
				

				tempNode.type=5;
				STACK=Push(tempNode,STACK);

				sline=GetLastFunctionCall(STACK);
				if (sline==0)
				{
					/* WE FOUND THE RESULT! */
					printf("Output=%d",LastExpReturn);

				}
				//type -> 3
				else
				{
					int j;
					int foundCall=0;
					LastFunctionReturn=LastExpReturn;
					/* get to the last line that have been a function calling */

						fclose(filePtr);
							filePtr=fopen(argv[1],"r");
							curLine=0;
							/* file reversed to start postion */
							/* now go codeline lines to go, to the functions line */
							for(j=1;j<sline;j++)
							{
								fgets(dummy,4096,filePtr); /* read the file by Line by Line */
								curLine++;
							}


						/* clear all the stack up to the last function call */
						while(foundCall==0)
						{
							Pop(&tempNode,STACK);
							if (tempNode.type==3)
							{
								foundCall=1;
							}
						}

				}


			}
		}
		else
		{
			//we need to tokinize
			firstword=strtok(line," ");


			if (!strcmpi("int",firstword))
			{
				if (foundMain)
				{
					tempNode.type=1; /*integer*/
					firstword=strtok(NULL," ");
					tempNode.exp_data=firstword[0];


					firstword=strtok(NULL," ");

					/* check for = */
					if (!strcmpi("=",firstword))
					{
						firstword=strtok(NULL," ");
					}

					tempNode.val=atoi(firstword);
					tempNode.line=0;
					STACK=Push(tempNode,STACK);
				}
			}
			else if (!strcmpi("function",firstword))
			{
				tempNode.type=2;
				firstword=strtok(NULL," ");
				tempNode.exp_data=firstword[0];
				tempNode.line=curLine;
				tempNode.val=0;
				STACK=Push(tempNode,STACK);

				if ( (firstword[0]=='m') & (firstword[1]=='a') & (firstword[2]=='i') & (firstword[3]=='n') )
				{
					/*printf("Found function main() in line %d. Starting to running the script...\n",curLine);*/
					foundMain=1;


				}
				else
				{
					if (foundMain)
					{
						firstword=strtok(NULL," ");
						tempNode.type=1;
						tempNode.exp_data=firstword[0];
						tempNode.val=CalingFunctionArgVal;
						tempNode.line=0;
						STACK=Push(tempNode,STACK);



					}

				}


			}
			else if (firstword[0]=='(')
			{

			if (foundMain)
				{

				int i=0;
				int y=0;


				MathStack->top=NULL;
				/* now make the postfix calculcation */





				while(lineyedek[i]!='\x0')
				{
					/* evulate the function */
					if (isdigit(lineyedek[i])) {
						postfix[y]=lineyedek[i];
						y++;
					}
				     /*	else if (lineyedek[i]=='(')
					{
						MathStack=PushOp(lineyedek[i],MathStack);
					}*/

					else if (lineyedek[i]==')')
					{
						if (!isStackEmpty(MathStack))
						{
							postfix[y]=PopOp(MathStack);
							y++;
						}
					}
					else if ((lineyedek[i]=='+') | (lineyedek[i]=='-') | (lineyedek[i]=='*') | (lineyedek[i]=='/'))
					{
						/*operators*/
						if (isStackEmpty(MathStack))
						{
					/* if stack empty push the operator to stack */

							MathStack=PushOp(lineyedek[i],MathStack);
						}
						else
						{
							/* check for presedence */
							if (Priotry(lineyedek[i]) <= Priotry(MathStack->top->op) )
							{
								/* higher presedence for example + < *  */
								/* pop the last operator */

								/* add the poped operator to the postfix */
								postfix[y]=PopOp(MathStack);
								y++;


								MathStack=PushOp(lineyedek[i],MathStack);


							}
							else
							{
								/* lower presedence for example / > + */
								/* push to stack */

								MathStack=PushOp(lineyedek[i],MathStack);
							}

						}
					}
					else if (isalpha(lineyedek[i])>0)
					{
						int codeline=0;
						int dummyint=0;
						/*look if it's a variable or function call 						*/
						int retVal=0;
						retVal=GetVal(lineyedek[i],&codeline,STACK);
						if ((retVal!=-1) & (retVal!=-999))
						{
							/* if variable */
							postfix[y]=retVal+48; /* in ascii table numeric values start from 48 */
							y++;

						}
						else
						{

						    if (LastFunctionReturn==-999)
						    {

							/* if function */
							/* add to our system stack that we are making a call to function */
							int j;
							tempNode.type=3;
							tempNode.line=curLine;
							STACK=Push(tempNode,STACK);

							/* get function's arguments value */
							CalingFunctionArgVal=GetVal(lineyedek[i+2],&dummyint,STACK);



							fclose(filePtr);
							filePtr=fopen(argv[1],"r");
							curLine=0;
							/* file reversed to start postion */
							/* now go codeline lines to go, to the functions line */


/* !!!!!!!!!!!!!!! j <= codeline olabilir */
							for(j=1;j<codeline;j++)
							{
								fgets(dummy,4096,filePtr); /* read the file by Line by Line */
								curLine++;
							}

							WillBreak=1;
							break;
						    }
						    else

						    {


							postfix[y]=LastFunctionReturn+48; /* in ascii table numeric values start from 48 */
							y++;
							i=i+3;
							LastFunctionReturn=-999;


						    }
						}
					}

					i++;
				}

				if (WillBreak==0)
				{
				/* get out items left in the mathstack */
				while (isStackEmpty(MathStack)==0)
				{

					/* add the poped operator to the postfix */
					postfix[y]=PopOp(MathStack);
					y++;
				}

				postfix[y]='\0';

				//MathStack=FreeAll(MathStack);

				/* now calculate the postfix */
				/*printf("\nCURRENT POSTFIX=%s\n",postfix);*/

				i=0;

				CalcStack->top=NULL;
				while(postfix[i]!='\x0')
				{
					if (isdigit(postfix[i])) {
						/* push to stack */
						CalcStack=PushPostfix(postfix[i]-'0',CalcStack);
					}
					else if ((postfix[i]=='+') | (postfix[i]=='-') | (postfix[i]=='*') | (postfix[i]=='/'))
					{
						val1=PopPostfix(CalcStack);

						val2=PopPostfix(CalcStack);


						switch (postfix[i])
						{
							case '+': resultVal=val2+val1;break;
							case '-': resultVal=val2-val1;break;
							case '/': resultVal=val2/val1;break;
							case '*': resultVal=val2*val1;break;
						}

						CalcStack=PushPostfix(resultVal,CalcStack);
					}
					i++;
				}

				//CalcStack=FreeAll(CalcStack);
				LastExpReturn=CalcStack->top->val;


				}
				WillBreak=0;
				}
			}
		}


	}

	fclose(filePtr);

	//printAllStack(STACK);
	STACK=FreeAll(STACK);

	printf("\nPress a key to exit...");
	getch();
	return  0;
}

Stack * FreeAll(Stack * stck)
{
Node * temp;
Node * head;

	if (stck->top != NULL )
	{
		head=stck->top;
		do
		{

			temp=head;
			head=head->next;
			free(temp);

		} while (head->next!=NULL);
	}

return NULL;
}
int GetLastFunctionCall(Stack *stck)
{
Node * head;

	if (stck->top == NULL )
	{
		printf("ERROR, empty stack...");
	}
	else
	{
		//head= newnode
		head=stck->top;
		do
		{
			//newnode.type =3이면
			if ( head->type==3 )
			{
				return head->line;
			}
			else
				//head->next이 Null이 될때까지 반복
			{
				head=head->next;
			}
		} while (head->next!=NULL);
	}

	return 0;



}

int GetVal(char exp_name,int * line,Stack *stck)
{

Node * head;
*line=0;
	if (stck->top == NULL )
	{
		printf("ERROR, empty stack...");
	}
	else
	{
		head=stck->top;
		do
		{
			if ( head->exp_data==exp_name )
			{

				if (head->type==1)
				{
					/* return the variables value */
					return head->val;
				}
				else if (head->type==2)
				{
					*line=head->line;
					return -1;
					/* it's a function so return -1 */
				}

			}
			else
			{
				head=head->next;
			}
		} while (head->next!=NULL);
		/* check agin once more */
		if (head->exp_data==exp_name)
		{

				if (head->type==1)
				{
					/* return the variables value */
					return head->val;
				}
				else if (head->type==2)
				{
					*line=head->line;
					return -1;
					/* it's a function so return -1 */
				}


		}
	}

	return -999;
}

