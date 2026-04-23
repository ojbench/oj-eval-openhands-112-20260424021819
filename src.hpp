
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <functional>
// WARNING: NO more headers allowed!

using std::pair;
using std::string;
using std::vector;
using std::map;
using std::function;
namespace final {
    class arguments {
    private:
        // WARNING: You cannot add more member variables.
        int _argc;
        char** _argv;
    public:
        arguments() : _argc(0), _argv(nullptr) {}
        explicit arguments(const string& cmd) {
            // Parse the command string
            if (cmd.empty()) {
                _argc = 0;
                _argv = nullptr;
                return;
            }
            
            // First, count the number of arguments
            _argc = 0;
            vector<string> args;
            string current;
            bool in_arg = false;
            
            for (char c : cmd) {
                if (c == ' ') {
                    if (in_arg) {
                        args.push_back(current);
                        current.clear();
                        in_arg = false;
                    }
                } else {
                    current += c;
                    in_arg = true;
                }
            }
            
            // Add the last argument if there is one
            if (in_arg) {
                args.push_back(current);
            }
            
            _argc = args.size();
            
            if (_argc == 0) {
                _argv = nullptr;
                return;
            }
            
            // Allocate argv array
            _argv = new char*[_argc];
            
            // Copy each argument to argv
            for (int i = 0; i < _argc; i++) {
                _argv[i] = new char[args[i].length() + 1];
                strcpy(_argv[i], args[i].c_str());
            }
        }
        
        ~arguments() {
            // Clean up dynamically allocated memory
            if (_argv != nullptr) {
                for (int i = 0; i < _argc; i++) {
                    delete[] _argv[i];
                }
                delete[] _argv;
            }
        }
        
        // WARNING: You cannot modify the following functions
        int argc() const { return _argc; }
        char** argv() const { return _argv; }
    };

    // You don't need to modify shell.
    class shell {
    private:
        map<int, arguments> running_list;
    public:
        shell() = default;

        void run(int pid, const string& cmd, const function<void(int, char**)>& invoked) {
            running_list.emplace(pid, cmd);
            invoked(running_list[pid].argc(), running_list[pid].argv());
        }

        int subprocessExit(int pid, int return_value) {
            running_list.erase(pid);
            return return_value;
        }
        vector<int> getRunningList() const {
            vector<int> rt;
            for (auto& pair:running_list)rt.push_back(pair.first);
            return rt;
        }
    };
}
