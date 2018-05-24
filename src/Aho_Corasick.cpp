#include "../hdr/Aho_Corasick_hdr.h"

int main()
{
	std::wstring text;
	size_t number_of_patterns;
	std::wcin >> text;
	std::wcin >> number_of_patterns;
	std::vector<std::wstring> patterns(number_of_patterns);
	for (size_t i = 0; i < number_of_patterns; i++)
	{
		std::wcin >> patterns[i];
	}
	std::vector<res_pair > results = Aho_Corasick(text, patterns, number_of_patterns); // typedef in Aho_Corasick_hdr
	for (size_t i = 0; i < results.size(); i++)
		std::cout << results[i].first << " " << results[i].second << std::endl;
	return 0;
}