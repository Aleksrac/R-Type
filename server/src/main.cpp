/*
** EPITECH PROJECT, 2025
** r-type_server
** File description:
** main
*/
#include "parser/Parser.hpp"

int main(int argc, char **argv)
{
    server::Parser parser{};

    if (!parser.isEveryArgValid(argc, argv)) {
        return parser.checkHelp(argc, argv);
    }
}

