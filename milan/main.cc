#include <stdio.h>
#include <string>
#include <list>
#include <cassert>
#include <vector>
#include <iostream>

#include "lexer.h"

const std::string SRC = "fn main() int {\n"
	"	return 0;\n"
	"}";

int main() {

	milan::TokenStream tkstream{ SRC };

	tkstream.dump_tokens();
	return 0;
}