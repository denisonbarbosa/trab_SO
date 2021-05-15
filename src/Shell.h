#ifndef __Shell_h
#define __Shell_h
#include <list>
#include <unordered_map>
const int MAX = 100;

class Shell
{
private:
    char cmd[MAX];
    char cmd2[MAX];
    char *argv[MAX];
    int historySize = 0;
    list<char *> history;
    unordered_map<char *, char *> env_vars; // TODO read & change content || matrix[2][n] -> n = number of var

public:
    Shell();
    ~Shell();
    void push_history(char *command); // Adds to the end
    void pop_history();               // Removes from the beginning
    auto get_cmd() const;
    auto get_cmd2() const;
    auto get_argv() const;
    void set_cmd(char *new_cmd);
    void set_cmd2(char *new_cmd2);
    void set_argv(char **new_argv);
};

auto Shell::get_cmd() const
{
    return this->cmd;
}
auto Shell::get_cmd2() const
{
    return this->cmd2;
}

auto Shell::get_argv() const
{
    return this->argv;
}
#endif __Shell_h
