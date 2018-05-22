/*
 * Z3ModelBuildField.cpp
 *
 *  Created on: May 18, 2018
 *      Author: ballance
 */

#include "Z3ModelBuilder.h"
#include "z3.h"
#include <math.h>
#include <limits.h>
#include "Z3ModelBuildField.h"

Z3ModelBuildField::Z3ModelBuildField(Z3ModelBuilder *builder) : m_builder(builder) {
	// TODO Auto-generated constructor stub

}

Z3ModelBuildField::~Z3ModelBuildField() {
	// TODO Auto-generated destructor stub
}

void Z3ModelBuildField::visit_field(IField *f) {
	fprintf(stdout, "visit_field: %s\n", f->getName().c_str());

	if (f->getDataType()->getType() == IBaseItem::TypeScalar) {
		std::string id = m_builder->prefix() + "." + f->getName();
		uint32_t bits = compute_bits(
				dynamic_cast<IScalarType *>(f->getDataType()));
		VarValType type;
		IScalarType *st = dynamic_cast<IScalarType *>(f->getDataType());

		switch (st->getScalarType()) {
		case IScalarType::ScalarType_Bit: type = VarVal_Uint; break;
		case IScalarType::ScalarType_Int: type = VarVal_Int; break;
		case IScalarType::ScalarType_Bool: type = VarVal_Bool; break;
		case IScalarType::ScalarType_String: type = VarVal_String; break;
		}

		fprintf(stdout, "Note: scalar variable %s\n", id.c_str());

		Z3_ast var = Z3_mk_const(m_builder->ctxt(),
				Z3_mk_string_symbol(m_builder->ctxt(), id.c_str()),
				Z3_mk_bv_sort(m_builder->ctxt(), bits));
		m_builder->add_variable(new Z3ModelVar(id, var, bits, type));

	} else if (f->getDataType()->getType() == IBaseItem::TypeArray) {

	} else if (f->getDataType()->getType() == IBaseItem::TypeRefType) {
		IRefType *ref = dynamic_cast<IRefType *>(f->getDataType());
		m_builder->push_prefix(f->getName());
		switch (ref->getTargetType()->getType()) {
		case IBaseItem::TypeStruct:
			visit_struct(dynamic_cast<IStruct *>(ref->getTargetType()));
			break;
		case IBaseItem::TypeAction:
			visit_action(dynamic_cast<IAction *>(ref->getTargetType()));
			break;
		default:
			fprintf(stdout, "Error: unknown composite type\n");
		}
		m_builder->pop_prefix();
	} else if (f->getDataType()->getType() == IBaseItem::TypeComponent) {
		if (dynamic_cast<IAction *>(f->getParent())) {
			// Ignore component-type fields in action, since this
			// is 'comp'
		} else {
			fprintf(stdout, "TODO: determine whether component-type field should be ignored\n");
		}
	} else {
		// composite data type?
		fprintf(stdout, "Error: unknown data type %d\n",
				f->getDataType()->getType());
	}
}

uint32_t Z3ModelBuildField::compute_bits(IScalarType *t) {
	uint32_t ret = 32;

	switch (t->getScalarType()) {
		case IScalarType::ScalarType_Bool:
			ret = 1;
			break;
		case IScalarType::ScalarType_Bit: {
			if (!t->getLSB() && !t->getMSB()) {
				ret = 1;
			} else if (t->getLSB() && t->getMSB()) {
				fprintf(stdout, "TODO: MSB:LSB bit-width\n");
			} else {
				// Assume bit-width is specified
				if (t->getLSB()->getType() == IExpr::ExprType_Literal) {
					ILiteral *l = dynamic_cast<ILiteral *>(t->getLSB());
					ret = l->getBit();
				} else {
					fprintf(stdout, "Error: unsupported bit-width\n");
				}
			}
		} break;
		case IScalarType::ScalarType_Int: {
			if (!t->getLSB() && !t->getMSB()) {
				ret = 32;
			} else if (t->getLSB() && t->getMSB()) {
				fprintf(stdout, "TODO: MSB:LSB bit-width\n");
			} else {
				// Assume bit-width is specified
				if (t->getLSB()->getType() == IExpr::ExprType_Literal) {
					ILiteral *l = dynamic_cast<ILiteral *>(t->getLSB());
					ret = l->getBit();
				} else {
					fprintf(stdout, "Error: unsupported bit-width\n");
				}
			}
		} break;

		case IScalarType::ScalarType_String: {
			ret = m_builder->strtab().bits();
			fprintf(stdout, "String bits: %d\n", ret);
		} break;

		default:
			fprintf(stdout, "Error: unhandled compute-bits case %d\n",
					t->getScalarType());
			break;
	}

	return ret;
}

bool Z3ModelBuildField::compute_sign(IScalarType *t) {
	bool is_signed = true;

	switch (t->getScalarType()) {
		case IScalarType::ScalarType_Bool:
			is_signed = false;
			break;
		case IScalarType::ScalarType_Bit:
			is_signed = false;
		break;
		case IScalarType::ScalarType_Int:
			is_signed = true;
		break;

		case IScalarType::ScalarType_String:
			is_signed = false;
		break;

		default:
			fprintf(stdout, "Error: unhandled compute-bits case %d\n",
					t->getScalarType());
			break;
	}

	return is_signed;
}



void Z3ModelBuildField::compute_domain(Z3ModelVar &var) {
	if (var.is_signed()) {
		int64_t min = LLONG_MIN;
		int64_t max = LLONG_MAX;


	} else {

	}
}
