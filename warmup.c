/*
 * Author: Markus Padourek
 *
 * Code pieces for character-by-character input reading adapted from
 * http://stackoverflow.com/a/10005522/2085007
 *
*/

#include <stdio.h>
#include <unistd.h>
#include <termios.h>

char output[11];
//TODO: fix bug when asterisks is 10th character and 11th is something else
//TODO: Cleanup code, possibly put into functions, test if EOF works on Linux

void setTerminal(){
	//maybe not necessary
}

char replaceNewLine(){
	return ' ';
}

//char replaceAsterisks(){
//
//}

int main(void){
	char buffer[2], userInput;
	buffer[1] = '\0';
	
	//iterator-variable and helper to keep track of the buffer
	int i, helper;

	//how many characters the program will output
	int outputLength = 80;

	//error number for the read() function. 0 = EOF, -1 = Error
	int errorNr = 1;

	//helper to set the terminal to read character-by-character
	struct termios old, new;

	//So printf gets printed immediately
  	setbuf(stdout, NULL);

  	  		//Set the terminal, so the program can read character-by-character in 'realtime'
  	tcgetattr(0, &old);
  	new = old;
  	new.c_lflag &=~ ICANON;
  	tcsetattr(0, TCSANOW, &new);

  	//outer loop to basically reset or 'restart' everything after characters have been output, if there is no EOF
  	while(errorNr > 0){
  		//resetting variables 
  		i = 0;
  		helper = 0;

  		//inner loop, for printing out each single time the 80 characters, if there is no EOF
	  	while (i < outputLength && (errorNr = read(STDIN_FILENO, &userInput, 1)) > 0){
	  		if(i == 0 && buffer[1] != '\0'){
	  			output[i++] = buffer[1];
	  			buffer[1] = '\0';
	  		}
	  		//simply changning a newline to a space
	  		if(userInput == '\n') userInput = replaceNewLine();

	  		//checking if asterisks.
			if(userInput == '*'){
				//checking if first and then saving into buffer
				if(helper == 0){
					buffer[helper] = userInput;
					helper++;	
				}else{
					//if second consecutive asterisks, replace both with a '^'
					userInput = '^';
					output[i++] = userInput;
					//resetting helper, since 2 asterisks have been replaced
					helper = 0;
				}
			}else{
				//if character after an asterisks is any other character print out that asterisks and reset helper
				if(helper == 1){
					output[i++] = buffer[0];
					helper = 0;
				}
				if(i < outputLength){
					output[i++] = userInput;	
				}else{
					buffer[1] = userInput;
				}	
			}
	  	}

	  	//only print the characters if there is no EOF and if there are 80.
	  	if(errorNr > 0 && i == outputLength){
	  		output[i] = '\n';
	  		//tcsetattr(0, TCSANOW, &old);
	  		printf("%s", output);
	  		output[0] = '\0';
	  	}
	}
	return 0;
}