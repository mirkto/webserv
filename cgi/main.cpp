#include "cgi-bin/cpp/main.hpp"

int		ft_error(std::string message)
{
	PRINT(CLR_RED << message << CLR_END);
	return 127;
}

void		ft_wait()
{
	PRINT("\n10 seconds to complete the test\n...");
	sleep(5);
	for (int i = 5; i != 1; sleep(1))
	{
		PRINT(i-- << " seconds left");
		// if(std::cin)
		// 	return ;
	}
	PRINT("1 second left");
	sleep(1);
}

bool		execute_sgi_script(std::string filename)
{
	int				fd_pipe[2], status, ret;
	int				save_in = dup(0);	// для чтения
	int				save_out = dup(1);	// для записи
	char			buff[1024];
	std::string		response_body;

	pipe(fd_pipe);
	dup2(fd_pipe[0], 0);
	dup2(fd_pipe[1], 1);
	pid_t	pid = fork();
	if (pid == 0)
	{
		if(execve(filename.c_str(), NULL, NULL) == -1) // 3 argument = _env
		{
			close(fd_pipe[0]);
			close(fd_pipe[1]);
			// free _env();
			return false;
		}
	}
	wait(&status); //waitpid(pid, &status, WNOHANG);
	for(ret = 1024; ret == 1024 ; response_body += std::string(buff, ret)) // ret is need?
		ret = read(fd_pipe[0], buff, 1024);

	// // // --- test write to file	// ---
	int	file_fd = open("file.txt", O_RDWR | O_CREAT | O_APPEND, S_IWRITE | S_IREAD, 0755);
	if(file_fd == -1)
		ft_error("error: file.txt not create");
	dup2(file_fd, 1);
	PRINT("   ---");
	std::cout << response_body;
	close(file_fd);
	// // // --- test write to file	// ---

	dup2(save_in, 0);
	dup2(save_out, 1);
	close(fd_pipe[0]);
	close(fd_pipe[1]);
	if(ret == -1)
		return false;
	PRINT(response_body);
	// parse_cgi();
	// free _env();
	return true;
}

int			main(int argc, char **argv, char **env)
{
	bool			test;
	std::string		file_cpp = "/Users/ngonzo/Desktop/projects/webserv/cgi/cgi-bin/cpp/hello";
	std::string		file_py = "/Users/ngonzo/Desktop/projects/webserv/cgi/cgi-bin/py/hello.py";
	std::string		cgi_tester = "/Users/ngonzo/Desktop/projects/webserv/cgi/cgi-bin/cgi_tester";

	PRINT_TITLE("cpp")
	ft_hello();

	PRINT_TITLE("execve")
	test = execute_sgi_script(file_cpp);
	if(test == false)
		return ft_error("file_cpp not found!");
	
	PRINT_TITLE("python")
	test = execute_sgi_script(file_py);
	if(test == false)
		return ft_error("file_py not found!");

	PRINT_TITLE("cgi_tester")
	test = execute_sgi_script(cgi_tester);
	if(test == false)
		return ft_error("cgi_tester not found!");

	ft_wait();
	return 0;
}