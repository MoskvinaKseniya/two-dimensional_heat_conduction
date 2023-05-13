#include <iostream>
#include <fstream>
#include <string>
#include <C:/Users/ksksks/CLionProjects/untitled3/nlohmann/json.hpp>
using namespace std;
using json = nlohmann::json;

int main() {
    string path = "results.txt";
    ofstream fout(path, ios_base::trunc);
    std::ifstream f("C:/Users/ksksks/CLionProjects/untitled3/data.json");
    json data = json::parse(f);
    string length_x = data.value("length_x", "not found");
    string length_y = data.value("length_y", "not found");
    string t_end = data.value("t_end", "not found");
    string step_x = data.value("step_x", "not found");
    string step_y = data.value("step_y", "not found");
    string step_t = data.value("step_t", "not found");
    string T_left = data.value("T_left", "not found");
    string T_right = data.value("T_right", "not found");
    string T_start = data.value("T_start", "not found");
    string Lambda = data.value("lambda", "not found");
    string Ro = data.value("ro", "not found");
    string C = data.value("c", "not found");

    double X = atof(length_x.c_str());
    double Y = atof(length_y.c_str());
    double T_end = atof(t_end.c_str());
    int N = atof(step_x.c_str());
    int M = atof(step_y.c_str());
    int K = atof(step_t.c_str());
    double x[N];
    double y[M];
    double t[K];
    double dx = X/N;
    double dy = Y/M;
    double dt = T_end/K;

    double T_0 = atof(T_start.c_str());
    double T_l = atof(T_left.c_str());
    double T_r = atof(T_right.c_str());
    double T[N][M];
    double lambda = atof(Lambda.c_str());  //коэфф теплопроводности
    double ro = atof(Ro.c_str()); //плотность
    double c = atof(C.c_str()); //теплоемкость
    double a = lambda/(ro*c);
    double alfa[M];
    double beta[M];

    double a_x = lambda/(dx*dx);;
    double b_x = 2*a_x + ro*c/dt;
    double c_x = a_x;
    double f_x;

    double a_y = lambda/(dy*dy);;
    double b_y = 2*a_y + ro*c/dt;
    double c_y = a_y;
    double f_y;


    // задаем начальное поле температур и шаги
    for(int i=0; i < N; i++){x[i] = i*dx;}
    for(int i=0; i < M; i++){y[i] = i*dy;}
    for(int i=0; i < K; i++){t[i] = i*dt;}
    for (int j = 0; j < N; j++) {
        for (int i = 2; i< M-2; i++){
            T[i][j] = T_0;
        }
        for (int i = M-2; i< M; i++){
            T[i][j] = T_0+40;
        }
        for (int i = 0; i< 2; i++){
            T[i][j] = T_0-40;
        }
    }
    for (int j = 0; j < M; j++) {
        T[j][0] = T_l;
        T[j][N-1] = T_r;
    }
    //вывод начального поля температур
    fout << N << " " << X << " " << M << " " << Y << " " << K << " " << T_end << " ";
    for (int i = 0; i < N-6; i++){fout << 0 << " ";}
    fout << std::endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            fout << T[i][j] << " ";
        }
        fout << std::endl;
    }

    //цикл по t
    for (int k = 0; k < K; k++){
        //цикл по y
        for (int j = 0; j < M; j++) {
            alfa[0] = 0;
            beta[0] = T_l;
            for (int i = 1; i < N - 1; i++) {
                f_x = -ro * c * T[j][i] / dt;
                alfa[i] = a_x / (b_x - c_x * alfa[i - 1]);
                beta[i] = (c_x * beta[i - 1] - f_x) / (b_x - c_x * alfa[i - 1]);
            }
            for (int i = N - 2; i > 0; i--) {
                T[j][i] = alfa[i] * T[j][i + 1] + beta[i];
            }
        }
        //цикл по x
        for(int i = 1; i < N-1; i++){
            alfa[0] = 2*a*dt/(2*a*dt+sqrt(dy));
            beta[0] = sqrt(dy)*T[0][i]/(2*a*dt+sqrt(dy));
            for(int j = 1; j < M-1; j++){
                f_y = -ro*c*T[j][i]/dt;
                alfa[j] = a_y/(b_y-c_y*alfa[j-1]);
                beta[j] = (c_y*beta[j-1]-f_y)/(b_y-c_y*alfa[j-1]);
            }
            T[M-1][i] = (2*a*dt*beta[M-2]+sqrt(dy)*T[M-1][i])/(2*a*dt*(1-alfa[M-2])+sqrt(dy));
            for(int j = M-2; j > -1; j--){
                T[j][i] = alfa[j] * T[j+1][i]+beta[j];
            }
        }

        //вывод поля температур
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                    fout << T[i][j] << " ";
            }
                fout << std::endl;
        }
    }
    fout.close();
    return 0;
}
