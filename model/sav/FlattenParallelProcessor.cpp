/*
 * FlattenParallelProcessor.cpp
 *
 *  Created on: Jan 26, 2017
 *      Author: ballance
 */

#include "FlattenParallelProcessor.h"
#include "RulesParallelItem.h"

namespace qpssc {

FlattenParallelProcessor::FlattenParallelProcessor() {
	// TODO Auto-generated constructor stub

}

FlattenParallelProcessor::~FlattenParallelProcessor() {
	// TODO Auto-generated destructor stub
}

bool FlattenParallelProcessor::process(IModel *model) {

	return true;
}

void FlattenParallelProcessor::visit_graph_block_stmt(IGraphBlockStmt *block) {

}

} /* namespace qpssc */