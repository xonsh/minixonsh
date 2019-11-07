#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <sys/wait.h>

#include <stdio.h>
#include <unistd.h>

int run_command(const std::vector<std::string> &args)
{
    char *cargs[args.size()+1];
    for (size_t i=0; i < args.size(); i++) {
        cargs[i] = const_cast<char*>(args[i].c_str());
    }
    cargs[args.size()] = nullptr;

    pid_t pid = fork();
    if (pid == 0)
    {
        // Child process
        execvp(cargs[0], &cargs[0]);
        // Execv failed
        throw std::runtime_error("execv() failed");
    }
    else if (pid < 0)
    {
        // Fork failed
        throw std::runtime_error("fork() failed");
    }

    // Parent process
    int child_status;
    pid_t tpid;
    do {
        tpid = wait(&child_status);
        if (tpid != pid) {
            std::cout << "Process terminated:" << tpid << std::endl;
        }
    } while (tpid != pid);
    return child_status;
}

int print_command(const std::vector<std::string> &args)
{
    std::cout << "+ ";
    for (size_t i=0; i < args.size(); i++) {
        std::cout << args[i];
        if (i < args.size()-1) std::cout << " ";
    }
    std::cout << std::endl;
}

std::vector<std::string> readlines(const std::string filename)
{
    std::vector<std::string> lines;
    std::fstream in;
    in.open("test1.xsh");
    std::string line;
    std::getline(in, line);
    while (in.rdstate() == std::ios_base::goodbit) {
        lines.push_back(line);
        std::getline(in, line);
    }
    return lines;
}

std::vector<std::string> split(const std::string str)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, ' ')) {
        tokens.push_back(token);
    };
    return tokens;
}

int main(int argc, char **argv)
{
    std::vector<std::string> lines = readlines(argv[1]);

    for (auto && line : lines) {
        std::vector<std::string> args = split(line);
        print_command(args);
        int c = run_command(args);
        if (c != 0) return c;
    }

    return 0;
}
