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
    int evaluate_command(char *command);
    char *get_cmd();
    char *get_cmd2();
    char **get_argv();
    void set_cmd(char *new_cmd);
    void set_cmd2(char *new_cmd2);
    void set_argv(char **new_argv);
};

Shell::Shell()
{
}

Shell::~Shell()
{
}

char *Shell::get_cmd()
{
    return this->cmd;
}
char *Shell::get_cmd2()
{
    return this->cmd2;
}
char **Shell::get_argv()
{
    return this->argv;
}

void Shell::set_cmd(char* new_cmd)
{
    *cmd = *new_cmd;
}
void Shell::set_cmd2(char* new_cmd2)
{
    *cmd2 = *new_cmd2;
}
void Shell::set_argv(char **new_argv)
{
    **argv = **new_argv;
}