#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int helper(int read_fd){
	int fd[2], pid, prime, result, received;
	result=read(read_fd, &prime, 4);
	if (result != 0) {
		printf("prime %d\n", prime);
		pipe(fd);
		pid = fork();
		if (pid == 0){
			close(fd[0]);
			result=1;
			while (result){
				result = read(read_fd, &received, 4);
				if (received % prime != 0){
					write(fd[1], &received, 4);		
				}
			}
			close(fd[1]);
		}
		if (pid > 0) {
			close(fd[1]);
			helper(fd[0]);
			close(fd[0]);
		}
	}
	exit(0);
}

int main(int argc, char *argv[])
{
	int fd[2], pid;
	pipe(fd);
	pid=fork();

	if (pid == 0){
		close(fd[0]);
		for (int i=2; i<35; i++){
			write(fd[1], &i, 4);
		}
		close(fd[1]);
	}
	else{
		close(fd[1]);
		helper(fd[0]);
		close(fd[0]);
	}
	exit(0);
}
