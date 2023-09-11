#include <iostream>
#include <math.h>
#include <random>
#include <vector>
#include <algorithm>
#include <iomanip>
class genetic
{
public:
    std::vector<std::vector<double>> populations;
    std::vector<std::vector<double>> samples;
    int populationsize;
    int samplesize;
    int chromosomeLength;
    int num =0;
public:
    genetic(int size,int length,int samplelength){
        populationsize = size;
        chromosomeLength = length;
        samplesize = samplelength;
        initializePopulation();
    }
    void initializePopulation(){
        std::random_device device;
        std::uniform_real_distribution<double> unif(-100,100);
        for(int i=0;i<populationsize;i++){
            std::vector<double> chromosome;
            for(int j=0;j<chromosomeLength;j++){
                chromosome.push_back(unif(device));
            }
            chromosome.push_back(0);
            populations.push_back(chromosome);
        }
        for(auto& s:populations){fitness(s);}
        sort(populations.begin(),populations.end(),
        [this](const auto& lhs,const auto& rhs){
            return lhs[chromosomeLength]>rhs[chromosomeLength];
        });
    }


    void fitness(std::vector<double>& s){
        double ans = (6*s[0]-s[1]+std::pow(s[2],200))-25;
        s[3] = (ans==0)?9999:(1/ans);
    }


    void getsample(){
        std::random_device device;
        // take top solutions
        samples.clear();
        std::copy(
            populations.begin(),
            populations.begin()+samplesize,
            std::back_inserter(samples)
        );
        // // Mutate the top populations by % 
        std::uniform_real_distribution<double> m(0.99,1.01);
        std::for_each(samples.begin(),samples.end(),[&](auto& s){
            for(int i=0;i<chromosomeLength;i++){
                s[i] *= m(device);
            }
        });

        // cross over
        std::uniform_int_distribution<int> cross(0,samplesize-1);
        for(int i=0;i<populationsize;i++){
            std::vector<double> chromosome;
            for(int j=0;j<chromosomeLength;j++){
                chromosome.push_back(samples[cross(device)][j]);
            }
            chromosome.push_back(0);
            populations.push_back(chromosome);
        }
        // sort
        for(auto& s:populations){fitness(s);}
        sort(populations.begin(),populations.end(),
        [this](const auto& lhs,const auto& rhs){
            return lhs[chromosomeLength]>rhs[chromosomeLength];
        });
        printTopPopulations();
    }

    void printTopPopulations(){
        std::cout<<"------- print No :"<<num<<"--------------"<<"\n";
        num++;
        std::for_each(populations.begin(),populations.begin()+10,[this](const auto& s){
            std::cout<<std::fixed
            <<std::setprecision(2)
            <<"Rank :"<<static_cast<int>(s[chromosomeLength])<<"\n";
            for(int i=0;i<chromosomeLength;i++){
                std::cout<<"  No."<<i<<":"<<s[i];
            }
            std::cout<<"\n";

        });
    }

    ~genetic(){

    }
};


int main(){
    genetic gen(1000000,3,1000);
    gen.printTopPopulations();
    gen.getsample();
    gen.getsample();
    return 0;


}
