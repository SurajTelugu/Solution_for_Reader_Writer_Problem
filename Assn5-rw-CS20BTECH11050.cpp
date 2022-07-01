#include <iostream>
#include <chrono>
#include <sys/time.h>
#include <random>
#include <unistd.h>
#include <pthread.h>
#include <mutex>
#include <atomic>
#include <semaphore.h>

using namespace std;
using namespace std::chrono;

mutex mx;
atomic_int rthread_id(0);
atomic_int wthread_id(0);

unsigned nw, nr;
unsigned kw, kr;
double ucs, urem;

pthread_t *wthread;
pthread_t *rthread;

sem_t rw_mutex, sem_mutex;
unsigned read_count = 0;

double Avg_Reader_Time = 0, Avg_Writer_Time = 0;
double Worst_Reader_Time = 0, Worst_Writer_Time = 0;

FILE *out;

// This function gives time at a particular instant at a precision of microseconds
string getSysTime()
{
    mx.lock();
    string SysTime;
    string temp;
    time_t t = time(NULL);
    temp = ctime(&t);

    // Getting Milliseconds and MicroSeconds
    struct timeval min_time;
    gettimeofday(&min_time, NULL);
    int ms = min_time.tv_usec / 1000, musec = min_time.tv_usec % 1000;

    int hour = stoi(temp.substr(11, 2));
    int min = stoi(temp.substr(14, 2));

    SysTime = to_string(hour) + ":" + to_string(min) + ":" +
              temp.substr(17, 2) + ":" + to_string(ms) + ":" + to_string(musec);

    mx.unlock();
    return SysTime;
}

void Print_Message(int num, int tid, char c, string row, string time)
{
    if (c == 'r')
    {
        if (num % 10 == 1 && num % 100 != 11)
            fprintf(out, "%dst CS request by %s Thread %d at %s\n", num, row.c_str(), tid, time.c_str());

        else if (num % 10 == 2 && num % 100 != 12)
            fprintf(out, "%dnd CS request by %s Thread %d at %s\n", num, row.c_str(), tid, time.c_str());

        else if (num % 10 == 3 && num % 100 != 13)
            fprintf(out, "%drd CS request by %s Thread %d at %s\n", num, row.c_str(), tid, time.c_str());

        else
            fprintf(out, "%dth CS request by %s Thread %d at %s\n", num, row.c_str(), tid, time.c_str());
    }

    else if (c == 'e')
    {
        if (num % 10 == 1 && num % 100 != 11)
            fprintf(out, "%dst CS Entry by %s Thread %d at %s\n", num, row.c_str(), tid, time.c_str());

        else if (num % 10 == 2 && num % 100 != 12)
            fprintf(out, "%dnd CS Entry by %s Thread %d at %s\n", num, row.c_str(), tid, time.c_str());

        else if (num % 10 == 3 && num % 100 != 13)
            fprintf(out, "%drd CS Entry by %s Thread %d at %s\n", num, row.c_str(), tid, time.c_str());

        else
            fprintf(out, "%dth CS Entry by %s Thread %d at %s\n", num, row.c_str(), tid, time.c_str());
    }

    else if (c == 'x')
    {
        if (num % 10 == 1 && num % 100 != 11)
            fprintf(out, "%dst CS Exit by %s Thread %d at %s\n", num, row.c_str(), tid, time.c_str());

        else if (num % 10 == 2 && num % 100 != 12)
            fprintf(out, "%dnd CS Exit by %s Thread %d at %s\n", num, row.c_str(), tid, time.c_str());

        else if (num % 10 == 3 && num % 100 != 13)
            fprintf(out, "%drd CS Exit by %s Thread %d at %s\n", num, row.c_str(), tid, time.c_str());

        else
            fprintf(out, "%dth CS Exit by %s Thread %d at %s\n", num, row.c_str(), tid, time.c_str());
    }
}

void *Reader(void *args)
{
    rthread_id++;
    int tid = rthread_id;

    default_random_engine R_rand_gen(time(NULL));
    exponential_distribution<double> R_Dist1(1 / ucs);
    exponential_distribution<double> R_Dist2(1 / urem);

    string reqEnterTime, enterTime, exitTime;

    for (int i = 0; i < kr; i++)
    {
        reqEnterTime = getSysTime();
        Print_Message(i + 1, tid, 'r', "Reader", reqEnterTime);

        auto start = high_resolution_clock::now();
        sem_wait(&sem_mutex);
        read_count++;
        if (read_count == 1)
            sem_wait(&rw_mutex);
        auto stop = high_resolution_clock::now();

        double looptime = duration_cast<microseconds>(stop - start).count();

        Avg_Reader_Time = Avg_Reader_Time + looptime;
        Worst_Reader_Time = max(Worst_Reader_Time, looptime);

        enterTime = getSysTime();
        Print_Message(i + 1, tid, 'e', "Reader", enterTime);

        sem_post(&sem_mutex);

        double randCSTime = R_Dist1(R_rand_gen);
        usleep(randCSTime * 1000);

        sem_wait(&sem_mutex);
        read_count--;
        if (read_count == 0)
            sem_post(&rw_mutex);
        exitTime = getSysTime();
        Print_Message(i + 1, tid, 'x', "Reader", exitTime);
        sem_post(&sem_mutex);

        double randRemTime = R_Dist2(R_rand_gen);
        usleep(randRemTime * 1000);
    }
    pthread_exit(NULL);
}

void *Writer(void *args)
{
    wthread_id++;
    int tid = wthread_id;

    default_random_engine W_rand_gen(time(NULL));
    exponential_distribution<double> W_Dist1(1 / ucs);
    exponential_distribution<double> W_Dist2(1 / urem);

    string reqEnterTime, enterTime, exitTime;

    for (int i = 0; i < kw; i++)
    {
        reqEnterTime = getSysTime();
        Print_Message(i + 1, tid, 'r', "Writer", reqEnterTime);

        auto start = high_resolution_clock::now();
        sem_wait(&rw_mutex);
        auto stop = high_resolution_clock::now();

        double looptime = duration_cast<microseconds>(stop - start).count();

        Avg_Writer_Time = Avg_Writer_Time + looptime;
        Worst_Writer_Time = max(Worst_Writer_Time, looptime);

        enterTime = getSysTime();
        Print_Message(i + 1, tid, 'e', "Writer", enterTime);

        double randCSTime = W_Dist1(W_rand_gen);
        usleep(randCSTime * 1000);

        exitTime = getSysTime();
        Print_Message(i + 1, tid, 'x', "Writer", exitTime);

        sem_post(&rw_mutex);

        double randRemTime = W_Dist2(W_rand_gen);
        usleep(randRemTime * 1000);
    }

    pthread_exit(NULL);
}

int main()
{
    FILE *inp = fopen("inp-params.txt", "r");
    fscanf(inp, "%u %u %u %u %lf %lf", &nw, &nr, &kw, &kr, &ucs, &urem);
    fclose(inp);

    out = fopen("RW-log.txt", "w");

    sem_init(&rw_mutex, 0, 1);
    sem_init(&sem_mutex, 0, 1);

    // Allocating memory for threads
    wthread = (pthread_t *)malloc(nw * sizeof(pthread_t));
    rthread = (pthread_t *)malloc(nr * sizeof(pthread_t));

    for (int i = 0; i < nr; i++)

        pthread_create(&rthread[i], NULL, Reader, NULL);

    for (int i = 0; i < nw; i++)

        pthread_create(&wthread[i], NULL, Writer, NULL);

    for (int i = 0; i < nr; i++)

        pthread_join(rthread[i], NULL);

    for (int i = 0; i < nw; i++)

        pthread_join(wthread[i], NULL);

    fclose(out);

    // double ar = (Avg_Reader_Time / (nr * kr)) / 1000;
    // double aw = (Avg_Writer_Time / (nw * kw)) / 1000;
    // double maxr = Worst_Reader_Time / 1000;
    // double maxw = Worst_Writer_Time / 1000;

    // cout << "{" << ar << "," << aw << "," << maxr << "," << maxw << "};" << endl;

    FILE *avg = fopen("RW_Average_time.txt", "w");

    fprintf(avg, "Average time taken for a thread to gain entry to the Critical Section for Writer threads: %lf\n", Avg_Writer_Time / (nw * kw));
    fprintf(avg, "Average time taken for a thread to gain entry to the Critical Section for Reader threads: %lf", Avg_Reader_Time / (nr * kr));

    fclose(avg);

    sem_destroy(&rw_mutex);
    sem_destroy(&sem_mutex);
    free(wthread);
    free(rthread);

    return 0;
}