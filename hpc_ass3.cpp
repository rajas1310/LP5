#include <iostream>
#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

void max(vector<int> arr){

    //Sequential
    int max_ele = INT_MIN;
    double start = omp_get_wtime();
    for(auto i=arr.begin(); i!=arr.end(); i++){
        if (*i>max_ele){
            max_ele = *i;
        }
    }
    double end = omp_get_wtime();
    cout<<"Seq redn (MAX): "<<max_ele<<" \t"<<(end-start)<<endl;

    //Parallel
    max_ele = INT_MIN;
    start = omp_get_wtime();
    #pragma omp parallel for reduction(max:max_ele)
    for(auto i=arr.begin(); i!=arr.end(); i++){
        if (*i>max_ele){
            max_ele = *i;
        }
    }
    end = omp_get_wtime();
    cout<<"Para redn (MAX): "<<max_ele<<" \t"<<(end-start)<<endl;

}

void min(vector<int> arr){
    //Sequential
    int min_ele = INT_MAX;
    double start = omp_get_wtime();
    for(auto i=arr.begin(); i!=arr.end(); i++){
        if (*i<min_ele){
            min_ele = *i;
        }
    }
    double end = omp_get_wtime();
    cout<<"Seq redn (MIN): "<<min_ele<<" \t"<<(end-start)<<endl;

    //Parallel
    min_ele = INT_MAX;
    start = omp_get_wtime();
    #pragma omp parallel for reduction(min:min_ele)
    for(auto i=arr.begin(); i!=arr.end(); i++){
        if (*i<min_ele){
            min_ele = *i;
        }
    }
    end = omp_get_wtime();
    cout<<"Para redn (MIN): "<<min_ele<<" \t"<<(end-start)<<endl;
}

void sum(vector<int> arr){
    //Sequential
    int sum = 0;
    double start = omp_get_wtime();
    for(auto i=arr.begin(); i!=arr.end(); i++){
        sum += *i;
    }
    double end = omp_get_wtime();
    cout<<"Seq redn (SUM): "<<sum<<" \t"<<(end-start)<<endl;

    //Parallel
    sum = 0;
    start = omp_get_wtime();
    #pragma omp parallel for reduction(+:sum)
    for(auto i=arr.begin(); i!=arr.end(); i++){
        sum += *i;
    }
    end = omp_get_wtime();
    cout<<"Para redn (SUM): "<<sum<<" \t"<<(end-start)<<endl;
}

void avg(vector<int> arr){
    //Sequential
    int sum = 0;
    double start = omp_get_wtime();
    for(auto i=arr.begin(); i!=arr.end(); i++){
        sum += *i;
    }
    sum = sum / arr.size();
    double end = omp_get_wtime();
    cout<<"Seq redn (AVG): "<<sum<<" \t"<<(end-start)<<endl;

    //Parallel
    sum = 0;
    start = omp_get_wtime();
    #pragma omp parallel for reduction(+:sum)
    for(auto i=arr.begin(); i!=arr.end(); i++){
        sum += *i;
    }
    sum = sum / arr.size();
    end = omp_get_wtime();
    cout<<"Para redn (AVG): "<<sum<<" \t"<<(end-start)<<endl;
}

int main(){
    
    int n;
    cout<<"Enter number of elements: ";
    cin>>n;

    int MAX = 1000;
    vector<int> v;
    srand(time(NULL));
    for(int i=0; i<n; i++){
        v.push_back(rand()%(MAX));
    }

    max(v);
    min(v);
    sum(v);
    avg(v);
    return 0;
}