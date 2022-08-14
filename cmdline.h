#pragma once

#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <initializer_list>

// Header-only class to parse command line and provide options and values.  Options are switches
// that can be specified with a -O shor char name format, or as --OptionName long name format. 
// Short name matches are case sensitive, long name matches are case sensitive when for the calling 
// code (getOption() and hasOption()) but are case insensitive for command line usage.  CmdLine object 
// can be passed along as config to other parts of program. Basic errors are cought and reported 
// via false return from init() and details in errMsg()
//
//    CmdLine cmd;
//    cmd.init(argc, argv, {
//        {'o', "option"},
//        {'a', "add"},
//        {'d', "delete"},
//        {'r', "reboot"},
//        {'c', "config"},
//        {'?', "help"}
//    });
//
// C:> prog.exe subcmd --config filename0 --reboot -a -d filename1 filename2
//
//      errmsg = []
//      option config = [filename0]
//      optopn add = []
//      option delete = [filename1]
//      option reboot = []
//      value subcmd
//      value filename2

class CmdLine
{ 
public:
    CmdLine() = default;
    ~CmdLine() = default;
    bool init(int argc, char** argv, std::initializer_list<std::pair<char, const char*>> optionslist);
    size_t valueCount() const
    {
        return mValues.size();
    }
    std::string getValue(size_t index)
    {
        return (index < mValues.size()) ? mValues[index] : "";
    }
    bool valueEquals(size_t index, const std::string& eqvalue) const
    {
        if (index < mValues.size()) 
        {
            return _stricmp(mValues[index].c_str(), eqvalue.c_str()) == 0;
        }
        return false;
    }    
    std::string getOption(const char* name)
    {
        auto it = mOptions.find(name);
        return (it != mOptions.end()) ? it->second : "";
    }
    bool hasOption(const char* name)
    {
        auto it = mOptions.find(name);
        return (it != mOptions.end());
    }
    std::string errMsg()
    {
        return mErrMsg;
    }

private:
    std::map<std::string, std::string> mOptions;
    std::vector<std::string> mValues;
    std::string mErrMsg;
};

inline bool CmdLine::init(int argc, char** argv, std::initializer_list<std::pair<char, const char*>> optionslist)
{
    const char* prevopt = nullptr;
    bool ret = true;
    for (int i = 1; i < argc; i++)
    {
        const char* param = argv[i];

        // Count number of dash
        int dashes = 0;
        while (param[dashes] == '-')
        {
            dashes++;
        }

        // If there are no dashses, add as option or a value based on prev param
        if (dashes == 0)
        {
            if (prevopt)
            {
                mOptions[prevopt] = std::string(param);
                prevopt = nullptr;
            }
            else
            {
                mValues.push_back(std::string(param));
            }
        }
        else
        {
            // Here, we have an option (starting with 1 or 2 dashes). If there is already an option, 
            // we need to close that one by adding with a blank option value.
            if (prevopt)
            {
                mOptions[prevopt] = std::string();
                prevopt = nullptr;
            }
            for (auto opt : optionslist)
            {
                // If we have 1 dash, match the char values (case sensitve), if we have 2 dashes, match the option 
                // string name (case insensitve)
                if (dashes == 1 && opt.first == param[1] || dashes == 2 && _stricmp(opt.second, param + 2) == 0)
                {
                    prevopt = opt.second;
                    break;
                }
            }
            if (prevopt == nullptr)
            {
                if (mErrMsg.empty())
                {
                    mErrMsg = "Invalid option(s): ";
                }
                mErrMsg += param;
                ret = false;
            }
        }
    }

    // If there are any left over prev option, add it without a blank option value
    if (prevopt)
    {
        mOptions[prevopt] = std::string();
    }
    return ret;
}

