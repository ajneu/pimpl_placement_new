#include <iostream>
#include <cctype>
#include <cstdlib>
#include <string>
#include <algorithm>

using std::string;

void includer_files(const char * const *argv)
{
   for (; *argv; ++argv) {
      std::cout << "#include \"" << *argv << "\"\n";
   }
}

string strip_scope(string str)
// example: strip_scope("AA::BB::CC") returns only the last part "CC"
{
   string::size_type idx = str.find_last_of(":");
   if (idx == string::npos) {
      idx = 0;
   } else {
      ++idx; // skip over last ':'
   }
   return str.substr(idx);
}

int main(int argc, char *argv[])
{
   if (argc < 2) {
      std::cerr << "Usage: " << argv[0] << " Classname myincludefile1.h myincludefile2.cpp ..." << std::endl;
      return EXIT_FAILURE;
   }



   std::cout << "#include <iostream>\n"
                "#define ALIGNINFO_GENERATION_BUSY\n";
   includer_files(argv+2);
   
   string classname = argv[1];
   string classname_stripped = strip_scope(classname);
   string guardname = "ALIGNINFO_";
   std::transform(classname_stripped.begin(), classname_stripped.end(), std::back_inserter(guardname), ::toupper);
   guardname += "_H";
   
   std::cout << "\n"
      "void aligninfo_generator()\n"
      "{\n"
      "   std::cout << \"#ifndef " << guardname << "\\n\"\n"
      "                \"#define " << guardname << "\\n\"\n"
      "                \"\\n\"\n"
      "                \"#include <cstddef>\\n\"\n"
      "                \"\\n\"\n"
      "                \"constexpr std::size_t aligninfo_Len_" << classname_stripped << "   = \" << sizeof(           " << classname << ")        << \";\\n\"\n"
      "                \"constexpr std::size_t aligninfo_Align_" << classname_stripped << " = \" << std::alignment_of<" << classname << ">::value << \";\\n\"\n"
      "                \"\\n\"\n"      
      "                \"#endif\\n\";\n"
      "}\n"
      "\n"
      "int main()\n"
      "{\n"
      "   aligninfo_generator();\n"
      "   return 0;\n"
      "}\n";

   return EXIT_SUCCESS;
}
