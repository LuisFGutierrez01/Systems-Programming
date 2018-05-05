
#include <stdio.h>
#include <string.h> //for strlen() and strcmp()
#include <stdbool.h> // for booleans
#include <stdlib.h> //for system commands


int main(){

char entry[200]; //size user can enter
char command[20]; //1st entry / command TODO do they go out of index?
char v1[20]; //1st paramater
char v2[20]; //2nd paramater
int length = 0; //length of user entry
int index = 0; 
int count = 0; //how many entries
int commandcount = -1;

void breakup(char *, char *, char *, char *, int *, int *); //splits up the words
void clearout(char *, char *, char *); //puts nulls on every array part
int getcmd(char *, char *, char *);

	//continue the program unil user types "exit"
	while(true)
	{
		count  = 0;
		printf("\nEnter your command: ");
		
			
		//gets entry from the user
		fgets(entry, 100, stdin); 
		length = strlen(entry) -1;
		
		//empties out the words first
		clearout(command, v1, v2);




		//splits inputs and places them in arrays
		breakup(entry, command, v1, v2, &count, &length);

		//gets commands from the user imput and spit out a number and runs it like an enum
		// TODO the only fully working ones are help, directory and exit
		commandcount = getcmd(command, v1, v2);
		
		if (commandcount == 1)
		{
			printf("loading...\n");
		}

		else if (commandcount == 2)
		{
			printf("executing...\n");
		}

		else if (commandcount == 3)
		{
			printf("debugging...\n");		
		}

		else if (commandcount == 4)
		{
			printf("dumping...\n");		
		}

		else if (commandcount == 5)
		{
			printf("Available comamnds: load <filename>, execute, debug, dump <start> <end>, help, assemble <filename>, directory, exit\n"); //list of help instructions
		}

		else if (commandcount == 6)
		{
			printf("assembling...\n");
		}

		else if (commandcount == 7)
		{
			system("ls"); //directory
		}

		else if (commandcount == 8)
		{
			printf("exiting...\n");
			exit(0); //exits the program
		}

		else if (commandcount == -1)
		{
			//added beccause the first pass could possibly give an enror
			//"its just smart and knows what you wanted to type *cough cough*"
			//totally intentional
			if (command[8] == 'y' && command[0] == 'd' && command[4] == 'c')
			{
				if ( v1[0] != '\0' || v2[0] != '\0')
				{
					printf("Error. Type 'help' for available commands \n"); //hot wirez
				}
				else
				{system("ls");}
			}
			else
				{printf("Error. Type 'help' for available commands \n");} //given error if stuff goes wrong
		}

	}

 return 0;
}



//breaks up the word, inserts them into their proper charcater arrays, gives a word count
void breakup(char *entry, char *command, char *v1, char *v2, int *count, int *length)
{  
	int pointer = 0; //this tells it where to put the letters
	bool cmd = false, p1 = false, p2 = false; //this tells you if the words are filled or not

	for (int i = 0; i < 200; i++)
	{      
		if (entry[i] == '\0' || entry[i] == '\n')
			{break;} //needs to be here or else it adds null pointers.
		
		//checks if it just went over a word, if it did, it puts a null character at the end
		//otherwise it just resets the pointer to 0 and to set up for next word
		if (entry[i] ==' ') 
		{
			if (command[0] != '\0' && pointer != 0 && cmd == false) 
			{ 
				cmd = true;
				command[pointer] = '\0';
			}  				 
			if (v1[0] != '\0' && pointer != 0 && p1 == false)
			{
				p1 = true;
				v1[pointer] = '\0';
			}    
			if (v2[0] != '\0' && pointer != 0 && p2 == false)
			{
				p2 = true;
				v2[pointer] = '\0';
			}   
			pointer = 0;
		}

		//triggers if its not an empty space
		else if (entry[i] != ' ')
		{
			if (cmd == false) //first word 
			{
				command[pointer] = entry[i];
				pointer++;
			}	

			else if(p1 == false) //second word
			{
				v1[pointer] = entry[i];
				pointer++;
			}

			else if(p2 == false) //third word
			{
				v2[pointer] = entry[i];
				pointer++;
			}
		}
	}

}


//sets entire sring into \0
void clearout(char *array, char *array2, char *array3)
{
	int length = strlen(array);
	for (int i = 0; i < length; i++)
		{array[i] = '\0';}

	    length = strlen(array2);
	for (int i = 0; i < length; i++)
		{array2[i] = '\0';}

	    length = strlen(array3);
	for (int i = 0; i < length; i++)
		{array3[i] = '\0';}

}


//gets command to print out to screen
int getcmd(char *command, char *v1, char *v2)
{
	if (strcmp(command, "load") == 0)
	{	
		if (v1[0] == '\0' || v2[0] != '\0')
			{return -1;}
		return 1;
	}

	if (strcmp(command, "execute") == 0)
	{	
		if (v1[0] != '\0' || v2[0] != '\0')
			{return -1;}
		
		return 2;
	}

	if (strcmp(command, "debug") == 0)
	{	
		if (v1[0] != '\0' || v2[0] != '\0')
			{return -1;}
		
		return 3;
	}

	if (strcmp(command, "dump") == 0)
	{	
		if (v1[0] == '\0' || v2[0] == '\0')
			{return -1;}
		
		return 4;
	}

	if (strcmp(command, "help") == 0)
	{	
		if (v1[0] != '\0' || v2[0] != '\0')
			{return -1;}
		
		return 5;
	}

	if (strcmp(command, "assemble") == 0)
	{	
		if (v1[0] == '\0' || v2[0] != '\0')
			{return -1;}
		
		return 6;
	}

	if (strcmp(command, "directory") == 0)
	{	
		if (v1[0] != '\0' || v2[0] != '\0')
		{
			return -1;
		}
		
		return 7;
	}

	if (strcmp(command, "exit") == 0)
	{	
		if (v1[0] != '\0' || v2[0] != '\0')
			{return -1;}
		
		return 8;
	}



	return -1;
}











