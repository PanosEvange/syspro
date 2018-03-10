/***************************************************************************************
Header file : DocMap.h
Programmer	: PANAGIOTIS EVANGELIOU  AM:1115201500039
Description :



****************************************************************************************/
#ifndef DOC_MAP_H
#define DOC_MAP_H

class Words{

private:
		int num_of_words;
		char **words_map;
public:
		Words( int n );
		~Words();
		int InsertWord( char *word_to_insert, int index );
		int GetSize();
		bool IsWordIn( char *word_to_check );
};

class DocMap{

private:
		int size;
		char **map;
public:
		DocMap( int num_of_docs );
		~DocMap();
		int InsertDoc( char *doc_to_insert, int index );
		int PrintDoc(  Words &words_to_highlight, int index, char *special_info );
		bool IsIndexIn( int index_to_check );
		int HighlightText( char *original, char **highlighting_string, Words &words_to_highlight );
		int PrintHighlightedText( char *text, char *highlighting_string, char *special_info );
		int GetSize();
};

#endif /* DOC_MAP_H */