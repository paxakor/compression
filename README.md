# My codecs for compression-project

## Build status
* Master: [![Build Status](https://travis-ci.org/paxakor/compression.svg?branch=master)](https://travis-ci.org/paxakor/compression)
* Dev: [![Build Status](https://travis-ci.org/paxakor/compression.svg?branch=dev)](https://travis-ci.org/paxakor/compression)

## Что хочется получить в результате?
* Кодеки для сжатия больших количеств малых записей.
 + Сжатие должно производиться для каждой записи отдельно для возможности отдельного раскодирования в будущем.
 + Алгоритмы должны сжимать максимально быстро и максимально сильно.

## Что нужно сделать?
* Изучить строковые структуры и алгоритмы:
 + BWT.
 + MTF.
 + LZ*
* Оптимизировать написаный код.

## Что уже сделано?
* Для написания кодеков.
 + Написан тестер для проверки скорости, степени сжатия и корректности кодеков.
 + Написан кодек, основаный на алгоритме Хаффмана.

Автор: Корозевцев Павел
