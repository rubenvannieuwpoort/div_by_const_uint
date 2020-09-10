#ifndef COMPILER_H
#define COMPILER_H

#include <string>
#include "bits.h"

class Expression {
public:
	virtual std::string code() = 0;
	virtual std::string access();
	virtual uint evaluate() = 0;
};

typedef Expression* expression_t;

class Constant : public Expression {
	uint c;
public:
	Constant(uint);
	std::string code();
	std::string access() override;
	uint evaluate();
};
Constant *constant(uint);

class Variable : public Expression {
	uint v;
public:
	Variable(uint);
	void assign(uint);
	std::string code();
	uint evaluate();
};
Variable *variable(uint);

typedef Variable* variable_t;

class Shr : public Expression {
	Expression *a, *b;
public:
	Shr(Expression *, Expression *);
	std::string code();
	uint evaluate();
};
Shr *shr(Expression *a, Expression *b);

class Umulhi : public Expression {
	Expression *a, *b;
public:
	Umulhi(Expression *, Expression *);
	std::string code();
	uint evaluate();
};
Umulhi* umulhi(Expression*, Expression*);

class Add : public Expression {
	Expression *a, *b;
public:
	Add(Expression *, Expression *);
	std::string code();
	uint evaluate();
};
Add* add(Expression *, Expression *);

class Gte : public Expression {
	Expression *a, *b;
public:
	Gte(Expression *, Expression *);
	std::string code();
	uint evaluate();
};
Gte *gte(Expression *, Expression *);

class Sbb : public Expression {
	Expression *a, *b;
public:
	Sbb(Expression *, Expression *);
	std::string code();
	uint evaluate();
};
Sbb* sbb(Expression *, Expression *);

#endif