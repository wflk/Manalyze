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

#ifndef _OUTPUT_TREE_NODE_
#define _OUTPUT_TREE_NODE_

#include <vector>
#include <string>
#include <set>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include <boost/optional.hpp>

#include "color.h"
#include "threat_level.h"

#if defined BOOST_WINDOWS_API && !defined DECLSPEC_SGCOMMONS
#ifdef SGSTATIC_EXPORT
#define DECLSPEC_SGCOMMONS    __declspec(dllexport)
	#else
#define DECLSPEC_SGCOMMONS    __declspec(dllimport)
	#endif
#elif !defined BOOST_WINDOWS_API && !defined DECLSPEC_SGCOMMONS
	#define DECLSPEC_SGCOMMONS
#endif

namespace io
{

typedef std::vector<std::string> strings;
typedef std::shared_ptr<strings> shared_strings;
typedef std::set<std::string> string_set;
typedef boost::shared_ptr<std::string> pString;

/**
*	@brief	A tree representing the data to output.
*/
class OutputTreeNode
{
public:
	typedef boost::shared_ptr<OutputTreeNode> pNode;
	typedef std::vector<pNode> nodes;
	typedef boost::shared_ptr<nodes> pNodes;
	typedef boost::shared_ptr<boost::optional<boost::uint32_t> > shared_opt_uint32;
	typedef boost::shared_ptr<boost::optional<boost::uint16_t> > shared_opt_uint16;
	typedef boost::shared_ptr<boost::optional<boost::uint64_t> > shared_opt_uint64;
	typedef boost::shared_ptr<boost::optional<float> > shared_opt_float;
	typedef boost::shared_ptr<boost::optional<double> > shared_opt_double;
	typedef boost::shared_ptr<boost::optional<std::string> > shared_opt_string;
	typedef boost::shared_ptr<boost::optional<nodes> > shared_opt_nodes;
	typedef boost::shared_ptr<boost::optional<strings> > shared_opt_strings;
	typedef boost::shared_ptr<boost::optional<plugin::LEVEL> > shared_opt_level;

	enum node_type { LIST, UINT32, UINT16, UINT64, FLOAT, DOUBLE, STRING, STRINGS, THREAT_LEVEL };

	/**
	*	@brief	Modifiers that control the way a node's content is displayed.
	*
	*	Formatters may chose to ignore some modifiers.
	*
	*	NONE		Nothing
	*	DEC			Print as a decimal number (for UINT64, UINT32 and UINT16)
	*	HEX			Print as an hexadecimal number (for UINT64, UINT32 and UINT16)
	*/
	enum display_modifier { NONE, DEC, HEX };

	// ----------------------------------------------------------------------------

	DECLSPEC_SGCOMMONS OutputTreeNode(const std::string& name, boost::uint32_t i, display_modifier mod = DEC)
		: _name(new std::string(name)), _type(UINT32), _uint32_data(new boost::optional<boost::uint32_t>(i)), _modifier(mod)
	{}

	DECLSPEC_SGCOMMONS OutputTreeNode(const std::string& name, boost::uint16_t s, display_modifier mod = DEC)
		: _name(new std::string(name)), _type(UINT16), _uint16_data(new boost::optional<boost::uint16_t>(s)), _modifier(mod)
	{}

	DECLSPEC_SGCOMMONS OutputTreeNode(const std::string& name, boost::uint64_t l, display_modifier mod = DEC)
		: _name(new std::string(name)), _type(UINT64), _uint64_data(new boost::optional<boost::uint64_t>(l)), _modifier(mod)
	{}

	DECLSPEC_SGCOMMONS OutputTreeNode(const std::string& name, float f, display_modifier mod = NONE)
		: _name(new std::string(name)), _type(FLOAT), _float_data(new boost::optional<float>(f)), _modifier(mod)
	{}

	DECLSPEC_SGCOMMONS OutputTreeNode(const std::string& name, double d, display_modifier mod = NONE)
		: _name(new std::string(name)), _type(DOUBLE), _double_data(new boost::optional<double>(d)), _modifier(mod)
	{}

	DECLSPEC_SGCOMMONS OutputTreeNode(const std::string& name, const std::string& s, display_modifier mod = NONE)
		: _name(new std::string(name)), _type(STRING), _string_data(new boost::optional<std::string>(s)), _modifier(mod)
	{}

	DECLSPEC_SGCOMMONS OutputTreeNode(const std::string& name, const nodes& n, display_modifier mod = NONE)
		: _name(new std::string(name)), _type(LIST), _list_data(new boost::optional<nodes>(n)), _modifier(mod)
	{}

	DECLSPEC_SGCOMMONS OutputTreeNode(const std::string& name, const strings& strs, display_modifier mod = NONE)
		: _name(new std::string(name)), _type(STRINGS), _strings_data(new boost::optional<strings>(strs)), _modifier(mod)
	{}

	DECLSPEC_SGCOMMONS OutputTreeNode(const std::string& name, const string_set& strs, display_modifier mod = NONE)
		: _name(new std::string(name)), _type(STRINGS), _modifier(mod)
	{
		_strings_data = shared_opt_strings(new boost::optional<strings>(strings(strs.begin(), strs.end())));
	}

	DECLSPEC_SGCOMMONS OutputTreeNode(const std::string& name, plugin::LEVEL level, display_modifier mod = NONE)
		: _name(new std::string(name)), _type(THREAT_LEVEL), _level_data(new boost::optional<plugin::LEVEL>(level)), _modifier(mod)
	{}

	// ----------------------------------------------------------------------------

	DECLSPEC_SGCOMMONS OutputTreeNode(const std::string& name, enum node_type type, enum display_modifier mod = NONE)
		: _name(new std::string(name)), _type(type), _modifier(mod)
	{
		switch (type)
		{
		case LIST:
			_list_data = shared_opt_nodes(new boost::optional<nodes>(nodes()));
			break;
		case STRINGS:
			_strings_data = shared_opt_strings(new boost::optional<strings>(strings()));
			break;
		default:
			break;
		}
	}

	// ----------------------------------------------------------------------------

	DECLSPEC_SGCOMMONS pString get_name() const {
		return _name;
	}

	// ----------------------------------------------------------------------------

	DECLSPEC_SGCOMMONS node_type get_type() const {
		return _type;
	}

	// ----------------------------------------------------------------------------

	DECLSPEC_SGCOMMONS display_modifier get_modifier() const {
		return _modifier;
	}

	// ----------------------------------------------------------------------------

	DECLSPEC_SGCOMMONS void set_modifier(display_modifier mod) {
		_modifier = mod;
	}

	// ----------------------------------------------------------------------------

	/**
	*	@brief	Returns a string representation of the data contained by the node.
	*
	*	For instance, a UINT32 node containing 10 and the HEX modifier will be displayed as "0xA".
	*	LIST and STRINGS nodes cannot be displayed this way.
	*
	*	@return	A string representation of the contained data.
	*/
	DECLSPEC_SGCOMMONS pString to_string() const
	{
		if (_type == STRING) {
			return pString(new std::string(**_string_data));
		}

		std::stringstream ss;
		if (_modifier == HEX) {
			ss << std::hex << "0x";
		}
		else if (_modifier == DEC) {
			ss << std::dec;
		}

		switch (_type)
		{
		case UINT32:
			ss << **_uint32_data;
			break;
		case UINT16:
			ss << **_uint16_data;
			break;
		case UINT64:
			ss << **_uint64_data;
			break;
		case FLOAT:
			ss << **_float_data;
			break;
		case DOUBLE:
			ss << **_double_data;
			break;
		case THREAT_LEVEL:
			ss << **_level_data;
			break;
		case LIST:
		case STRINGS:
			PRINT_WARNING << "[OutputTreeNode] Called to_string() on a LIST or a STRINGS node!" << DEBUG_INFO << std::endl;
			break;
		default:
			PRINT_WARNING << "[OutputTreeNode] No _to_string() implementation for " << _type << "!" << std::endl;
		}
		return pString(new std::string(ss.str()));
	}

	// ----------------------------------------------------------------------------

	DECLSPEC_SGCOMMONS plugin::LEVEL get_level() const
	{
		if (_type != THREAT_LEVEL)
		{
			PRINT_WARNING << "[OutputTreeNode] Tried to get a level, but is not a THREAT_LEVEL node!" << DEBUG_INFO << std::endl;
			return plugin::NO_OPINION;
		}

		if (!_level_data || !*_level_data)
		{
			PRINT_WARNING << "[OutputTreeNode] A THREAT_LEVEL node's data is empty!" << DEBUG_INFO << std::endl;
			return plugin::NO_OPINION;
		}

		return **_level_data;
	}

	// ----------------------------------------------------------------------------

	DECLSPEC_SGCOMMONS shared_strings get_strings() const
	{
		if (_type != STRINGS)
		{
			PRINT_WARNING << "[OutputTreeNode] Tried to get strings, but is not a STRINGS node!" << DEBUG_INFO << std::endl;
			return shared_strings();
		}

		if (!_strings_data || !*_strings_data)
		{
			PRINT_WARNING << "[OutputTreeNode] A STRINGS node's data is empty!" << DEBUG_INFO << std::endl;
			return shared_strings();
		}

		return shared_strings(new strings(**_strings_data));
	}

	// ----------------------------------------------------------------------------

	/**
	*	@brief	Appends a node to a LIST node.
	*
	*	@param	pNode node	The list to append.
	*/
	DECLSPEC_SGCOMMONS void append(pNode node)
	{
		if (_type != LIST)
		{
			PRINT_WARNING << "[OutputTreeNode] Tried to append a node, but is not a list of nodes!" << DEBUG_INFO << std::endl;
			return;
		}

		if (!_list_data || !*_list_data) {
			_list_data = shared_opt_nodes(new boost::optional<nodes>(nodes()));
		}
		(*_list_data)->push_back(node);
	}

	// ----------------------------------------------------------------------------

	/**
	*	@brief	Returns the data contained by a LIST node (a vector of nodes).
	*/
	DECLSPEC_SGCOMMONS pNodes get_children()
	{
		if (_type != LIST)
		{
			PRINT_WARNING << "[OutputTreeNode] Tried to get the children of a non-LIST node!" << std::endl;
			return pNodes();
		}
		if (!_list_data || !*_list_data)
		{
			PRINT_WARNING << "[OutputTreeNode] A LIST node's data is empty!" << std::endl;
			return pNodes();
		}

		return pNodes(new nodes(**_list_data));
	}

	// ----------------------------------------------------------------------------

	/**
	*	@brief	Empties the contents of a LIST node.
	*/
	DECLSPEC_SGCOMMONS void clear()
	{
		if (_type != LIST)
		{
			PRINT_WARNING << "[OutputTreeNode] Tried to clear a non-LIST node!" << std::endl;
			return;
		}

		if (!_list_data || !*_list_data)
		{
			PRINT_WARNING << "[OutputTreeNode] A LIST node's data is empty!" << std::endl;
			return;
		}

		(*_list_data)->clear();
	}

	// ----------------------------------------------------------------------------

	DECLSPEC_SGCOMMONS void update_value(const std::string& s)
	{
		if (_type != STRING)
		{
			PRINT_WARNING << "[OutputTreeNode] Tried to set a string in a non-STRING node!" << std::endl;
			return;
		}
		if (!_string_data || !*_string_data)
		{
			PRINT_WARNING << "[OutputTreeNode] A STRING node's data is empty!" << std::endl;
			return;
		}

		*_string_data = boost::optional<std::string>(s);
	}

	// ----------------------------------------------------------------------------

	DECLSPEC_SGCOMMONS void update_value(plugin::LEVEL level)
	{
		if (_type != THREAT_LEVEL)
		{
			PRINT_WARNING << "[OutputTreeNode] Tried to set a LEVEL in a non-THREAT_LEVEL node!" << std::endl;
			return;
		}
		if (!_level_data || !*_level_data)
		{
			PRINT_WARNING << "[OutputTreeNode] A LEVEL node's data is empty!" << std::endl;
			return;
		}

		*_level_data = boost::optional<plugin::LEVEL>(level);
	}

	// ----------------------------------------------------------------------------

	/**
	*	@brief	Returns the data contained by a STRINGS node (a vector of strings).
	*/
	DECLSPEC_SGCOMMONS shared_strings get_strings()
	{
		if (_type != STRINGS)
		{
			PRINT_WARNING << "[OutputTreeNode] Tried to get the strings of a non-STRING node!" << std::endl;
			return shared_strings();
		}
		if (!_strings_data || !*_strings_data)
		{
			PRINT_WARNING << "[OutputTreeNode] A STRINGS node's data is empty!" << std::endl;
			return shared_strings();
		}

		return shared_strings(new strings(**_strings_data));
	}

	// ----------------------------------------------------------------------------

	/**
	*	@brief	Appends a string to a STRINGS node.
	*
	*	@param	const std::string& s The string to append.
	*/
	DECLSPEC_SGCOMMONS void append(const std::string& s)
	{
		if (_type != STRINGS)
		{
			PRINT_WARNING << "[OutputTreeNode] Tried to append a string, but is not a list of strings!" << std::endl;
			return;
		}

		if (!_strings_data || !*_strings_data) {
			_strings_data = shared_opt_strings(new boost::optional<strings>(strings()));
		}
		(*_strings_data)->push_back(s);
	}

	// ----------------------------------------------------------------------------

	/**
	*	@brief	Appends a list of strings to a STRINGS node.
	*
	*	@param	const strings& strs The strings to append.
	*/
	DECLSPEC_SGCOMMONS void append(const strings& strs)
	{
		if (_type != STRINGS)
		{
			PRINT_WARNING << "[OutputTreeNode] Tried to append strings, but is not a list of strings!" << std::endl;
			return;
		}

		if (!_strings_data || !*_strings_data) {
			_strings_data = shared_opt_strings(new boost::optional<strings>(strings(strs)));
		}
		else {
			(*_strings_data)->insert((*_strings_data)->end(), strs.begin(), strs.end());
		}
	}

	// ----------------------------------------------------------------------------

	/**
	*	@brief	Find a node in a list of nodes based on its name.
	*
	*	The search will stop at the first occurrence of the name, so using duplicate
	*	node names is not a good idea.
	*
	*	@param	const std::string& name The name of the node to locate.
	*
	*	@return	A pointer which may point to the located node, or be NULL.
	*/
	DECLSPEC_SGCOMMONS pNode find_node(const std::string& name) const;

private:
	pString _name;
	enum node_type _type;

	shared_opt_uint32	_uint32_data;
	shared_opt_uint16	_uint16_data;
	shared_opt_uint64	_uint64_data;
	shared_opt_float	_float_data;
	shared_opt_double	_double_data;
	shared_opt_string	_string_data;
	shared_opt_nodes	_list_data;
	shared_opt_strings	_strings_data;
	shared_opt_level	_level_data;
	display_modifier	_modifier;		// Additional info hinting at how the data should be displayed,
										// i.e. hexadecimal or decimal for ints.

};

typedef boost::shared_ptr<OutputTreeNode> pNode;
typedef std::vector<pNode> nodes;
typedef boost::shared_ptr<nodes> pNodes;

/**
*	@brief	For LIST nodes, returns the size of the biggest child's name.
*
*	Used for pretty printing purposes with the RawFormatter.
*
*	@param	pNode node The (LIST) node to work on.
*
*	@return	The maximum size of the children's names.
*/
DECLSPEC_SGCOMMONS unsigned int determine_max_width(pNode node);

} // !namespace io

#endif // !_OUTPUT_TREE_NODE_