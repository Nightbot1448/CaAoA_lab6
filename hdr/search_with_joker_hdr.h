#ifndef H_STEP2_H
#define H_STEP2_H
#include "includes.h"

std::vector<int> search_with_joker(std::string text, std::string jokerPattern, char joker);


struct Bohr_vertex_J
{
	Bohr_vertex_J(char symbol, int parrent_link);

    std::map<char,int> next_vertices;
	std::map<char,int> moves;
	std::vector<int> pattern_number;
	int suffix_link ;
	int correct_suffix_link;
	int parrent_link;
	bool is_end_of_pattern;
	char symbol;
};

struct BohrTree_J
{
	BohrTree_J(size_t size);

	void getParseStrApart(std::stringstream &pattern_stream, char joker);

	void addStrToBohr(const std::string &pattern);

	int getSuffixLink(int vertexNum);

	int getAutoMove(int vertexNum, char symbol);

	int getCorrectSuffixLink(int vertexNum);

	void check(int vertexNum, int i);

	void findIn(const std::string &text);

	//data
	std::vector<Bohr_vertex_J> bohr;
	std::vector<int> lengths;
	std::vector<int> count;
	std::vector<std::string> patterns;
};

#endif //H_STEP2_H