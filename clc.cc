#include <boost/optional.hpp>
#include <boost/program_options.hpp>

#include <iostream>
#include <limits>

#include "src/parser.h"

boost::optional<boost::program_options::variables_map> input(
	int argc,
	char** argv
) {
	boost::program_options::options_description optionDescription(
		"Supported options"
	);

	optionDescription.add_options()
		(
			"input",
			boost::program_options::value<std::vector<std::string>>(),
			"terms to be evaluated"
		)
		(
			"tree",
			boost::program_options::value<bool>()->implicit_value(true),
			"return the binary expression tree instead of evaluating"
		)
	;

	boost::program_options::variables_map variables;

	try {
		boost::program_options::store(
			boost::program_options::parse_command_line(
				argc, argv, optionDescription
			),
			variables
		);

		boost::program_options::notify(variables);
	}
	catch ( const std::exception& exception ) {
		std::cerr << exception.what()  << std::endl;
		std::cout << optionDescription << std::endl;

		return boost::optional<boost::program_options::variables_map>();
	}

	return boost::make_optional(variables);
}

bool process(const boost::program_options::variables_map& variables) {
	const bool replMode{ variables.count("input") == 0 };
	const bool treeMode{ variables.count("tree") ? variables["tree"].as<bool>()
	                                             : false };

	if ( replMode ) {
		std::cout.precision(
			std::numeric_limits<double>::digits10
		);

		std::string term;

		while ( std::cin >> term ) {
			try {
				if ( treeMode) {
					std::cout << SimpleParser::print(term);
				} else {
					std::cout << SimpleParser::calculate(term);
				}

				std::cout << std::endl;
			}
			catch ( std::exception &exception ) {
				std::cerr << exception.what() << std::endl;
			}
		}

		return true;
	} else {
		const std::vector<std::string> inputTerms{
			variables["input"].as<std::vector<std::string>>()
		};

		try {
			std::for_each(
				inputTerms.cbegin(),
				inputTerms.cend(),
				[treeMode](const std::string& term) {
					if ( treeMode) {
						std::cout << SimpleParser::print(term);
					} else {
						std::cout << term
						          << " = "
						          << SimpleParser::calculate(term);
					}

					std::cout << std::endl;
				}
			);
		}
		catch ( std::exception &exception ) {
			std::cerr << exception.what() << std::endl;

			return false;
		}

		return true;
	}
}

int main(int argc, char** argv) {
	if ( auto variables = input(argc, argv) ) {
		if ( process(*variables) ) {
			return 0;
		} else {
			return 1;
		}
	} else {
		return 1;
	}
}