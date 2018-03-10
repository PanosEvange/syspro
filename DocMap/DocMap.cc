/**************************************************************************
Source file	: DocMap.cc
Programmer	: PANAGIOTIS EVANGELIOU  AM:1115201500039
Description	: 		Words
			  > InsertWord :
			  > GetSize :
			  > IsWordIn :
			  >


					DocMap
			  > InsertDoc :
			  > PrintDoc :
			  > IsIndexIn :
			  > HighlightText :
			  > PrintHighlightedText :
			  > GetSize :
			  >
			  >
***************************************************************************/

#include <cstring>
#include <sys/ioctl.h>
#include <iostream>

#include "DocMap.h"

using namespace std;

/* Initialize Words data structure */
Words::Words( int n )
:num_of_words(n)
{

	words_map = new char* [ n ];

	for( int i = 0; i < num_of_words; i++ ){
		words_map[i] = NULL;
	}

}

// Words::Words( const Words &words_to_copy ){
//
// 	num_of_words = words_to_copy.GetSize();
// 	words_map = new char* [ num_of_words ];
//
// 	for( int i = 0; i < num_of_words; i++ ){
// 		words_map[i] = NULL;
// 	}
//
// }


/* Destroy Words data structure */
Words::~Words(){

	for( int i = 0; i < num_of_words; i++ ){
		if( words_map[i] != NULL ){
			delete[] words_map[i];
		}
	}

	delete[] words_map;

}

/* Insert word in words_map at index position */
int Words::InsertWord( char *word_to_insert, int index ){

	if( (index < 0) || (index >= num_of_words) ){
		return -1;
	}

	if( words_map[index] != NULL ){
		return -2;
	}

	words_map[index] = new char [ strlen(word_to_insert) + 1 ];

	strcpy(words_map[index],word_to_insert);

	return 1;

}

/* Return size of words_map */
int Words::GetSize(){
	return num_of_words;
}

/* Checking if word_to_check is in words_map */
bool Words::IsWordIn( char *word_to_check ){

	for( int i = 0; i < num_of_words; i++ ){
		if( words_map[i] != NULL ){
			if( strcmp(words_map[i],word_to_check) == 0 ){
				return true;
			}
		}
	}

	return false;
}

/* Initialize DocMap data structure */
DocMap::DocMap( int num_of_docs )
:size(num_of_docs)
{

	map = new char* [ size ];

	for( int i = 0; i < size; i++ ){
		map[i] = NULL;
	}

}

/* Destroy DocMap data structure */
DocMap::~DocMap(){

	for( int i = 0; i < size; i++ ){
		if( map[i] != NULL ){
			delete[] map[i];
		}
	}

	delete[] map;

}

/* Insert given doc_to_insert document at index position */
int DocMap::InsertDoc( char *doc_to_insert, int index ){

	if( !IsIndexIn(index) ){
		return -1;
	}

	if( map[index] == NULL ){

		map[index] = new char[strlen(doc_to_insert) + 1];
		strcpy(map[index],doc_to_insert);

	}
	else{
		return -2;
	}

	return 1;
}

/* Print document with index id and highlight the words_to_highlight Words */
int DocMap::PrintDoc( Words &words_to_highlight, int index, char *special_info ){

	char *highlighting_string = NULL;

	if( !IsIndexIn(index) ){
		return -1;
	}

	if( HighlightText(map[index], &highlighting_string, words_to_highlight) != 1 ){
		return -2;
	}

	if( PrintHighlightedText( map[index], highlighting_string , special_info) != 1 ){
		return -4;
	}

	delete[] highlighting_string;

	return 1;
}

/* Checking if index is in the range of valid indexes */
bool DocMap::IsIndexIn( int index_to_check ){
	return ( (index_to_check >= 0) && (index_to_check < size) );
}

/* Constructing highlighting_string given original string, words to highlight and special info string */
int DocMap::HighlightText( char *original, char **highlighting_string, Words &words_to_highlight ){

	char *word;
	int offset,offset_original;
	char *temp = NULL;

	if( (*highlighting_string) != NULL ){
		return -1;
	}

	(*highlighting_string) = new char[strlen(original) + 1];

	/* We need a copy of document as we will use strtok. */
	temp = new char[strlen(original) + 1];
	strcpy(temp,original);

	word = strtok(temp," \t");

	offset = 0;
	offset_original = 0;

	while( word != NULL ){

		/* Copy spaces or tabs till we find the beginning of the word */
		while( (original[offset_original] == ' ') || (original[offset_original] == '\t') ){
			memset((*highlighting_string) + offset,original[offset_original],1);
			offset++;
			offset_original++;
		}

		if( words_to_highlight.IsWordIn(word) ){ /* Write ^^^^ */

			memset((*highlighting_string) + offset,'^',strlen(word));
			offset += strlen(word);
			offset_original += strlen(word);

		}
		else{ /* Write spaces */

			memset((*highlighting_string) + offset,' ',strlen(word));
			offset += strlen(word);
			offset_original += strlen(word);

		}

		word = strtok(NULL," \t");

	}

	/* Write spaces/tabs which may exists after the last word of the document */
	if( offset < ( (int) strlen(original) ) ){
		memset((*highlighting_string) + offset,' ',strlen(original) - offset );
	}

	(*highlighting_string)[strlen(original)] = '\0';

	delete[] temp;

	return 1;
}

/* Printing document text and special info at the beginning, with ^^^ above the words we need to mark.*/
int DocMap::PrintHighlightedText( char *text, char *highlighting_string, char *special_info ){

	struct winsize ws;
	int terminal_width;
	int counter_original,counter_highlight,counter_width;
	bool first_time = true;
	char *temp = NULL;
	char *word;

	if( ioctl(1,TIOCGWINSZ,&ws) == 0 ){ /* File descriptor of STDOUT is 1 */
		terminal_width = ws.ws_col;
	}
	else{
		return -1;
	}

	/* We need a copy of document as we will use strtok. */
	temp = new char[strlen(text) + 1];

	strcpy(temp,text);

	word = strtok(temp," \t");

	cout << "terminal_width is " << terminal_width << endl;

	counter_width = 0;
	counter_original = 0;
	counter_highlight = 0;

	/* Print special_info */
	cout << special_info ;

	counter_width += strlen(special_info);

	while( word != NULL ){

		if( !first_time ){

			/* Print strlen(special_info) spaces so as to have alignment. */
			for( int i=0; i<( (int) strlen(special_info) ); i++ ){
				cout << " ";
			}

			counter_width += strlen(special_info);

		}

		/* Print words or spaces/tabs of original text */
		while( (counter_original < ( (int)  strlen(text) ) ) && (counter_width < terminal_width) && (word != NULL) ){

			/* Print spaces or tabs till we find the beginning of the word */
			while( ( (text[counter_original] == ' ') || (text[counter_original] == '\t') ) && (counter_width < terminal_width) ){
				cout << text[counter_original];
				counter_width++;
				counter_original++;
			}

			/* Print word if there is space for it, else break */
			if( ( ( (int) strlen(word) ) + counter_width) <= terminal_width ){
				cout << word;
				counter_original += strlen(word);
				counter_width += strlen(word);
				word = strtok(NULL," \t");
			}
			else{
				break;
			}

		}

		cout << endl;

		counter_width = 0;

		/* Print strlen(special_info) spaces so as to have alignment. */
		for( int i=0; i<( (int) strlen(special_info) ); i++ ){
			cout << " ";
		}

		counter_width += strlen(special_info);

		for( int i=counter_highlight; i<counter_original; i++ ){
			cout << highlighting_string[counter_highlight];
			counter_highlight ++;
			counter_width ++;
		}

		cout << endl;
		counter_width = 0;

		first_time = false;
	}

	delete[] temp;

	return 1;

}

/* Return size of DocMap data structure */
int DocMap::GetSize(){
	return size;
}