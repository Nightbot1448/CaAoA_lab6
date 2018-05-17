#include <fstream>
#include "../hdr/search_with_joker_hdr.h"


int main()
{
	std::string text;
	std::string joker_pattern;
	char joker;
	std::cin >> text >> joker_pattern >> joker;

	std::vector<int> results = search_with_joker(text, joker_pattern, joker);

	std::sort(results.begin(), results.end());

	for (size_t i = 0; i < results.size(); i++)
		std::cout << results[i] << std::endl;

	return 0;
}