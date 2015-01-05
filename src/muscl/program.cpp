/*
 * $Id: program.cpp 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
 *
 * This file is part of the Symphony project <http://code.google.com/p/symphonymud/>
 * Copyright 2005-2010 Chris Tonkinson <cmtonkinson@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
