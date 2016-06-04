# My codecs for compression-project

## Что здесь происходит?
 + Кодеки для сжатия больших количеств малых записей.
 + Сжатие должно производиться для каждой записи отдельно для возможности отдельного раскодирования в будущем.
 + Написаны тестеры для проверки скорости, степени сжатия и корректности кодеков.
 + Кодек, основаный на алгоритме Хаффмана.
 + Кодек, заменяющий частые подстроки на их номера в словаре.
 + Кодек-шаблон, способный объединять несколько существующих алгоритмов.

## Какие технологии использовались в проекте?
 + C++14
 + git
 + cmake
 + perf
 + gdb

Автор: Корозевцев Павел
