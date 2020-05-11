#include <iostream>
#include <ConfigParser.h>
#include <string>
#include <sstream>
#include <Server.h>
#include <unistd.h>
#include <csignal>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <wait.h>
#include <string.h>
#include <stdlib.h>

const int ERROR = -1;
char* error_text;

struct InitSettings {
public:
	std::string config_path;
	InitSettings() {
		config_path = "/etc/httpd.conf";
	}
};

int parse_cmd_args(const int, char*[], InitSettings&);

int inflate_config(std::shared_ptr<Config> ptr, const std::string filename);

//	Commands
//	-c [path]	- path to config file, default: ./httpd.conf
int main(int argc, char* argv[]) {
	std::cout << "Hello!" << std::endl;
	InitSettings preconfig;
	if(ERROR == parse_cmd_args(argc, argv, preconfig)) {
		printf("Cannot parse cmd arguments!\n");
		return ERROR;
	}
	std::cout << "Starting inflting config\n";
	std::shared_ptr<Config> server_config = std::make_shared<Config>(Config());
	inflate_config(server_config, preconfig.config_path);
	std::cout << "Config is inflated\n";
	// TODO: config validate

	Server server(
			server_config->GetInt("cpu_limit"),
			server_config->GetInt("thread_limit"),
			server_config->GetString("document_root"));

	int pid = fork();
	std::cerr << pid << "\n";
	if(pid < 0) {
		perror("Error during main fork");
		return pid;
	} else if(pid == 0) {
		signal(SIGINT, [](int s) {
			std::cerr << "Server process stopping...\n";
			exit(0);
		});
		std::cerr << "Starting a server...\n";
//		char* args[] = {std::to_string(pid).c_str(), "-c", std::to_string(1).c_str(), '\0'};
//        execl("cpulimit",
//              "-p",
//              std::to_string(pid).c_str(),
//              "-c",
//              std::to_string(server_config->GetInt("cpu_limit")).c_str(),
//              "-l",
//              std::to_string(100*server_config->GetInt("cpu_limit")).c_str(),
//              NULL);
		if(ERROR == server.Run()) {
			std::cerr << "Error during server work!\n";
			int ppid = getppid();
			kill(ppid, SIGINT);
			return ERROR;
		}
	} else {
	    int pid2 = fork();
        if(pid2 < 0) {
            perror("Error during main fork");
            return pid;
        } else if(pid2 == 0) {
            signal(SIGINT, [](int s) {
                std::cerr << "CPULIMIT proc stopping...\n";
                exit(0);
            });
            std::cerr << "cpulimit starting...\n";
//            system()
            int res = execlp("cpulimit", "cpulimit",
              "-p",
              std::to_string(pid).c_str(),
              "-c",
              std::to_string(server_config->GetInt("cpu_limit")).c_str(),
              "-l",
              std::to_string(100*server_config->GetInt("cpu_limit")).c_str(),
              NULL);
            perror("");
            std::cerr << "res: "<< res << ": cpulimit ended\n";
        } else {
            std::string buffer;
            signal(SIGINT, [](int s) {
                std::cerr << "Server process stopped. Stopping parent...\n";
                exit(1);
                return;
            });
            std::cerr << "Parent. Waiting for command...\n";
            while(true) {
                std::cin >> buffer;
                if(buffer == "stop" || buffer == "s") {
                    std::cerr << "Stopping server...\n";
                    kill(pid, SIGINT);
                    kill(pid2, SIGINT);
                    wait(NULL);
                    return 0;
                }
            }
        }
//		std::string buffer;
//		signal(SIGINT, [](int s) {
//			std::cerr << "Server process stopped. Stopping parent...\n";
//			exit(1);
//			return;
//		});
//		while(true) {
//			std::cin >> buffer;
//			if(buffer == "stop" || buffer == "s") {
//				std::cerr << "Stopping server...\n";
//				kill(pid, SIGINT);
//				wait(NULL);
//				return 0;
//			}
//		}
	}

	return 0;
}

template<typename T>
class Option {
public:
	enum TYPES{INT, STR, BOOL};
private:
	std::string _name;
	std::string _help;
public:
	Option(std::string name, std::string help = "") : _name(name), _help(help) {}
	bool compare(std::string tested_value) {
		return _name == tested_value;
	}
	T inflate(TYPES type, const char* value) {
		switch(type) {
			case STR:
				return static_cast<T>(std::string(value));
			default:
				return T();
		}
	}
};

int parse_cmd_args(const int argc, char* argv[], InitSettings& strct) {
	if(argc <= 0) {
		error_text = "Wrong argc count";
		return ERROR;
	}
	Option<std::string> commands[] = {
			Option<std::string>("-c")
	};

	for(int i = 1; i < argc; i++) {
		if(commands[0].compare(argv[i])) {
		    std::cout << "Got an arg: " << argv[i] << "\n";
			if(argc - 1 < i + 1) {
				error_text = "Wrong args count";
				return ERROR;
			}
			strct.config_path = commands[0].inflate(Option<std::string>::STR, argv[i+1]);
            std::cout << strct.config_path << "\n";
		}
	}
	return 0;
}

int inflate_config(std::shared_ptr<Config> ptr, const std::string filename) {
	class HttpdConfigAdaper : public ConfigAdapter {
		std::pair<std::string, std::any> Get() {
		    std::cerr << "Start Get()\n";
			std::string line;
			std::getline(*_stream, line);
            std::cerr << "get line Get()\n";
			std::pair<std::string, std::any> p;
			if(line.empty()) {
				return p;
			}
			std::stringstream s;
			s.str(line);
			//	config form:
			//	key value #comment
			while(!s.eof()) {	// read key
                std::cerr << "read key Get()\n";
				s >> line;
				if(line[0] != '#') {
					p.first = line;
					break;
				}
			}
			while(!s.eof()) {	// read value
                std::cerr << "read value Get()\n";
				s >> line;
				if(line[0] != '#') {
					//	if there is a number in string,
					//	we convert it.
					try {
						p.second = stoi(line);
					} catch(std::invalid_argument e) {
						p.second = line;
					}
					break;
				}
			}
			return p;
		}
	};

	HttpdConfigAdaper adapter;
	if(HttpdConfigAdaper::OK != adapter.Open(filename)) {
		std::cerr << "Cannot open config: " << filename << "\n";
		return ERROR;
	} else {
	    std::cout << "Config is opened\n";
	}
	return ConfigParser(ptr).parse(adapter);
}