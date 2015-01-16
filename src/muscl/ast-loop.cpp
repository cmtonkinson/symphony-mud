
#include <stdio.h>
#include "ast-loop.h"
#include "ast-reference.h"
#include "datum.h"
#include "record.h"
#include "vm.h"

AstLoop::AstLoop(void) {
  return;
}

AstLoop::AstLoop(const unsigned& type, AstReference* iterator, Ast* expression1, Ast* expression2, Ast* bodycode):
    Ast(type),
    _iterator(iterator),
    _expression1(expression1),
    _expression2(expression2),
    _bodycode(bodycode) {
  return;
}

AstLoop::~AstLoop(void) {
  return;
}

Datum* AstLoop::eval(void) {
  return NULL;
}

void AstLoop::print(std::string indent, bool last) {
  printf("%s", indent.c_str());
  if (last) {
    printf("\\-");
    indent += "  ";
  } else {
    printf("|-");
    indent += "| ";
  }
  printf("%s\n", stringType());
  if (iterator()) iterator()->print(indent, false);
  if (expression1()) expression1()->print(indent, false);
  if (expression2()) expression2()->print(indent, false);
  bodycode()->print(indent, true);
  return;
}

InstrList& AstLoop::codegen(void) {
  Record branch_out;  // leave/skip
  Record branch_in;   // enter/re-enter
  Record branch_over; // skip something
  Record compare;     // loop control
  Record start;   // initial value
  Record stop;    // final value
  Record assign;  // store initial value
  Record inc;     // increment
  Record iternew;
  Record iterinc;
  Record iterend;
  InstrList& body = bodycode()->codegen();

  switch (type()) {
    case AST_CONT_FOR_TO:
      // run the code for the initial value
      append(expression1()->codegen());
      // store the initial value in the iterator
      assign.opcode(OPCODE_STORE);
      assign.left().symbol(iterator()->symbol());
      getArg(assign.right(), expression1(), -1);
      append(assign);
      // run the code for the end value
      append(expression2()->codegen());
      // test expr1 <= expr2
      compare.opcode(OPCODE_LE);
      compare.left().symbol(iterator()->symbol());
      getArg(compare.right(), expression2(), -1);
      append(compare);
      // if the test was false, bail out of the loop
      branch_out.opcode(OPCODE_BRF);
      branch_out.left().address(body.size()+3);
      append(branch_out);
      // run the body of the loop
      append(body);
      // increment the iterator
      inc.opcode(OPCODE_ADDSTORE);
      inc.left().symbol(iterator()->symbol());
      inc.right().value() = 1;
      append(inc);
      // branch back up to the test
      branch_in.opcode(OPCODE_BRA);
      branch_in.left().address(-(expression2()->instructions().size()+body.size()+3));
      append(branch_in);
      break;
    case AST_CONT_FOR_IN:
      // the list
      append(expression1()->codegen());
      // create the iterator
      iternew.opcode(OPCODE_ITER_NEW);
      iternew.left().symbol(iterator()->symbol());
      getArg(iternew.right(), expression1(), -1);
      append(iternew);
      // skip the increment the first time through
      branch_over.opcode(OPCODE_BRA);
      branch_over.left().address(2);
      append(branch_over);
      // increment the iterator
      iterinc.opcode(OPCODE_ITER_INC);
      iterinc.left().symbol(iterator()->symbol());
      append(iterinc);
      // test that iter == end
      iterend.opcode(OPCODE_ITER_END);
      iterend.left().symbol(iterator()->symbol());
      append(iterend);
      // if the test was true, branch out
      branch_out.opcode(OPCODE_BRT);
      branch_out.left().address(body.size()+2);
      append(branch_out);
      // body of the loop
      append(body);
      // branch back up to the iteration step
      branch_in.opcode(OPCODE_BRA);
      branch_in.left().address(-(body.size()+3));
      append(branch_in);
      break;
    case AST_CONT_WHILE:
      // test
      append(expression1()->codegen());
      compare.opcode(OPCODE_COMP);
      getArg(compare.left(), expression1(), -1);
      append(compare);
      // branch if test was false
      branch_out.opcode(OPCODE_BRF);
      branch_out.left().address(body.size()+2);
      append(branch_out);
      // body
      append(body);
      // branch back
      branch_in.opcode(OPCODE_BRA);
      branch_in.left().address(-(body.size() + expression1()->instructions().size() + 2));
      append(branch_in);
      break;
    case AST_CONT_DO_WHILE:
      // body
      append(body);
      // test
      append(expression1()->codegen());
      compare.opcode(OPCODE_COMP);
      getArg(compare.left(), expression1(), -1);
      append(compare);
      // branch back if test was true
      branch_in.opcode(OPCODE_BRT);
      branch_in.left().address(-(body.size() + expression1()->instructions().size() + 1));
      append(branch_in);
      break;
    case AST_CONT_UNTIL:
      // test
      append(expression1()->codegen());
      compare.opcode(OPCODE_COMP);
      getArg(compare.left(), expression1(), -1);
      append(compare);
      // branch if test was false
      branch_out.opcode(OPCODE_BRT);
      branch_out.left().address(body.size()+2);
      append(branch_out);
      // body
      append(body);
      // branch back
      branch_in.opcode(OPCODE_BRA);
      branch_in.left().address(-(body.size() + expression1()->instructions().size() + 2));
      append(branch_in);
      break;
    case AST_CONT_DO_UNTIL:
      // body
      append(body);
      // test
      append(expression1()->codegen());
      compare.opcode(OPCODE_COMP);
      getArg(compare.left(), expression1(), -1);
      append(compare);
      // branch back if test was true
      branch_in.opcode(OPCODE_BRF);
      branch_in.left().address(-(body.size() + expression1()->instructions().size() + 1));
      append(branch_in);
      break;
  }

  return instructions();
}
