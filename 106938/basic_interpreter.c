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
	struct node * next; //�ڱ�����/���� ����� �ּҸ� ������ ������

};

// struct node ��Ī : Node
typedef struct node Node;

struct stack{
	Node * top;
};

//struct stack ��Ī : Stack
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
else
{
sNode->exp_data=stck->top->exp_data;
sNode->type=stck->top->type;
sNode->line=stck->top->line;
sNode->val=stck->top->val;
temp=stck->top;
stck->top=stck->top->next;
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

	//ó���� ������ ����Ű�� ���� ����������
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
	clrscr();

	//�����Լ��� ���޵Ǵ� ������ ������ 2���� �ƴ϶��
	//
	if (argc!=2)
	{
		//Incorrect arguments!�ֿܼ� ���
		printf("Incorrect arguments!\n");
		//Usage: argv[0]�ǰ� <inputfile.spl>�� �ֿܼ� ���
		printf("Usage: %s <inputfile.spl>",argv[0]);

		return 1; //��ȯ�� 1
	}





	//�����Լ����� ���޵Ǵ� ������ ���� 1��°�� �б��������� ���� ���  filePtr������ ��ȯ�� ����  ���� ������ ���ٸ� NULL��ȯ
	//���� ������ ���� ���
	if ( ( filePtr=fopen(argv[1],"r") ) == NULL )
	{
		// Can't open argv[1]�ǰ� Check the file please �� �ֿܼ� ���
		printf("Can't open %s. Check the file please",argv[1]);
		return 2; // ��ȯ�� 2
	}

	//������ ppt�� �߾��.

	//������ ���� �����ߴ��� ���θ� Ȯ��
	//������ ���� �������� ������ ��쿡 0 ��ȯ
	//���� ��Ʈ���� ���� ���� ������ �ݺ�
	while (!feof(filePtr))
	{
		//���� k�� 0 
		int k=0;

		//���Ͽ��� �� �� ������ ���ڿ��� �о���δ�.
		//filePtr�������� �ִ� 4095������ �а� line�迭�� ����.
		fgets(line,4096,filePtr); 

		// whil�� line[k]�� '\0'( null )�� �ƴ� ������ �ݺ�.
		while(line[k]!='\0')
		{
			// 	4-1) ���� line[k]�� \t(��Ű)�� ��,  
		 if (line[k]=='\t')
		 {
			 //line[k]�� �� ������ �ٲ۴�.
			line[k]=' ';
		 }
		  //k�� 1����
		 k++;
		}
		//5. line�� lineyedek�� ���ڿ� ����
		strcpy(lineyedek,line);

		//curLine�� 1����
		curLine++;
		tempNode.val=-999; //tempNode.val���� -999�� ����
		tempNode.exp_data=' '; //tempNode.exp_data���� ������ ����
		tempNode.line=-999;//tempNode.line �� -999�� ����
		tempNode.type=-999;//tempNode.type �� -999�� ����


		// ���ϳ�
		//strcmpi :  ���� �����ϸ� 0  ��ȯ�ϰ� �������� ������ 1
		//! �� �������ν� ���� �����ϸ� 1(True) �������� ������ 0(False) 
		//"begin"�� line�� �����ϴٸ�
		if (!strcmpi("begin\n",line) | !strcmpi("begin",line))
		{
			//foundMain == 0�̸� False 0�� �ƴϸ� True
			//foundMain �� 1�̸� if�� ����
			if (foundMain)
			{
				//tempNode.type �� 4�̴�.
				tempNode.type=4;
				////STACK�� Push(tempNode,STACK); ������ ��ȯ���̴�.
				STACK=Push(tempNode,STACK);
			}
		}


		//��
		// //strcmpi :  ���� �����ϸ� 0  ��ȯ�ϰ� �������� ������ 1
		//! �� �������ν� ���� �����ϸ� 1(True) �������� ������ 0(False) 
		//line�� "end"�̰ų� line�� "end \n"�̶��
		else if (!strcmpi("end	\n",line) | !strcmpi("end",line) )
		{
			//(1)

			//foundMain == 0�̸� False 0�� �ƴϸ� True
			//foundMain �� 1�̸� ����
			if (foundMain)
			{
				//sline ���� ����
				int sline;

				//tempNode�� typ�� 5
				tempNode.type=5;
				//STACK��Push(tempNode,STACK)�� ��ȯ��
				STACK=Push(tempNode,STACK);

				//sline��GetLastFunctionCall(STACK)�� ��ȯ��
				sline=GetLastFunctionCall(STACK);

				//���� sline�� 0�̶��
				if (sline==0)
				{
					// Output = LastExpReturn�ǰ��� �ܼ� ���
					printf("Output=%d",LastExpReturn);

				}
				//sline==0 �� �ƴ϶��
				else
				{
					//���� j����
					int j;
					//���� foundCall�� 0
					int foundCall=0;

					//LastFunctionReturn�� LastExpReturn�� ���̴�
					LastFunctionReturn=LastExpReturn;
						//filePtr ���� �ݱ�
						fclose(filePtr);
						/// filePtr= argv[1] ���� �б���� ���Ͽ���
							filePtr=fopen(argv[1],"r");

							//curLine�� 0
							curLine=0;

							//1���� sline���� ���������� 1�� �����ϸ� �ݺ�
							for(j=1;j<sline;j++)
							{
								//filePtr�������� �ִ� 4095������ ���� �� ������, dummy�迭�� ����.
								fgets(dummy,4096,filePtr); 
								//curLine 1����
								curLine++;
							}

						//���� foundCall�� 0�� ���
						while(foundCall==0)
						{
							//Pop(&tempNode,STACK)����
							Pop(&tempNode,STACK);
							//tempNode�� type�� 3�� ���
							if (tempNode.type==3)
							{
								//foundCall�� 1�̴�
								foundCall=1;
							}
						}

				}

			}
		}

		//�� begin�� end�� �ƴϸ�
		else
		{
		

			// ������� \0���� �ٲ�
			//line���� ���� ���� ù����
			firstword=strtok(line," ");
			

			// //strcmpi :  ���� �����ϸ� 0  ��ȯ�ϰ� �������� ������ 1
		    //! �� �������ν� ���� �����ϸ� 1(True) �������� ������ 0(False) 

			//firstword�� "int"���
			if (!strcmpi("int",firstword))
			{
				//foundMain ==0
				//foundMain == 0�̸� false 0�� �ƴϸ� True
				if (foundMain)
				{	
					//ttempNode.type �� 1
					tempNode.type=1; /*integer*/

					//" "���� �������� ���� 
					//firstword�� �������� �ɰ��� ���ڿ� 
					firstword=strtok(NULL," ");

					//tempNode.exp_data�� exp_data = firstword[0]
					tempNode.exp_data=firstword[0];

					//" "���� �������� ���� 
					//firstword�� �������� �ɰ��� ���ڿ� 
					firstword=strtok(NULL," ");

					// //strcmpi :  ���� �����ϸ� 0  ��ȯ�ϰ� �������� ������ 1
					//! �� �������ν� ���� �����ϸ� 1(True) �������� ������ 0(False) 
					//firstword�� '='�̶�� 
					if (!strcmpi("=",firstword))
					{
						//" "���� �������� ���� 
						//firstword�� �������� �ɰ��� ���ڿ� 
						firstword=strtok(NULL," ");
					}

					//���ڿ��� ���� Ÿ�� ��ȭ ex) 1
					//tempNode�� val = firstword�� ���� ��ȯ
					tempNode.val=atoi(firstword);
					//tempNode�� line��  0
					tempNode.line=0;
					//STACK�� Push(tempNode,STACK)�� ��
					STACK=Push(tempNode,STACK);
				}
			}

			// //strcmpi :  ���� �����ϸ� 0  ��ȯ�ϰ� �������� ������ 1
			//! �� �������ν� ���� �����ϸ� 1(True) �������� ������ 0(False) 
			// firstword�� "function"�̶��
			else if (!strcmpi("function",firstword))
			{	
				//tempNode.type�� 2�̴�
				tempNode.type=2;

				//" "���� �������� ���� 
				//firstword�� �������� �ɰ��� ���ڿ� 
				firstword=strtok(NULL," "); 

				//tempNode.exp_data�� firstword[0]�̴�
				tempNode.exp_data=firstword[0]; 

				//tempNode.line �� curLine�̴�
				tempNode.line=curLine;
				//tempNode.val�� 0�̴�.
				tempNode.val=0;
				//STACK�� Push(tempNode,STACK); ������ ��ȯ���̴�.
				STACK=Push(tempNode,STACK);

				//���� firstword�迭�� 0������ 'm'�̸� firstword�迭�� 1������ 'a'�̸� firstword�迭�� 2������ 'i'�̸� firstword�迭�� 3��°�� 'n'�̶��
				if ( (firstword[0]=='m') & (firstword[1]=='a') & (firstword[2]=='i') & (firstword[3]=='n') )
				{
					//foundMain �� 1�̴�.
					foundMain=1;


				}
				//���� firstword�迭�� 0������ 'm'�̸� firstword�迭�� 1������ 'a'�̸� firstword�迭�� 2������ 'i'�̸� firstword�迭�� 3��°�� 'n'�� �ƴ϶��
				else
				{
					//(3)
					//foundMain == 0�̸� False 0�� foundMain == 0�� �ƴϸ� True 
					//foundMain�� 0�� �ƴϸ� ����
					if (foundMain)
					{
					
						//" "���� �������� ���� 
						//firstword�� �������� �ɰ��� ���ڿ� 
						firstword=strtok(NULL," "); 

						//tempNode.type �� 1�̴�
						tempNode.type=1;

						//tempNode.exp_data�� firstword[0]�̴�
						tempNode.exp_data=firstword[0]; 

						//tempNode.val�� CalingFunctionArgVal
						tempNode.val=CalingFunctionArgVal;

						//tempNode��line�� 0�̴�;
						tempNode.line=0;
						////STACK�� Push(tempNode,STACK); ������ ��ȯ���̴�.
						STACK = Push(tempNode, STACK);

					}

				}


			}

			//firstword[0]�� '(' �̶��
			else if (firstword[0]=='(')
			{

			//foundMain == 0�̸� False 0�� foundMain == 0�� �ƴϸ� True 
			//foundMain�� 1�̸� ����
			if (foundMain)
				{

				//���� i�� 0
				int i=0;
				//���� y�� 0
				int y=0; 

				//MathStack.top�� NULL
				MathStack->top=NULL;




				//lineyedek[i]�� \x0(NULL)'�� �ƴҶ����� �ݺ�
				while(lineyedek[i]!='\x0')
				{
					//lineyedek[i]�� ���ڶ�� True
					if (isdigit(lineyedek[i])) {
						//postfix[y��lineyedek[i] �ֱ�
						postfix[y]=lineyedek[i];

						y++;  //y�� 1����

					}
				 
					//lineyedek[i]�� ')'�̶��
					else if (lineyedek[i]==')')
					{
						//0�̸� true 1�̸� false
						//0�ϋ� 
						if (!isStackEmpty(MathStack) != 0 ) 
						{
							//postfix[y]�� PopOp(MathStack)�� ��ȯ�� �ֱ�
							postfix[y]=PopOp(MathStack); 
							y++; // y�� 1����
						}
					}
					
					////(3)-3-3
					//lineyedek[i]�� '+'�̰ų� lineyedek[i]�� '-' �̰ų� lineyedek[i]�� '+' �̰ų� lineyedek[i]�� '/'�̶��
					else if ((lineyedek[i]=='+') | (lineyedek[i]=='-') | (lineyedek[i]=='*') | (lineyedek[i]=='/'))
					{
						////(3)-3-3-1
						/*operators*/
						//0�̸� false 1�̸� true
						//isStackEmpty(MathStack)�� ��ȯ�� 0�� �ƴ϶��
						if (isStackEmpty(MathStack) != 0 ) 
						{
							// MathStack�� PushOp(lineyedek[i],MathStack)�� ��ȯ��
							MathStack=PushOp(lineyedek[i],MathStack);
						}
						

						//isStackEmpty(MathStack)�� ��ȯ���� 0�̶��
						else
						{
							//Priotry(MathStack->top->op)�� ��ȯ���� Priotry(lineyedek[i])�� ��ȯ������ ũ�ų� ������
							if (Priotry(lineyedek[i]) <= Priotry(MathStack->top->op) )
							{
								//postfix[y]�� PopOp(MathStack)��ȯ��
								postfix[y]=PopOp(MathStack);

								//y�� 1����
								y++;

								//MathStack�� PushOp(lineyedek[i],MathStack)�̴�
								MathStack=PushOp(lineyedek[i],MathStack);


							}
							// Priotry(lineyedek[i])�� ��ȯ���� Priotry(MathStack->top->op)�� ��ȯ������ Ŭ ��
							else
							{
								//MathStack�� PushOp(lineyedek[i],MathStack)�� ��ȯ��
								MathStack=PushOp(lineyedek[i],MathStack);
							}

						}
					}

					//���ĺ� �빮�� "A-Z"�� 1�� ��ȯ.���ĺ� �ҹ��� 'a-z"�� 2�� ��ȯ.
					//lineyedek[i]�� ������
					else if (isalpha(lineyedek[i])>0)
					{
						//���� codeline�� 0�̴�
						int codeline=0;

						//���� dummyint�� 0�̴�
						int dummyint=0;

						//���� retVal�� 0�̴�
						int retVal=0;
						//retVal�� GetVal(lineyedek[i],&codeline,STACK);�� ��ȯ��
						retVal=GetVal(lineyedek[i],&codeline,STACK); 
						//int codeline =1


						//���� retVal�� -1�� �ƴϸ鼭 -999�� �ƴ϶��
						if ((retVal!=-1) & (retVal!=-999))
						{
							//postfix[y]��retVal+48���Ѵ�.
							postfix[y]=retVal+48;
							//y�� 1����
							y++; 

						}
						////���� retVal�� -1�̰ų� -999�̶��
						else
						{
							
							//���� LastFunctionReturn�� -999�̶��
						    if (LastFunctionReturn==-999)
						    {

							//���� j����
							int j;

							//tempNode.type�� 3�̴�.
							tempNode.type=3;

							//tempNode.line�� curLine�̴�
							tempNode.line=curLine;

							//STACK�� Push(tempNode,STACK)�� ��ȯ��
							STACK=Push(tempNode,STACK);


							//CalingFunctionArgVal�� GetVal(lineyedek[i+2],&dummyint,STACK)�� ��ȯ��
							CalingFunctionArgVal=GetVal(lineyedek[i+2],&dummyint,STACK);


							//filePtr�� ������ �ݴ´�.
							fclose(filePtr);
							//filePtr�� argv[1]��°�� �б� ���� ������ ����
							filePtr=fopen(argv[1],"r");
							//curLine�� 0�̴�.
							curLine=0;


							//1���� codeline���� ���������� 1�� �����Ͽ� �ݺ�
							for(j=1;j<codeline;j++)
							{
								//filePtr�������� �ִ� 4095������ ���� �� ������, dummy�迭�� ����.
								fgets(dummy,4096,filePtr); /* read the file by Line by Line */
								//curLine 1����
								curLine++;
							}

							//WillBreak�� 1�̴�.
							WillBreak=1;

							//whlie�� ������
							break;
						    }
							

							//���� LastFunctionReturn�� -999�ƴ϶��
						    else

						    {

							//postfix[y]��LastFunctionReturn+48�̴�.
							postfix[y]=LastFunctionReturn+48; 
							y++; // y�� 1����
							i=i+3; //i�� i+3�̴�.
							LastFunctionReturn=-999; //LastFunctionReturn�� -999�̴�.


						    }
						}
						}
					}

					//i�� 1 ����
					i++;
				}//while��



				//WillBreak�� 0�̶��
				if (WillBreak==0)
				{

				//isStackEmpty(MathStack)�� 0�̶�� �ݺ�
				while (isStackEmpty(MathStack)==0)
				{

					//postfix[y]= PopOp(MathStack)�� ��ȯ��
					postfix[y]=PopOp(MathStack);
					//y�� 1����
					y++;
				}
				//postfix[y]�� '\0'�� �ִ´�.
				postfix[y]='\0';

				
				// i �� 0
				i=0;

				//CalcStack�� top�� NULL
				CalcStack->top=NULL;

				//postfix[i]�� \x0�ϋ����� �ݺ�
				while(postfix[i]!='\x0')
				{
					//postfix[i]�� ���ڶ��
					if (isdigit(postfix[i])) {

						//CalcStack PushPostfix(postfix[i]-'0',CalcStack);�� ��ȯ��
						CalcStack=PushPostfix(postfix[i]-'0',CalcStack);

					}
					//postfix[i]�� '+'�̰ų� postfix[i]�� '-' �̰ų� postfix[i]�� '*' �̰ų� postfix[i]�� '/' �̶��
					else if ((postfix[i]=='+') | (postfix[i]=='-') | (postfix[i]=='*') | (postfix[i]=='/'))
					{
						//val1 �� PopPostfix(CalcStack)�� ��ȯ��
						val1=PopPostfix(CalcStack);

						//val2 �� PopPostfix(CalcStack)�� ��ȯ��
						val2=PopPostfix(CalcStack);

						//switch��
						//postfix[i]�� �Ǵ��� ������ case�� �Է°��� �ִٸ� �ش� case�� ����
						switch (postfix[i])
						{
							//postfix[i]�� '+'�ϋ� resultVal�� val2+val1�� ���̴�.
							//�� break������ switch�� ����������.
							case '+': resultVal=val2+val1;break;
							//postfix[i]�� '-' �ϋ� resultVal�� val2-val1�� ���̴�.
							//�� break������ switch�� ����������
							case '-': resultVal=val2-val1;break;

							///postfix[i]�� '/'�ϋ� resultVal�� val2/val1�� ���̴�
							//�� break������ switch�� ����������
							case '/': resultVal=val2/val1;break;

							//postfix[i]�� '*'�ϋ� resultVal�� val2*val1�� ���̴�.
							// //�� break������ switch�� ����������
							case '*': resultVal=val2*val1;break;
						}

						//CalcStack �� PushPostfix(resultVal,CalcStack)�� ��ȯ��
						CalcStack=PushPostfix(resultVal,CalcStack);
					}
					// i�� 1 ����
					i++;
				}


				//LastExpReturn �� CalcStack��  top�� val�̴�.
				LastExpReturn=CalcStack->top->val;


				}
				
				//WillBreak �� 0
				WillBreak=0;
				}
			}
		}


	}
	//filePtr ���� �ݱ�
	fclose(filePtr);
	//printAllStack(STACK);

	//STACK��FreeAll(STACK)�� ��ȯ��
	STACK=FreeAll(STACK);

	//Press a key to exit...�ֿܼ� ���
	printf("\nPress a key to exit...");
	//Ű�� �Է¹޴´�.(input)
	getch();

	//��ȯ�� 0
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
		head=stck->top;
		do
		{
			if ( head->type==3 )
			{
				return head->line;
			}
			else
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

