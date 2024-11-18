#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>

using namespace std;

const int num_philosophers = 5; // количество философов

mutex forks[num_philosophers]; 
mutex output_mutex;
mutex waiter; 

void philosopher(int id) {
    while (true) {
        // Философ размышляет
        {
            lock_guard<mutex> lock(output_mutex); // захватываем мьютексом вывод в консоль
            cout << "Философ " << id << " размышляет.\n"; 
        }
        this_thread::sleep_for(chrono::milliseconds(5000));

        // Запрос на разрешение официанта
        waiter.lock();

        // Взятие вилок по порядку
        mutex& first_fork = forks[min(id, (id + 1) % num_philosophers)]; //сначала берем вилку с меньшим номером 
        mutex& second_fork = forks[max(id, (id + 1) % num_philosophers)]; // потом берем вилку с большим номером

        // Захват вилок
        lock(first_fork, second_fork); // Зафиксируем обе вилки
        lock_guard<mutex> lg1(first_fork, adopt_lock); // Зафиксировать первую вилку (передача через параметр adopt_lock уже ранее захваченного мьютекса)
        lock_guard<mutex> lg2(second_fork, adopt_lock); // Зафиксировать вторую вилку

        // Философ ест
        {
            lock_guard<mutex> lock(output_mutex); // забираем мьютексом вывод
            cout << "Философ " << id << " начал есть.\n"; // Изменено сообщение
        }
        //this_thread::sleep_for(chrono::milliseconds(1000));

        // Философ освобождает вилки (выйдет из области видимости, и вилки будут освобождены)
        {
            lock_guard<mutex> lock(output_mutex); // забираем мьютексом вывод
            cout << "Философ " << id << " освободил вилки.\n"; // Новое сообщение о освобождении вилок
        }

        // Освобождение официанта
        waiter.unlock();
    }
}

int main() {
    vector<thread*> philosophers; // вектор с философами 

    for (int i = 0; i < num_philosophers; ++i) {
        philosophers.push_back(new thread(philosopher, i)); // создаем поток(философа) и заплняяем вектор
    }

    for (size_t i = 0; i < philosophers.size(); ++i) { //запускаем потоки
        philosophers[i]->join(); 
        delete philosophers[i]; // освобождаем память
    }

    return 0;
}
