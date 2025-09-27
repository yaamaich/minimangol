#include "minishell.h"

void exec_echo(t_cmd_node *cmd)
{
    // Implementation of echo command

    int i,n_found,j;

    i = 1;
    n_found = 0;
    while (cmd->args && cmd->args[i] && cmd->args[i][0] == '-' && cmd->args[i][1] == 'n')
    {
        j = 0;
        while (cmd->args[i][j] == 'n')
            j++;
        while (cmd->args[i][j] != '\0')
            break;
        n_found = 1;
        i++;
    }
    // print the args
    while (cmd->args && cmd->args[i])
    {
        printf("%s", cmd->args[i]);
        if (cmd->args[i + 1] != NULL)
            printf(" ");
        i++;
    }
    if (!n_found)
        printf("\n");
}
void exec_pwd(t_cmd_node *cmd)
{
    // Implementation of pwd command
}
