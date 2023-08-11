// #include "symbol_table.h"
// #include <iostream>
// #include "io.h"
// #include "parser.h"
// #include "spdlog/spdlog.h"

// int main(int argc, char* argv[]) {
//   if (argc != 2) {
//     std::cerr << "Usage: " << argv[0] << " <filename>\n";
//     return 1;
//   }

//   const auto text = Pascal::read_file(argv[1]);

//   Pascal::Parser parser(text);
//   auto tree = parser.parse();
//   spdlog::set_level(spdlog::level::debug);
//   Pascal::SymbolTableBuilder visitor;
//   tree->accept(&visitor);
//   visitor.print_symbol_table();

//   return 0;
// }