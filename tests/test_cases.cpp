#include <gtest/gtest.h>
#include "../hdr/Aho_Corasick_hdr.h"
#include "../hdr/search_with_joker_hdr.h"

typedef std::vector<std::pair<int, int> > res_vect_type;
typedef std::vector<std::string> vec_of_str;


TEST(Aho_Corasick_T, Incomplete)
{
	res_vect_type res_vect;
	EXPECT_EQ(Aho_Corasick("CCCA", vec_of_str({"AC"}), 1), res_vect);
}

TEST(Aho_Corasick_T, Simple)
{
	res_vect_type res_vect({{1,1}, {2,1}});
	EXPECT_EQ(Aho_Corasick("CCCA", vec_of_str({"CC"}), 1), res_vect);
	res_vect = res_vect_type({{1,1}});
	EXPECT_EQ(Aho_Corasick("AACA", vec_of_str({"AA"}), 1), res_vect);
}

TEST(Aho_Corasick_T, Normal)
{
	res_vect_type res_vect({{1,3},{3,5},{9,4},{11,1},{16,2},{19,1},{20,5},{23,2},{26,2},{28,1},{29,3}});
	EXPECT_EQ(Aho_Corasick("ACANCNACTNGNANANNCGANCNNANNGACA", 
			  vec_of_str({"G", "NN", "ACA", "TN", "ANC"}), 5), res_vect);
}

TEST(Aho_Corasick_T, Repeated)
{
	res_vect_type res_vect({{1,1},{1,2},{1,3},{4,1},{4,2},{4,3},{7,1},{7,2},{7,3}});
	EXPECT_EQ(Aho_Corasick("ACCACCACCA", 
			  vec_of_str({"ACCA", "ACC", "AC", "CAA"}), 4), res_vect);
}



TEST(Joker_T, Incomplete)
{
	EXPECT_EQ(search_with_joker("AACT", "T$",'$'), std::vector<int>());
	EXPECT_EQ(search_with_joker("AGNTNNAAGTCNTACGNTACNGTNCGACGTGGACGTANGTANGTC", "A$C$A",'$'), std::vector<int>());
}

TEST(Joker_T, Simle)
{
	EXPECT_EQ(search_with_joker("AACT", "A$",'$'), std::vector<int>({1,2}));
	EXPECT_EQ(search_with_joker("CCAT", "C%",'%'), std::vector<int>({1,2}));
	EXPECT_EQ(search_with_joker("AACT", "A$$",'$'), std::vector<int>({1,2}));
}

TEST(Joker_T, Normal)
{
	EXPECT_EQ(search_with_joker("AGNTNNAAGTCNTACGNTACNGTNCGACGTGGACGTANGTANGTC", "A$GT$",'$'), std::vector<int>({7,27,33,37,41}));
}