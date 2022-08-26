#ifndef STATEMENT_H
#define STATEMENT_H

class Expression;
class Variable;
class Statement {
  public:
    void append_statement(Statement* atend);
    Statement() {}
    virtual void execute() const=0;
    virtual ~Statement() {delete next;}
  protected:
    Statement* next{nullptr};
};

//##
// NullStatement
//##
class NullStatement : public Statement {
  public:
    NullStatement(){}
    virtual void execute() const;
};

#endif
