/*
 * $Id: vm.cpp 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#include "vm.h"

#include <math.h>
#include "frame.h"
#include "record.h"
#include "datum.h"

VirtualMachine::VirtualMachine(void) {
  // misc
  _functions[OPCODE_ERR]  = &VirtualMachine::op_err;
  _functions[OPCODE_HALT] = &VirtualMachine::op_halt;
  _functions[OPCODE_SEND] = &VirtualMachine::op_send;
  // data
  _functions[OPCODE_VALUE]  = &VirtualMachine::op_value;
  _functions[OPCODE_CONST]  = &VirtualMachine::op_const;
  _functions[OPCODE_VAR]    = &VirtualMachine::op_var;
  _functions[OPCODE_STORE]  = &VirtualMachine::op_store;
  // objects
  _functions[OPCODE_OBJ_READ]   = &VirtualMachine::op_obj_read;
  _functions[OPCODE_OBJ_WRITE]  = &VirtualMachine::op_obj_write;
  // lists
  _functions[OPCODE_ELMT]     = &VirtualMachine::op_elmt;
  _functions[OPCODE_LIST]     = &VirtualMachine::op_list;
  _functions[OPCODE_ITER_NEW] = &VirtualMachine::op_iter_new;
  _functions[OPCODE_ITER_INC] = &VirtualMachine::op_iter_inc;
  _functions[OPCODE_ITER_END] = &VirtualMachine::op_iter_end;
  // branching
  _functions[OPCODE_BRA] = &VirtualMachine::op_bra;
  _functions[OPCODE_BRT] = &VirtualMachine::op_brt;
  _functions[OPCODE_BRF] = &VirtualMachine::op_brf;
  // functions
  _functions[OPCODE_FUNC]   = &VirtualMachine::op_func;
  _functions[OPCODE_PARAM]  = &VirtualMachine::op_param;
  _functions[OPCODE_ARG]    = &VirtualMachine::op_arg;
  _functions[OPCODE_CALL]   = &VirtualMachine::op_call;
  _functions[OPCODE_RETURN] = &VirtualMachine::op_return;
  // logical
  _functions[OPCODE_AND]  = &VirtualMachine::op_and;
  _functions[OPCODE_OR]   = &VirtualMachine::op_or;
  _functions[OPCODE_NOT]  = &VirtualMachine::op_not;
  // comparisons
  _functions[OPCODE_COMP] = &VirtualMachine::op_comp;
  _functions[OPCODE_EQ]   = &VirtualMachine::op_eq;
  _functions[OPCODE_NE]   = &VirtualMachine::op_ne;
  _functions[OPCODE_LT]   = &VirtualMachine::op_lt;
  _functions[OPCODE_LE]   = &VirtualMachine::op_le;
  _functions[OPCODE_GT]   = &VirtualMachine::op_gt;
  _functions[OPCODE_GE]   = &VirtualMachine::op_ge;
  // math
  _functions[OPCODE_ADD]  = &VirtualMachine::op_add;
  _functions[OPCODE_SUB]  = &VirtualMachine::op_sub;
  _functions[OPCODE_MUL]  = &VirtualMachine::op_mul;
  _functions[OPCODE_DIV]  = &VirtualMachine::op_div;
  _functions[OPCODE_MOD]  = &VirtualMachine::op_mod;
  _functions[OPCODE_EXP]  = &VirtualMachine::op_exp;
  _functions[OPCODE_MIN]  = &VirtualMachine::op_min;
  _functions[OPCODE_CARD] = &VirtualMachine::op_card;
  // complex storage
  _functions[OPCODE_ADDSTORE] = &VirtualMachine::op_addstore;
  _functions[OPCODE_SUBSTORE] = &VirtualMachine::op_substore;
  _functions[OPCODE_MULSTORE] = &VirtualMachine::op_mulstore;
  _functions[OPCODE_DIVSTORE] = &VirtualMachine::op_divstore;
  _functions[OPCODE_MODSTORE] = &VirtualMachine::op_modstore;
  _functions[OPCODE_POWSTORE] = &VirtualMachine::op_powstore;
  return;
}

VirtualMachine::~VirtualMachine(void) {
  cleanup();
  return;
}

void VirtualMachine::operation(Record& operation) {
  _operation = &operation;
  _left = &_operation->left();
  _right = &_operation->right();
  return;
}

void VirtualMachine::run(CodeVect* code) {
  init(code);
  try {
    execute();
  } catch (std::string s) {
    printf("\n\nBones Exception (intsruction %ld): %s\n", ip(), s.c_str());
  }
  cleanup();
  return;
}

void VirtualMachine::init(CodeVect* code) {
  // set ourselves up for a good time
  goBabyGo(true);
  ip(0);
  quantum(1000);
  instructions(code);
  // I use the "+1" here because instructions are 1-indexed, not 0-indexed
  results().resize(instructions().size()+1);
  frames().push(Frame()); // default stack frame
  return;
}

void VirtualMachine::cleanup(void) {
  // when you make a mess...
  goBabyGo(false);
  clear(&_results);
  instructions(NULL);
  while (!frames().empty()) frames().pop();
  return;
}

void VirtualMachine::clear(ResultList* list) {
  while (!list->empty()) list->pop_back();
  return;
}

Frame& VirtualMachine::current(void) {
  return frames().top();
}

void VirtualMachine::execute(void) {
  long ops_performed = 0;
  OpMap::const_iterator it;
  while (goBabyGo()) {
    operation(instructions()[nextIP()]);
    //printf("\n%2d: %s", operation()->number(), operation()->opcodeToString());
    if ((it = functions().find(operation()->opcode())) == functions().end()) op_err();
    else (this->*it->second)();
    //printf(" [%s]", results()[ip()].toString().c_str());
    if (++ops_performed > quantum()) throw std::string("quantum reached");
  }
  printf("\n");
  return;
}

Datum& VirtualMachine::fetch(Operand& op) {
  if (op.address() > 0) return results()[op.address()];
  else return op.value();
}

void VirtualMachine::dump_results(void) {
  int i = -1;
  printf("\n");
  for (ResultList::iterator it = results().begin(); it != results().end(); ++it) printf("%2d: %s\n", ++i, (*it).toString().c_str());
  printf("\n");
  return;
}

// misc
void VirtualMachine::op_err(void) {
  printf("unknown opcode: %s\n", operation()->opcodeToString());
  goBabyGo(false);
  return;
}

void VirtualMachine::op_halt(void) {
  goBabyGo(false);
  return;
}

void VirtualMachine::op_send(void) {
  printf("%s\n", fetch(*left()).toString().c_str());
  return;
}

// data
void VirtualMachine::op_value(void) {
  results()[ip()] = fetch(*left());
  return;
}

void VirtualMachine::op_const(void) {
  results()[ip()] = fetch(*right());
  return;
}

void VirtualMachine::op_var(void) {
  results()[ip()] = fetch(*right());
  return;
}

void VirtualMachine::op_store(void) {
  results()[left()->address()] = fetch(*right());
  return;
}

// objects
void VirtualMachine::op_obj_read(void) {
  context()->read(fetch(*left()).toString(), results()[ip()]);
  return;
}

void VirtualMachine::op_obj_write(void) { // (replacement of op_store for objects)
  context()->write(fetch(*left()).toString(), fetch(*right()));
  return;
}

// lists
void VirtualMachine::op_elmt(void) {
  current().elements().push(fetch(*left()));
  return;
}

void VirtualMachine::op_list(void) {
  while (!current().elements().empty()) {
    results()[ip()].append(current().elements().front());
    current().elements().pop();
  }
  return;
}

void VirtualMachine::op_iter_new(void) {
  const Datum& r = fetch(*right());
  current().iter_new(left()->address(), r);
  results()[left()->address()] = current().get_iter(left()->address());
  return;
}

void VirtualMachine::op_iter_inc(void) {
  current().iter_inc(left()->address());
  results()[left()->address()] = current().get_iter(left()->address());
  return;
}

void VirtualMachine::op_iter_end(void) {
  results()[ip()] = current().iter_end(left()->address()) ? 1 : 0;
  return;
}

// branching
void VirtualMachine::op_bra(void) {
  ip(left()->address()-1);
  return;
}

void VirtualMachine::op_brt(void) {
  if (results()[ip()-1].truth()) ip(left()->address()-1);
  return;
}

void VirtualMachine::op_brf(void) {
  if (!(results()[ip()-1].truth())) ip(left()->address()-1);
  return;
}

// functions
void VirtualMachine::op_func(void) {
  // this instruction is just a placeholder; it's a no-op
  return;
}

void VirtualMachine::op_param(void) {
  // load a value from the param-list into the "local" space
  results()[ip()] = current().params().front();
  // remove the loaded value
  current().params().pop();
  return;
}

void VirtualMachine::op_arg(void) {
  // save a value (to pass into a function)
  current().arguments().push(fetch(*left()));
  return;
}

void VirtualMachine::op_call(void) {
  // save a reference to the arg-list
  ValList& args = current().arguments();
  // push a new activation record
  frames().push(Frame());
  // store the return address
  current().returnAddress(ip());
  // pass our args in (by value)
  current().params(args);
  // dump the arg queue (so it'l clear for the next function call from this frame)
  while (!args.empty()) args.pop();
  // set the IP to the start of the function
  ip(left()->address()-1);
  return;
}

void VirtualMachine::op_return(void) {
  // restore the IP
  ip(current().returnAddress());
  // store the return value (if there was one)
  results()[ip()] = fetch(*left());
  // kill the activation record for the function
  frames().pop();
  return;
}

// logical
void VirtualMachine::op_and(void) {
  Datum l = fetch(*left());
  Datum r = fetch(*right());
  results()[ip()] = (long)l && (long)r;
  return;
}

void VirtualMachine::op_or(void) {
  Datum l = fetch(*left());
  Datum r = fetch(*right());
  results()[ip()] = (long)l || (long)r;
  return;
}

void VirtualMachine::op_not(void) {
  Datum l = fetch(*left());
  results()[ip()] = !(long)l;
  return;
}

// comparisons
void VirtualMachine::op_comp(void) {
  const Datum& l = fetch(*left());
  results()[ip()] = l.truth() ? 1 : 0;
  current().comparison(l.truth());
  return;
}

void VirtualMachine::op_eq(void) {
  const Datum& l = fetch(*left());
  const Datum& r = fetch(*right());
  results()[ip()] = l == r ? 1 : 0;
  return;
}

void VirtualMachine::op_ne(void) {
  const Datum& l = fetch(*left());
  const Datum& r = fetch(*right());
  results()[ip()] = l != r ? 1 : 0;
  return;
}

void VirtualMachine::op_lt(void) {
  const Datum& l = fetch(*left());
  const Datum& r = fetch(*right());
  results()[ip()] = l < r ? 1 : 0;
  return;
}

void VirtualMachine::op_le(void) {
  const Datum& l = fetch(*left());
  const Datum& r = fetch(*right());
  results()[ip()] = l <= r ? 1 : 0;
  return;
}

void VirtualMachine::op_gt(void) {
  const Datum& l = fetch(*left());
  const Datum& r = fetch(*right());
  results()[ip()] = l > r ? 1 : 0;
  return;
}

void VirtualMachine::op_ge(void) {
  const Datum& l = fetch(*left());
  const Datum& r = fetch(*right());
  results()[ip()] = l >= r ? 1 : 0;
  return;
}

// math
void VirtualMachine::op_add(void) {
  Datum l = fetch(*left());
  Datum r = fetch(*right());
  std::string str;
  Datum::upcast(l, r);
  switch (l.type()) {
    case DATUM_TYPE_LONG:
      results()[ip()] = (long)l + (long)r;
      break;
    case DATUM_TYPE_DOUBLE:
      results()[ip()] = (double)l + (double)r;
      break;
    case DATUM_TYPE_STRING:
      str.append((const char*)l).append((const char*)r);
      results()[ip()] = str;
      break;
    case DATUM_TYPE_LIST:
      results()[ip()].append_list(l);
      results()[ip()].append_list(r);
      break;
  }
  return;
}

void VirtualMachine::op_sub(void) {
  Datum l = fetch(*left());
  Datum r = fetch(*right());
  Datum::upcast(l, r);
  switch (l.type()) {
    case DATUM_TYPE_LONG:
      results()[ip()] = (long)l - (long)r;
      break;
    case DATUM_TYPE_DOUBLE:
      results()[ip()] = (double)l - (double)r;
      break;
  }
  return;
}

void VirtualMachine::op_mul(void) {
  Datum l = fetch(*left());
  Datum r = fetch(*right());
  std::string str, dest, foo;
  Datum& a = l.type() < l.type() ? l : r;
  Datum& b = r.type() < r.type() ? r : l;
  /* if we're multiplying a string or list */
  if (a.type() == DATUM_TYPE_LONG && (b.type() == DATUM_TYPE_STRING || b.type() == DATUM_TYPE_LIST)) {
    /* multiplying a string */
    if (b.type() == DATUM_TYPE_STRING) {
      if ((long)a >= 0) {
        str.assign(b.toString());
        for (long i=0; i < (long)a; ++i) dest.append(str);
        results()[ip()] = dest;
      } else {
        foo.assign(b.toString());
        str.assign(foo.rbegin(), foo.rend());
        for (long i=0; i < (long)a; --i) dest.append(str);
        results()[ip()] = dest;
      }
    /* multiplying a list */
    } else {
      // CODE ME
    }
  /* if we're doing mathematical multiplication */
  } else {
    Datum::upcast(l, r);
    switch (l.type()) {
      case DATUM_TYPE_LONG:
        results()[ip()] = (long)l * (long)r;
        break;
      case DATUM_TYPE_DOUBLE:
        results()[ip()] = (double)l * (double)r;
        break;
    }
  }
  return;
}

void VirtualMachine::op_div(void) {
  Datum l = fetch(*left());
  Datum r = fetch(*right());
  Datum::upcast(l, r);
  switch (l.type()) {
    case DATUM_TYPE_LONG:
      if ((long)r == 0) throw std::string("divide by zero");
      results()[ip()] = (long)l / (long)r;
      break;
    case DATUM_TYPE_DOUBLE:
      if ((double)r == 0.0) throw std::string("divide by zero");
      results()[ip()] = (double)l / (double)r;
      break;
  }
  return;
}

void VirtualMachine::op_mod(void) {
  Datum l = fetch(*left());
  Datum r = fetch(*right());
  Datum::upcast(l, r);
  switch (l.type()) {
    case DATUM_TYPE_LONG:
      if ((long)r == 0) throw std::string("mod by zero");
      results()[ip()] = (long)l % (long)r;
      break;
  }
  return;
}

void VirtualMachine::op_exp(void) {
  Datum l = fetch(*left());
  Datum r = fetch(*right());
  Datum::upcast(l, r);
  switch (l.type()) {
    case DATUM_TYPE_LONG:
      results()[ip()] = pow((long)l, (long)r);
      break;
    case DATUM_TYPE_DOUBLE:
      results()[ip()] = pow((double)l, (double)r);
      break;
  }
  return;
}

void VirtualMachine::op_min(void) {
  const Datum& l = fetch(*left());
  std::string str;
  switch (l.type()) {
    case DATUM_TYPE_LONG:
      results()[ip()] = -l.longVal();
      break;
    case DATUM_TYPE_DOUBLE:
      results()[ip()] = -l.doubleVal();
      break;
  }
  return;
}

void VirtualMachine::op_card(void) {
  Datum l = fetch(*left());
  switch (l.type()) {
    case DATUM_TYPE_LONG:
      results()[ip()] = labs((long)l);
      break;
    case DATUM_TYPE_DOUBLE:
      results()[ip()] = fabs((double)l);
      break;
    case DATUM_TYPE_STRING:
      results()[ip()] = strlen((const char*)l);
      break;
    case DATUM_TYPE_LIST:
      results()[ip()] = ((std::list<Datum>*)l.data())->size();
      break;
  }
  return;
}

// complex storage operations
void VirtualMachine::op_addstore(void) {
  op_add();
  results()[left()->address()] = results()[ip()];
  return;
}

void VirtualMachine::op_substore(void) {
  op_sub();
  results()[left()->address()] = results()[ip()];
  return;
}

void VirtualMachine::op_mulstore(void) {
  op_mul();
  results()[left()->address()] = results()[ip()];
  return;
}

void VirtualMachine::op_divstore(void) {
  op_div();
  results()[left()->address()] = results()[ip()];
  return;
}

void VirtualMachine::op_modstore(void) {
  op_mod();
  results()[left()->address()] = results()[ip()];
  return;
}

void VirtualMachine::op_powstore(void) {
  op_exp();
  results()[left()->address()] = results()[ip()];
  return;
}
