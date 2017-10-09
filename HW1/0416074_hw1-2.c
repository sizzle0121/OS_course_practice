#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
	int i, j, k;

	int dim;
	printf("Dimension: ");
	scanf("%d", &dim);
	/*shmget return the ID of the request shared memory*/
	int MemA = shmget(IPC_PRIVATE, sizeof(unsigned int)*dim*dim, IPC_CREAT | IPC_EXCL | 0666);
	int MemB = shmget(IPC_PRIVATE, sizeof(unsigned int)*dim*dim, IPC_CREAT | IPC_EXCL | 0666);
	int MemC = shmget(IPC_PRIVATE, sizeof(unsigned int)*dim*dim, IPC_CREAT | IPC_EXCL | 0666);
	if(MemA == -1 || MemB == -1 || MemC == -1){
		perror("shmget got error");
		exit(0);
	}
	unsigned int *attachA = (unsigned int *)shmat(MemA, NULL, 0);
	unsigned int *attachB = (unsigned int *)shmat(MemB, NULL, 0);
	unsigned int *attachC = (unsigned int *)shmat(MemC, NULL, 0);
	if(*attachA == -1 || *attachB == -1 || *attachC == -1){
		perror("shmat got error");
		exit(0);
	}
	/*initialize matrix value*/
	for(i=0; i<dim*dim; i++){
		attachA[i] = i;
		attachB[i] = i;
		attachC[i] = 0;
	}
	shmdt(attachA);
	shmdt(attachB);
	shmdt(attachC);

	int process_num = 1;
	bool flag = false;
	while(process_num < 4){
		if(flag)	process_num = 4;
		unsigned int checksum = 0;

		unsigned int *ma = (unsigned int *)shmat(MemA, NULL, 0);
		unsigned int *mb = (unsigned int *)shmat(MemB, NULL, 0);
		unsigned int *mc = (unsigned int *)shmat(MemC, NULL, 0);
		for(i=0; i<dim*dim; i++){
			mc[i] = 0;
		}

		
		//printf("\nmc debug %d %d\n\n",mc[87], mc[800*800 - 1]);
		//printf("\nma debug %d %d\n\n",ma[87], ma[800*800 - 1]);
		//printf("\nmb debug %d %d\n\n",mb[87], mb[800*800 - 1]);


		if(*attachA == -1 || *attachB == -1 || *attachC == -1){
			perror("shmat got error");
			exit(0);
		}
		struct timeval start, end;

		if(process_num == 1){
			gettimeofday(&start, 0);
			//unsigned int checksumA = 0, checksumB = 0, checksumC = 0, checksumD = 0;
			//int part = dim/4;
			for(i=0; i<dim; i++){
				for(j=0; j<dim; j++){
					for(k=0; k<dim; k++){
						//mc[i][j] += ma[i][k]*mb[k][j]
						mc[dim*i + j] += (ma[dim*i + k] * mb[dim*k + j]);
					}
					/*
					if(i < part)		checksumA += mc[dim*i + j];
					else if(i < part*2)	checksumB += mc[dim*i + j];
					else if(i < part*3)	checksumC += mc[dim*i + j];	
					else 				checksumD += mc[dim*i + j];
					*/
				}
			}
			/*for(i=0; i<dim*dim; i++)
				printf("%u ", mc[i]);*/
			/*
			printf("\npart A : %u\n", checksumA);
			printf("part B : %u\n", checksumB);
			printf("part C : %u\n", checksumC);
			printf("part D : %u\n\n", checksumD);
			*/
			gettimeofday(&end, 0);
		}else if(process_num == 4){
			gettimeofday(&start, 0);
			//unsigned int checksumA = 0, checksumB = 0, checksumC = 0, checksumD = 0;
			int part = dim / 4;
			int rs, re;
			pid_t cpid;
			//printf("%d\n", getpid());
			cpid = fork();
			/*set row start & row end*/
			if(cpid == 0){
				//printf("%d\n", getpid());
				pid_t p1, p2;
				if((p1 = fork()) == 0){
					//printf("%d\n", getpid());
					rs = 0;
					re = part - 1;
				}else if((p2 = fork()) == 0){
					//printf("%d\n", getpid());
					rs = part;
					re = part * 2 - 1;
				}else{
					rs = part * 2;
					re = part * 3 - 1;
				}

				for(i=rs; i<=re; i++){
					for(j=0; j<dim; j++){
						for(k=0; k<dim; k++){
							mc[dim*i + j] += (ma[dim*i + k] * mb[dim*k + j]);
						}
						/*
						if(i < part)		checksumA += mc[dim*i + j];
						else if(i < part*2)	checksumB += mc[dim*i + j];
						else if(i < part*3)	checksumC += mc[dim*i + j];
						*/
					}
				}

				if(p1 != 0 && p2 != 0){
					waitpid(p1, NULL, 0);
					waitpid(p2, NULL, 0);
					//printf("leave by %d\n", getpid());
					//printf("part A : %u\n", checksumA);
					exit(0);
					
				}else{
					//printf("leave by %d\n", getpid());
					//printf("part B : %u\n", checksumB);
					//printf("part C : %u\n", checksumC);
					exit(0);
					
				}
			}else{
				rs = part * 3;
				re = part * 4 - 1;
				for(i=rs; i<=re; i++){
					for(j=0; j<dim; j++){
						for(k=0; k<dim; k++){
							mc[dim*i + j] += (ma[dim*i + k] * mb[dim*k + j]);
						}
						//checksumD += mc[dim*i + j];
					}
				}
				wait(NULL);
				//printf("leave by %d\n", getpid());
				//printf("part D : %u\n", checksumD);
			}
			gettimeofday(&end, 0);
		}

		//printf("%d\n", getpid());
		for(i=0; i<dim*dim; i++){
			checksum += mc[i];
		}
		int sec = end.tv_sec - start.tv_sec;
		int usec = end.tv_usec - start.tv_usec;
		printf("%d-process, checksum = %u\n", process_num, checksum);
		printf("elapsed %f  s\n", sec + (usec/1000000.0));
		shmdt(ma);
		shmdt(mb);
		shmdt(mc);
		flag = true;
	}
	/*delete shared memory*/
	shmctl(MemA, IPC_RMID, NULL);
	shmctl(MemB, IPC_RMID, NULL);
	shmctl(MemC, IPC_RMID, NULL);

	return 0;
}

