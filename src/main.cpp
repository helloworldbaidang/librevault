/* Copyright (C) 2014-2015 Alexander Shishenko <GamePad64@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "pch.h"
#include "directory/Key.h"
#include "Session.h"

using namespace librevault;	// This is allowed only because this is main.cpp file and it is extremely unlikely that this file will be included in any other file.

int main(int argc, char** argv){
	po::options_description allowed_options("Allowed Options");
	allowed_options.add_options()
		("help,h", "Display help message")
		("config,c", po::value<fs::path>(), "Application data path")
		("gen-key", "Generate Key type A")
		("derive,d", po::value<char>(), "Derive lower-type Key from higher-type (e.g. C from A)")
		("key, k", po::value<std::string>(), "Sets Key for -d")
	;

	po::variables_map variables;
	po::store(po::parse_command_line(argc, argv, allowed_options), variables);
	po::notify(variables);

	if(variables.count("help") > 0){
		std::cout << allowed_options;
		return 0;
	}
	if(variables.count("gen-key") > 0){
		Key k;
		std::cout << k;
		return 0;
	}
	if(variables.count("derive") > 0 && variables.count("key") > 0){
		Key::Type type = (Key::Type)variables["derive"].as<char>();
		Key k(variables["key"].as<std::string>());
		std::cout << k.derive(type);
		return 0;
	}

	Session session(variables);
	session.run();

	return 0;
}