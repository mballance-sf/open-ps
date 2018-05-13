/*
 * type_decl.hpp
 *
 *  Created on: Apr 14, 2018
 *      Author: ballance
 */
#pragma once
#include "pss/type_decl.h"
#include "impl/pss_impl.h"

namespace pss {

template <class T> type_decl<T>::type_decl() :
		type_decl_impl<T>() {

}


}




