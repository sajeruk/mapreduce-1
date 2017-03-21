В файле mapreduce.cpp реализовано консольное приложение, позволяющее применять MapReduce к файлам, помещающимся в оперативную память.


Компиляция программы: g++ mapreduce.cpp --std=c++14 -O2 -o mapreduce

Запуск map:    ./mapreduce map path_to_map_script [map script parameters] input_file output_file

Запуск reduce: ./mapreduce reduce path_to_reduce_script [reduce script parameters] input_file output_file

Справка:       ./mapreduce help


Если map-скрипт или reduce-скрипт имеет свои флаги и параметры, то их можно передать при вызове ./mapreduce после пути к скрипту.


Файлы map.cpp и reduce.cpp -- скрипты для решения задачи WordCount (соответственно, для Map и Reduce).

Компиляция: g++ (map.cpp or reduce.cpp) --std=c++14 -O2 -o (name)
