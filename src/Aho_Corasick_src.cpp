#include "../hdr/Aho_Corasick_hdr.h"

std::vector<res_pair > Aho_Corasick(std::string text, 
									std::vector<std::string> patterns, 
									size_t number_of_patterns) // typedef in Aho_Corasick_hdr
{
	strToAlphabet(text);
	for (size_t i = 0; i < number_of_patterns; i++)
		strToAlphabet(patterns[i]);

	BohrTree_AC bohr;
	for (size_t i = 0; i<number_of_patterns; i++)
		bohr.addStrToBohr(patterns[i], i);
	std::vector<res_pair> results = bohr.findIn(text, patterns);
	std::sort(results.begin(), results.end(), compareForSort);
	return results;
}

void strToAlphabet(std::string &str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		switch (str[i])
		{
		case 'A': 
			str[i] = A; 
			break;
		case 'C':
			str[i] = C; 
			break;
		case 'G':
			str[i] = G; 
			break;
		case 'T':
			str[i] = T; 
			break;
		case 'N':
			str[i] = N; 
			break;
		default: 
			str[i] = UNDEF; 
			break;
		}
	}
}

bool compareForSort( res_pair el1, res_pair el2)
{
	if (el1.first == el2.first)
		return el1.second < el2.second;
	else
		return el1.first < el2.first;
}

Bohr_vertex_AC::Bohr_vertex_AC(char symbol, int parrent_link) : parrent_link(parrent_link), 
													  symbol(symbol), 
													  is_end_of_pattern(false)
{
	memset(this->next_vertices, 255,
	sizeof(this->next_vertices) + sizeof(this->moves)+(sizeof(int)*3));
}

BohrTree_AC::BohrTree_AC()
{
	bohr.push_back(Bohr_vertex_AC(UNDEF));
}

void BohrTree_AC::addStrToBohr(const std::string &pattern, int numberOfPattern)
{
	int vertexNum = 0;

	for (size_t i = 0; i < pattern.size(); i++)
	{
		if (bohr[vertexNum].next_vertices[pattern[i]] == -1)
		{
			bohr.push_back(Bohr_vertex_AC(pattern[i], vertexNum));
			bohr[vertexNum].next_vertices[pattern[i]] = bohr.size() - 1;
		}

		vertexNum = bohr[vertexNum].next_vertices[pattern[i]];

	}

	bohr[vertexNum].pattern_number = numberOfPattern;
	bohr[vertexNum].is_end_of_pattern = true;
}

int BohrTree_AC::getSuffixLink(int vertexNum)
{
	if (bohr[vertexNum].suffix_link == -1)
	{
		if (vertexNum == 0 || bohr[vertexNum].parrent_link == 0)
			bohr[vertexNum].suffix_link = 0;
		else
			bohr[vertexNum].suffix_link = getAutoMove(getSuffixLink(bohr[vertexNum].parrent_link), 
													  bohr[vertexNum].symbol);
	}

	return bohr[vertexNum].suffix_link;
}

int BohrTree_AC::getAutoMove(int vertexNum, char symbol)
{
	if (bohr[vertexNum].moves[symbol] == -1)
	{
		if (bohr[vertexNum].next_vertices[symbol] != -1)
			bohr[vertexNum].moves[symbol] = bohr[vertexNum].next_vertices[symbol];
		else
		{
			if (vertexNum == 0)
				bohr[vertexNum].moves[symbol] = 0;
			else
				bohr[vertexNum].moves[symbol] = getAutoMove(getSuffixLink(vertexNum), symbol);
		}
	}
	return bohr[vertexNum].moves[symbol];
}

int BohrTree_AC::getCorrectSuffixLink(int vertexNum)
{
	if (bohr[vertexNum].correct_suffix_link == -1)
	{
		int tmpLink = getSuffixLink(vertexNum);
		if (tmpLink == 0)
			bohr[vertexNum].correct_suffix_link = 0;
		else
		{
			if (bohr[tmpLink].is_end_of_pattern == true)
				bohr[vertexNum].correct_suffix_link = tmpLink;
			else
				bohr[vertexNum].correct_suffix_link = getCorrectSuffixLink(tmpLink);
		}
	}
	return bohr[vertexNum].correct_suffix_link;
}

void BohrTree_AC::check(int vertexNum, int i, 
					 std::vector <std::string> &patterns, 
					 std::vector<res_pair > &results)
{
	for (int nextVertex = vertexNum; nextVertex != 0; nextVertex = getCorrectSuffixLink(nextVertex))
	{
		if (bohr[nextVertex].is_end_of_pattern)
		{
			res_pair tmp(i - patterns[bohr[nextVertex].pattern_number].size() + 1,
						 bohr[nextVertex].pattern_number + 1);
			results.push_back(tmp);
		}
	}
}

std::vector<res_pair > BohrTree_AC::findIn(const std::string &text, std::vector <std::string> &patterns)
{
	std::vector<res_pair > results;
	int vertexNum = 0;
	for (size_t i = 0; i < text.size(); i++)
	{
		vertexNum = getAutoMove(vertexNum, text[i]);
		check(vertexNum, i + 1, patterns, results);
	}

	return results;
}