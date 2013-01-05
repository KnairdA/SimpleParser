#include "src/parser.h"
#include "gtest/gtest.h"

class ParserTest : public ::testing::Test {

};

TEST_F(ParserTest, BasicCalc) {
	SimpleParser::Parser p;

	EXPECT_EQ(4,  p.calculate("2+2"));
	EXPECT_EQ(0,  p.calculate("2-2"));
	EXPECT_EQ(42, p.calculate("21*2"));
	EXPECT_EQ(21, p.calculate("42/2"));
}

TEST_F(ParserTest, OperatorPriority) {
	SimpleParser::Parser p;

	EXPECT_EQ(10,    p.calculate("2+2*4"));
	EXPECT_EQ(4,     p.calculate("2+4/2"));
	EXPECT_EQ(42,    p.calculate("5+10*4-3"));
	EXPECT_EQ(17,    p.calculate("10+20/2-3"));
	EXPECT_EQ(261,   p.calculate("5+2^8"));
	EXPECT_EQ(32768, p.calculate("2*2^16/4"));
	EXPECT_EQ(32772, p.calculate("2*2^16/4+4"));
}

TEST_F(ParserTest, BracketCalc) {
	SimpleParser::Parser p;

	EXPECT_EQ(16,   p.calculate("(2+2)*4"));
	EXPECT_EQ(10,   p.calculate("2+(2*4)"));
	EXPECT_EQ(10,   p.calculate("(10-5)*(5-3)"));
	EXPECT_EQ(4,    p.calculate("((((2))*((2))))"));
	EXPECT_EQ(37,   p.calculate("(((5))*(((((3+2*2)))))+2)"));
	EXPECT_EQ(6.25, p.calculate("2.5*(2+3-(3/2+1))"));
}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
