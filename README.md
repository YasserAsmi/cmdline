# cmdline
## Modern and flexible command line C++17 header only

Header-only class to parse command line and provide options and values.  Options are switches
that can be specified with a -O shor char name format, or as --OptionName long name format. 
Short name matches are case sensitive, long name matches are case sensitive when for the calling 
code (getOption() and hasOption()) but are case insensitive for command line usage.  CmdLine object 
can be passed along as config to other parts of program. Basic errors are cought and reported 
via false return from init() and details in errMsg()

Example usage in C++:
```c++
   CmdLine cmd;
   cmd.init(argc, argv, {
       {'o', "option"},
       {'a', "add"},
       {'d', "delete"},
       {'r', "reboot"},
       {'c', "config"},
       {'?', "help"}
   });
```
Example run:
```bash
> prog.exe subcmd --config filename0 --reboot -a -d filename1 filename2
```
Receives the following options and values:

```bash
     option config = [filename0]
     optopn add = []
     option delete = [filename1]
     option reboot = []
     value subcmd
     value filename2
     errmsg = []
```
