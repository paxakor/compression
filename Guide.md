### Инструкция

* В корневой папке проекта (рядом с CMakeLists.txt) должен лежать файл tester.conf с таким содержанием:
 ```data_file path/to/data```

* В файле с данными должны храниться записи как plain text, каждая запись на отдельной строке.

* Проект собирается при помощи следующей команды:
 ```mkdir build && cd build && cmake .. && make```