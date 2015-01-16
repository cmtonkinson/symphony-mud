
#include "program.h"
#include "ast.h"
#include "vm.h"

extern FILE* yyin;
extern VirtualMachine bones;
extern Ast* ast_root;
void yyparse(void);

Program::Program(void) {
  return;
}

Program::Program(const string& name):
    _name(name) {
  return;
}

Program::~Program(void) {
  return;
}

/* Commit the new source to file before updating the internal member */
bool Program::source(const string& source) {
  FILE* back = NULL;
  string filename = name().append(".msl");
  if ((back = fopen(filename.c_str(), "w")) == NULL) {
    fprintf(stderr, "PROG: source(): failed to open file '%s' for writing\n", filename.c_str());
    return false;
  }
  fputs(source.c_str(), back);
  if (fclose(back)) {
    fprintf(stderr, "PROG: source(): failed to close file '%s'\n", filename.c_str());
    return false;
  }
  _source = source;
  return true;
}

bool Program::compile(string& feedback) {
  FILE* back = NULL;
  string filename = name().append(".msl");
  InstrList instructions;
  if ((back = fopen(filename.c_str(), "r")) == NULL) {
    fprintf(stderr, "PROG: compile(): failed to open file '%s' for reading\n", filename.c_str());
    feedback = "unable to write file to disk";
    return false;
  }
  yyin = back;
  try {
    yyparse();
    ast_root->codegen();
    ast_root->dumpcode();
/* bytecode serialization */
    for (InstrList::const_iterator it = ast_root->instructions().begin(); it != ast_root->instructions().end(); ++it) {
      printf("%d", it->opcode());
      if (!it->left().empty()) {
        printf("|%s", it->left().serialize().c_str());
        if (!it->right().empty()) printf("|%s", it->right().serialize().c_str());
      }
      printf("\n");
    }
/* !bytecode serialization */
    bones.run(&ast_root->code());
  } catch (std::string message) {
    feedback = message;
    return false;
  }
  if (fclose(back)) {
    fprintf(stderr, "PROG: compile(): failed to close file '%s'\n", filename.c_str());
    feedback = "unable to close file handle";
    return false;
  }
  return true;
}
