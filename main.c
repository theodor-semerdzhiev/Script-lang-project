#include "./src/syntax_tree/syntax_tree.h"

int main(int argc, char *argv[]) {



  CommandList *instruc = create_Command_list();
  parse_syntax_tree(instruc, "test_script.txt");

  return 0;
}
