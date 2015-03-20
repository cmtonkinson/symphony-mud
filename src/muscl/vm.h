
#ifndef H_SYMPHONY_BONES_VM
#define H_SYMPHONY_BONES_VM

#include <list>
#include <map>
#include <stack>
#include <vector>
#include "cl.h"
#include "datum.h"

// miscellaneous
#define OPCODE_ERR      0
#define OPCODE_HALT     1
#define OPCODE_SEND     2
// data
#define OPCODE_VALUE    100
#define OPCODE_CONST    101
#define OPCODE_VAR      102
#define OPCODE_STORE    105
// items
#define OPCODE_ITEM_NAME   200
#define OPCODE_ITEM_INIT   201
#define OPCODE_ITEM_READ   202
#define OPCODE_ITEM_WRITE  203
// lists
#define OPCODE_ELMT     300
#define OPCODE_LIST     301
#define OPCODE_ITER_NEW 302 // create a new iterator on the list
#define OPCODE_ITER_INC 303 // increment the iterator
#define OPCODE_ITER_END 304 // test if the iterator is the end of the list
// branching
#define OPCODE_BRA    400
#define OPCODE_BRT    401
#define OPCODE_BRF    402
// functions
#define OPCODE_FUNC   500
#define OPCODE_PARAM  501
#define OPCODE_ARG    502
#define OPCODE_CALL   503
#define OPCODE_RETURN 504
// logical operations
#define OPCODE_AND    600
#define OPCODE_OR     601
#define OPCODE_NOT    602
// comparisons
#define OPCODE_COMP   700
#define OPCODE_EQ     701
#define OPCODE_NE     702
#define OPCODE_LT     703
#define OPCODE_LE     704
#define OPCODE_GT     705
#define OPCODE_GE     706
// mathematical operations
#define OPCODE_ADD    800
#define OPCODE_SUB    801
#define OPCODE_MUL    802
#define OPCODE_DIV    803
#define OPCODE_MOD    804
#define OPCODE_EXP    805
#define OPCODE_MIN    806
#define OPCODE_CARD   807
// complex storage operations
#define OPCODE_ADDSTORE 900
#define OPCODE_SUBSTORE 901
#define OPCODE_MULSTORE 902
#define OPCODE_DIVSTORE 903
#define OPCODE_MODSTORE 904
#define OPCODE_POWSTORE 905

// built-in functions (numeric)
#define OPCODE_FN_LOG     1000
#define OPCODE_FN_SIN     1001
#define OPCODE_FN_COS     1002
#define OPCODE_FN_TAN     1003
// built-in functions (strings)
#define OPCODE_FN_SUBSTR  1100
#define OPCODE_FN_SPLIT   1101
#define OPCODE_FN_MATCH   1102
// built-in functions (lists)
#define OPCODE_FN_SLICE   1200
#define OPCODE_FN_FIND    1201
#define OPCODE_FN_SORT    1202

class Frame;
class Operand;
class Operation;
class Record;
class VirtualMachine;

typedef std::vector<Record> CodeVect;
typedef std::vector<Datum> ResultList;
typedef void(VirtualMachine::*VMFunc)(void);
typedef std::map<int,VMFunc> OpMap;

class VirtualMachine {
  public:
    // constructors
    VirtualMachine(void);
    ~VirtualMachine(void);

    // public accessors
    void                goBabyGo(const bool& goBabyGo)          { _goBabyGo = goBabyGo; }
    bool                goBabyGo(void) const                    { return _goBabyGo; }
    void                quantum(const long& quantum)            { _quantum = quantum; }
    long                quantum(void) const                     { return _quantum; }
    void                depth(const long& depth)                { _depth = depth; }
    long                depth(void) const                       { return _depth; }
    void                operation(Record& operation);
    Record*             operation(void) const                   { return _operation; }
    Operand*            left(void) const                        { return _left; }
    Operand*            right(void) const                       { return _right; }
    void                ip(const long& ip)                      { _ip = ip; }
    long                ip(void) const                          { return _ip; }
    const OpMap&        functions(void) const                   { return _functions; }
    void                instructions(CodeVect* instructions)    { _instructions = instructions; }
    CodeVect&           instructions(void)                      { return *_instructions; }
    std::stack<Frame>&  frames(void)                            { return _frames; }
    ResultList&         results(void)                           { return _results; }
    ContextLayer*       context(void)                           { return &_context; }

    // other methods
    void  run(CodeVect* code);

  private:
    // data members
    bool                _goBabyGo;      // keeps the VM inner-loop churning
    long                _quantum;       // maximum number of instructions to run (to prevent infitite execution)
    long                _depth;         // maximum size of activation record stack
    Record*             _operation;     // "current" instruction
    Operand*            _left;          // left operand of the "current" instruction
    Operand*            _right;         // right operand of the "current" instruction
    long                _ip;            // instruction pointer
    OpMap               _functions;     // mapping from opcodes to instruction methods
    CodeVect*           _instructions;  // code to be executed
    std::stack<Frame>   _frames;        // activation records
    ResultList          _results;       // array (parallel to instructions) for opcode results
    ContextLayer        _context;       // for resolving item paths

    // methods
    void    init(CodeVect* code);
    void    cleanup(void);
    void    clear(ResultList* list);
    Frame&  current(void);
    void    execute(void);
    Datum&  fetch(Operand& op);
    void    dump_results(void);
    long    nextIP(void)                { return ++_ip; }

    // misc
    void  op_err(void);
    void  op_halt(void);
    void  op_send(void);
    // data
    void  op_value(void);
    void  op_const(void);
    void  op_var(void);
    void  op_store(void);
    // items
    void  op_obj_read(void);
    void  op_obj_write(void);
    // lists
    void  op_elmt(void);
    void  op_list(void);
    void  op_iter_new(void);
    void  op_iter_inc(void);
    void  op_iter_end(void);
    // branching
    void  op_bra(void);
    void  op_brt(void);
    void  op_brf(void);
    // functions
    void  op_func(void);
    void  op_param(void);
    void  op_arg(void);
    void  op_call(void);
    void  op_return(void);
    // logical
    void  op_and(void);
    void  op_or(void);
    void  op_not(void);
    // comparisons
    void  op_comp(void);
    void  op_eq(void);
    void  op_ne(void);
    void  op_lt(void);
    void  op_le(void);
    void  op_gt(void);
    void  op_ge(void);
    // math
    void  op_add(void);
    void  op_sub(void);
    void  op_mul(void);
    void  op_div(void);
    void  op_mod(void);
    void  op_exp(void);
    void  op_min(void);
    void  op_fact(void);
    void  op_card(void);
    // complex storage
    void  op_addstore(void);
    void  op_substore(void);
    void  op_mulstore(void);
    void  op_divstore(void);
    void  op_modstore(void);
    void  op_powstore(void);
};

#endif // #ifndef H_SYMPHONY_BONES_VM
