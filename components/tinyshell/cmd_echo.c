#include "command.h"
#include "tinyshell.h"

int do_echo(struct interactive_session * ss,cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    for(int i=0;i<argc;i++){
        minicli_printf(ss,argv[i]);
        minicli_printf(ss," ");
    }
    minicli_printf(ss,"\n");
    // minicli_prompt(ss);
    return 0;

}