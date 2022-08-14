#include "../cmdline.h"

int main(int argc, char **argv)
{
    CmdLine cmd;
    cmd.init(argc, argv, 
    {
        {'o', "option"},
        {'a', "add"},
        {'d', "delete"},
        {'r', "reboot"},
        {'c', "config"},
        {'?', "help"}
    });

    // Check/show usage
    if (cmd.hasOption("help") || cmd.valueCount() == 0)
    {
        printf("Usage:\n"
            "    -?, --help - Show usage\n");
        return 1;
    }

    for (int i = 0; i < cmd.valueCount(); i++)
    {
        printf("Value %d: %s\n", i, cmd.getValue(i).c_str());
    }

    return 0;
}