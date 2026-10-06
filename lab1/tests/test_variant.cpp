#include "str_ops.h"

#include <gtest/gtest.h>


// str_len

TEST(StrLenTest, NormalString) {
    EXPECT_EQ(str_len("Hello"), 5);
}

TEST(StrLenTest, EmptyString) {
    EXPECT_EQ(str_len(""), 0);
}

TEST(StrLenTest, StringWithSpaces) {
    EXPECT_EQ(str_len("Hello World"), 11);
}

TEST(StrLenTest, StringWithSymbolsAndDigits) {
    EXPECT_EQ(str_len("Abc123!?"), 8);
}

TEST(StrLenTest, Nullptr) {
    EXPECT_EQ(str_len(nullptr), 0);
}


// str_copy

TEST(StrCopyTest, CopiesNormalString) {
    char buffer[6];

    str_copy(buffer, "Hello");

    EXPECT_STREQ(buffer, "Hello");
}

TEST(StrCopyTest, CopiesEmptyString) {
    char buffer[1];

    str_copy(buffer, "");

    EXPECT_STREQ(buffer, "");
}

TEST(StrCopyTest, CopiesStringWithSpacesAndSymbols) {
    char buffer[13];

    str_copy(buffer, "Hello 123!?");

    EXPECT_STREQ(buffer, "Hello 123!?");
}

TEST(StrCopyTest, NullSourceLeavesDestinationUnchanged) {
    char buffer[] = "Hello";

    str_copy(buffer, nullptr);

    EXPECT_STREQ(buffer, "Hello");
}

TEST(StrCopyTest, NullDestinationDoesNotCrash) {
    EXPECT_NO_THROW(str_copy(nullptr, "Hello"));
}


// str_alloc

TEST(StrAllocTest, AllocatesNormalString) {
    char* result = str_alloc("Hello");

    ASSERT_NE(result, nullptr);
    EXPECT_STREQ(result, "Hello");

    str_delete(result);
}

TEST(StrAllocTest, AllocatesEmptyString) {
    char* result = str_alloc("");

    ASSERT_NE(result, nullptr);
    EXPECT_STREQ(result, "");

    str_delete(result);
}

TEST(StrAllocTest, CreatesIndependentCopy) {
    char source[] = "Hello";

    char* result = str_alloc(source);

    ASSERT_NE(result, nullptr);

    source[0] = 'X';

    EXPECT_STREQ(source, "Xello");
    EXPECT_STREQ(result, "Hello");

    str_delete(result);
}

TEST(StrAllocTest, NullptrReturnsNullptr) {
    char* result = str_alloc(nullptr);

    EXPECT_EQ(result, nullptr);
}


// str_delete

TEST(StrDeleteTest, SetsPointerToNullptr) {
    char* text = str_alloc("Hello");

    ASSERT_NE(text, nullptr);

    str_delete(text);

    EXPECT_EQ(text, nullptr);
}

TEST(StrDeleteTest, CanDeleteNullptr) {
    char* text = nullptr;

    EXPECT_NO_THROW(str_delete(text));
    EXPECT_EQ(text, nullptr);
}


// str_to_upper

TEST(StrToUpperTest, ConvertsLowercaseToUppercase) {
    char text[] = "Hello, Mai";

    str_to_upper(text);

    EXPECT_STREQ(text, "HELLO, MAI");
}

TEST(StrToUpperTest, ConvertsAllLowercaseLetters) {
    char text[] = "abcdefghijklmnopqrstuvwxyz";

    str_to_upper(text);

    EXPECT_STREQ(text, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
}

TEST(StrToUpperTest, LeavesUppercaseLettersUnchanged) {
    char text[] = "HELLO";

    str_to_upper(text);

    EXPECT_STREQ(text, "HELLO");
}

TEST(StrToUpperTest, LeavesDigitsAndSymbolsUnchanged) {
    char text[] = "abc123!?";

    str_to_upper(text);

    EXPECT_STREQ(text, "ABC123!?");
}

TEST(StrToUpperTest, HandlesEmptyString) {
    char text[] = "";

    str_to_upper(text);

    EXPECT_STREQ(text, "");
}

TEST(StrToUpperTest, NullptrDoesNotCrash) {
    EXPECT_NO_THROW(str_to_upper(nullptr));
}


// str_count_char

TEST(StrCountCharTest, CountsExistingCharacter) {
    EXPECT_EQ(str_count_char("Hello", 'l'), 2);
}

TEST(StrCountCharTest, ReturnsZeroForMissingCharacter) {
    EXPECT_EQ(str_count_char("Hello", 'x'), 0);
}

TEST(StrCountCharTest, IsCaseSensitive) {
    EXPECT_EQ(str_count_char("Hello", 'L'), 0);
}

TEST(StrCountCharTest, CountsUppercaseCharacter) {
    EXPECT_EQ(str_count_char("HeLLo", 'L'), 2);
}

TEST(StrCountCharTest, CountsSpace) {
    EXPECT_EQ(str_count_char("Hello World Test", ' '), 2);
}

TEST(StrCountCharTest, CountsSymbol) {
    EXPECT_EQ(str_count_char("!!!abc!", '!'), 4);
}

TEST(StrCountCharTest, HandlesEmptyString) {
    EXPECT_EQ(str_count_char("", 'a'), 0);
}

TEST(StrCountCharTest, NullptrReturnsZero) {
    EXPECT_EQ(str_count_char(nullptr, 'a'), 0);
}