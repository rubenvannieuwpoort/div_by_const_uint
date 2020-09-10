#include "compiler.h"

std::string Expression::access() { return "r0"; }

Constant::Constant(uint c) : c(c) { }
std::string Constant::code() { return ""; }
std::string Constant::access() { return std::to_string(c); }
uint Constant::evaluate() { return c; }
Constant* constant(uint c) { return new Constant(c); }

Variable::Variable(uint v) : v(v) { }
std::string Variable::code() { return ""; }
uint Variable::evaluate() { return v; }
void Variable::assign(uint v) { this->v = v; }
Variable* variable(uint v) { return new Variable(v); }

Shr::Shr(Expression* a, Expression* b) : a(a), b(b) { }
std::string Shr::code() { return a->code() + b->code() + "\tshr r0, " + a->access() + ", " + b->access() + "\n"; }
uint Shr::evaluate() { return a->evaluate() >> b->evaluate(); }
Shr* shr(Expression* a, Expression* b) { return new Shr(a, b); }

Umulhi::Umulhi(Expression* a, Expression* b) : a(a), b(b) { }
std::string Umulhi::code() { return a->code() + b->code() + "\tumulhi r0, " + a->access() + ", " + b->access() + "\n"; }
uint Umulhi::evaluate() { return ((big_uint)a->evaluate() * b->evaluate()) >> N; }
Umulhi* umulhi(Expression* a, Expression* b) { return new Umulhi(a, b); }

bool carry = false;
Add::Add(Expression* a, Expression* b) : a(a), b(b) { }
std::string Add::code() { return a->code() + b->code() + "\tadd r0, " + a->access() + ", " + b->access() + "\n"; }
uint Add::evaluate() {
	uint left = a->evaluate();
	uint right = b->evaluate();
	carry = left > MAX - right;
	return left + right;
}
Add *add(Expression* a, Expression* b) { return new Add(a, b); }

Gte::Gte(Expression* a, Expression* b) : a(a), b(b) { }
std::string Gte::code() { return a->code() + b->code() + "\tgte r0, " + a->access() + ", " + b->access() + "\n"; }
uint Gte::evaluate() { return a->evaluate() >= b->evaluate(); }
Gte* gte(Expression* a, Expression* b) { return new Gte(a, b); }

Sbb::Sbb(Expression* a, Expression* b) : a(a), b(b) { }
std::string Sbb::code() { return a->code() + b->code() + "\tsbb r0, " + a->access() + ", " + b->access() + "\n"; }
uint Sbb::evaluate() { return a->evaluate() - b->evaluate() - carry; }
Sbb* sbb(Expression* a, Expression* b) { return new Sbb(a, b); }