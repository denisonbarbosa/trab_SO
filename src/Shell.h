#ifndef __Shell_h
#define __Shell_h
const int MAX = 100;

class Shell
{
private:
    char cmd[MAX];
    char cmd2[MAX];
    char *argv[MAX];

public:
    Shell();
    ~Shell();
    auto evaluate_command(char *command);
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