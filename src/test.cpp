#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <string.h>
#include <fstream>

#define ALPHABET_SIZE 6

enum Alphabet {A, C, G, T, N, UNDEF};

struct Bohr_vertex
{
public:
	Bohr_vertex(char symbol, int parrent_link = -1) : parrent_link(parrent_link), symbol(symbol), is_end_of_pattern(false)
	{
		memset(this->next_vertices, 255,
			sizeof(this->next_vertices) + sizeof(this->moves)+(sizeof(int)*2));
	}

	int next_vertices[ALPHABET_SIZE];
	int moves[ALPHABET_SIZE];
	int suffix_link;
	int correct_suffix_link;
	std::vector<int> pattern_number;
	int parrent_link;
	bool is_end_of_pattern;
	char symbol;

};

struct BohrTree
{
	BohrTree()
	{
		bohr.push_back(Bohr_vertex(UNDEF));
	}

	void addStrToBohr(const std::string &pattern, int numberOfPattern)
	{
		int vertexNum = 0;

		for (size_t i = 0; i < pattern.size(); i++)
		{
			if (bohr[vertexNum].next_vertices[pattern[i]] == -1)
			{
				bohr.push_back(Bohr_vertex(pattern[i], vertexNum));
				bohr[vertexNum].next_vertices[pattern[i]] = bohr.size() - 1;
			}
			vertexNum = bohr[vertexNum].next_vertices[pattern[i]];

		}

		bohr[vertexNum].pattern_number.push_back(numberOfPattern);
		bohr[vertexNum].is_end_of_pattern = true;

	}

	int getSuffixLink(int vertexNum)
	{
		if (bohr[vertexNum].suffix_link == -1)
		{
			if (vertexNum == 0 || bohr[vertexNum].parrent_link == 0)
				bohr[vertexNum].suffix_link = 0;
			else
				bohr[vertexNum].suffix_link = getAutoMove(getSuffixLink(bohr[vertexNum].parrent_link), bohr[vertexNum].symbol);
		}

		return bohr[vertexNum].suffix_link;
	}

	int getAutoMove(int vertexNum, char symbol)
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

	int getCorrectSuffixLink(int vertexNum)
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

	void check(int vertexNum, int i, 
			   std::vector<std::string> &patterns, 
			   std::vector<int> &results,
			   std::vector<int> &background, 
			   std::vector<size_t> &lengths, 
			   size_t jokerLength)
	{
		for (int nextVertex = vertexNum; nextVertex != 0; nextVertex = getCorrectSuffixLink(nextVertex))
		{
			if (bohr[nextVertex].is_end_of_pattern)
			{	
				for (int m = 0; m < bohr[nextVertex].pattern_number.size(); m++)
				{
					if((int)(i-lengths[bohr[nextVertex].pattern_number[m]]) >= 0)
						if ((++background[i-lengths[bohr[nextVertex].pattern_number[m]]]) == patterns.size())
						{
							if(background.size()-(i - lengths[bohr[nextVertex].pattern_number[m]]) >= jokerLength)
								results.push_back(i - lengths[bohr[nextVertex].pattern_number[m]] + 1);
						}
				}
			}
		}
	}

	std::vector<int> findIn(const std::string &text, 
								  std::vector<std::string> &patterns,
								  std::vector<size_t> &lengths, 
								  size_t jokerLength)
	{
		std::vector<int> results;
		std::vector<int> background(text.size(), 0);
		int vertexNum = 0;
		
		for (size_t i = 0; i < text.size(); i++)
		{
			vertexNum = getAutoMove(vertexNum, text[i]);
			check(vertexNum, i + 1, patterns, results, background, lengths, jokerLength);
		}

		return results;
	}

	std::vector<Bohr_vertex> bohr;
};


void strToAlphabet(std::string &str, char joker)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == joker)
		{
			str[i] = UNDEF;
			continue;
		}

		switch (str[i])
		{
		case 'A': { str[i] = A;			break; }
		case 'C': { str[i] = C;			break; }
		case 'G': { str[i] = G;			break; }
		case 'T': { str[i] = T;			break; }
		case 'N': { str[i] = N;			break; }
		default:  { break; }
		}
	}
}


void getParseStrApart(std::vector<std::string> &patterns, std::vector<size_t> &lengths, std::string &jokerPattern)
{
	std::string tmp;

	for (size_t i = 0; i < jokerPattern.size(); i++)
	{
		if (jokerPattern[i] == UNDEF)
		{
			if (tmp.size() > 0)
			{
				patterns.push_back(tmp);
				lengths.push_back(i);
				tmp.clear();
			}
			continue;
		}
		else
			tmp.push_back(jokerPattern[i]);

		if ((i == (jokerPattern.size() - 1)) && tmp.size())
		{
			patterns.push_back(tmp);
			lengths.push_back(i + 1);
		}
	}
}

std::vector<int> startAhoCorasikSearch(std::string text, std::string jokerPattern, char joker)
{
	strToAlphabet(text, joker);
	strToAlphabet(jokerPattern, joker);

	std::vector<std::string> patterns;
	std::vector<size_t> lengths;
	getParseStrApart(patterns, lengths, jokerPattern);

	BohrTree bohrTree;
	for (size_t i = 0; i<patterns.size(); i++)
		bohrTree.addStrToBohr(patterns[i], i);

	return bohrTree.findIn(text, patterns, lengths, jokerPattern.size());
}

int main()
{
	std::ifstream fin("input.txt");
	std::string text;
	std::string jokerPattern;
	char joker;
	fin >> text >> jokerPattern >> joker;

	std::vector<int> results = startAhoCorasikSearch(text, jokerPattern, joker);

	std::sort(results.begin(), results.end());

	for (size_t i = 0; i < results.size(); i++)
		std::cout << results[i] << std::endl;

	return 0;
}
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <string.h>
#include <fstream>

// using namespace std;

#define ALPHABET_SIZE 6

enum Alphabet {A, C, G, T, N, UNDEF};

struct Bohr_Vertex
{
	Bohr_Vertex(char symbol, int parrent_link = -1) : parrent_link(parrent_link),
													  symbol(symbol), 
													  is_end_of_pattern(false)
	{
		memset(this->next_vertices, 255,
			sizeof(this->next_vertices) + sizeof(this->moves)+(sizeof(int)*2));
	}
	int next_vertices[ALPHABET_SIZE];
	int moves[ALPHABET_SIZE];
	int suffix_link;
	int correct_suffix_link;
	std::vector<int> pattern_number;
	int parrent_link;
	bool is_end_of_pattern;
	char symbol;

};

void strToAlphabet(std::string &str, char joker)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == joker)
		{
			str[i] = UNDEF;
			continue;
		}
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

			break;
		}
	}
}

void addStrToBohr(const std::string &pattern, int numberOfPattern, std::vector<Bohr_Vertex> &bohr)
{
	int vertexNum = 0;

	for (size_t i = 0; i < pattern.size(); i++)
	{
		if (bohr[vertexNum].next_vertices[pattern[i]] == -1)
		{
			bohr.push_back(Bohr_Vertex(pattern[i], vertexNum));
			bohr[vertexNum].next_vertices[pattern[i]] = bohr.size() - 1;
		}

		vertexNum = bohr[vertexNum].next_vertices[pattern[i]];

	}

	bohr[vertexNum].pattern_number.push_back(numberOfPattern);
	bohr[vertexNum].is_end_of_pattern = true;

}

int getAutoMove(int vertexNum, char symbol, std::vector<Bohr_Vertex> &bohr);

int getSuffixLink(int vertexNum, std::vector<Bohr_Vertex> &bohr)
{
	if (bohr[vertexNum].suffix_link == -1)
	{
		if (vertexNum == 0 || bohr[vertexNum].parrent_link == 0)
			bohr[vertexNum].suffix_link = 0;
		else
			bohr[vertexNum].suffix_link = getAutoMove(getSuffixLink(bohr[vertexNum].parrent_link, bohr), bohr[vertexNum].symbol, bohr);
	}

	return bohr[vertexNum].suffix_link;
}

int getAutoMove(int vertexNum, char symbol, std::vector<Bohr_Vertex> &bohr)
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
				bohr[vertexNum].moves[symbol] = getAutoMove(getSuffixLink(vertexNum, bohr), symbol, bohr);
		}
	}
	return bohr[vertexNum].moves[symbol];
}

int getCorrectSuffixLink(int vertexNum, std::vector<Bohr_Vertex> &bohr)
{
	if (bohr[vertexNum].correct_suffix_link == -1)
	{
		int tmpLink = getSuffixLink(vertexNum, bohr);
		if (tmpLink == 0)
			bohr[vertexNum].correct_suffix_link = 0;
		else
		{
			if (bohr[tmpLink].is_end_of_pattern == true)
				bohr[vertexNum].correct_suffix_link = tmpLink;
			else
				bohr[vertexNum].correct_suffix_link = getCorrectSuffixLink(tmpLink, bohr);
		}
	}
	return bohr[vertexNum].correct_suffix_link;
}

void check(int vertexNum, int i, std::vector<std::string> &patterns, std::vector<Bohr_Vertex> &bohr,
	std::vector<int> &results, std::vector<int> &background, std::vector<size_t> &lengths, size_t jokerLength)
{
	for (int nextVertex = vertexNum; nextVertex != 0; nextVertex = getCorrectSuffixLink(nextVertex, bohr))
	{
		if (bohr[nextVertex].is_end_of_pattern)
		{	
			for (int m = 0; m < bohr[nextVertex].pattern_number.size(); m++)
			{
				if((int)(i-lengths[bohr[nextVertex].pattern_number[m]]) >= 0)
					if ((++background[i - lengths[bohr[nextVertex].pattern_number[m]]]) == patterns.size())
					{
						if(background.size()-(i - lengths[bohr[nextVertex].pattern_number[m]]) >= jokerLength)
							results.push_back(i - lengths[bohr[nextVertex].pattern_number[m]] + 1);
					}
			}
		}
	}
}

std::vector<int> searchByBohr(const std::string &text, std::vector<std::string> &patterns,
	std::vector<size_t> &lengths, std::vector<Bohr_Vertex> &bohr, size_t jokerLength)
{
	std::vector<int> results;
	std::vector<int> background(text.size(), 0);
	int vertexNum = 0;
	
	for (size_t i = 0; i < text.size(); i++)
	{
		vertexNum = getAutoMove(vertexNum, text[i], bohr);
		check(vertexNum, i + 1, patterns, bohr, results, background, lengths, jokerLength);
	}

	return results;
}

void getParseStrApart(std::vector<std::string> &patterns, std::vector<size_t> &lengths, std::string &jokerPattern)
{
	std::string tmp;

	for (size_t i = 0; i < jokerPattern.size(); i++)
	{
		if (jokerPattern[i] == UNDEF)
		{
			if (tmp.size() > 0)
			{
				patterns.push_back(tmp);
				lengths.push_back(i);
				tmp.clear();
			}
			continue;
		}
		else
			tmp.push_back(jokerPattern[i]);

		if ((i == (jokerPattern.size() - 1)) && tmp.size())
		{
			patterns.push_back(tmp);
			lengths.push_back(i + 1);
		}
	}
}

std::vector<int> startAhoCorasikSearch(std::string text, std::string jokerPattern, char joker)
{
	strToAlphabet(text, joker);
	strToAlphabet(jokerPattern, joker);

	std::vector<std::string> patterns;
	std::vector<size_t> lengths;
	getParseStrApart(patterns, lengths, jokerPattern);

	std::vector<Bohr_Vertex> bohrTree;
	bohrTree.push_back(Bohr_Vertex(UNDEF));
	for (size_t i = 0; i<patterns.size(); i++)
		addStrToBohr(patterns[i], i, bohrTree);

	return searchByBohr(text, patterns, lengths, bohrTree, jokerPattern.size());
}

int main()
{
	std::ifstream fin("input.txt");
	std::string text;
	std::string jokerPattern;
	char joker;
	fin >> text >> jokerPattern >> joker;

	std::vector<int> results = startAhoCorasikSearch(text, jokerPattern, joker);

	std::sort(results.begin(), results.end());

	for (size_t i = 0; i < results.size(); i++)
		std::cout << results[i] << std::endl;

	return 0;
}
