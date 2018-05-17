#ifndef H_STEP2_H
#define H_STEP2_H
#include "includes.h"

void strToAlphabet(std::string &str, char joker);
void getParseStrApart(std::vector<std::string> &patterns, 
					  std::vector<size_t> &lengths, 
					  std::string &jokerPattern);
std::vector<int> search_with_joker(std::string text, std::string jokerPattern, char joker);


struct Bohr_vertex_J
{
	Bohr_vertex_J(char symbol, int parrent_link = -1);
	
	int next_vertices[ALPHABET_SIZE];
	int moves[ALPHABET_SIZE];
	int suffix_link;
	int correct_suffix_link;
	std::vector<int> pattern_number;
	int parrent_link;
	bool is_end_of_pattern;
	char symbol;

};

struct BohrTree_J
{
	BohrTree_J();

	void addStrToBohr(const std::string &pattern, int numberOfPattern);

	int getSuffixLink(int vertexNum);

	int getAutoMove(int vertexNum, char symbol);

	int getCorrectSuffixLink(int vertexNum);

	void check(int vertexNum, int i, 
			   std::vector<std::string> &patterns, 
			   std::vector<int> &results,
			   std::vector<int> &background, 
			   std::vector<size_t> &lengths, 
			   size_t jokerLength);

	std::vector<int> findIn(const std::string &text, 
								  std::vector<std::string> &patterns,
								  std::vector<size_t> &lengths, 
								  size_t jokerLength);

	//data
	std::vector<Bohr_vertex_J> bohr;
};

#endif //H_STEP2_H