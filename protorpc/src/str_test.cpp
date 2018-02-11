#include <gtest/gtest.h>
#include <protorpc/str.h>

TEST(protorpc, str) {
	str_t s = STR_INIT;
	str_set(&s, "foo bar foo bar");
	str_replace_all(&s, "foo", "abcde");
	EXPECT_STREQ("abcde bar abcde bar", s.c_str);
	str_replace_all(&s, "bar", "a");
	EXPECT_STREQ("abcde a abcde a", s.c_str);
	str_replace_all(&s, "abcde", "ghijk");
	EXPECT_STREQ("ghijk a ghijk a", s.c_str);
}
