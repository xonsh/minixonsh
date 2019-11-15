#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>

#ifdef _WIN32
#else
#    include <sys/wait.h>
#    include <unistd.h>
#endif


int run_command(const std::vector<std::string> &args)
{
    const int err_exec = 123;
    const size_t cargs_max = 256;
    if (args.size()+1 > cargs_max) {
        throw std::runtime_error("Too many arguments.");
    }
    char *cargs[cargs_max];
    for (size_t i=0; i < args.size(); i++) {
        cargs[i] = const_cast<char*>(args[i].c_str());
    }
    cargs[args.size()] = nullptr;

#ifdef _WIN32
    // FIXME: Implement fork() on Windows
    int pid = -1;
#else
    pid_t pid = fork();
#endif
    if (pid == 0)
    {
#ifdef _WIN32
        // FIXME: Implement execvp() on Windows
        throw std::runtime_error("execvp() not implemented on Windows");
#else
        // Child process
        execvp(cargs[0], &cargs[0]);
        // Execv failed
        std::cout << "Command '" << args[0] << "' not found" << std::endl;
        exit(err_exec);
#endif
    }
    else if (pid < 0)
    {
        // Fork failed
        throw std::runtime_error("fork() failed");
    }

    // Parent process
    int child_status;
#ifdef _WIN32
    int tpid;
#else
    pid_t tpid;
#endif
    do {
#ifdef _WIN32
        // FIXME: Implement wait() on Windows
        tpid = pid + 1;
        child_status = 0;
#else
        tpid = wait(&child_status);
#endif
        if (tpid != pid) {
            std::cout << "Process terminated:" << tpid << std::endl;
        }
    } while (tpid != pid);

    // For some reason the child error code gets shifted by 8...
    if (child_status == (err_exec << 8)) child_status = -1;
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
    in.open(filename);
    if (in.rdstate() != std::ios_base::goodbit) {
        std::cout << "File could not be opened." << std::endl;
        exit(1);
    }
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
