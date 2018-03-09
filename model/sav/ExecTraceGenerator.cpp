/*
 * ExecTraceGenerator.cpp
 *
 *  Created on: Jan 3, 2017
 *      Author: ballance
 */

#include "ExecTraceGenerator.h"
#include "QPSSCUtils.h"
#include "automataManager.h"
#include "ModelImpl.h"
#include <stdint.h>
#include <stdio.h>
#include <map>
#include <stdarg.h>

namespace qpssc {

class ProxyCallbackContext : public virtual ICallbackContext {
public:

	virtual IImportFunc *getFunction(psshandle_t hndl) {
		return reinterpret_cast<ICallbackContext *>(hndl)->getFunction(hndl);
	}

	virtual int64_t getIntField(
			psshandle_t 	hndl,
			IField 			*field) {
		return reinterpret_cast<ICallbackContext *>(hndl)->getIntField(hndl, field);
	}

	virtual void setIntField(
			psshandle_t 	hndl,
			IField 			*field,
			int64_t 		v) {
		reinterpret_cast<ICallbackContext *>(hndl)->setIntField(hndl, field, v);
	}

	virtual uint64_t getBitField(
			psshandle_t 	hndl,
			IField 			*field) {
		return reinterpret_cast<ICallbackContext *>(hndl)->getBitField(hndl, field);
	}

	virtual void setBitField(
			psshandle_t 	hndl,
			IField 			*field,
			uint64_t 		v) {

	}

	virtual bool getBoolField(
			psshandle_t		hndl,
			IField			*field) {
		return reinterpret_cast<ICallbackContext *>(hndl)->getBoolField(hndl, field);
	}

	virtual void setBoolField(
			psshandle_t		hndl,
			IField			*field,
			bool			v) {

	}

	virtual void *getChandleField(
			psshandle_t 	hndl,
			IField 			*field) {
		return reinterpret_cast<ICallbackContext *>(hndl)->getChandleField(hndl, field);
	}

	virtual void setChandleField(
			psshandle_t 	hndl,
			IField 			*field,
			void 			*v) {

	}

	virtual std::string getStringField(
			psshandle_t 	hndl,
			IField 			*field) {
		return reinterpret_cast<ICallbackContext *>(hndl)->getStringField(hndl, field);
	}

	virtual void setStringField(
			psshandle_t 		hndl,
			IField 				*field,
			const std::string 	&v) {

	}

	/**
	 * Returns the psshandle_t handle for a field of type struct or action
	 */
	virtual psshandle_t getHandleField(
			psshandle_t 		hndl,
			IField 				*field) {
		return reinterpret_cast<ICallbackContext *>(hndl)->getHandleField(hndl, field);
	}
};

class FunctionCallbackContext : public virtual ICallbackContext {
public:

	FunctionCallbackContext(IImportFunc *f) {
		m_function = f;
	}

	virtual IImportFunc *getFunction(psshandle_t hndl) {
		return m_function;
	}

	virtual int64_t getIntField(
			psshandle_t 	hndl,
			IField 			*field) {
		std::map<IField *, uint64_t>::iterator it = m_values.find(field);
		return it->second;
	}

	virtual void setIntField(
			psshandle_t 	hndl,
			IField 			*field,
			int64_t 		v) {
		std::map<IField *, uint64_t>::iterator it = m_values.find(field);
		if (it == m_values.end()) {
			// Insert it
			m_values[field] = v;
		} else {
			it->second = v;
		}
	}

	virtual uint64_t getBitField(
			psshandle_t 	hndl,
			IField 			*field) {
		std::map<IField *, uint64_t>::iterator it = m_values.find(field);
		return it->second;
	}

	virtual void setBitField(
			psshandle_t 	hndl,
			IField 			*field,
			uint64_t 		v) {
		std::map<IField *, uint64_t>::iterator it = m_values.find(field);
		if (it == m_values.end()) {
			// Insert it
			m_values[field] = v;
		} else {
			it->second = v;
		}
	}

	virtual bool getBoolField(
			psshandle_t		hndl,
			IField			*field) {
		std::map<IField *, uint64_t>::iterator it = m_values.find(field);
		return (it->second != 0);
	}

	virtual void setBoolField(
			psshandle_t		hndl,
			IField			*field,
			bool			v) {
		std::map<IField *, uint64_t>::iterator it = m_values.find(field);
		if (it == m_values.end()) {
			// Insert it
			m_values[field] = v;
		} else {
			it->second = v;
		}
	}

	virtual void *getChandleField(
			psshandle_t 	hndl,
			IField 			*field) {
		std::map<IField *, uint64_t>::iterator it = m_values.find(field);
		return reinterpret_cast<void *>(it->second);
	}

	virtual void setChandleField(
			psshandle_t 	hndl,
			IField 			*field,
			void 			*v) {
		std::map<IField *, uint64_t>::iterator it = m_values.find(field);
		if (it == m_values.end()) {
			// Insert it
			m_values[field] = reinterpret_cast<uint64_t>(v);
		} else {
			it->second = reinterpret_cast<uint64_t>(v);
		}
	}

	virtual std::string getStringField(
			psshandle_t 	hndl,
			IField 			*field) {
		return "";
	}

	virtual void setStringField(
			psshandle_t 		hndl,
			IField 				*field,
			const std::string 	&v) {
		// TODO:
	}

	/**
	 * Returns the psshandle_t handle for a field of type struct or action
	 */
	virtual psshandle_t getHandleField(
			psshandle_t 		hndl,
			IField 				*field) {
		return 0; // TODO:
	}

private:
	IImportFunc							*m_function;
	std::map<IField *, uint64_t>		m_values;

};

class EngineCallbackContext : public virtual ICallbackContext {
public:

	EngineCallbackContext(
			Engine 				*eng,
			const std::string 	&prefix,
			IImportFunc			*function) {
		m_eng = eng;
		m_prefix = prefix;
		m_function = function;
	}

	virtual IImportFunc *getFunction(psshandle_t hndl) {
		return m_function;
	}

	virtual int64_t getIntField(
			psshandle_t 	hndl,
			IField 			*field) {
		std::string path = m_prefix;
		return m_eng->get_field(m_prefix + "." + field->getName());
	}

	virtual void setIntField(
			psshandle_t 	hndl,
			IField 			*field,
			int64_t 		v) = 0;

	virtual uint64_t getBitField(
			psshandle_t 	hndl,
			IField 			*field) = 0;

	virtual void setBitField(
			psshandle_t 	hndl,
			IField 			*field,
			uint64_t 		v) = 0;

	virtual bool getBoolField(
			psshandle_t		hndl,
			IField			*field) = 0;

	virtual void setBoolField(
			psshandle_t		hndl,
			IField			*field,
			bool			v) = 0;

	virtual void *getChandleField(
			psshandle_t 	hndl,
			IField 			*field) = 0;

	virtual void setChandleField(
			psshandle_t 	hndl,
			IField 			*field,
			void 			*v) = 0;

	virtual std::string getStringField(
			psshandle_t 	hndl,
			IField 			*field) = 0;

	virtual void setStringField(
			psshandle_t 		hndl,
			IField 				*field,
			const std::string 	&v) = 0;

	/**
	 * Returns the psshandle_t handle for a field of type struct or action
	 */
	virtual psshandle_t getHandleField(
			psshandle_t 		hndl,
			IField 				*field) = 0;

private:
	Engine				*m_eng;
	std::string			m_prefix;
	IImportFunc			*m_function;

};

class PrintImportCallCB : public IExecCallback {
public:
	PrintImportCallCB(
			IImportFunc		*func) {
		m_func = func;
	}

	virtual void exec(
			IModel			*model,
			psshandle_t		instance_h) {
		ICallbackContext *ctxt = model->getCallbackContext();

		fprintf(stdout, "exec: %s(", m_func->getName().c_str());
		for (uint32_t i=0; i<m_func->getParameters().size(); i++) {
			IField *p = m_func->getParameters().at(i);

			fprintf(stdout, "%s=", p->getName().c_str());
			switch (p->getDataType()->getType()) {
			case IBaseItem::TypeScalar: {
				IScalarType *st = dynamic_cast<IScalarType *>(p->getDataType());
				switch (st->getScalarType()) {
				case IScalarType::ScalarType_Bit:
					fprintf(stdout, "%llu", (unsigned long long)
							ctxt->getBitField(instance_h, p));
					break;
				case IScalarType::ScalarType_Bool:
					fprintf(stdout, "%s", (ctxt->getBoolField(instance_h, p))?"true":"false");
					break;
				case IScalarType::ScalarType_Chandle:
					break;
				case IScalarType::ScalarType_Int:
					fprintf(stdout, "%lld", (long long)ctxt->getIntField(instance_h, p));
					break;
				case IScalarType::ScalarType_String:
					break;
				}

				} break;
			default:
				fprintf(stdout, "<unknown>");
			}

			if (i+1 < m_func->getParameters().size()) {
				fprintf(stdout, ", ");
			}
		}
		fprintf(stdout, ")\n");
	}

private:
	IImportFunc				*m_func;

};

class StopLoopExpansion : public IExecCallback {
public:
	StopLoopExpansion(Engine *eng) {
		m_eng = eng;
	}

	virtual void exec(
			IModel				*model,
			psshandle_t			instance_h) {
		m_eng->stopLoopExpansionAfter(0, 0);
	}

private:
	Engine			*m_eng;
};

class ThreadContextCreate : public IExecCallback {
public:

	ThreadContextCreate(ExecTraceGenerator *trace) {
		m_trace = trace;
	}

	virtual void exec(
			IModel				*model,
			psshandle_t			instance_h) {
		m_trace->enter_parallel();
	}

private:
	ExecTraceGenerator		*m_trace;
};

class ThreadContextJoin : public IExecCallback {
public:

	ThreadContextJoin(ExecTraceGenerator *trace) {
		m_trace = trace;
	}

	virtual void exec(
			IModel				*model,
			psshandle_t			instance_h) {
		m_trace->join_parallel();
	}

private:
	ExecTraceGenerator		*m_trace;
};

class ThreadBegin : public IExecCallback {
public:

	ThreadBegin(ExecTraceGenerator *trace) {
		m_trace = trace;
	}

	virtual void exec(
			IModel				*model,
			psshandle_t			instance_h) {
		m_trace->start_thread();
	}

private:
	ExecTraceGenerator		*m_trace;
};

class ThreadEnd : public IExecCallback {
public:

	ThreadEnd(ExecTraceGenerator *trace) {
		m_trace = trace;
	}

	virtual void exec(
			IModel				*model,
			psshandle_t			instance_h) {
		m_trace->end_thread();
	}

private:
	ExecTraceGenerator		*m_trace;
};

class RepeatBegin : public IExecCallback {
public:

	RepeatBegin(ExecTraceGenerator *trace) {
		m_trace = trace;
	}

	virtual void exec(
			IModel				*model,
			psshandle_t			instance_h) {
		m_trace->repeat_begin();
	}

private:
	ExecTraceGenerator			*m_trace;
};

class RepeatEnd : public IExecCallback {
public:

	RepeatEnd(ExecTraceGenerator *trace) {
		m_trace = trace;
	}

	virtual void exec(
			IModel				*model,
			psshandle_t			instance_h) {
		m_trace->repeat_end();
	}

private:
	ExecTraceGenerator			*m_trace;
};

ExecTraceGenerator::ExecTraceGenerator(const std::string &outdir) {
	m_outdir = outdir;
	m_engine = 0;
	m_model = 0;
}

ExecTraceGenerator::~ExecTraceGenerator() {
	// TODO Auto-generated destructor stub
}

bool ExecTraceGenerator::process(IModel *model) {
	std::string aut_path, aut_name;
	IComponent *comp;
	IAction *action;
	uint32_t eid;
	const char *ctxt;
	uint32_t cnt=0, max=100000, have_next_method;
	ProxyCallbackContext cb_ctxt;

	m_model = model;
	dynamic_cast<ModelImpl *>(model)->setCallbackContext(&cb_ctxt);

	if (!QPSSCUtils::get_roots(model, &comp, &action)) {
		return false;
	}

	aut_name = comp->getName() + "_" + action->getName();
	aut_path = m_outdir + "/" + aut_name + ".aut";

	// Load up the .aut file
	AutomataManager &mgr = AutomataManager::instance();

	mgr.registerNewAutomaton(aut_path.c_str());

	// Create an instance of the engine using the .aut file
	debug("--> new Engine\n");
	m_engine = new Engine(aut_name.c_str(), aut_name.c_str());
	debug("<-- new Engine\n");

	m_engine->init();

	m_pass = 0;
	visit_model(model);

	m_pass = 1;
	m_exec_path.clear();
	m_exec_path.push_back("__root");
	visit_action(action);

	// Hook thread-management
	{
		uint32_t id;
		std::string name;

		id = m_exec_list.size();
		name = "__thread_context_create";
		m_engine->set_method_id(name, id+1, false);
		m_id_exec_map[id+1] = new ExecImpl(IExec::Body,
				new ThreadContextCreate(this));
		m_exec_list.push_back(name);

		id = m_exec_list.size();
		name = "__thread_context_join";
		m_engine->set_method_id(name, id+1, false);
		m_id_exec_map[id+1] = new ExecImpl(IExec::Body,
				new ThreadContextJoin(this));
		m_exec_list.push_back(name);

		id = m_exec_list.size();
		name = "__thread_begin";
		m_engine->set_method_id(name, id+1, false);
		m_id_exec_map[id+1] = new ExecImpl(IExec::Body,
				new ThreadBegin(this));
		m_exec_list.push_back(name);

		id = m_exec_list.size();
		name = "__thread_end";
		m_engine->set_method_id(name, id+1, false);
		m_id_exec_map[id+1] = new ExecImpl(IExec::Body,
				new ThreadEnd(this));
		m_exec_list.push_back(name);

	}

	// TODO: hook built-in import methods
	m_pass = 2;
	visit_model(model);

	while ((have_next_method=m_engine->get_next_method(&eid, &ctxt)) && (cnt++ < max)) {
		debug("Method: %d %s\n", eid, ctxt);
		std::map<uint32_t,IExec *>::iterator it = m_id_exec_map.find(eid);
		if (it != m_id_exec_map.end() && it->second) {
			do_exec(it->second, ctxt);
		} else {
			warn("Exec: %d not found\n", eid);
		}
		m_engine->end_of_method();
	}

	if (!have_next_method) {
		note("Terminated due to end of activity\n");
	} else {
		note("Terminated due to timeout\n");
	}

	delete m_engine;

	return true;
}

void ExecTraceGenerator::visit_field(IField *f) {
	if (m_pass == 1) {
		m_exec_path.push_back(f->getName());
		if (f->getDataType()->getType() == IBaseItem::TypeAction) {
			visit_action(dynamic_cast<IAction *>(f->getDataType()));
		} else if (f->getDataType()->getType() == IBaseItem::TypeStruct) {
			visit_struct(dynamic_cast<IStruct *>(f->getDataType()));
		}
		m_exec_path.pop_back();
	}
}

void ExecTraceGenerator::visit_exec(IExec *e) {
	std::string type;
	switch (e->getExecKind()) {
	case IExec::Declaration: type = "declaration"; break;
	case IExec::Body: type = "body"; break;
	case IExec::PreSolve: type = "pre_solve"; break;
	case IExec::PostSolve: type = "post_solve"; break;
	}

	INamedItem *e_parent = dynamic_cast<INamedItem *>(e->getParent());

	std::string exec_name = "exec_";
	exec_name.append(e_parent->getName());
	exec_name.append("_");
	exec_name.append(type);

	if (m_pass == 0) { // Building up list of execs
		m_exec_list.push_back(exec_name);
		m_id_exec_map[m_exec_list.size()] = e;
	} else if (m_pass == 1) {
		switch (e->getExecKind()) {
		case IExec::Body: m_exec_path.push_back("__body"); break;
		case IExec::PreSolve: m_exec_path.push_back("__pre_solve"); break;
		case IExec::PostSolve: m_exec_path.push_back("__post_solve"); break;
		default: m_exec_path.push_back("__unsupported"); break;
		}

		std::string exec_path = list2path(m_exec_path);
		int32_t id = -1;

		for (uint32_t i=0; i<m_exec_list.size(); i++) {
			if (m_exec_list.at(i) == exec_name) {
				id = (i+1);
				break;
			}
		}

		m_engine->set_method_id(exec_path, id);

		m_exec_path.pop_back();
	}
}

void ExecTraceGenerator::visit_extend(IExtend *e) {
	// NOP
}

void ExecTraceGenerator::visit_rules_action_exec_item(RulesExecActionItem *it) {
	std::string exec_name = "exec_";
	exec_name.append(it->getName());

	if (m_pass == 0) {
		m_exec_list.push_back(exec_name);
		m_id_exec_map[m_exec_list.size()] = it->getExec();
	} else if (m_pass == 1) {
		m_exec_path.push_back(it->getName());

		std::string exec_path = list2path(m_exec_path);
		int32_t id = -1;

		for (uint32_t i=0; i<m_exec_list.size(); i++) {
			if (m_exec_list.at(i) == exec_name) {
				id = (i+1);
				break;
			}
		}

		m_engine->set_method_id(exec_path, id);

		m_exec_path.pop_back();
	}
}

void ExecTraceGenerator::visit_import_func(IImportFunc *f) {
	if (m_pass == 2) {
		if (f->hasAttribute("BUILTIN")) {
			std::string name = f->getName();

			if (name == "stop_loop_expansion") {
				f->setCallback(new StopLoopExpansion(m_engine));
			} else if (name == "thread_context_create") {
				f->setCallback(new ThreadContextCreate(this));
			} else if (name == "thread_context_join") {
				f->setCallback(new ThreadContextJoin(this));
			} else if (name == "thread_begin") {
				f->setCallback(new ThreadBegin(this));
			} else if (name == "thread_end") {
				f->setCallback(new ThreadEnd(this));
			} else if (name == "repeat_begin") {
				f->setCallback(new RepeatBegin(this));
			} else if (name == "repeat_end") {
				f->setCallback(new RepeatEnd(this));
			}
			debug("BUILTIN: %s\n", f->getName().c_str());
		} else {
			// Regular function
			f->setCallback(create_import_func_cb(f));
		}
	}
}

void ExecTraceGenerator::repeat_begin() {
	note("RepeatBegin");
}

void ExecTraceGenerator::repeat_end() {
	note("RepeatEnd");
}

IExecCallback *ExecTraceGenerator::create_import_func_cb(IImportFunc *f) {
	return new PrintImportCallCB(f);
}

IExecCallback *ExecTraceGenerator::create_template_exec_cb(IExec *e) {
	// TODO:
	return 0;
}

void ExecTraceGenerator::enter_parallel() {

}

void ExecTraceGenerator::start_thread() {

}

void ExecTraceGenerator::end_thread() {

}

void ExecTraceGenerator::join_parallel() {

}

void ExecTraceGenerator::do_exec(
		IExec 				*e,
		const std::string 	&ctxt) {
	debug("--> do_exec %s %d\n", ctxt.c_str(), e->getStmts().size());
	switch (e->getExecType()) {
	case IExec::Native: {
		for (std::vector<IExecStmt *>::const_iterator it=e->getStmts().begin();
				it!=e->getStmts().end(); it++) {
			exec_stmt(e->getParent(), *it, ctxt);
		}
	} break;

	case IExec::Inline: {
		e->getInlineExec()->exec(m_model, 0);
	} break;
	}

	debug("<-- do_exec %s\n", ctxt.c_str());
}

void ExecTraceGenerator::exec_stmt(
		IBaseItem			*scope,
		IExecStmt 			*s,
		const std::string 	&ctxt_s) {

	switch (s->getStmtType()) {

	case IExecStmt::StmtType_Expr: {
		IExecExprStmt *stmt = dynamic_cast<IExecExprStmt *>(s);

		if (stmt->getRhs()) {
			std::string varname = ctxt_s;
			int64_t val = -1;
			if (stmt->getLhs()->getType() != IExpr::ExprType_FieldRef) {
				fprintf(stdout, "Error: assignment statement with a non-fieldref LHS\n");
			} else {
				IFieldRef *ref = dynamic_cast<IFieldRef *>(stmt->getLhs());
				for (std::vector<IField *>::const_iterator it=ref->getFieldPath().begin();
					it!=ref->getFieldPath().end(); it++) {
					varname += ".";
					varname += (*it)->getName();
				}
				val = eval_expr_s(scope, ctxt_s, stmt->getRhs());
			}

			switch (stmt->getOp()) {
			case IExecExprStmt::AssignOp_Eq:
				debug("Assign: %s=%lld\n", varname.c_str(), val);
				m_engine->set_field(varname, val);
				break;
			case IExecExprStmt::AssignOp_AndEq:
				m_engine->set_field(varname,
						m_engine->get_field(varname) & val);
				break;
			case IExecExprStmt::AssignOp_OrEq:
				m_engine->set_field(varname,
						m_engine->get_field(varname) | val);
				break;
			case IExecExprStmt::AssignOp_PlusEq:
				debug("Assign(+=): %s=%lld\n", varname.c_str(),
						m_engine->get_field(varname) + val);
				m_engine->set_field(varname,
						m_engine->get_field(varname) + val);
				break;
			default:
				fprintf(stdout, "Error: Unhandled assign-op %d\n", stmt->getOp());
				break;
			}
		} else {
			// Simple expression
			eval_expr_s(scope, ctxt_s, stmt->getLhs());
		}
	} break;

	case IExecStmt::StmtType_Vendor: {
		RulesExecStmtItem *stmt = dynamic_cast<RulesExecStmtItem *>(s);
		switch (stmt->getRulesStmtType()) {
			case RulesExecStmtItem::If: {
				RulesExecStmtIfItem *if_i = dynamic_cast<RulesExecStmtIfItem *>(stmt);
				uint64_t if_cond = eval_expr_u(scope, ctxt_s, if_i->getCond());

				debug("if_cond: %d\n", if_cond);

				if (if_cond) {
					for (std::vector<IExecStmt *>::const_iterator it=if_i->getTrue().begin();
							it!=if_i->getTrue().end(); it++) {
						exec_stmt(scope, *it, ctxt_s);
					}
				} else {
					for (std::vector<IExecStmt *>::const_iterator it=if_i->getFalse().begin();
							it!=if_i->getFalse().end(); it++) {
						exec_stmt(scope, *it, ctxt_s);
					}
				}
			} break;
			default:
				fprintf(stdout, "Error: unhandled stmt Vendor %d\n", stmt->getRulesStmtType());
				break;
		}
	} break;

	default:
		fprintf(stdout, "Error: unknown stmt %d\n", s->getStmtType());
		break;

	}
}

uint64_t ExecTraceGenerator::eval_expr_u(
		IBaseItem			*scope,
		const std::string	&ctxt,
		IExpr				*e) {
	uint64_t ret = 0;

	switch (e->getType()) {
	case IExpr::ExprType_BinOp: {
		IBinaryExpr *bin_e = dynamic_cast<IBinaryExpr *>(e);
		switch (bin_e->getBinOpType()) {
		case IBinaryExpr::BinOp_And:
			ret = (eval_expr_u(scope, ctxt, bin_e->getLHS()) & eval_expr_u(scope, ctxt, bin_e->getRHS()));
			break;
		case IBinaryExpr::BinOp_AndAnd:
			ret = (eval_expr_u(scope, ctxt, bin_e->getLHS()) && eval_expr_u(scope, ctxt, bin_e->getRHS()));
			break;
			// ArrayRef
		case IBinaryExpr::BinOp_Divide:
			ret = (eval_expr_u(scope, ctxt, bin_e->getLHS()) / eval_expr_u(scope, ctxt, bin_e->getRHS()));
			break;
			// Eq
		case IBinaryExpr::BinOp_EqEq:
			ret = (eval_expr_u(scope, ctxt, bin_e->getLHS()) == eval_expr_u(scope, ctxt, bin_e->getRHS()));
			break;
		case IBinaryExpr::BinOp_NotEq:
			ret = (eval_expr_u(scope, ctxt, bin_e->getLHS()) != eval_expr_u(scope, ctxt, bin_e->getRHS()));
			break;
		case IBinaryExpr::BinOp_GE:
			ret = (eval_expr_u(scope, ctxt, bin_e->getLHS()) >= eval_expr_u(scope, ctxt, bin_e->getRHS()));
			break;
		case IBinaryExpr::BinOp_GT:
			ret = (eval_expr_u(scope, ctxt, bin_e->getLHS()) > eval_expr_u(scope, ctxt, bin_e->getRHS()));
			break;
		case IBinaryExpr::BinOp_LE:
			ret = (eval_expr_u(scope, ctxt, bin_e->getLHS()) <= eval_expr_u(scope, ctxt, bin_e->getRHS()));
			break;
		case IBinaryExpr::BinOp_LT:
			ret = (eval_expr_u(scope, ctxt, bin_e->getLHS()) < eval_expr_u(scope, ctxt, bin_e->getRHS()));
			break;

		default:
			fprintf(stdout, "Error: Unhandled eval_expr_u: %d\n", bin_e->getBinOpType());
		}
	} break;

	case IExpr::ExprType_FieldRef: {
		IFieldRef *ref = dynamic_cast<IFieldRef *>(e);
		std::string varname = ctxt;
		for (std::vector<IField *>::const_iterator it=ref->getFieldPath().begin();
				it!=ref->getFieldPath().end(); it++) {
			varname += ".";
			varname += (*it)->getName();
		}
		ret = (uint64_t)m_engine->get_field(varname);
	} break;

	case IExpr::ExprType_Literal: {
		ret = dynamic_cast<ILiteral *>(e)->getBit();
	} break;

	case IExpr::ExprType_MethodCall: {
		IMethodCallExpr *stmt = dynamic_cast<IMethodCallExpr *>(e);
		IImportFunc *f = stmt->getFunc();
		IExecCallback *cb = f->getCallback();
		FunctionCallbackContext ctxt_f(f);

		// TODO: probably need parameters to actually be IExecStmt
		// TODO: must evaluate parameter expressions and set them in ctxt

		for (uint32_t i=0; i<f->getParameters().size(); i++) {
			IExpr *p = stmt->getParameters().at(i);
			IBaseItem *dt = f->getParameters().at(i)->getDataType();

			switch (dt->getType()) {
			case IBaseItem::TypeScalar: {
				IScalarType *st = dynamic_cast<IScalarType *>(dt);
				// TODO:
				switch (st->getScalarType()) {
				case IScalarType::ScalarType_Bit:
					ctxt_f.setBitField(0, f->getParameters().at(i),
							eval_expr_u(scope, ctxt, p));
					break;
				case IScalarType::ScalarType_Int:
					ctxt_f.setIntField(0, f->getParameters().at(i),
							eval_expr_s(scope, ctxt, p));
					break;
				}

			} break;
			case IBaseItem::TypeStruct: {
				// TODO:
				fprintf(stdout, "Error: struct-type parameter\n");
			}
			}
		}

		if (cb) {
			// Client calls back with references to IField parameters
			ICallbackContext *cb_ctxt = &ctxt_f;
			cb->exec(m_model, reinterpret_cast<psshandle_t>(cb_ctxt));
		} else {
			fprintf(stdout, "Error: no callback for import function %s\n",
					f->getName().c_str());
		}
	} break;

	}
	return ret;
}

int64_t ExecTraceGenerator::eval_expr_s(
		IBaseItem			*scope,
		const std::string	&ctxt,
		IExpr				*e) {
	int64_t ret = 0;

	switch (e->getType()) {
	case IExpr::ExprType_BinOp: {
		IBinaryExpr *bin_e = dynamic_cast<IBinaryExpr *>(e);
		switch (bin_e->getBinOpType()) {
		case IBinaryExpr::BinOp_And:
			ret = (eval_expr_s(scope, ctxt, bin_e->getLHS()) & eval_expr_s(scope, ctxt, bin_e->getRHS()));
			break;
		case IBinaryExpr::BinOp_AndAnd:
			ret = (eval_expr_s(scope, ctxt, bin_e->getLHS()) && eval_expr_s(scope, ctxt, bin_e->getRHS()));
			break;
			// ArrayRef
		case IBinaryExpr::BinOp_Divide:
			ret = (eval_expr_s(scope, ctxt, bin_e->getLHS()) / eval_expr_s(scope, ctxt, bin_e->getRHS()));
			break;
			// Eq
		case IBinaryExpr::BinOp_EqEq:
			ret = (eval_expr_s(scope, ctxt, bin_e->getLHS()) == eval_expr_s(scope, ctxt, bin_e->getRHS()));
			break;
		}
	} break;

	case IExpr::ExprType_FieldRef: {
		IFieldRef *ref = dynamic_cast<IFieldRef *>(e);
		std::string varname = ctxt;
		for (std::vector<IField *>::const_iterator it=ref->getFieldPath().begin();
				it!=ref->getFieldPath().end(); it++) {
			varname += ".";
			varname += (*it)->getName();
		}
		ret = m_engine->get_field(varname);
	} break;

	case IExpr::ExprType_Literal: {
		ret = dynamic_cast<ILiteral *>(e)->getInt();
	} break;

	case IExpr::ExprType_MethodCall: {
		IMethodCallExpr *stmt = dynamic_cast<IMethodCallExpr *>(e);
		IImportFunc *f = stmt->getFunc();
		IExecCallback *cb = f->getCallback();
		FunctionCallbackContext ctxt_f(f);

		// TODO: must evaluate parameter expressions and set them in ctxt

		for (uint32_t i=0; i<f->getParameters().size(); i++) {
			IExpr *p = stmt->getParameters().at(i);
			IBaseItem *dt = f->getParameters().at(i)->getDataType();

			switch (dt->getType()) {
			case IBaseItem::TypeScalar: {
				IScalarType *st = dynamic_cast<IScalarType *>(dt);
				// TODO:
				switch (st->getScalarType()) {
				case IScalarType::ScalarType_Bit:
					ctxt_f.setBitField(0, f->getParameters().at(i),
							eval_expr_u(scope, ctxt, p));
					break;
				case IScalarType::ScalarType_Int:
					ctxt_f.setIntField(0, f->getParameters().at(i),
							eval_expr_s(scope, ctxt, p));
					break;
				}

			} break;
			case IBaseItem::TypeStruct: {
				// TODO:
				fprintf(stdout, "Error: struct-type parameter\n");
			}
			}
		}

		if (cb) {
			// Client calls back with references to IField parameters
			ICallbackContext *cb_ctxt = &ctxt_f;
			cb->exec(m_model, reinterpret_cast<psshandle_t>(cb_ctxt));
		} else {
			fprintf(stdout, "Error: no callback for import function %s\n",
					f->getName().c_str());
		}
	} break;

	}
	return ret;
}

void ExecTraceGenerator::debug(const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
//	vfprintf(stdout, fmt, ap);
//	fprintf(stdout, "\n");

	va_end(ap);

}

void ExecTraceGenerator::note(const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	fprintf(stdout, "Note: ");
	vfprintf(stdout, fmt, ap);
	fprintf(stdout, "\n");

	va_end(ap);

}

void ExecTraceGenerator::warn(const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	fprintf(stdout, "Warning: ");
	vfprintf(stdout, fmt, ap);
	fprintf(stdout, "\n");

	va_end(ap);
}

std::string ExecTraceGenerator::list2path(const std::vector<std::string> &l) {
	std::string ret;

	for (uint32_t i=0; i<l.size(); i++) {
		ret.append(l.at(i));

		if (i+1<l.size()) {
			ret.append(".");
		}
	}

	return ret;
}

} // qpssc
