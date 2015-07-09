/*
    This file is part of Spike Guard.

    Spike Guard is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Spike Guard is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Spike Guard.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _COLOR_H_
#define _COLOR_H_

#include <iostream>
#include <string>
#include <boost/system/api_config.hpp>
#ifdef BOOST_WINDOWS_API
	#include "windows.h"
#else
	#include <unistd.h>
	#include <stdio.h>
#endif

#if defined BOOST_WINDOWS_API && !defined DECLSPEC_SGCOMMONS
	#ifdef SGCOMMONS_EXPORT
		#define DECLSPEC_SGCOMMONS    __declspec(dllexport)
	#else
		#define DECLSPEC_SGCOMMONS    __declspec(dllimport)
	#endif
#elif !defined BOOST_WINDOWS_API && !defined DECLSPEC_SGCOMMONS
	#define DECLSPEC_SGCOMMONS
#endif

namespace utils
{

enum Color { RED, GREEN, YELLOW, RESET };

/**
 *	@brief	Set the font in the terminal to the specified color.
 *
 *	@param	Color c	The new color of the text.
 */
void set_color(Color c);

/**
 *	@brief	Prints the input text to a stream, after having applied color to the terminal.
 *
 *	@param	const std::string& text The text to write.
 *	@param	Color c The color to write the text in.
 *	@param	The stream into which the text should be written.
 *	@param	const std::string& prefix An optional prefix string, which will be displayed in
 *			the default color.
 *	@param	const std::string& suffix An optional suffix string, which will be displayed in
 *			the default color.
 *
 *	@return	A reference to sink, so the operator "<<" can be chained.
 */
DECLSPEC_SGCOMMONS std::ostream& print_colored_text(const std::string& text,
								 Color c, 
								 std::ostream& sink = std::cout, 
								 const std::string& prefix = "", 
								 const std::string& suffix = "");

#define PRINT_ERROR utils::print_colored_text("!", utils::RED, std::cerr, "[", "] Error: ")
#define PRINT_WARNING utils::print_colored_text("*", utils::YELLOW, std::cerr, "[", "] Warning: ")

// TODO: Add these macros to all errors and warnings.
#ifdef _DEBUG
	#define DEBUG_INFO " (" << __FILE__ << ":" << std::dec << std::dec << __LINE__ << ")"
	#define DEBUG_INFO_PE " (" << __FILE__ << ":" << std::dec << __LINE__ << ", " << *pe.get_path() << ")"
	#define DEBUG_INFO_INSIDEPE " (" << __FILE__ << ":" << std::dec << __LINE__ << ", " << *get_path() << ")"
#else
	#define DEBUG_INFO ""
	#define DEBUG_INFO_PE ""
	#define DEBUG_INFO_INSIDEPE ""
#endif

} //namespace utils

#endif // !_COLOR_H_