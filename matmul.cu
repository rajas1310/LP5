#include <iostream>
#include <bits/stdc++.h>
#include <cuda.h>

#define BLOCK_SIZE 16

using namespace std;

void initialize_matrix(int *arr, int rows, int cols){
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			arr[i*cols + j] = rand()%10;
		}
	}
}

void multiply_cpu(int *mat1, int *mat2, int *res, int rows, int common, int cols){
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			int sum = 0;
			for(int k=0; k<common; k++){
				sum += mat1[i*common + k] * mat2[k*cols + j];
			}
			res[i*cols + j] = sum;
		}
	}
}


__global__ void multiply_gpu(int *mat1, int *mat2, int *res, int rows, int common, int cols){
	int r = blockIdx.x * blockDim.x + threadIdx.x;
	int c = blockIdx.y * blockDim.y + threadIdx.y;
	int sum = 0;
	
	if (r <rows && c <cols){
		for(int i=0; i<common; i++){
			sum += mat1[r*common + i] * mat2[i*cols + c]; 
		}
		res[r*cols + c] = sum;
	}
}

int main(){
	int R1, C1, R2, C2, R3, C3;
	cout<<"Enter no. of row for Mat1: ";
	cin>>R1;
	cout<<"Enter no. of columns for Mat1: ";
	cin>>C1;
	cout<<"Enter no. of columns for Mat2: ";
	cin>>C2;
	
	R2 = C1;
	R3 = R1;
	C3 = C2;
	
	int *mat1_cpu, *mat2_cpu, *res_cpu;
	mat1_cpu = new int[R1*C1];
	mat2_cpu = new int[R2*C2];
	res_cpu = new int[R3*C3];
	
	initialize_matrix(mat1_cpu, R1, C1);
	initialize_matrix(mat2_cpu, R2, C2);
	
	/* CPU multiplication */
	auto start = chrono::high_resolution_clock::now();
	multiply_cpu(mat1_cpu, mat2_cpu, res_cpu, R1, C1, C2);	
	auto end = chrono::high_resolution_clock::now();
	cout<<"CPU Duration: "<< chrono::duration_cast<chrono::microseconds>(end-start).count()<< " microseconds"<<endl;
	
	/* GPU multiplication */
	
	dim3 dimGrid(R3 / BLOCK_SIZE, C3 / BLOCK_SIZE);
	dim3 dimBlock(BLOCK_SIZE , BLOCK_SIZE);
	
	int *mat1_gpu, *mat2_gpu, *res_gpu;
	
	cudaMallocManaged(&mat1_gpu, R1*C1*sizeof(int));
	cudaMallocManaged(&mat2_gpu, R2*C2*sizeof(int));
	cudaMallocManaged(&res_gpu, R3*C3*sizeof(int));
	
	cudaMemcpy(mat1_gpu, mat1_cpu, R1*C1*sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(mat2_gpu, mat2_cpu, R2*C2*sizeof(int), cudaMemcpyHostToDevice);
	
	start = chrono::high_resolution_clock::now();
	multiply_gpu<<<dimGrid, dimBlock>>>(mat1_cpu, mat2_cpu, res_cpu, R1, C1, C2);
	end = chrono::high_resolution_clock::now();
	cout<<"GPU Duration: "<< chrono::duration_cast<chrono::microseconds>(end-start).count()<< " microseconds"<<endl;
	
	cudaMemcpy(res_cpu, res_gpu, R3*C3*sizeof(int), cudaMemcpyDeviceToHost);
	
	cudaFree(mat1_gpu);
	cudaFree(mat2_gpu);
	cudaFree(res_gpu);
	
	return 0;
}
