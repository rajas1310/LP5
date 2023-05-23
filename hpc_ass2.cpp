#include <iostream>
#include <bits/stdc++.h>
#include <omp.h>
     
using namespace std;

void seq_BS(int arr[], int n){
    for(int i=0; i<n; i++){
        for(int j=0; j<n-i-1; j++){
            if(arr[j]>arr[j+1]){
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

void par_BS(int arr[], int n){
    #pragma omp parallel
    {
        for(int i=0; i<n; i++){
            int thread_num = omp_get_thread_num();
            int num_threads = omp_get_num_threads();
            for(int j=thread_num; j<n-i-1; j += num_threads){
                if(arr[j]>arr[j+1]){
                    int temp = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = temp;
                }
            }
        }
    }
}

void merge(int arr[], int l, int m, int r){
    int n1 = m - l + 1;
    int n2 = r - (m+1) + 1;

    int L[n1], R[n2];

    for(int i=0; i<n1; i++){
        L[i] = arr[l+i];
    }
    for(int i=0; i<n2; i++){
        R[i] = arr[m+1+i];
    }

    int i=0, j=0, k=0;
    
    while(i<n1 && j<<n2){
        if(L[i]<=R[j]){
            arr[k] = L[i];
            i++;
            k++;
        }
        else{
            arr[k] = R[j];
            j++;
            k++;
        }
    }

    while(i<n1){
        arr[k] = L[i];
        i++;
        k++;
    }
    while(j<n2){
        arr[k] = R[j];
        j++;
        k++;
    }

}

void seq_MS(int arr[], int l, int r){
    int m = l + (r-l)/2;
    if(l<r){
        seq_MS(arr, l, m);
        seq_MS(arr, m+1, r);
        merge(arr, l, m, r);
    }
}


void para_MS(int arr[], int l, int r, int threads){
    
    if(l<r){
        int m = l + (r-l)/2;
        if(threads>1 && (r-l)>threads*1000){
            #pragma omp parallel sections
            {
                #pragma omp section
                {
                    para_MS(arr, l, m, threads/2);  
                }
                #pragma omp section
                {
                    para_MS(arr, m+1, r, threads - threads/2);  
                }

            }

        }
        else{
            seq_MS(arr, l, m);
            seq_MS(arr, m+1, r);
            merge(arr, l, m, r);
        }
        
    }
}


int main(){
    int n;
    cout<<"Enter: ";
    cin>>n;

    int arr[n];
    srand(time(NULL));
    for(int i=0; i<n; i++){
        arr[i] = rand();
    }

    auto start = chrono::high_resolution_clock::now();
    seq_BS(arr, n);
    auto end = chrono::high_resolution_clock::now();
    cout<<"Sequential Bubble Sort:\t"<<chrono::duration_cast<chrono::microseconds>(end-start).count()<<" microseconds"<<endl;

    start = chrono::high_resolution_clock::now();
    par_BS(arr, n);
    end = chrono::high_resolution_clock::now();
    cout<<"Parallel Bubble Sort:\t"<<chrono::duration_cast<chrono::microseconds>(end-start).count()<<" microseconds"<<endl;

    start = chrono::high_resolution_clock::now();
    seq_MS(arr, 0,n-1);
    end = chrono::high_resolution_clock::now();
    cout<<"Sequenial Merge Sort:\t"<<chrono::duration_cast<chrono::microseconds>(end-start).count()<<" microseconds"<<endl;

    start = chrono::high_resolution_clock::now();
    para_MS(arr, 0,n-1, omp_get_max_threads());
    end = chrono::high_resolution_clock::now();
    cout<<"Parallel Merge Sort:\t"<<chrono::duration_cast<chrono::microseconds>(end-start).count()<<" microseconds"<<endl;


    return 0;
}