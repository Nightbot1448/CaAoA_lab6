#include <gtest/gtest.h>
#include "../hdr/Aho_Corasick_hdr.h"
#include "../hdr/search_with_joker_hdr.h"

typedef std::vector<std::pair<int, int> > res_vect_type;
typedef std::vector<std::wstring> vec_of_str;


TEST(Aho_Corasick_T, Incomplete)
{
	res_vect_type res_vect;
	EXPECT_EQ(Aho_Corasick(L"CCCA", vec_of_str({L"AC"}), 1), res_vect);
}

TEST(Aho_Corasick_T, Simple)
{
	res_vect_type res_vect({{1,1}, {2,1}});
	EXPECT_EQ(Aho_Corasick(L"CCCA", vec_of_str({L"CC"}), 1), res_vect);
	res_vect = res_vect_type({{1,1}});
	EXPECT_EQ(Aho_Corasick(L"AACA", vec_of_str({L"AA"}), 1), res_vect);
}

TEST(Aho_Corasick_T, Normal)
{
	res_vect_type res_vect({{1,3},{3,5},{9,4},{11,1},{16,2},{19,1},{20,5},{23,2},{26,2},{28,1},{29,3}});
	EXPECT_EQ(Aho_Corasick(L"ACANCNACTNGNANANNCGANCNNANNGACA", 
			  vec_of_str({L"G", L"NN", L"ACA", L"TN", L"ANC"}), 5), res_vect);
}

TEST(Aho_Corasick_T, Repeated)
{
	res_vect_type res_vect({{1,1},{1,2},{1,3},{4,1},{4,2},{4,3},{7,1},{7,2},{7,3}});
	EXPECT_EQ(Aho_Corasick(L"ACCACCACCA", 
			  vec_of_str({L"ACCA", L"ACC", L"AC", L"CAA"}), 4), res_vect);
}

TEST(Aho_Corasick_T, Nikolenko)
{
	res_vect_type res_vect({{4,1}});
	EXPECT_EQ(Aho_Corasick(L"SЫFЯ", 
			  vec_of_str({L"Я"}), 1),res_vect);
}



TEST(Joker_T, Incomplete)
{
	EXPECT_EQ(search_with_joker(L"AACT", L"T$",L'$'), std::vector<int>());
	EXPECT_EQ(search_with_joker(L"JOKER", L"J&K&R", L'&'), std::vector<int>({1}));
}

TEST(Joker_T, Simle)
{
	EXPECT_EQ(search_with_joker(L"AACT", L"A$",L'$'), std::vector<int>({1,2}));
	EXPECT_EQ(search_with_joker(L"CCAT", L"C%",L'%'), std::vector<int>({1,2}));
	EXPECT_EQ(search_with_joker(L"AACT", L"A$$",L'$'), std::vector<int>({1,2}));
}

TEST(Joker_T, Normal)
{
	EXPECT_EQ(search_with_joker(L"AAGASGS", L"A$G$",L'$'), std::vector<int>({1,4}));
	EXPECT_EQ(search_with_joker(L"ФАФФСВААФЫФЫСВОАЛОЫФЫФЫСВП", L"Ф$Ф$СВ$", L'$'), std::vector<int>({1, 9, 20}));
}

TEST(Joker_T, Rus)
{
	EXPECT_EQ(search_with_joker(L"ТЕСТОВАЯ СТРОКА", L"Т$",L'$'), std::vector<int>({1,4,11}));
}