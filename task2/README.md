Теперь также реализована внешняя сортировка MergeSort для успешного выполнения операции Reduce в случае файлов большого размера.


--------------------------------------------

В файле <code>mapreduce.cpp</code> реализовано консольное приложение, позволяющее применять MapReduce.


Компиляция программы: <code>g++ mapreduce.cpp --std=c++14 -O2 -o mapreduce</code>

Запуск map:    <code>./mapreduce map path_to_map_script [map script parameters] input_file output_file</code>

Запуск reduce: <code>./mapreduce reduce path_to_reduce_script [reduce script parameters] input_file output_file</code>

Справка:       <code>./mapreduce help</code>


Если map-скрипт или reduce-скрипт имеет свои флаги и параметры, то их можно передать при вызове <code>./mapreduce</code> после пути к скрипту.


Файлы <code>map.cpp</code> и <code>reduce.cpp</code> -- скрипты для решения задачи WordCount (соответственно, для Map и Reduce).

Компиляция: <code>g++ (map.cpp or reduce.cpp) --std=c++14 -O2 -o (name)</code>
