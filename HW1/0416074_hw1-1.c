#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
	
	pid_t pid;
	int lv = 1;
	printf("Main Process ID : %d\n\n",getpid());

	pid = fork();
	
	

	if(pid < 0){
		printf("Fork Failed");
		return 1;
	}else if(pid == 0){
		printf("Fork %d, I'm the child %d, my parent is %d.\n", lv++, getpid(), getppid());
		pid_t p1, p2;
		if((p1 = fork()) == 0){
			printf("Fork %d, I'm the child %d, my parent is %d.\n", lv++, getpid(), getppid());
			if((p1 = fork()) == 0){
				printf("Fork %d, I'm the child %d, my parent is %d.\n", lv++, getpid(), getppid());
				exit(0);
			}else{
				wait(NULL);
				exit(0);
			}
		}else if((p2 = fork()) == 0){
			printf("Fork %d, I'm the child %d, my parent is %d.\n", lv++, getpid(), getppid());
			if((p2 = fork()) == 0){
				printf("Fork %d, I'm the child %d, my parent is %d.\n", lv++, getpid(), getppid());
				exit(0);
			}else{
				wait(NULL);
				exit(0);
			}
		}else{
			waitpid(p1, NULL, 0);
			waitpid(p2, NULL, 0);
			exit(0);
		}
	}else{
		wait(NULL);
	}
	
	return 0;
}
//linux1.cs.nctu.edu.tw