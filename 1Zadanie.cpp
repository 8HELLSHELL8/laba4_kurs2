#include <iostream>
#include <string>
#include <cctype>
#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include <mutex>
#include <semaphore>
#include <barrier>
#include <queue>
#include <condition_variable>
#include <atomic>
using namespace std;

mutex mtx;

char generateRandomSymbol()
{
    random_device randomizer;
    mt19937 gen(randomizer());
    uniform_int_distribution<> distribution(33, 126);
    
    return static_cast<char>(distribution(gen));
}

void raceTesting(int numberOfThreads)
{
    vector<jthread> threads;

    for (int i = 0; i < numberOfThreads; i++)
    {
        threads.push_back(jthread([i]()
        {
            for(int j = 0; j < 3; j++)
            {
                char sym = generateRandomSymbol();
                {
                    lock_guard<mutex> lock(mtx);
                    cout << "Поток с id: "<< this_thread::get_id() << " сгенерировал символ: " << sym << endl;
                }
                this_thread::sleep_for(chrono::milliseconds(30));
            }
        }));
    }
}

void mutexTesting(int numberOfThreads)
{
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Проверка работы MUTEX" << endl;
    auto start = chrono::high_resolution_clock::now(); // таймер начала

    vector<jthread> threads;
    for (int i = 0; i < numberOfThreads; i++)
    {
        threads.push_back(jthread([i]()
        {
            {
                mtx.lock();
                cout << "Мьютекс заблокировался -> "; 
                cout << " Общий ресурс использовался -> ";
                mtx.unlock();
                cout << " Мьютекс разблокировался" << endl;
            }
        }));
    }

    auto end = chrono::high_resolution_clock::now(); 
    chrono::duration<double> duration = end - start; 
    this_thread::sleep_for(chrono::milliseconds(200));
    {
        lock_guard<mutex> lock(mtx);
        cout << "Mutex сработал за: " << duration.count() << " секунд." << endl;
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    }
    
}

void semaphoreTesting(int numberOfThreads)
{
    cout << "Проверка работы SEMAPHORE" << endl;
    auto start = chrono::high_resolution_clock::now(); // таймер начала

    vector<jthread> threads;
    counting_semaphore<3> semaphore(3); //пропускает 3 потока одновременно

    for (int i = 0; i < numberOfThreads; i++)
    {
        threads.push_back(jthread([i, &semaphore]()
        {
            semaphore.acquire();
            {
                lock_guard<mutex> lock(mtx);
                cout << "Схватили семафор ->";
                cout << " Использование общих ресурсов ->";
                cout << " Освободили семафор" << endl;
            }
            semaphore.release();
        }));
    }

    auto end = chrono::high_resolution_clock::now(); 
    chrono::duration<double> duration = end - start; 
    this_thread::sleep_for(chrono::milliseconds(200));
    {
        lock_guard<mutex> lock(mtx);
        cout << "Semaphore сработал за: " << duration.count() << " секунд." << endl;
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    }
    
}

void semaphoreSlimTesting(int numOfThreads)
{
    cout << "Проверка работы SEMAPHORESLIM" << endl;
    auto start = chrono::high_resolution_clock::now(); // таймер начала

    vector<jthread> threads;
    counting_semaphore<1> semaphore(1); //пропускает 3 потока одновременно

    for (int i = 0; i < numOfThreads; i++)
    {
        threads.push_back(jthread([i, &semaphore]()
        {
            semaphore.acquire();
            {
                lock_guard<mutex> lock(mtx);
                cout << "Схватили семафор ->";
                cout << " Использование общих ресурсов ->";
                cout << " Освободили семафор" << endl;
            }
            semaphore.release();
        }));
    }

    auto end = chrono::high_resolution_clock::now(); 
    chrono::duration<double> duration = end - start; 
    this_thread::sleep_for(chrono::milliseconds(200));
    
    {
        lock_guard<mutex> lock(mtx);
        cout << "SemaphoreSlim сработал за: " << duration.count() << " секунд." << endl;
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    }
    
}

void barrierTesting(int numOfThreads)
{
    cout << "Проверка работы BARRIER" << endl;
    auto start = chrono::high_resolution_clock::now(); // таймер начала

    vector<jthread> threads;
    barrier barrierLock(numOfThreads);

    for (int i = 0; i < numOfThreads; i++)
    {
        threads.push_back(jthread([i, &barrierLock]()
        {
            {
                lock_guard<mutex> lock(mtx);
                cout << "Барьер был достинут" << endl;
            }
            barrierLock.arrive_and_wait();
            {
                lock_guard<mutex> lock(mtx);
                cout << "Барьер был пройден" << endl;
            }
        }));
    }

    auto end = chrono::high_resolution_clock::now(); 
    chrono::duration<double> duration = end - start; 


    this_thread::sleep_for(chrono::milliseconds(200));
    {
        lock_guard<mutex> lock(mtx);
        cout << "Barrier сработал за: " << duration.count() << " секунд." << endl;
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    }
    
}

class Spinlock
{
private:
    std::atomic_flag atomic_flag = ATOMIC_FLAG_INIT;

public:
    void lock()
    {
       while (atomic_flag.test_and_set(std::memory_order_acquire)){}
    }
    void unlock()
    {
        atomic_flag.clear(std::memory_order_release);
    }
};

void spinLockTesting(int numOfThreads)
{
    cout << "Проверка работы SPINLOCK" << endl;
    auto start = chrono::high_resolution_clock::now(); // таймер начала
    Spinlock slock;
    vector<jthread> threads;
    
    for (int i = 0; i < numOfThreads; i++)
    {
        threads.push_back(jthread([i, &slock]()
        {
            slock.lock();
            {
                lock_guard<mutex> lock(mtx);
                cout << "Схватили лок ->";
                cout << " Использование общих ресурсов ->";
                cout << " Освободили лок" << endl;
            }
            slock.unlock();
        }));
    }

    auto end = chrono::high_resolution_clock::now(); 
    chrono::duration<double> duration = end - start; 
    this_thread::sleep_for(chrono::milliseconds(200));
    {
        lock_guard<mutex> lock(mtx);
        cout << "spinlock сработал за: " << duration.count() << " секунд." << endl;
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    }
}

class Spinwait 
{
private:

    atomic<bool> flag = false;

public:

    void lock() 
    {
        while (flag.exchange(true, memory_order_acquire)) 
        {
            std::this_thread::yield();
        }
    }

    void unlock() 
    {
        flag.store(false, memory_order_release);
    }
};

void spinWaitTesting(int numOfThreads)
{
    cout << "Проверка работы SPINWAIT" << endl;
    auto start = chrono::high_resolution_clock::now(); // таймер начала
    Spinwait swlock;
    vector<jthread> threads;
    
    for (int i = 0; i < numOfThreads; i++)
    {
        threads.push_back(jthread([i, &swlock]()
        {
            swlock.lock();
            {
                lock_guard<mutex> lock(mtx);
                cout << "Схватили лок ->";
                cout << " Использование общих ресурсов ->";
                cout << " Освободили лок" << endl;
            }
            swlock.unlock();
        }));
    }

    auto end = chrono::high_resolution_clock::now(); 
    chrono::duration<double> duration = end - start; 
    this_thread::sleep_for(chrono::milliseconds(200));
    {
        lock_guard<mutex> lock(mtx);
        cout << "spinwait сработал за: " << duration.count() << " секунд." << endl;
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    }
}

class Monitor 
{
private:
    mutex mtx;
    condition_variable cv;
    queue<char> dataQueue;

public:
    void addData(char value) 
    {
        unique_lock<mutex> lock(mtx);
        dataQueue.push(value);
        cout << "Data added: " << value << endl;
        cv.notify_one();
    }

    char getData() 
    {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this]() { return !dataQueue.empty(); });
        char value = dataQueue.front();
        dataQueue.pop();
        cout << "Data removed: " << value << endl;
        return value;
    }
};

void monitorTesting(int numOfThreads)
{
    cout << "Проверка работы MONITOR" << endl;
    auto start = chrono::high_resolution_clock::now(); // таймер начала
    Monitor monitor;
    vector<jthread> threads;
    
    for (int i = 0; i < numOfThreads; i++)
    {
        threads.push_back(jthread([i, &monitor]()
        {
            monitor.addData(generateRandomSymbol());
        }));
    }

    auto end = chrono::high_resolution_clock::now(); 
    chrono::duration<double> duration = end - start; 
    this_thread::sleep_for(chrono::milliseconds(200));
    {
        lock_guard<mutex> lock(mtx);
        cout << "monitor сработал за: " << duration.count() << " секунд." << endl;
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    }
}


int main()
{
    setlocale(LC_ALL, "RU");

    int numOfThreads = 0;
    while (true) 
    {
        cout << "Введите число работающих потоков: ";
        string input;
        cin >> input;

        bool isValid = true;
        for (char c : input) 
        {
            if (!isdigit(c)) 
            {
                isValid = false;
                break;
            }
        }

        if (!isValid) 
        {
            cout << "Некорректный ввод! Введите целое положительное число." << endl;
        } 
        else 
        {
            numOfThreads = stoi(input);
            cout << "Столько потоков будет запущено --> " << numOfThreads << endl;
            cout << "---------------------------------------------------------------" << endl;
            break;
        }
    }
    
    raceTesting(numOfThreads);
    mutexTesting(numOfThreads);
    semaphoreTesting(numOfThreads);
    semaphoreSlimTesting(numOfThreads);
    barrierTesting(numOfThreads);
    spinLockTesting(numOfThreads);
    spinWaitTesting(numOfThreads);
    monitorTesting(numOfThreads);

    return 0;
}