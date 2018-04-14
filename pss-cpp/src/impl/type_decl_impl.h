/*
 * type_decl_impl.h
 *
 *  Created on: Apr 14, 2018
 *      Author: ballance
 */
#pragma once

template <class T> class type_decl_impl {
public:
	type_decl_impl();

	virtual ~type_decl_impl();

protected:

	static T *get_type();

private:
	static T					*m_type;

};

template <class T> T *type_decl_impl<T>::get_type() {
	if (!m_type) {
		m_type = new T();
	}
	return m_type;
}

template <class T> T type_decl_impl<T>::m_type = 0;

