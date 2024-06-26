/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
// SPDX-License-Identifier: GPL-3.0
/** @file Whiskers.cpp
 * @author Chris <chis@ethereum.org>
 * @date 2017
 *
 * Moustache-like templates.
 */

#include <libsolutil/Whiskers.h>

#include <libsolutil/Assertions.h>

#include <regex>

using namespace std;
using namespace solidity::util;

Whiskers::Whiskers(string _template):
	m_template(std::move(_template))
{
	checkTemplateValid();
}

Whiskers& Whiskers::operator()(string _parameter, string _value)
{
	checkParameterValid(_parameter);
	checkParameterUnknown(_parameter);
	checkTemplateContainsTags(_parameter, {""});
	m_parameters[std::move(_parameter)] = std::move(_value);
	return *this;
}

Whiskers& Whiskers::operator()(string _parameter, bool _value)
{
	checkParameterValid(_parameter);
	checkParameterUnknown(_parameter);
	checkTemplateContainsTags(_parameter, {"?", "/"});
	m_conditions[std::move(_parameter)] = _value;
	return *this;
}

Whiskers& Whiskers::operator()(
	string _listParameter,
	vector<map<string, string>> _values
)
{
	checkParameterValid(_listParameter);
	checkParameterUnknown(_listParameter);
	checkTemplateContainsTags(_listParameter, {"#", "/"});
	for (auto const& element: _values)
		for (auto const& val: element)
			checkParameterValid(val.first);
	m_listParameters[std::move(_listParameter)] = std::move(_values);
	return *this;
}

string Whiskers::render() const
{
	return replace(m_template, m_parameters, m_conditions, m_listParameters);
}

void Whiskers::checkTemplateValid() const
{
	regex validTemplate("<[#?!\\/]\\+{0,1}[a-zA-Z0-9_$-]+(?:[^a-zA-Z0-9_$>-]|$)");
	smatch match;
	assertThrow(
		!regex_search(m_template, match, validTemplate),
		WhiskersError,
		"Template contains an invalid/unclosed tag " + match.str()
	);
}

void Whiskers::checkParameterValid(string const& _parameter) const
{
	static regex validParam("^" + paramRegex() + "$");
	assertThrow(
		regex_match(_parameter, validParam),
		WhiskersError,
		"Parameter" + _parameter + " contains invalid characters."
	);
}

void Whiskers::checkParameterUnknown(string const& _parameter) const
{
	assertThrow(
		!m_parameters.count(_parameter),
		WhiskersError,
		_parameter + " already set as value parameter."
	);
	assertThrow(
		!m_conditions.count(_parameter),
		WhiskersError,
		_parameter + " already set as condition parameter."
	);
	assertThrow(
		!m_listParameters.count(_parameter),
		WhiskersError,
		_parameter + " already set as list parameter."
	);
}

void Whiskers::checkTemplateContainsTags(string const& _parameter, vector<string> const& _prefixes) const
{
	for (auto const& prefix: _prefixes)
	{
		string tag{"<" + prefix + _parameter + ">"};
		assertThrow(
			m_template.find(tag) != string::npos,
			WhiskersError,
			"Tag '" + tag + "' not found in template:\n" + m_template
		);
	}
}

namespace
{
template<class ReplaceCallback>
string regex_replace(
	string const& _source,
	regex const& _pattern,
	ReplaceCallback _replace,
	regex_constants::match_flag_type _flags = regex_constants::match_default
)
{
	sregex_iterator curMatch(_source.begin(), _source.end(), _pattern, _flags);
	sregex_iterator matchEnd;
	string::const_iterator lastMatchedPos(_source.cbegin());
	string result;
	while (curMatch != matchEnd)
	{
		result.append(curMatch->prefix().first, curMatch->prefix().second);
		result.append(_replace(*curMatch));
		lastMatchedPos = (*curMatch)[0].second;
		++curMatch;
	}
	result.append(lastMatchedPos, _source.cend());
	return result;
}
}

string Whiskers::replace(
	string const& _template,
	StringMap const& _parameters,
	map<string, bool> const& _conditions,
	map<string, vector<StringMap>> const& _listParameters
)
{
	static regex listOrTag(
		"<(" + paramRegex() + ")>|"
		"<#(" + paramRegex() + ")>((?:.|\\r|\\n)*?)</\\2>|"
		"<\\?(\\+?" + paramRegex() + ")>((?:.|\\r|\\n)*?)(<!\\4>((?:.|\\r|\\n)*?))?</\\4>"
	);
	return regex_replace(_template, listOrTag, [&](match_results<string::const_iterator> _match) -> string
	{
		string tagName(_match[1]);
		string listName(_match[2]);
		string conditionName(_match[4]);
		if (!tagName.empty())
		{
			assertThrow(
				_parameters.count(tagName),
				WhiskersError,
				"Value for tag " + tagName + " not provided.\n" +
				"Template:\n" +
				_template
			);
			return _parameters.at(tagName);
		}
		else if (!listName.empty())
		{
			string templ(_match[3]);
			assertThrow(
				_listParameters.count(listName),
				WhiskersError, "List parameter " + listName + " not set."
			);
			string replacement;
			for (auto const& parameters: _listParameters.at(listName))
				replacement += replace(templ, joinMaps(_parameters, parameters), _conditions);
			return replacement;
		}
		else
		{
			assertThrow(!conditionName.empty(), WhiskersError, "");
			bool conditionValue = false;
			if (conditionName[0] == '+')
			{
				string tag = conditionName.substr(1);

				if (_parameters.count(tag))
					conditionValue = !_parameters.at(tag).empty();
				else if (_listParameters.count(tag))
					conditionValue = !_listParameters.at(tag).empty();
				else
					assertThrow(false, WhiskersError, "Tag " + tag + " used as condition but was not set.");
			}
			else
			{
				assertThrow(
					_conditions.count(conditionName),
					WhiskersError, "Condition parameter " + conditionName + " not set."
				);
				conditionValue = _conditions.at(conditionName);
			}
			return replace(
				conditionValue ? _match[5] : _match[7],
				_parameters,
				_conditions,
				_listParameters
			);
		}
	});
}

Whiskers::StringMap Whiskers::joinMaps(
	Whiskers::StringMap const& _a,
	Whiskers::StringMap const& _b
)
{
	Whiskers::StringMap ret = _a;
	for (auto const& x: _b)
		assertThrow(
			ret.insert(x).second,
			WhiskersError,
			"Parameter collision"
		);
	return ret;
}

