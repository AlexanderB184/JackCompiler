typedef unsigned long long size_t;
typedef unsigned short u16;

enum class BinaryOperation { Add, Sub, And, Or, Eq, Gt, Lt };
enum class UnaryOperation { Neg, Not };
enum class MemorySegment { Static, This, That, Argument, Local, Temp, Pointer };
struct Function {  // Function has already been preprocessed in previous steps,
                   // i.e. if it is a method then the calling object has been
                   // added as the first arg
  char const* name;  // name of the function in the form
                     // CLASS_NAME.FUNCTION_NAME
};
struct Variable {
  MemorySegment segment;
  u16 offset;
};
struct LValueExpression {
  enum class Type { Variable, Array } type;
  union {
    Variable variable;
    struct {
      Variable array;
      Expression* index;
    } arrayData;
  } data;
};
struct Expression {
  enum class Type {
    IntConst,
    StrConst,
    Variable,
    Array,
    FunctionCall,
    BinaryOp,
    UnaryOp
  } type;
  union {
    struct {
      u16 number;
    } integerConstants;
    struct {
      char const* str;
    } stringConstant;
    struct {
      Function fn;
      size_t nArgs;
      Expression* args;
    } functionCall;
    struct {
      BinaryOperation op;
      Expression* lhs;
      Expression* rhs;
    } binaryExpression;
    struct {
      UnaryOperation op;
      Expression* target;
    } unaryExpression;
    Variable variable;
    struct {
      Variable arr;
      Expression* index;
    } arrayAccess;
  } data;
};

/**
 * A fixed size list of Statements
 */
struct Statements {
 private:
  size_t Length;
  Statement* List;

 public:
  inline size_t size() { return Length; }
  inline Statement& operator[](size_t i) { return List[i]; }
};
/**
 * A statement
 */
struct Statement {
 private:
  enum class Type { Let, Do, If, While, Return } type;
  union {
    struct {
      LValueExpression Lhs;
      Expression Rhs;
    } Let;
    struct {
      Expression Thing;
    } Do;
    struct {
      Expression Cond;
      Statements Then;
      Statements Else;
    } If;
    struct {
      Expression Cond;
      Statements Do;
    } While;
    struct {
      Expression ReturnValue;
    } Return;
  } data;

 public:
  // Specialized Constructors

  inline static Statement let_(LValueExpression lhs, Expression rhs) {
    Statement statement;
    statement.type = Type::Let;
    statement.data.Let.Lhs = lhs;
    statement.data.Let.Rhs = rhs;
    return statement;
  }
  inline static Statement do_(Expression thing) {
    Statement statement;
    statement.type = Type::Do;
    statement.data.Do.Thing = thing;
    return statement;
  }
  inline static Statement if_(Expression cond, Statements thenStmts,
                              Statements elseStmts) {
    Statement statement;
    statement.type = Type::If;
    statement.data.If.Cond = cond;
    statement.data.If.Then = thenStmts;
    statement.data.If.Else = elseStmts;
    return statement;
  }
  inline static Statement while_(Expression cond, Statements doStmts) {
    Statement statement;
    statement.type = Type::While;
    statement.data.While.Cond = cond;
    statement.data.While.Do = doStmts;
    return statement;
  }
  inline static Statement return_(Expression returnValue) {
    Statement statement;
    statement.type = Type::Return;
    statement.data.Return.ReturnValue = returnValue;
    return statement;
  }
  // Accessors

  inline Type getType() { return type; };
  inline LValueExpression getLetLHS() { return data.Let.Lhs; }
  inline Expression getLetRHS() { return data.Let.Rhs; }
  inline Expression getDoThing() { return data.Do.Thing; }
  inline Expression getIfCond() { return data.If.Cond; }
  inline Statements getIfThen() { return data.If.Then; }
  inline Statements getIfElse() { return data.If.Else; }
  inline Expression getWhileCondition() { return data.If.Cond; }
  inline Statements getWhileDoStatements() { return data.While.Do; }
  inline Expression getReturnValue() { return data.Return.ReturnValue; }
};
