#include"similarity.h"

double calculate_cosine_similarity(const int *vecA,const int *vecB,int size){
	//Similarity=(A * B) / (||A|| * ||B||)
    double dot_product=0.0;
    double norm_a=0.0;
    double norm_b=0.0;
    for(int i=0;i<size;i++){
        dot_product+=vecA[i]*vecB[i];
        norm_a+=(double)vecA[i]*vecA[i];
        norm_b+=(double)vecB[i]*vecB[i];
    }
    norm_a=sqrt(norm_a);
    norm_b=sqrt(norm_b);
    double denominator=norm_a*norm_b;
    if(denominator==0.0){
        return 0.0;
    }
    return dot_product/denominator;
}