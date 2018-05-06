//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Programmer: Luis Gutierrez
//Instructor: Dr. Egle
//Section: 3334 Monday / Wednesday
//Phase 2
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdbool.h> // for booleans
#include <string.h> //for strlen() and strcmp()
#include <ctype.h>
#include <stdlib.h> //for system commands
#define DISTANCE(x) (sizeof(x) / sizeof(x[0]))




void assemble(FILE *sTable, FILE *inter); //Assembles the SIC FILES
int getcmd(char *, int *); //Returns int and letter for cases
void debug(); //Opens debug
void loadFile(char *,char *); //Not Implemented yet
void dump(char *, char *); //Dump file
void exe(char *); //Execute loaded file
void split(char *, char*, char*, char*, int*); //Splits Line into words






int toHex(int n); //Converts decimal to hex

void addLabel(); //Adds label to the Struct
bool labelIsFound(); //Checks if there is a label

void error(); //Show error prompt 
void help(); //Show help prompt

FILE *source, *intermediate, *opcode, *symbolTable; //files used //////////DONT FORGET TO ADD THEM//////////

//these are the labels       is this format allowed?
typedef struct{
	char label[10];
	int address;  
}LABELS; //this is the type name that will be used


LABELS lab[500]; //size
int labelsCounter = 0; //how many labels in the system

//allows use of enums ish
typedef struct {
	char *label;
	char *mnemonic; //pointers
	char *operand;
} TOKEN; //this is the type name that will be used

TOKEN token; //declaring the name of the token type

typedef struct {
	char OP[7];
	short FORM;
} OPCODE; //this is the type name that will be used

//short is in hex
OPCODE opcode[] = {{"ADD", 18}, {"AND", 58}, {"COMP", 28}, {"DIV", 24}, {"J", 3}, {"JEQ", 3}, {"JGT", 3}, {"JLT", 3},
				{"JSUB", 3}, {"LDA", 3}, {"LDCH", 3}, {"LDL", 3}, {"LDX", 3}, {"MUL", 3}, {"OR", 3}, {"RD", 3},
                {"RSUB", 3}, {"STA", 3}, {"STCH", 3}, {"STL", 3}, {"STX", 3}, {"SUB", 3}, {"TD", 3}, {"TIX", 3}, {"WD", 3}};

				
				
				
				
int main()
{
	//can use this to define enums in c ALL CAPS BECUASE CASES
	int len, n;
	typedef enum {ERROR = -1,LOAD,EXECUTE,DEBUG,DUMP,DIRECTORY,ASSEMBLE,HELP,EXIT} COMMAND;  
	char ans[80], cmd[50], param1[50], param2[50];
	char buff[255]; //buffer to make sure they dont go over
	
	
	
	
	//Loops Until user enters "Exit"
	while(true) 
	{
		
		printf("Enter your command:  ");
		fgets(ans,80, stdin); //Gets input from user

		len = strlen(ans) - 1;
		
		//checks if they did not enter anything
		if(ans[len] == '\n') 
			ans[len] = '\0';

		split(ans,cmd,param1,param2,&n); //splits answer into words command, parameter1, parameter2
		
		
		//gets command using cases
		switch(getcmd(cmd, &n)) 
		{
			case LOAD:
				loadFile(param1,buff);
				break;
				
			case EXECUTE:
				exe(buff);
				break;
				
			case DEBUG:
				debug();
				break;
				
			case DUMP:
				dump(param1,param2);
				break;
				
			case HELP:
				help();
				break;
				
			case ASSEMBLE:
				//DOES NOTHING
				break;
				
			case DIRECTORY:
				system("ls");
				break;
				
			case EXIT:
				exit(0);
				break;
				
			case ERROR:
				error();
				break;
				
			default:
				printf("'%s': is not understood, type 'help' for info.\n", cmd);
				break;
		}
	}

	return 0;
}




int toHex(int n)
{   
    // char array stores hexadimal numbers
    char hexaDeciNum[100];
    int i = 0;
	
    while(n!=0)
    {   
        // temporary variable to store remainder
        int temp  = 0;
         
        // remainder
        temp = n % 16;
         
        if(temp < 10)
        {
            hexaDeciNum[i] = temp + 48;
            i++;
        }
		
        else
        {
            hexaDeciNum[i] = temp + 55;
            i++;
        }
         
        n = n/16;
    }
	
    //need to reverse the array
    char tempArray[100];
	int tempIndex = 0;
	int length = strlen(hexaDeciNum) - 1;
	
    while(length >= 0)
    {
    	tempArray[tempIndex++] = hexaDeciNum[length--];
    }
	
    return (int)strtol(temp, NULL, 10);
}





void split(char *str, char *c, char *param1, char *param2, int *n) //splits the answer into words
{
	bool param1 = false, param2 = false, cmd = false;
	c[0] = param1[0] = param2[0] = '\0'; //initializes with empty characters
	int indexCtr = 0, count = 1; //used to keep track of and manage the index pointer
	
	//Loops through the entire line
	//80 because of the size of the buffer
	for(int i = 0; i < 80; i++)
	{
		if(str[i] == '\n' || str[i] == '\0')
			{break;} //breaks if the next charcter is a new line 
		
		if(str[i] == ' ') //checks for space
		{
			if(c[0] != '\0')
			{cmd = true;} // if array is empty
		
			if(param1[0] != '\0')
			{param1 = true;}// if param1 is epmty
		
			if(param2[0] != '\0')
			{param2 = true;}// if param2 is empty
		
			indexCtr = 0;//reset index
			
			continue;//Continues with the loop
		}

		else if(str[i] != ' ') //if current is not a space, then do ....
		{
			if(str[i + 1] == ' ' && str[i + 2] != ' ') //checks for forward spaces
				count++;
				
			if(cmd == false) // goes into c 
			{
				c[indexCtr] = str[i];
				indexCtr++;
				c[indexCtr] = '\0';
				continue;
			}
			
			if(param1 == false) // goes into param1
			{
				param1[indexCtr] = str[i];
				indexCtr++;
				param1[indexCtr] = '\0';
				continue;
			}
			
			if(param2 == false) // Goes into param2
			{
				param2[indexCtr] = str[i];
				indexCtr++;
				param2[indexCtr] = '\0';
				continue;
			}
		}		
	}
	
	*n = count; //being used to count
}






//Return an int for cases in the command prompt
int getcmd(char *c, int *n) 
{
	
	//Check for correct number of parameter
	if(strcmp(c, "load") == 0)
	{
		if(*n - 1 >= 2 || *n - 1 < 1) 
			return -1;
		
		return 0;
	}
	
	else if(strcmp(c, "execute") == 0 || strcmp(c, "exe") == 0)
		return 1;
	
	else if(strcmp(c, "debug") == 0)
		return 2;
	
	else if(strcmp(c, "dump") == 0)
	{
		//Checks for correct number of parameter
		if(*n - 1 >= 3 || *n - 1 <= 1) 
			return -1; 
		
		return 3;
	}
	
	else if(strcmp(c, "help") == 0) 
		return 4;
	
	else if(strcmp(c, "assemble") == 0 || strcmp(c, "ass") == 0)
	{
		if(*n - 1 >= 2 || *n - 1 < 1) 
			return -1; //Checks for correct number of parameters
		
		return 5;
	}
	
	else if(strcmp(c, "dir") == 0 || strcmp(c, "directory") == 0) 
		return 6;
	
	else if(strcmp(c, "exit") == 0)
	{
		if(*n >1) 
			return -1;
		
		return 7;
	}

	return -2; //none met error
}






void addLabel(int locCounter)
{
	if(labelsCounter >= 500) return; //Stop adding more

	static int labelCounter = 0;
	strcpy(lab[labelCounter].label, token.label);
	lab[labelCounter++].address = locCounter;
	labelsCounter++;
}






bool labelIsFound()
{
	for(int i = 0; i < labelsCounter; i++)
	{
		if(strcmp(lab[i].label, token.label) == 0) return true;
	}
	
	return false;
}






void assemble(FILE *sTable, FILE *inter)
{
	// not implemented yet
}





//loads files
void loadFile(char *param1, char *buff) 
{
	//ITS ALL ENUMS
	typedef enum { NO_ERROR = 0, INVALID_MNEMONIC, INVALID_LABEL, INVALID_OPERAND, DUP_LABEL, MIS_START, MISS_OPERAND, TO_MANY_LABELS, PROG_LONG } ERRORS;
	int startingAddress, addressCounter;
	char line[500]; // String for each line in the files "param1"
	char *token; 	//STRTOK
	
	const char *symbols[500]; //Keep track of labels
	
	//Opening READ only FILES
	source = fopen(param1, "r");
	opcode = fopen("opcode.txt","r");
	
	
	//Opening Write Files
	intermediate = fopen("intermediate.txt","w");
	symbolTable = fopen("symbolTable.txt","w");
	
	//checks if files open
	if(source == NULL || opcode == NULL || intermediate == NULL || symbolTable == NULL) 
	{
		printf("File did not opened Correctly\n"); 
		return;
	}
	
	
	//memory allocation
	token.label = (char *) malloc(6);
	token.mnemonic = (char *) malloc(6);
	token.operand = (char *) malloc(6);
	
	//Keep track of past tokens
	char *pastToken; 
	token = line;	
	int labelCounter = 0; //Keep track of labels
	
	
	//reading from "param1"
	while(fgets(line, 500, source))
	{
		bool labelHERE = true, _Error = true;
		int _ERROR = NO_ERROR;
		
		if(line[0] == ' ' || line[0] == '\t') 
			abelFound = false;
		
		if(line[0] == '.') 
			continue;

		token = strtok(line, " \t\r\n\v\f");
		line[strcspn(line, "\n")] = '\0';

		if(labelHERE)
		{
			int meme = 0; //counter repository
			strcpy(token.label, token); //Sets label
			token = strtok(NULL, " \t\r\n\v\f"); // gets next word on the line
			
			if(labelsCounter > 0)
			{
				if(labelsCounter > 500) 
					_ERROR = TO_MANY_LABELS;
				
				if(strcmp(token.label, "START") == 0) 
					_ERROR = MIS_START;
				
				if(labelIsFound() && _ERROR == NO_ERROR)
					_ERROR = DUP_LABEL;
			}
			
			strcpy(token.mnemonic, token); //sets mnemonics
			
			token = strtok(NULL, " \t\r\n\v\f");

			strcpy(token.operand, token); //sets operands

			if(strcmp(token.mnemonic, "START") == 0) //Checks for START at the start
			{
				startingAddress = atoi(token.operand);
				addressCounter = startingAddress;
			}

			
			//label check
			if(token.label[0] < 'A' || token.label[0] > 'Z') _ERROR = INVALID_LABEL;

			//mnemonic check
			if( _ERROR == NO_ERROR)
			{
				for(int x = 0; x < DISTANCE(opcode); ++x)
				{
					if(strcmp(opcode[x].OP, token.mnemonic) == 0) 
						_Error = false;
				}
				
				if(strcmp(token.mnemonic, "START") == 0) 
					_Error = false;
				
				else if(strcmp(token.mnemonic, "END") == 0) 
					_Error = false;
				
				else if(strcmp(token.mnemonic, "BYTE") == 0) 
					_Error  = false;
				
				else if(strcmp(token.mnemonic, "WORD") == 0)
					_Error = false;
				
				else if(strcmp(token.mnemonic, "RESB") == 0)  
					_Error = false;
				
				else if(strcmp(token.mnemonic, "RESW") == 0) 
					_Error = false;
				
			}
			
			//directive check
			if(_Error && _ERROR == NO_ERROR) 
				_ERROR = INVALID_MNEMONIC;
			
			if(strcmp(token.mnemonic, "WORD") == 0)
			{
				meme = 2;
			}
			
			if(strcmp(token.mnemonic, "RESW") == 0)
			{
				meme = 3 * (int)strtol(token.operand, NULL, 10);
			}
			
			if(strcmp(token.mnemonic, "RESB") == 0)
			{
				meme = (int)strtol(token.operand, NULL, 10);
				meme = toHex(meme);
			}
			
			if(strcmp(token.mnemonic, "BYTE") == 0 && _ERROR == NO_ERROR)
			{
				if(token.operand[0] != 'X' && token.operand[0] != 'C') 
					_ERROR = INVALID_OPERAND; //this means its not AC or X
				
				else
				{
					if(token.operand[0] == 'C')
					{
						int charCount = 0;
						int index = 2;
						
						while(token.operand[index++] != '\'')
						{
							charCount++;
						}
						
						meme += charCount * 2;
					}
					
					else if(token.operand[0] == 'X')
					{
						int index = 1;
						char *hex = (char*) malloc(6); 

						int iHex = 0;

						while(token.operand[index++] != '\0')
						{
							if(token.operand[index] == '\'') 
								continue; // this is a safety net
							hex[iHex] = token.operand[index]; //takes whats inside
							iHex++;
						}
						
						int hexr = (int)strtol(hex, NULL, 16);
						meme =  hexr; //hax0r memer
					}
				}
				
				if(token.operand[1] != '\'' ||  token.operand[strlen(token.operand)-1] != '\'')
					_ERROR = INVALID_OPERAND; //checks for missing 's
			}
			
			
			if(strcmp(token.mnemonic, "START") == 0 || strcmp(token.mnemonic, "RESB") == 0 || strcmp(token.mnemonic, "RESW") == 0)
			{
				for(int i = 0; i < strlen(token.operand); ++i)
				{
					if(!isdigit(token.operand[i])) //I DONT KNOW THIS COMMAND
					{
					    _ERROR = INVALID_OPERAND;
						break;
					}
				}
			}
			
			
			//intermediate
			fprintf(intermediate, "%d\t %s\t %s\t %s\t %d\n", addressCounter, token.label, token.mnemonic, token.operand, _ERROR);
			fprintf(symbolTable, "%d\t %s\n", addressCounter, token.label);
			
			//adds labels
			addLabel(addressCounter);
			
			//Incrementation
			if(strcmp(token.mnemonic, "BYTE") == 0) 
				addressCounter += meme;
			
			else if(strcmp(token.mnemonic, "RESB") == 0) 
				addressCounter += meme;
			
			else if(strcmp(token.mnemonic, "RESW") == 0) 
				addressCounter += meme;
			
			else if(strcmp(token.mnemonic, "WORD") == 0) 
				addressCounter += meme;
			
			else if(strcmp(token.mnemonic, "START") > 0 || strcmp(token.mnemonic, "START") < 0) 
				addressCounter += 3;


		}
		
		else 
		{
			if(strcmp(token, "BYTE") == 0 || strcmp(token, "START") == 0 || strcmp(token, "WORD") == 0 || strcmp(token, "RESB") == 0 || strcmp(token, "RESW") == 0) _ERROR = INVALID_LABEL;	
			if(strcmp(token, "RSUB") == 0)
			{
				fprintf(intermediate, "%d\t\t %s\t\t %d\n", addressCounter, token, _ERROR);
				addressCounter += 3;
				continue; //continues on the loop
			}

			strcpy(token.mnemonic, token);
			token = strtok(NULL, " \t\r\n\v\f");
			
			if(token == NULL)
			{
				 _ERROR = MISS_OPERAND;
				strcpy(token.operand, " ");	
			}
			
			else 
				strcpy(token.operand, token);
			
			
			
			
			//THESE ARE THE ERRORS
			if(_Error && _ERROR == NO_ERROR)
			{
				for(int x = 0; x < DISTANCE(opcode); ++x)
				{
					if(strcmp(opcode[x].OP, token.mnemonic) == 0) //same ones
					{
						_Error = false;
						break;
					}
				}
				
				if(strcmp(token.mnemonic, "START") == 0)
					_Error = false;
				else if(strcmp(token.mnemonic, "END") == 0)
					_Error = false;
				else if(strcmp(token.mnemonic, "BYTE") == 0) 
					_Error  = false;
				else if(strcmp(token.mnemonic, "WORD") == 0) 
					_Error = false;
				else if(strcmp(token.mnemonic, "RESB") == 0)  
					_Error = false;
				else if(strcmp(token.mnemonic, "RESW") == 0)  
					_Error = false;
			}

			if(_Error) //OBFUSCATED CODE
				_ERROR = INVALID_MNEMONIC;
			
			if(strcmp(token.operand, "BYTE") == 0 && _ERROR == NO_ERROR) //counter has been added
			{
				if(token.operand[1] != '\'' || token.operand[strlen(token.operand) - 1] != '\'') //THIS CHECKS FOR THE ''''''''''s
					_ERROR = INVALID_OPERAND;
			}

			fprintf(intermediate, "%d\t\t %s\t %s\t %d \n", addressCounter, token.mnemonic, token.operand, _ERROR);
			addressCounter += 3;
		}
		//this is an assembler call set for phase 3, it doesnt do anything right now
		assemble(symbolTable, intermediate);

	}

	fclose(intermediate);
	fclose(source);
	fclose(opcode);
	fclose(symbolTable);

}


void exe(char *buff) //Executes Loaded File
{
	printf("Executing File\n");
	printf("%s\n",buff);
}


void debug() //open in debug mode
{
	printf("Opening in debug Mode\n");
}


void dump(char *param1, char *param2) //Dump files
{
	printf("Dumping\n");
}


void error() //Shows Error
{
	printf("Error, Try Again.\n");
}


void help() //Shows Available commads
{
	printf("load [filename]\nexecute\ndebug\ndump [start] [end]\nassemble [filename]\n");
}
