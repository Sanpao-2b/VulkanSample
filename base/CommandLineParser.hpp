/*
 * Simple command line parse
 *
 * Copyright (C) 2016-2022 by Sascha Willems - www.saschawillems.de
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <vector>
#include <unordered_map>
#include <string>

class CommandLineParser
{
public:
	struct CommandLineOption {
		std::vector<std::string> commands;
		std::string value;
		bool hasValue = false;
		std::string help;
		bool set = false;
	};
	std::unordered_map<std::string, CommandLineOption> options;

	void add(std::string name, std::vector<std::string> commands, bool hasValue, std::string help)
	{
		options[name].commands = commands;	// name选项对应的命令，即使用该选项时需要输入的命令字符串（用数组存放是因为一种选项 可以用多种命令来指定如help 可以用-h --help）
		options[name].help = help;			// 选项的帮助信息，用于在用户需要帮助时 提供对该选项的解释
		options[name].set = false;			// set 被初始化为 false，表示该选项在解析命令行参数之前还没有被设置过
		options[name].hasValue = hasValue;	// 该选项是否需要一个值 比如 -w1080 就需要指定一个值
		options[name].value = "";			// 如果需要传入值，这个值的内容
	}

	void printHelp()
	{
		std::cout << "Available command line options:\n";
		for (auto option : options) {
			std::cout << " ";
			for (size_t i = 0; i < option.second.commands.size(); i++) {
				std::cout << option.second.commands[i];
				if (i < option.second.commands.size() - 1) {
					std::cout << ", ";
				}
			}
			std::cout << ": " << option.second.help << "\n";
		}
		std::cout << "Press any key to close...";
	}

	void parse(std::vector<const char*> arguments)
	{
		bool printHelp = false;
		// Known arguments
		for (auto& option : options) {// 对于每一个选项（如选项help）
			for (auto& command : option.second.commands) {// 对于每一种命令（-h --help)
				for (size_t i = 0; i < arguments.size(); i++) {// 对于每一个传入的参数
					if (strcmp(arguments[i], command.c_str()) == 0) {// 如果传入参数与选项的其中一个命令一致
						option.second.set = true;// 将选项标记为已设置
						// Get value
						if (option.second.hasValue) {// 如果该选项需要参数值
							if (arguments.size() > i + 1) {// 如果用户输入了参数值
								option.second.value = arguments[i + 1];// 将参数值设置为选项的值
							}
							if (option.second.value == "") {// 如果用户没有输入参数值
								printHelp = true;// 设置需要打印帮助信息
								break;
							}
						}
					}
				}
			}
		}
		// Print help for unknown arguments or missing argument values
		if (printHelp) {
			options["help"].set = true;
		}
	}

	void parse(int argc, char* argv[])
	{
		std::vector<const char*> args;
		for (int i = 0; i < argc; i++) { 
			args.push_back(argv[i]); 
		};
		parse(args);
	}

	bool isSet(std::string name)
	{
		return ((options.find(name) != options.end()) && options[name].set);
	}

	std::string getValueAsString(std::string name, std::string defaultValue)
	{
		assert(options.find(name) != options.end());
		std::string value = options[name].value;
		return (value != "") ? value : defaultValue;
	}

	int32_t getValueAsInt(std::string name, int32_t defaultValue)
	{
		assert(options.find(name) != options.end());
		std::string value = options[name].value;
		if (value != "") {
			char* numConvPtr;
			int32_t intVal = strtol(value.c_str(), &numConvPtr, 10);
			return (intVal > 0) ? intVal : defaultValue;
		}
		else {
			return defaultValue;
		}
		return int32_t();
	}

};