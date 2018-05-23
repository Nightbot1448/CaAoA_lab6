#ifndef H_STEP1_H
#define H_STEP1_H

#include "includes.h"

typedef std::pair<int, int> res_pair;

// void strToAlphabet(std::string &str);
bool compareForSort(res_pair el1, res_pair el2);

std::vector<res_pair > Aho_Corasick(std::string text, 
									std::vector<std::string> patterns,
									size_t number_of_patterns);

struct Bohr_vertex_AC
{
	 Bohr_vertex_AC(char symbol, int parrent_link = -1);

	 //data
	std::map<char,int> next_vertices;
	std::map<char,int> moves;
	int pattern_number;
	int suffix_link ;
	int correct_suffix_link;
	int parrent_link;
	bool is_end_of_pattern;
	char symbol;

};

struct BohrTree_AC
{
	BohrTree_AC(); 
	void addStrToBohr(const std::string &pattern, int numberOfPattern);
	int getSuffixLink(int vertexNum);
	int getAutoMove(int vertexNum, char symbol);
	int getCorrectSuffixLink(int vertexNum);
	void check(int vertexNum, int i, std::vector <std::string> &patterns, std::vector<res_pair > &results);
	std::vector<res_pair > findIn(const std::string &text, std::vector <std::string> &patterns);

	//data
	std::vector<Bohr_vertex_AC> bohr;
};

#endif //H_STEP1_H