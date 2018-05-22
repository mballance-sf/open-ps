/*
 * Z3ModelBuildField.h
 *
 *  Created on: May 18, 2018
 *      Author: ballance
 */

#pragma once
#include "Z3ModelVar.h"
#include "IField.h"
#include "PSIVisitor.h"



class Z3ModelBuilder;
class Z3ModelBuildField : public virtual PSIVisitor {
public:
	Z3ModelBuildField(Z3ModelBuilder *builder);

	virtual ~Z3ModelBuildField();

	Z3ModelVar *build(IField *field);

protected:

	virtual void visit_field(IField *f) override;

	uint32_t compute_bits(IScalarType *t);

	bool compute_sign(IScalarType *t);

	void compute_domain(Z3ModelVar &var);

private:
	Z3ModelBuilder				*m_builder;

};

