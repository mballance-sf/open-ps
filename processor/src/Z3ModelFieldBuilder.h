/*
 * Z3ModelFieldBuilder.h
 *
 *  Created on: May 18, 2018
 *      Author: ballance
 */

#pragma once
#include "Z3ModelVar.h"
#include "IField.h"
#include "PSIVisitor.h"



class Z3ModelBuilder;
class Z3ModelFieldBuilder : public virtual PSIVisitor {
public:
	Z3ModelFieldBuilder(Z3ModelBuilder *builder);

	virtual ~Z3ModelFieldBuilder();

	Z3ModelVar *build(IField *field);

protected:

	virtual void visit_field(IField *f) override;

	uint32_t compute_bits(IScalarType *t);

	bool compute_sign(IScalarType *t);

	void compute_domain(Z3ModelVar &var);

private:
	Z3ModelBuilder				*m_builder;

};

