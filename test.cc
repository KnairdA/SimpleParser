#include "src/parser.h"
#include "gtest/gtest.h"

class ParserTest : public ::testing::Test {

};

TEST_F(ParserTest, BasicCalc) {
	EXPECT_EQ(4,  SimpleParser::calculate("2+2"));
	EXPECT_EQ(0,  SimpleParser::calculate("2-2"));
	EXPECT_EQ(42, SimpleParser::calculate("21*2"));
	EXPECT_EQ(21, SimpleParser::calculate("42/2"));
}

TEST_F(ParserTest, OperatorPriority) {
	EXPECT_EQ(10,    SimpleParser::calculate("2+2*4"));
	EXPECT_EQ(4,     SimpleParser::calculate("2+4/2"));
	EXPECT_EQ(60,    SimpleParser::calculate("100/5*3"));
	EXPECT_EQ(60,    SimpleParser::calculate("100*3/5"));
	EXPECT_EQ(42,    SimpleParser::calculate("5+10*4-3"));
	EXPECT_EQ(17,    SimpleParser::calculate("10+20/2-3"));
	EXPECT_EQ(261,   SimpleParser::calculate("5+2^8"));
	EXPECT_EQ(32768, SimpleParser::calculate("2*2^16/4"));
	EXPECT_EQ(32772, SimpleParser::calculate("2*2^16/4+4"));
}

TEST_F(ParserTest, BracketCalc) {
	EXPECT_EQ(16,   SimpleParser::calculate("(2+2)*4"));
	EXPECT_EQ(10,   SimpleParser::calculate("2+(2*4)"));
	EXPECT_EQ(10,   SimpleParser::calculate("(10-5)*(5-3)"));
	EXPECT_EQ(4,    SimpleParser::calculate("((((2))*((2))))"));
	EXPECT_EQ(37,   SimpleParser::calculate("(((5))*(((((3+2*2)))))+2)"));
	EXPECT_EQ(6.25, SimpleParser::calculate("2.5*(2+3-(3/2+1))"));
}

TEST_F(ParserTest, ConstantCalc) {
	SimpleParser::ConstantMap constants;
	constants["pi"]     = 3.1415926535;
	constants["answer"] = 42;
	constants["g"]      = 9.81;

	EXPECT_EQ(3.1415926535, SimpleParser::calculate("pi",                          &constants));
	EXPECT_EQ(6.283185307,  SimpleParser::calculate("2*pi*1",                      &constants));
	EXPECT_EQ(0,            SimpleParser::calculate("2*3*7-answer",                &constants));
	EXPECT_EQ(1347.58,      SimpleParser::calculate("1/2*(g*(-1))*answer^2+10000", &constants));
}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
