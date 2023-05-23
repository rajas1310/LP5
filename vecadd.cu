#include<iostream>
#include<bits/stdc++.h>
#include<cuda.h>

#define BLOCK_SIZE 64

using namespace std;

void initialize_array(int *arr, int size){
	for(int i=0; i<size; i++){
		arr[i]=rand()%100;
	}
}

void add_cpu(int *arr1, int *arr2, int *res, int size){
	for(int i=0; i<size; i++){
		res[i]=arr1[i] + arr2[i]; 
	}
}

void print_matrix(int *arr, int size){
	for(int i=0; i<size; i++){
		cout<<arr[i]<<" ";
	}
}

__global__ void add_gpu(int *arr1, int *arr2, int *res, int size){
	int id = blockIdx.x * blockDim.x + threadIdx.x;
	if(id < size){
		res[id] = arr1[id] + arr2[id];
	}
}

int main(){
	int *arr1_cpu, *arr2_cpu, *res_cpu;
	int n;
	cout<<"Enter size of array: ";
	cin>>n;
	
	arr1_cpu = new int[n];
	arr2_cpu = new int[n];
	res_cpu = new int[n];
	
	initialize_array(arr1_cpu, n);
	initialize_array(arr2_cpu, n);
	
	/* CPU addition */
	auto start = chrono::high_resolution_clock::now();	
	add_cpu(arr1_cpu, arr2_cpu, res_cpu, n);
	auto end = chrono::high_resolution_clock::now();
	cout << "CPU Duration: "<< chrono::duration_cast<chrono::microseconds>(end-start).count()<< "microseconds"<<endl;
	
	/* GPU addition */
	
	int *arr1_gpu, *arr2_gpu, *res_gpu;
	dim3 dimGrid(n/BLOCK_SIZE);
	dim3 dimBlock(BLOCK_SIZE);
	
	cudaMallocManaged(&arr1_gpu, n*sizeof(int));
	cudaMallocManaged(&arr2_gpu, n*sizeof(int));
	cudaMallocManaged(&res_gpu, n*sizeof(int));
	
	cudaMemcpy(arr1_gpu, arr1_cpu, n*sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(arr2_gpu, arr2_cpu, n*sizeof(int), cudaMemcpyHostToDevice);
	start = chrono::high_resolution_clock::now();
	add_gpu<<<dimGrid,dimBlock>>>(arr1_gpu, arr2_gpu, res_gpu, n);
	end = chrono::high_resolution_clock::now();
	cout << "GPU Duration: "<< chrono::duration_cast<chrono::microseconds>(end-start).count()<< "microseconds"<<endl;
	
	cudaMemcpy(arr1_cpu, arr1_gpu, n*sizeof(int), cudaMemcpyDeviceToHost);
	cudaMemcpy(arr2_cpu, arr2_gpu, n*sizeof(int), cudaMemcpyDeviceToHost);
	cudaMemcpy(res_cpu, res_gpu, n*sizeof(int), cudaMemcpyDeviceToHost);
	
	cudaFree(arr1_gpu);
	cudaFree(arr2_gpu);
	cudaFree(res_gpu);
	return 0;
}
