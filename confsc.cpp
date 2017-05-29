#include <iostream>
#include <string>
#include "confsc.h"

void report_error(const std::string& msg, size_t line_num)
{
	std::cout << "ERROR (line " << line_num << "): " << msg << "\n";
}

bool is_valid_name(string_view s)
{
	for (auto i : s)
		if (!(isalnum(i) || i == '_'))
			return false;

	return true;
}

bool is_valid_value(string_view s)
{
	for (auto i : s)
		if (i == '[' || i == ']' || i == '{' || i == '}')
			return false;

	return true;
}

std::vector<string_view> parse_group(string_view val, size_t line_num)
{
	if (val.count('[') > 1 || val.count(']') > 1)
		report_error("Found malformed group!", line_num);

	auto raw = val.trim_front('[').trim_back(']');

	if (raw.length() == 0)
		report_error("Group cannot be empty!", line_num);

	auto ret = raw.split(',');

	for (auto& i : ret)
		i = i.trim_whitespaces();

	return ret;
}

std::vector<string_view> parse_value(string_view val, size_t line_num)
{
	if (val.front() == '[' && val.back() == ']')
		return parse_group(val, line_num);

	else if (is_valid_value(val))
		return std::vector<string_view>{val};

	else
	{
		report_error("Found malformed value!", line_num);
		return std::vector<string_view>();
	}
}

std::pair<string_view, std::vector<string_view>> parse_var(string_view line, size_t line_num)
{
	auto tokens = line.split<1>(' ');

	for (auto& i : tokens)
		i = i.trim_whitespaces();

	if (tokens[1].length() == 0)
		report_error("Missing value!", line_num);

	return std::make_pair(tokens[0], parse_value(tokens[1], line_num));
}

void parse(string_view file, dict& values, std::unordered_map<string_view, dict>& named_objects)
{
	dict* data_holder = &values;
	size_t line_num = 1;
	bool inside_named_group = false;
	auto lines = file.split('\n');

	for (auto line : lines)
	{
		size_t comment = line.rfind('#');
		auto raw_line = (comment != NOT_FOUND ? line.substr(0, comment) : line).trim_whitespaces();

		if (raw_line.length() == 0)
		{
			++line_num;
			continue;
		}

		auto tokens = raw_line.split<1>(' ');

		for (auto& i : tokens)
			i = i.trim_whitespaces();
		
		if (tokens[0].back() == '{')
		{
			auto group_name = tokens[0].trim_back('{');

			if (group_name.length() == 0)
			{
				report_error("Found nameless object!", line_num);
				break;
			}

			inside_named_group = true;
			named_objects[group_name] = dict();
			data_holder = &named_objects[group_name];
			++line_num;
			continue;
		}

		else if (tokens[0] == "}")
		{
			inside_named_group = false;
			data_holder = &values;
			++line_num;
			continue;
		}

		else if (tokens[1].length() == 0)
		{
			report_error("Missing value!", line_num);
			++line_num;
			continue;
		}

		auto parsed = std::make_pair(tokens[0], parse_value(tokens[1], line_num));

		if (data_holder->find(std::get<0>(parsed)) != data_holder->end())
			report_error("Multiple definitions of the same name!", line_num);

		else
			data_holder->insert(std::move(parsed));

		++line_num;
	}

	if (inside_named_group)
		report_error("Missing } found!", line_num);
}