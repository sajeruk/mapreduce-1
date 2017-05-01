Компиляция файлов:
g++ mapreduce.cpp --std=c++14 -O2 -o mapreduce
g++ map1.cpp --std=c++14 -O2 -o map1
g++ map2.cpp --std=c++14 -O2 -o map2
g++ reduce1.cpp --std=c++14 -O2 -o reduce1
g++ reduce2.cpp --std=c++14 -O2 -o reduce2
g++ main.cpp --std=c++14 -O2 -o main

Запуск: <code>./main (mapreduce_file map1_file map2_file reduce1_file reduce2_file file_with_URLs file_with_words output_file</code>.

mapreduce.cpp делает то же самое, что и в предыдущих заданиях.
main.cpp вызывает 4 раза mapreduce.cpp с разными параметрами.

Алгоритм:

1. С помощью map1.cpp строки файла слов преобразуется к виду <code>word '\t' '#'</code>
2. С помощью map2.cpp из всех URL второго входного файла получаем содержимое и название статьи, получаем файл со строками вида <code>word '\t' page_title</code> (работает плохо).
3. С помощью reduce1.cpp из файла, полученного в п.2, получаем файл со строками вида <code>word '\t' page_title1#...#page_titleX</code>.
4. Сливаем файлы из п.1 и п.3
5. Делаем Reduce с помощью reduce2.cpp  полученного файла.


