/*
 * This file is part of the AdvanceMAME project.
 *
 * Copyright (C) 1999-2002 Andrea Mazzoleni
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details. 
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __CHOICE_H
#define __CHOICE_H

#include <string>
#include <vector>

// ------------------------------------------------------------------------
// Choice menu

enum tristate_t {
	include,
	exclude, // not
	exclude_not // only
};

union choice_data {
	void* ptr;
	int value;
};

class choice {
	std::string desc;
	std::string desc_def;
	std::string desc_not;
	std::string desc_only;
	int state;
	bool bistate;
	tristate_t tristate;
	choice_data association;

public:
	choice(const std::string& Adesc, int Avalue);
	choice(const std::string& Adesc, void* Aptr);
	choice(const std::string& Adesc, bool Abistate, int Avalue);
	choice(const std::string& Adesc, tristate_t Atristate, int Avalue);
	choice(const std::string& Adesc_def, const std::string& Adesc_not, const std::string& Adesc_only, tristate_t Atristate, int Avalue);

	int state_get() const { return state; }
	bool bistate_get() const { return bistate; }
	tristate_t tristate_get() const { return tristate; }
	void bistate_set(bool A) { bistate = A; }
	void tristate_set(tristate_t  A) { tristate = A; }

	const std::string& desc_get() const;
	const std::string& print_get() const;

	int value_get() const { return association.value; }
	void* ptr_get() const { return association.ptr; }
};

typedef std::vector<choice> choice_container;

class choice_bag : public choice_container {
	void draw(const std::string& title, int x, int y, int dx, int base, int rel, int rows);
public:

	int run(const std::string& title, int x, int y, int dx, choice_container::iterator& pos);

	choice_container::iterator find_by_value(int value);
	choice_container::iterator find_by_desc(const std::string& desc);
};

void menu_pos(int pos, int& pos_base, int& pos_rel, int pos_rel_max, int pos_base_upper, int coln, int pos_max);
int menu_key(int key, int& pos_base, int& pos_rel, int pos_rel_max, int pos_base_upper, int coln, int pos_max);


#endif