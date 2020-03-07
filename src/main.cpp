#include <iostream>
#include <ConfigParser.h>
#include <string>
#include <sstream>
#include <Server.h>
#include <unistd.h>
#include <csignal>
#include <sys/wait.h>

const int ERROR = -1;
char* error_text;

struct InitSettings {
public:
	std::string config_path;
	InitSettings() {
		config_path = "./httpd.conf";
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
	std::shared_ptr<Config> server_config = std::make_shared<Config>(Config());
	inflate_config(server_config, preconfig.config_path);
	// TODO: config validate

	Server server(
			server_config->GetInt("cpu_limit"),
			server_config->GetInt("thread_limit"),
			server_config->GetString("document_root"));

	int pid = fork();
	if(pid < 0) {
		perror("Error during main fork");
		return pid;
	} else if(pid > 0) {
		signal(SIGINT, [](int s) {
			std::cerr << "Server process stopping...\n";
			exit(0);
		});
		if(ERROR == server.Run()) {
			std::cerr << "Error during server work!\n";
			int ppid = getppid();
			kill(ppid, SIGINT);
			return ERROR;
		}
	} else {
		std::string buffer;
		signal(SIGINT, [](int s) {
			std::cerr << "Server process stopped. Stopping parent...\n";
			exit(1);
		});
		while(true) {
			std::cin >> buffer;
			if(buffer == "stop") {
				std::cerr << "Stopping server...\n";
				kill(pid, SIGINT);
				wait(NULL);
				return 0;
			}
		}
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
			if(argc - 1 < i + 1) {
				error_text = "Wrong args count";
				return ERROR;
			}
			strct.config_path = commands[0].inflate(Option<std::string>::STR, argv[i+1]);
		}
	}
	return 0;
}

int inflate_config(std::shared_ptr<Config> ptr, const std::string filename) {
	class HttpdConfigAdaper : public ConfigAdapter {
		std::pair<std::string, std::any> Get() {
			std::string line;
			std::getline(_stream, line);
			std::pair<std::string, std::any> p;
			if(line.empty()) {
				return p;
			}
			std::stringstream s;
			s.str(line);
			//	config form:
			//	key value #comment
			while(!s.eof()) {	// read key
				s >> line;
				if(line[0] != '#') {
					p.first = line;
					break;
				}
			}
			while(!s.eof()) {	// read value
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
		printf("Cannot open config");
		return ERROR;
	}
	return ConfigParser(ptr).parse(adapter);
}