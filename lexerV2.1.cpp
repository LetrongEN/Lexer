#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <stdlib.h>
#include <ctype.h>
#include <numeric>
#include <iomanip>
//pre processor directives necessary for file manipulation
//since using namespace is not portable code we will be forced to
//to include std:: before every type of output manipulation
using namespace std;

vector<string> textInput;  //this vector will house info from fstream
vector<char> wordVector;   //this vector will house a string taken from
vector<string> splitLexemes;	//vector that houses split lexemes

struct Token{
    string lexeme;
    string attribute;

    void PrintToken(){
        //cout <<"Token:          Lexeme: " <<endl;
		//21 spaces apart
		cout << setw(22) << left << attribute << lexeme<< endl; 
    }
};

vector<Token> storeTokens(vector<string> Lexemes, Token entry, vector<Token> all_tokens){
    string tempLexeme;
    for(int idx = 0; idx < Lexemes.size(); idx++ ){
        tempLexeme = Lexemes[idx];

        if(tempLexeme == "int" || tempLexeme == "float"||tempLexeme == "bool" || tempLexeme == "if"|| tempLexeme == "else"|| tempLexeme == "then"
        || tempLexeme == "do"|| tempLexeme == "while"|| tempLexeme == "whileend"|| tempLexeme == "int"|| tempLexeme == "doend"|| tempLexeme == "for"
        || tempLexeme == "and"|| tempLexeme == "or"|| tempLexeme == "function"|| tempLexeme == "return"|| tempLexeme == "put"){

            entry.lexeme = tempLexeme;
            entry.attribute = "KEYWORD";

            all_tokens.push_back(entry);
            continue;
        }

        if(tempLexeme == "{"|| tempLexeme == "}"|| tempLexeme == "["|| tempLexeme == "]"|| tempLexeme == "("|| tempLexeme == "}"|| tempLexeme == ","
        || tempLexeme == "."|| tempLexeme == ";"|| tempLexeme == ":"|| tempLexeme == "!"){

            entry.lexeme = tempLexeme;
            entry.attribute = "SEPERATOR";

            all_tokens.push_back(entry);
           continue;
        }
        if(tempLexeme == "*"|| tempLexeme == "+"|| tempLexeme == "-"|| tempLexeme == "="|| tempLexeme == "/"|| tempLexeme == ">"|| tempLexeme == "<"
        || tempLexeme == "%"){

            entry.lexeme = tempLexeme;
            entry.attribute = "OPERATOR";

            all_tokens.push_back(entry);
           continue;

        }
        else
            entry.lexeme = tempLexeme;
            entry.attribute ="IDENTIFIER";

            all_tokens.push_back(entry);
			
    }

	return all_tokens;
}

bool symbolHere = false;
bool inFront = false;

int symbolIndex = 0;
string begWord;
string tempString;

void splitString(string lexeme)
{
    //will split  the string into the word vector as a vector
    //of chars
    for(int strIdx	= 0; strIdx < lexeme.length(); strIdx++)
    {
        wordVector.push_back(lexeme.at(strIdx));
    }
}
//Takes in a Character vector and finds the location of the symbol and stores the index for later use
void isSymbol(vector<char> word)
{
    int wordPosition = 0;									//Temporary variable created for index to keep track of things

    while (symbolHere == false)							//As long as the symbol hasn't been found, we go through the array
    {
        //if(isalnum(word[wordPosition]) == false && isspace(word[wordPosition] == false && word[wordPosition != '$'])) <---thois is prone to errors
        if (word[wordPosition] == '!' ||word[wordPosition] == '(' || word[wordPosition] == ')' || word[wordPosition] == ','
			|| word[wordPosition] == '{' || word[wordPosition] == '}' || word[wordPosition] == ';' || word[wordPosition] =='*'|| word[wordPosition] == '+'
			|| word[wordPosition] == '-'|| word[wordPosition] == '='|| word[wordPosition] == '/'|| word[wordPosition] == '>'
			|| word[wordPosition] == '<'|| word[wordPosition] == '%'|| word[wordPosition] == '['|| word[wordPosition] == ']'|| word[wordPosition] =='.'
			|| word[wordPosition] == ':'|| word[wordPosition] == ':')
        {
        vector<Token> all_tokens;
    if (wordPosition == 0)							//Assuming that the symbol is in the front
            {
                symbolIndex = 0;						//resets symbol index at the front of CHAR vector
                symbolHere = true;						//exits the while loop
                inFront = true;							//marks the index at the front
            }
            else
            {
                //If the symbol is anywhere else but the front, it will mark the counter
                symbolIndex = wordPosition;					//makes index the same as the couter
                symbolHere = true;						//exits out of the loop
            }
        }
        else if (wordPosition == word.size()-1)		//Assuming we iterate until the end of the array, we assume there is no actual symbol
        {
            symbolHere = true;							//exits the loop
            wordPosition = word.size();				//Just making sure the counter is equal to the size of the vector
        }
        else
        {
            //assuming that the current element is not a symbol?
            wordPosition++;									//fucking increment counter, go through loop again at next index
        }
    }

}

void shiftVector(vector<char> word)
{

    int currentWordSize = word.size();

    if(currentWordSize != 1)
    {
        for(int wIdx = 1; wIdx < word.size(); wIdx++)
        {
            wordVector[wIdx -1] = wordVector[wIdx];
        }
        wordVector.pop_back();
    }

    if(word.size() == 1)
    {
        wordVector.clear();
    }

}

//THERE is a LATENT BUG IN HERE 
void splitSymbol(vector<char> tempWord)
{

	//If the symbol exists and is the only item in the vector
	//IE !
	if (symbolHere == true && symbolIndex == 0 && inFront == true && tempWord.size() == 1)
	{

		string tempSymbolString;							//initiate temporary string for this function
		tempSymbolString.push_back(tempWord[0]);			//push the symbol into the string by itself
		splitLexemes.push_back(tempSymbolString);			//Push the string with symbol into the back of the vector

		wordVector.clear();									//clears word Vector
																//Resets data used for this
		inFront = false;
		symbolHere = false;
		symbolIndex = 0;

	}

	//If the symbol exists in the first position vector and there is more elements in the vector
	// IE: (BOOOOOOB
	else if (symbolHere == true && symbolIndex == 0 && inFront == true && tempWord.size() > 1)
	{

		string tempSymbolString;							//initiate temporary string for this function
		tempSymbolString.push_back(tempWord[0]);			//push the symbol into the string by itself
		splitLexemes.push_back(tempSymbolString);			//Push the string with symbol into the back of the vector

		shiftVector(wordVector);							//shifts the vector and deletes the last element of vector

															//Resets data used for this
		inFront = false;
		symbolHere = false;
		symbolIndex = 0;

	}

	//We can assume that the symbol exists and is not at the front, therefore let's split the bitch
	//IE Bob!Lee
	else if (symbolHere == true && symbolIndex != 0 && inFront == false)
	{
		string tempString1;							//initialize two temporary strings that will be used
		string tempString2;							//to push two "Elements" into final array

		for (int idx = 0; idx < symbolIndex; idx++)		//Will go through the CHAR vector up until the symbol index
		{
			tempString1.push_back(tempWord[idx]);		//Populate the temporary first temporary string
			shiftVector(wordVector);					//decrements and shifts CHAR vector
		}
			//Once we exit the loop, string one should be fully populated
		splitLexemes.push_back(tempString1);				//Push the string into the last vector

			//This will push the remaining symbol onto the final array
		tempString2.push_back(wordVector[0]);			//This assumes that the symbol is in the front
		splitLexemes.push_back(tempString2);				//Push the string with the symbol onto the final array
		shiftVector(wordVector);						//Shift the array and pop the back of CHAR array

			//Resets the values
		symbolHere = false;
		symbolIndex = 0;

	}

	//We assume that there is no symbol, so let's forget about it and just shove in the string into last array
	//IE: BOB
	else
	{
		string tempFString;									//Temporary string created
		for (int idx = 0; idx < tempWord.size(); idx++)		//Loop to go through the vector
		{
			tempFString.push_back(tempWord[idx]);			//Pushes the character onto the string
			shiftVector(wordVector);						//Decrements vector size 
		}
		splitLexemes.push_back(tempFString);

		//Resetting the vectors properties if called again
		symbolHere = false;
		symbolIndex = 0;
	}
	

}



int main()
{
    //include means of both reading and writing to a file
    //then include a file
    string filename;
	cout << "Please specify the File in the current Directory that you wish to manipulate :";
    cin >> filename;
   
	Token insertion; 
	vector<Token> tokenStorage;   
	ifstream inputFile;
    
	
    string codeEntry;
    //NOTE: on my raspberry pi, my computer would not specify
    //the local directory for
    //the file location, i recommend you specify the file
    //name or the directory to ensure that the correct file is
    //being read in
	inputFile.open(filename);
    // this should take in a file that the user should specify
    //we now have the file accessed and open,it is here where we
    // make sure to close the file when finished processing
    while(!inputFile.eof())
    {
        //while input file is taking in some form of inpuit
        //we will need t process teh data within the text file
        inputFile >> codeEntry;
		//issues arise with input2 for some reason?, particularly when meeting the condition of this if Statement
		if(codeEntry.front() == '!')
        {
			string commentLine;
            //this should remove any line that uses! at the beginning,
            //it is a comment operator/indicator
            getline(inputFile,commentLine);
            cout <<"we are skipping a comment line: " << codeEntry <<commentLine << endl;
            inputFile >> codeEntry;
        }
        //take the data from the text file and place it into the vector
        //to meet our lexer needs
		else
		textInput.push_back(codeEntry);
    }

    inputFile.close();

    //we are transfering strings from textInput, into splitLexemes
    for(int idx = 0; idx < textInput.size(); idx++)
    {
        //take current index into a split function
        string currentLexeme = textInput[idx];

        //takes string, and will place it into the word vector
        splitString(currentLexeme);

        while(wordVector.empty() == false)
        {
            isSymbol(wordVector);
			//error occurs in split symbol, debug it from here
            splitSymbol(wordVector);
        }

    }

   tokenStorage = storeTokens(splitLexemes, insertion, tokenStorage);

   cout << " Token               Lexeme " << endl;
   for(int idx = 0; idx < splitLexemes.size(); idx++)
    {   //read the final; vector and compare it to identify what kind of lexeme it is
        //cout << splitLexemes[idx] << endl;  
		tokenStorage[idx].PrintToken();
    }


    system("pause");
}

